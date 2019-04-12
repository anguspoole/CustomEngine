#include <iostream>

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include "../Global/global.h"
#include "../Global/gOpenGL.h"
#include "../Shading/cShaderManager.h"
#include "../Lighting/cLightHelper.h"

// "HEAP" Variables
GLFWwindow* window = NULL;
int windowWidth = 0;
int windowHeight = 0;

cCamera* g_Camera = NULL;
cShaderManager* pTheShaderManager = NULL;
cVAOMeshManager* g_VAOMeshManager = NULL;
cBasicTextureManager* g_TheTextureManager = NULL;
cLightManager* g_LightManager = NULL;

std::vector< cEntity* > vec_pObjectsToDraw;

void main()
{
	//Make window
	window = InitWindow(1024, 480, "Hello World");

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
	g_VAOMeshManager = new cVAOMeshManager();
	g_TheTextureManager = new cBasicTextureManager();
	g_LightManager = new cLightManager();

	g_Camera->eye = glm::vec3(0.0f, 0.0f, -10.0f);

	// Point back to default frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Loading the uniform variables here (rather than the inner draw loop)
	GLint objectColour_UniLoc = glGetUniformLocation(program, "objectColour");
	GLint matModel_location = glGetUniformLocation(program, "matModel");
	GLint matView_location = glGetUniformLocation(program, "matView");
	GLint matProj_location = glGetUniformLocation(program, "matProj");
	GLint eyeLocation_location = glGetUniformLocation(program, "eyeLocation");

	LoadModelTypes_ASYNC(g_VAOMeshManager, program);
	LoadModelsIntoScene(vec_pObjectsToDraw);

	g_Camera->targetPos = vec_pObjectsToDraw[1]->m_EntityPhysics->position;

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

	//mainLight->position = glm::vec4(0.0f, 100.0f, -50.0f, 1.0f);
	mainLight->position = glm::vec4(0.0f, 0.0f, 5.0f, 1.0f);
	mainLight->atten.x = 0.0f;				// 	float constAtten = 0.0f;
	mainLight->atten.y = 0.00385720730f;		//	float linearAtten = 0.01f;
	mainLight->atten.z = 0.00001f;		//	float quadAtten = 0.001f;
	mainLight->diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);		// White light

	mainLight->SetLightType(sLight::POINT_LIGHT);

	g_LightManager->vecLights.push_back(mainLight);

	cLightHelper* pLightHelper = new cLightHelper();

	while (!glfwWindowShouldClose(window))
	{
		// Switch to the shader we want
		::pTheShaderManager->useShaderProgram("BasicUberShader");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);		// Points to the "regular" frame buffer

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
		g_Camera->eye = glm::vec3(0.0f, 0.0f, 0.0f);

		glm::mat4 matView = glm::lookAt(g_Camera->eye, g_Camera->getAtInWorldSpace(), g_Camera->up);

		//mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		glm::mat4 matProjection = glm::perspective(0.6f,			// FOV
			ratio,		// Aspect ratio
			0.1f,			// Near clipping plane
			10000.0f);	// Far clipping plane

		glUniform3f(eyeLocation_location, g_Camera->eye.x, g_Camera->eye.y, g_Camera->eye.z);

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

		{
			// ***************************************
			 // Draw the skybox first 
			cEntity* pSkyBox = findObjectByFriendlyName("SkyBoxObject");
			//			cMeshObject* pSkyBox = findObjectByFriendlyName("SkyPirate");
						// Place skybox object at camera location
			pSkyBox->m_EntityPhysics->position = g_Camera->eye;
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
			DrawObject(pSkyBox, matIdentity, program, 0, NULL);

			//glEnable( GL_CULL_FACE );
			//glCullFace( GL_BACK );

			pSkyBox->m_EntityMesh->bIsVisible = false;
			glUniform1f(useSkyBoxTexture_UniLoc, (float)GL_FALSE);

			//			pSkyBox->position = oldPosition;
			//			pSkyBox->nonUniformScale = oldScale;
						// ***************************************
		}

		DrawScene_Simple(vec_pObjectsToDraw, program, 0, NULL);

		glfwSwapBuffers(window);		// Shows what we drew

		glfwPollEvents();
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