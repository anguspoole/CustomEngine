#include <iostream>

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <time.h>

#include <iPhysicsFactory.h>
#include <../load/nLoad.h>

#include "../Global/global.h"
#include "../Global/gOpenGL.h"
#include "../Global/mazeGlobals.h"
#include "../Shading/cShaderManager.h"
#include "../Lighting/cLightHelper.h"
#include "../AI/AIController.h"

// "HEAP" Variables
GLFWwindow* window = NULL;
int windowWidth = 0;
int windowHeight = 0;

cCamera* g_Camera = NULL;
cCamera* player_Camera = NULL;
cCamera* pass_Camera = NULL;
cShaderManager* pTheShaderManager = NULL;
cVAOMeshManager* g_VAOMeshManager = NULL;
cBasicTextureManager* g_TheTextureManager = NULL;
cLightManager* g_LightManager = NULL;

cEntity* player = NULL;
std::vector<cEntity*> enemyList;
std::vector<cEntity*> globList;
std::vector<cEntity*> paintList;

std::vector< cEntity* > vec_pObjectsToDraw;

nLoad::sConfig config;

cFBO* g_pFBOMain;
cFBO* g_pFBOBlurA;
cFBO* g_pFBOBlurB;
cFBO* g_pFBOFinal;

double deltaTime = 0.0f;

float timeRemaining = 100.0f;

int mazeWidth = 30;
int mazeHeight = 30;
cMazeMaker* maze;

// For the particle emitter examples
void setUpParticleEmitters(void);							// in LoadModels.cpp
void updateAndDrawParticles(double deltaTime,				// in DrawObject_call.cpp
	GLuint shaderProgramID,
	glm::vec3 cameraEye);

bool debugging = true;

void main()
{
	srand(NULL);

	//Make window
	window = InitWindow(1024, 480, "Neon Samurai Ignition");

	//Create shader manager
	pTheShaderManager = new cShaderManager();
	pTheShaderManager->setBasePath("assets/shaders/");

	//Create vertex shader
	cShaderManager::cShader vertexShader;
	vertexShader.fileName = "vertex01.glsl";
	vertexShader.shaderType = cShaderManager::cShader::VERTEX_SHADER;

	//Create fragment shader
	cShaderManager::cShader fragmentShader;
	fragmentShader.fileName = "fragment01.glsl";
	fragmentShader.shaderType = cShaderManager::cShader::FRAGMENT_SHADER;

	if (pTheShaderManager->createProgramFromFile("BasicUberShader",
		vertexShader,
		fragmentShader))
	{		// Shaders are OK
		std::cout << "Compiled shaders OK." << std::endl;
	}
	else
	{
		std::cout << "OH NO! Compile error" << std::endl;
		std::cout << pTheShaderManager->getLastError() << std::endl;
	}

	// If we are here, the shaders comipled and linked OK
	std::cout << "The shaders comipled and linked OK" << std::endl;

	// Load the uniform location values (some of them, anyway)
	cShaderManager::cShaderProgram* pSP = ::pTheShaderManager->pGetShaderProgramFromFriendlyName("BasicUberShader");
	pSP->LoadUniformLocation("texture00");
	pSP->LoadUniformLocation("texture01");
	pSP->LoadUniformLocation("texture02");
	pSP->LoadUniformLocation("texture03");
	pSP->LoadUniformLocation("texture04");
	pSP->LoadUniformLocation("texture05");
	pSP->LoadUniformLocation("texture06");
	pSP->LoadUniformLocation("texture07");
	pSP->LoadUniformLocation("texBlendWeights[0]");
	pSP->LoadUniformLocation("texBlendWeights[1]");

	GLuint program = pTheShaderManager->getIDFromFriendlyName("BasicUberShader");

	g_Camera = new cCamera();
	player_Camera = new cCamera();
	pass_Camera = new cCamera();
	g_VAOMeshManager = new cVAOMeshManager();
	g_TheTextureManager = new cBasicTextureManager();
	g_LightManager = new cLightManager();

	::g_pFBOMain = new cFBO();
	::g_pFBOBlurA = new cFBO();
	::g_pFBOBlurB = new cFBO();
	::g_pFBOFinal = new cFBO();

	std::string FBOErrorString;
	int fbWidth, fbHeight;
	glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
	::g_pFBOMain->init(fbWidth, fbHeight, FBOErrorString);
	::g_pFBOBlurA->init(fbWidth, fbHeight, FBOErrorString);
	::g_pFBOBlurB->init(fbWidth, fbHeight, FBOErrorString);
	::g_pFBOFinal->init(fbWidth, fbHeight, FBOErrorString);

	// Point back to default frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Loading the uniform variables here (rather than the inner draw loop)
	GLint objectColour_UniLoc = glGetUniformLocation(program, "objectColour");
	GLint matModel_location = glGetUniformLocation(program, "matModel");
	GLint matView_location = glGetUniformLocation(program, "matView");
	GLint matProj_location = glGetUniformLocation(program, "matProj");
	GLint eyeLocation_location = glGetUniformLocation(program, "eyeLocation");


	if (!nLoad::LoadConfig("../Config/config.json", config))
	{
		std::cout << "Error loading json" << std::endl;
		system("pause");
		return;
	}

	std::cout << "loaded json" << std::endl;

	//LoadSkinnedMeshModel(vec_pObjectsToDraw, program);

	//********************************************************
	// TERM 2 PHYSICS FACTORY STUFF
	//********************************************************
	InitPhysicsStuff(config);
	buildPhysicsWorld();
	LoadModelTypes_ASYNC(g_VAOMeshManager, program);
	buildPhysicsObjects(config, ::vec_pObjectsToDraw, program);
	LoadModelsIntoScene(vec_pObjectsToDraw);
	//***********************************************************
	//***********************************************************

	g_Camera->eye = glm::vec3(0.0f, 0.0f, 0.0f);
	g_Camera->targetPos = glm::vec3(0.0f, 0.0f, 1.0f);
	player_Camera->eye = glm::vec3(0.0f, 50.0f, -50.0f);
	pass_Camera->eye = glm::vec3(0.0f, 30.0f, -30.0f);
	//g_Camera->eye = glm::vec3(0.0f, 10.0f, -30.0f);
	pass_Camera->targetPos = glm::vec3(0.0f, -1.0f, 1.0f);
	//g_Camera->targetPos = vec_pObjectsToDraw[1]->m_EntityPhysics->position;
	//g_Camera->targetPos = vec_pObjectsToDraw[1]->m_EntityPhysics->position;

	std::cout << "finished loading models" << std::endl;

	int renderPassNumber = 1;
	GLint renderPassNumber_UniLoc = glGetUniformLocation(program, "renderPassNumber");

	//May replace this with a function to create lights and assign these parameters
	sLight* mainLight = new sLight();
	// Do this once...
	mainLight->position_UniLoc = glGetUniformLocation(program, "theLights[0].position");
	mainLight->diffuse_UniLoc = glGetUniformLocation(program, "theLights[0].diffuse");
	mainLight->specular_UniLoc = glGetUniformLocation(program, "theLights[0].specular");
	mainLight->atten_UniLoc = glGetUniformLocation(program, "theLights[0].atten");
	mainLight->direction_UniLoc = glGetUniformLocation(program, "theLights[0].direction");
	mainLight->param1_UniLoc = glGetUniformLocation(program, "theLights[0].param1");
	mainLight->param2_UniLoc = glGetUniformLocation(program, "theLights[0].param2");

	mainLight->position = glm::vec4(0.0f, 100.0f, -50.0f, 1.0f);
	//mainLight->position = glm::vec4(0.0f, 0.0f, 5.0f, 1.0f);
	mainLight->atten.x = 0.0f;				// 	float constAtten = 0.0f;
	mainLight->atten.y = 0.00385720730f;		//	float linearAtten = 0.01f;
	mainLight->atten.z = 0.00001f;		//	float quadAtten = 0.001f;
	mainLight->diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);		// White light

	mainLight->SetLightType(sLight::POINT_LIGHT);

	g_LightManager->vecLights.push_back(mainLight);

	cLightHelper* pLightHelper = new cLightHelper();

	double lastTime = glfwGetTime();

	AIController* aiController = new AIController();

	bool testGlob = true;

	setUpParticleEmitters();

	while (!glfwWindowShouldClose(window))
	{

		if (timeRemaining <= 0.0f)
		{
			std::string newTitle = "Game Over";
			UpdateWindowTitle(window, newTitle);
			system("PAUSE");
			break;
		}

		//g_Camera->targetPos = vec_pObjectsToDraw[3]->m_EntityPhysics->position;
		//g_Camera->targetPos = glm::vec3(0.0f);

		// High res timer (likely in ms or ns)
		double currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		double dT = deltaTime;

		double MAX_DELTA_TIME = 0.1;	// 100 ms
		if (deltaTime > MAX_DELTA_TIME)
		{
			deltaTime = MAX_DELTA_TIME;
		}

		// The physics update loop
		//DoPhysicsUpdate( deltaTime, vec_pObjectsToDraw );
		gPhysicsWorld->Update(deltaTime);
		aiController->UpdateBehaviours(enemyList, player);

		for (int i = 0; i < enemyList.size(); i++)
		{
			if (enemyList[i]->status == eEntityStatus::DEAD && enemyList[i]->spray)// && enemyList[i]->healthTimer > 1.8f)
			{
				SpawnGlob(enemyList[i], vec_pObjectsToDraw, program);
				SpawnGlob(enemyList[i], vec_pObjectsToDraw, program);
				SpawnGlob(enemyList[i], vec_pObjectsToDraw, program);
				enemyList[i]->spray = false;
			}
		}

		{
			glm::mat4 targetTransform;
			cEntity * userSphere = findObjectByFriendlyName("Player");
			userSphere->m_EntityPhysics->rigidBody->GetTransform(targetTransform);
			glm::vec3 targetPos = glm::vec3(targetTransform[3].x, targetTransform[3].y, targetTransform[3].z);
			userSphere->m_EntityPhysics->position = targetPos;
		}

		CheckForGlobHits();

		/*cEntity* katana = player->vec_pChildrenEntities[0];
		if (katana->m_EntityPhysics->rigidBody->GetHitStatus())
		{
			katana->m_EntityPhysics->rigidBody->SetHitStatus(false);
		}*/

		currentCamera = player_Camera;

		mainLight->position;
		glm::vec3 playerPos;
		player->m_EntityPhysics->rigidBody->GetPosition(playerPos);
		mainLight->position = glm::vec4(playerPos, 1.0f);
		mainLight->position.y += 100.0f;
		mainLight->position.z += 100.0f;

		// Switch to the shader we want
		//::pTheShaderManager->useShaderProgram("BasicUberShader");

		//glBindFramebuffer(GL_FRAMEBUFFER, 0);		// Points to the "regular" frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, ::g_pFBOMain->ID);
		::g_pFBOMain->clearBuffers(true, true);
													// Get the size of the actual (screen) frame buffer
		glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
		float ratio = windowWidth / (float)windowHeight;
		glViewport(0, 0, windowWidth, windowHeight);

		glEnable(GL_DEPTH);		// Enables the KEEPING of the depth information
		glEnable(GL_DEPTH_TEST);	// When drawing, checked the existing depth
		glEnable(GL_CULL_FACE);	// Discared "back facing" triangles

		// 2. Clear everything **ON THE MAIN FRAME BUFFER** 
		//     (NOT the offscreen buffer)
		// This clears the ACTUAL screen framebuffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glUniform1f(renderPassNumber_UniLoc, 1.0f);	// Tell shader it's the 2nd pass

		//Assign camera to new point
		//g_Camera->eye = glm::vec3(0.0f, 0.0f, 0.0f);

		glm::mat4 matView = glm::lookAt(player_Camera->eye, player_Camera->getAtInWorldSpace(), player_Camera->getUpVector());

		//mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		glm::mat4 matProjection = glm::perspective(0.6f,			// FOV
			ratio,		// Aspect ratio
			0.1f,			// Near clipping plane
			10000.0f);	// Far clipping plane

		glUniform3f(eyeLocation_location, player_Camera->eye.x, player_Camera->eye.y, player_Camera->eye.z);

		glUniformMatrix4fv(matView_location, 1, GL_FALSE, glm::value_ptr(matView));
		glUniformMatrix4fv(matProj_location, 1, GL_FALSE, glm::value_ptr(matProjection));

		// Do all this ONCE per frame
		glUniform4f(g_LightManager->vecLights[0]->position_UniLoc, g_LightManager->vecLights[0]->position.x, 
					g_LightManager->vecLights[0]->position.y, g_LightManager->vecLights[0]->position.z, 1.0f);
		glUniform4f(g_LightManager->vecLights[0]->diffuse_UniLoc, g_LightManager->vecLights[0]->diffuse.x, 
					g_LightManager->vecLights[0]->diffuse.y, g_LightManager->vecLights[0]->diffuse.z, 1.0f);
		glUniform4f(g_LightManager->vecLights[0]->param2_UniLoc, 1.0f, 0.0f, 0.0f, 0.0f);	// Turns it "on")
		glUniform4f(g_LightManager->vecLights[0]->atten_UniLoc, g_LightManager->vecLights[0]->atten.x, 
					g_LightManager->vecLights[0]->atten.y, g_LightManager->vecLights[0]->atten.z, 
					g_LightManager->vecLights[0]->atten.w);

		// Now pass the things we need for spots and directional, too:
		glUniform4f(g_LightManager->vecLights[0]->direction_UniLoc, g_LightManager->vecLights[0]->direction.x,
					g_LightManager->vecLights[0]->direction.y, g_LightManager->vecLights[0]->direction.z,
					g_LightManager->vecLights[0]->direction.w);
		glUniform4f(g_LightManager->vecLights[0]->param1_UniLoc,
					g_LightManager->vecLights[0]->param1.x,		// lightType
					g_LightManager->vecLights[0]->param1.y,		// inner angle
					g_LightManager->vecLights[0]->param1.z,		// outer angle
					g_LightManager->vecLights[0]->param1.w);	// TBD

		//glUniform1f(renderPassNumber_UniLoc, 1.0f);	// Tell shader it's the 2nd pass

		if (paintList.size() > 0)
		{
			glUniform1f(renderPassNumber_UniLoc, 4.0f);	// Tell shader it's the 1st pass
			for (int i = 0; i < paintList.size(); i++)
			{
				glm::mat4 matmodel = glm::mat4(1.0f);
				DrawObject(paintList[i], matmodel, program, 0, NULL);
			}
		}

		glUniform1f(renderPassNumber_UniLoc, 1.0f);	// Tell shader it's the 1st pass

		{
			// ***************************************
			 // Draw the skybox first 
			cEntity* pSkyBox = findObjectByFriendlyName("SkyBoxObject");
			//			cMeshObject* pSkyBox = findObjectByFriendlyName("SkyPirate");
						// Place skybox object at camera location
			pSkyBox->m_EntityPhysics->position = player_Camera->eye;
			pSkyBox->m_EntityMesh->bIsVisible = true;
			pSkyBox->m_EntityMesh->bIsWireFrame = false;

			//			glm::vec3 oldPosition = pSkyBox->position;
			//			glm::vec3 oldScale = pSkyBox->nonUniformScale;
			//			pSkyBox->setUniformScale(100.0f);

			//			glDisable( GL_CULL_FACE );		// Force drawing the sphere
				//		                                // Could also invert the normals
						// Draw the BACK facing (because the normals of the sphere face OUT and we 
						//  are inside the centre of the sphere..
			//			glCullFace( GL_FRONT );

						// Bind the cube map texture to the cube map in the shader
			GLuint cityTextureUNIT_ID = 30;			// Texture unit go from 0 to 79
			glActiveTexture(cityTextureUNIT_ID + GL_TEXTURE0);	// GL_TEXTURE0 = 33984

			int cubeMapTextureID = ::g_TheTextureManager->getTextureIDFromName("CityCubeMap");

			// Cube map is now bound to texture unit 30
			//glBindTexture( GL_TEXTURE_2D, cubeMapTextureID );
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureID);

			//uniform samplerCube textureSkyBox;
			GLint skyBoxCubeMap_UniLoc = glGetUniformLocation(program, "textureSkyBox");
			glUniform1i(skyBoxCubeMap_UniLoc, cityTextureUNIT_ID);

			//uniform bool useSkyBoxTexture;
			GLint useSkyBoxTexture_UniLoc = glGetUniformLocation(program, "useSkyBoxTexture");
			glUniform1f(useSkyBoxTexture_UniLoc, (float)GL_TRUE);

			glm::mat4 matIdentity = glm::mat4(1.0f);
			DrawObject(pSkyBox, matIdentity, program, 1, NULL);

			//glEnable( GL_CULL_FACE );
			//glCullFace( GL_BACK );

			pSkyBox->m_EntityMesh->bIsVisible = false;
			glUniform1f(useSkyBoxTexture_UniLoc, (float)GL_FALSE);

			//			pSkyBox->position = oldPosition;
			//			pSkyBox->nonUniformScale = oldScale;
						// ***************************************
		}

		for (int i = 0; i < vec_pObjectsToDraw.size(); i++)
		{
			vec_pObjectsToDraw[i]->Update(deltaTime);
			for (int j = 0; j < vec_pObjectsToDraw[i]->vec_pChildrenEntities.size(); j++)
			{
				vec_pObjectsToDraw[i]->vec_pChildrenEntities[j]->Update(deltaTime);
			}
		}

		//CheckForHits();

		//************************************
		// Refocus Camera
		//************************************
		//glm::mat4 targetTransform;
		////cEntity * userSphere = findObjectByFriendlyName("sphere0");
		//cEntity * userSphere = findObjectByFriendlyName("Player");
		//userSphere->m_EntityPhysics->rigidBody->GetTransform(targetTransform);
		//glm::vec3 targetPos = glm::vec3(targetTransform[3].x, targetTransform[3].y, targetTransform[3].z);
		////glm::vec3 targetPos = userSphere->m_EntityPhysics->position;
		//glm::vec3 targetDir = glm::normalize(targetPos - player_Camera->eye);
		//glm::quat q = glm::inverse(glm::lookAt(targetPos - player_Camera->eye, targetDir, player_Camera->getUpVector()));
		//player_Camera->setMeshOrientationQ(q);
		//player_Camera->m_UpdateAtFromOrientation();

		//pTheShaderManager->useShaderProgram("Debug_Shader");
		// Render the DEBUG Physics
		
		//pTheShaderManager->useShaderProgram("BasicUberShader");

		if (debugging)
		{
			gPhysicsWorld->DrawDebug(program);
		}

		DrawScene_Simple(vec_pObjectsToDraw, program, 1, NULL);
		updateAndDrawParticles(deltaTime, program, currentCamera->eye);

		if (debugging)
		{
			gPhysicsWorld->DrawDebug(program);
		}


		currentCamera = g_Camera;

		//All objects have been drawn - now render scene to quad
			//cShaderManager::cShaderProgram* blurSP = ::pTheShaderManager->pGetShaderProgramFromFriendlyName("BlurShader");
			//glUseProgram(blurSP->ID);
			//**********************************
			//Third Pass
			//**********************************
			cEntity* fullQuad = findObjectByFriendlyName("2SidedQuadBloomA");
			cEntity* fullQuad2 = findObjectByFriendlyName("2SidedQuadBloomB");
			// 1. Set the Framebuffer output to the main framebuffer
			glBindFramebuffer(GL_FRAMEBUFFER, g_pFBOBlurA->ID);		// Points to the "regular" frame buffer
														// Get the size of the actual (screen) frame buffer
			glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
			ratio = windowWidth / (float)windowHeight;
			glViewport(0, 0, windowWidth, windowHeight);

			glEnable(GL_DEPTH);		// Enables the KEEPING of the depth information
			glEnable(GL_DEPTH_TEST);	// When drawing, checked the existing depth
			glEnable(GL_CULL_FACE);	// Discared "back facing" triangles

			// 2. Clear everything **ON THE MAIN FRAME BUFFER** 
			//     (NOT the offscreen buffer)
			// This clears the ACTUAL screen framebuffer
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// 3. Bind 1 texture (what we drew)
			fullQuad->m_EntityMesh->bIsVisible = true;
			fullQuad2->m_EntityMesh->bIsVisible = true;
			fullQuad->m_EntityMesh->b_HACK_UsesOffscreenFBO = true;
			fullQuad2->m_EntityMesh->b_HACK_UsesOffscreenFBO = true;
			fullQuad->m_EntityMesh->bDontLight = true;
			fullQuad2->m_EntityMesh->bDontLight = true;
			fullQuad->m_EntityMesh->bUseVertexColour = false;
			fullQuad2->m_EntityMesh->bUseVertexColour = false;
			fullQuad->m_EntityMesh->materialDiffuse = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			fullQuad2->m_EntityMesh->materialDiffuse = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

			fullQuad->m_EntityPhysics->nonUniformScale.x = (fullQuad->m_EntityPhysics->nonUniformScale.y / ratio);
			fullQuad2->m_EntityPhysics->nonUniformScale.x = (fullQuad2->m_EntityPhysics->nonUniformScale.y / ratio);

			//g_Camera->eye = glm::vec3(0.0f);

			fullQuad->m_EntityPhysics->setMeshOrientationEulerAngles(90.0f, 0.0f, 90.0f, true);
			fullQuad2->m_EntityPhysics->setMeshOrientationEulerAngles(90.0f, 0.0f, 90.0f, true);
			//fullQuad->position = ::g_pFlyCamera->eye + (40.0f * ::g_pFlyCamera->getCameraDirection());
			fullQuad->m_EntityPhysics->position = ::currentCamera->eye;
			fullQuad2->m_EntityPhysics->position = ::currentCamera->eye;
			//fullQuad->position = glm::vec3(7.0f, 7.5f, 3.0f);
			//fullQuad->position = glm::vec3(0.0f, 3.5f, 2.0f);
			fullQuad->m_EntityPhysics->position.z = ::currentCamera->eye.z + 15.0f;
			fullQuad2->m_EntityPhysics->position.z = ::currentCamera->eye.z + 15.0f;


			//glUniform3f(eyeLocation_location, ::g_pFlyCamera->eye.x, ::g_pFlyCamera->eye.y, ::g_pFlyCamera->eye.z);
			//matView = glm::lookAt(::g_pFlyCamera->eye,	// Eye
			//	::g_pFlyCamera->getAtInWorldSpace(),		// At
			//	::g_pFlyCamera->getUpVector());// Up

			glm::vec3 cameraFullScreenQuad = glm::vec3(0.0, 0.0, 0.0f);

			glUniform3f(eyeLocation_location, currentCamera->eye.x, currentCamera->eye.y, currentCamera->eye.z);
			matView = glm::lookAt(currentCamera->eye,			// Eye
				fullQuad->m_EntityPhysics->position,					// At
				glm::vec3(0.0f, 1.0f, 0.0f));			// Up

			glUniformMatrix4fv(matView_location, 1, GL_FALSE, glm::value_ptr(matView));
			//DrawScene_Simple(vec_pObjectsToDraw, program, 0);

			// Tell the shader this is the 3rd pass...
			// This will run a very simple shader, which
			//  does NOT lighting, and only samples from a single texture
			//  (for now: soon there will be multiple textures)
			//glUniform1f(renderPassNumber_UniLoc, 5.0f);	// Tell shader it's the 2nd pass


			// 4. Draw a single quad		
			glm::mat4 matModel = glm::mat4(1.0f);	// identity
			//fullQuad->m_EntityMesh->bIsVisible = true;
			//DrawObject(fullQuad, matModel, program, 0, g_pFBOMain);
			//DrawObject(fullQuad, matModel, program, 0, g_pFBOMain);

			//DrawObject(fullQuad, matModel, program, NULL);

			//fullQuad2->m_EntityMesh->bIsVisible = true;
			makeBlur(program, g_pFBOBlurA, g_pFBOBlurB, program);

			// 5. Enjoy.
			// Make this invisible for the "regular" pass
			fullQuad->m_EntityMesh->bIsVisible = false;
			fullQuad2->m_EntityMesh->bIsVisible = false;

			glUniform1f(renderPassNumber_UniLoc, 1.0f);	// Tell shader it's the 1st pass

					//All objects have been drawn - now render scene to quad
			{
				//cShaderManager::cShaderProgram* blurSP = ::pTheShaderManager->pGetShaderProgramFromFriendlyName("BlurShader");
				//glUseProgram(blurSP->ID);
				//glUseProgram(pSP->ID);
				//**********************************
				//Third Pass
				//**********************************
				cEntity* fullQuad = findObjectByFriendlyName("2SidedQuadBase");
				// 1. Set the Framebuffer output to the main framebuffer
				glBindFramebuffer(GL_FRAMEBUFFER, g_pFBOFinal->ID);		// Points to the "regular" frame buffer
															// Get the size of the actual (screen) frame buffer
				glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
				ratio = windowWidth / (float)windowHeight;
				glViewport(0, 0, windowWidth, windowHeight);

				currentCamera = g_Camera;

				glEnable(GL_DEPTH);		// Enables the KEEPING of the depth information
				glEnable(GL_DEPTH_TEST);	// When drawing, checked the existing depth
				glEnable(GL_CULL_FACE);	// Discared "back facing" triangles

				// 2. Clear everything **ON THE MAIN FRAME BUFFER** 
				//     (NOT the offscreen buffer)
				// This clears the ACTUAL screen framebuffer
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				// 3. Bind 1 texture (what we drew)
				fullQuad->m_EntityMesh->bIsVisible = true;
				fullQuad->m_EntityMesh->b_HACK_UsesOffscreenFBO = true;
				fullQuad->m_EntityMesh->bDontLight = true;
				fullQuad->m_EntityMesh->bUseVertexColour = false;
				fullQuad->m_EntityMesh->materialDiffuse = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

				fullQuad->m_EntityPhysics->nonUniformScale.x = (fullQuad->m_EntityPhysics->nonUniformScale.y / ratio);

				//g_Camera->eye = glm::vec3(0.0f);

				fullQuad->m_EntityPhysics->setMeshOrientationEulerAngles(90.0f, 0.0f, 90.0f, true);
				//fullQuad->position = ::g_pFlyCamera->eye + (40.0f * ::g_pFlyCamera->getCameraDirection());
				fullQuad->m_EntityPhysics->position = ::currentCamera->eye;
				//fullQuad->position = glm::vec3(7.0f, 7.5f, 3.0f);
				//fullQuad->position = glm::vec3(0.0f, 3.5f, 2.0f);
				fullQuad->m_EntityPhysics->position.z = ::currentCamera->eye.z + 15.0f;


				//glUniform3f(eyeLocation_location, ::g_pFlyCamera->eye.x, ::g_pFlyCamera->eye.y, ::g_pFlyCamera->eye.z);
				//matView = glm::lookAt(::g_pFlyCamera->eye,	// Eye
				//	::g_pFlyCamera->getAtInWorldSpace(),		// At
				//	::g_pFlyCamera->getUpVector());// Up

				//glm::vec3 cameraFullScreenQuad = glm::vec3(0.0, 0.0, 0.0f);

				//glUniform3f(eyeLocation_location, currentCamera->eye.x, currentCamera->eye.y, currentCamera->eye.z);
				//matView = glm::lookAt(currentCamera->eye,			// Eye
				//	fullQuad->m_EntityPhysics->position,					// At
				//	glm::vec3(0.0f, 1.0f, 0.0f));			// Up

				glUniformMatrix4fv(matView_location, 1, GL_FALSE, glm::value_ptr(matView));
				//DrawScene_Simple(vec_pObjectsToDraw, program, 0);

				// Tell the shader this is the 3rd pass...
				// This will run a very simple shader, which
				//  does NOT lighting, and only samples from a single texture
				//  (for now: soon there will be multiple textures)
				glUniform1f(renderPassNumber_UniLoc, 5.0f);	// Tell shader it's the 2nd pass

				// 4. Draw a single quad		
				glm::mat4 matModel = glm::mat4(1.0f);	// identity
				fullQuad->m_EntityMesh->bIsVisible = true;
				DrawObject(fullQuad, matModel, program, 5, g_pFBOBlurB);

				//DrawObject(fullQuad, matModel, program, NULL);

				// 5. Enjoy.
				// Make this invisible for the "regular" pass
				fullQuad->m_EntityMesh->bIsVisible = false;

				glUniform1f(renderPassNumber_UniLoc, 1.0f);	// Tell shader it's the 1st pass
			}

		//All objects have been drawn - now render scene to quad
		{
			//cShaderManager::cShaderProgram* blurSP = ::pTheShaderManager->pGetShaderProgramFromFriendlyName("BlurShader");
			//glUseProgram(blurSP->ID);
			//glUseProgram(pSP->ID);
			//**********************************
			//Third Pass
			//**********************************
			cEntity* fullQuad = findObjectByFriendlyName("2SidedQuadFinal");
			// 1. Set the Framebuffer output to the main framebuffer
			glBindFramebuffer(GL_FRAMEBUFFER, 0);		// Points to the "regular" frame buffer
														// Get the size of the actual (screen) frame buffer
			glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
			ratio = windowWidth / (float)windowHeight;
			glViewport(0, 0, windowWidth, windowHeight);

			currentCamera = g_Camera;

			glEnable(GL_DEPTH);		// Enables the KEEPING of the depth information
			glEnable(GL_DEPTH_TEST);	// When drawing, checked the existing depth
			glEnable(GL_CULL_FACE);	// Discared "back facing" triangles

			// 2. Clear everything **ON THE MAIN FRAME BUFFER** 
			//     (NOT the offscreen buffer)
			// This clears the ACTUAL screen framebuffer
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// 3. Bind 1 texture (what we drew)
			fullQuad->m_EntityMesh->bIsVisible = true;
			fullQuad->m_EntityMesh->b_HACK_UsesOffscreenFBO = true;
			fullQuad->m_EntityMesh->bDontLight = true;
			fullQuad->m_EntityMesh->bUseVertexColour = false;
			fullQuad->m_EntityMesh->materialDiffuse = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

			fullQuad->m_EntityPhysics->nonUniformScale.x = (fullQuad->m_EntityPhysics->nonUniformScale.y / ratio);

			//g_Camera->eye = glm::vec3(0.0f);

			fullQuad->m_EntityPhysics->setMeshOrientationEulerAngles(90.0f, 0.0f, 90.0f, true);
			//fullQuad->position = ::g_pFlyCamera->eye + (40.0f * ::g_pFlyCamera->getCameraDirection());
			fullQuad->m_EntityPhysics->position = ::currentCamera->eye;
			//fullQuad->position = glm::vec3(7.0f, 7.5f, 3.0f);
			//fullQuad->position = glm::vec3(0.0f, 3.5f, 2.0f);
			fullQuad->m_EntityPhysics->position.z = ::currentCamera->eye.z + 15.0f;


			//glUniform3f(eyeLocation_location, ::g_pFlyCamera->eye.x, ::g_pFlyCamera->eye.y, ::g_pFlyCamera->eye.z);
			//matView = glm::lookAt(::g_pFlyCamera->eye,	// Eye
			//	::g_pFlyCamera->getAtInWorldSpace(),		// At
			//	::g_pFlyCamera->getUpVector());// Up

			//glm::vec3 cameraFullScreenQuad = glm::vec3(0.0, 0.0, 0.0f);

			//glUniform3f(eyeLocation_location, currentCamera->eye.x, currentCamera->eye.y, currentCamera->eye.z);
			//matView = glm::lookAt(currentCamera->eye,			// Eye
			//	fullQuad->m_EntityPhysics->position,					// At
			//	glm::vec3(0.0f, 1.0f, 0.0f));			// Up

			glUniformMatrix4fv(matView_location, 1, GL_FALSE, glm::value_ptr(matView));
			//DrawScene_Simple(vec_pObjectsToDraw, program, 0);

			// Tell the shader this is the 3rd pass...
			// This will run a very simple shader, which
			//  does NOT lighting, and only samples from a single texture
			//  (for now: soon there will be multiple textures)
			glUniform1f(renderPassNumber_UniLoc, 2.0f);	// Tell shader it's the 2nd pass

			// 4. Draw a single quad		
			glm::mat4 matModel = glm::mat4(1.0f);	// identity
			fullQuad->m_EntityMesh->bIsVisible = true;
			DrawObject(fullQuad, matModel, program, 0, g_pFBOFinal);

			//DrawObject(fullQuad, matModel, program, NULL);

			// 5. Enjoy.
			// Make this invisible for the "regular" pass
			fullQuad->m_EntityMesh->bIsVisible = false;

			glUniform1f(renderPassNumber_UniLoc, 1.0f);	// Tell shader it's the 1st pass
		}

		//timeRemaining -= deltaTime;

		// #include sstream 
		//std::stringstream ssTitle;

		//ssTitle			// std::cout 
		//	<< "Time Remaining: " << std::to_string(timeRemaining);

		//UpdateWindowTitle(window, ssTitle.str());

		//gPhysicsWorld->Update(deltaTime);

		// update the "last time"
		lastTime = currentTime;

		glfwSwapBuffers(window);		// Shows what we drew

		glfwPollEvents();

		ProcessAsyncKeys(window);

		ProcessAsyncMouse(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

cEntity* findObjectByFriendlyName(std::string theNameToFind)
{
	for (unsigned int index = 0; index != vec_pObjectsToDraw.size(); index++)
	{
		// Is this it? 500K - 1M
		// CPU limited Memory delay = 0
		// CPU over powered (x100 x1000) Memory is REAAAAALLY SLOW
		if (vec_pObjectsToDraw[index]->friendlyName == theNameToFind)
		{
			return vec_pObjectsToDraw[index];
		}
	}

	// Didn't find it.
	return NULL;	// 0 or nullptr
}

void UpdateWindowTitle(GLFWwindow* window, std::string title)
{

	glfwSetWindowTitle(window, title.c_str());

	return;
}