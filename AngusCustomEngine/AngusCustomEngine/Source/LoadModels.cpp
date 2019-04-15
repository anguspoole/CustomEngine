#include "../Global/gOpenGL.h"
#include "../Global/global.h"

#include "../Meshes/AssimpFBX/cAnimationState.h"

extern cSimpleAssimpSkinnedMesh* g_pRPGSkinnedMesh = NULL;

// Here's the 'Assimp to VAO Converer" thing....
bool AssimpSM_to_VAO_Converter(cSimpleAssimpSkinnedMesh* pTheAssimpSM,
	unsigned int shaderProgramID)
{
	sModelDrawInfo theSMDrawInfo;

	theSMDrawInfo.meshFileName = pTheAssimpSM->fileName;
	theSMDrawInfo.friendlyName = pTheAssimpSM->friendlyName;

	// Copy the data from assimp format into the sModelDrawInfo format...

	theSMDrawInfo.numberOfVertices = pTheAssimpSM->pScene->mMeshes[0]->mNumVertices;
	theSMDrawInfo.numberOfTriangles = pTheAssimpSM->pScene->mMeshes[0]->mNumFaces;

	// We used the "triangulate" option when loading so all the primitives
	//	will be triangles, but BEWARE!
	theSMDrawInfo.numberOfIndices = theSMDrawInfo.numberOfTriangles * 3;


	// Allocate the vertex array (it's a c-style array)
	theSMDrawInfo.pMeshData = new cMesh();

	theSMDrawInfo.pMeshData->pVertices = new sVertex_xyz_rgba_n_uv2_bt_4Bones[theSMDrawInfo.numberOfVertices];

	// Danger Will Robinson! 
	// You don't really need to do this, but at least it will clear it to zero.
	// (compiler will complain that it's 'not safe', etc.)
	memset(theSMDrawInfo.pMeshData->pVertices, 0,
		sizeof(sVertex_xyz_rgba_n_uv2_bt_4Bones) * theSMDrawInfo.numberOfVertices);


	for (unsigned int vertIndex = 0; vertIndex != theSMDrawInfo.numberOfVertices; vertIndex++)
	{
		theSMDrawInfo.pMeshData->pVertices[vertIndex].x
			= pTheAssimpSM->pScene->mMeshes[0]->mVertices[vertIndex].x;
		theSMDrawInfo.pMeshData->pVertices[vertIndex].y
			= pTheAssimpSM->pScene->mMeshes[0]->mVertices[vertIndex].y;
		theSMDrawInfo.pMeshData->pVertices[vertIndex].z
			= pTheAssimpSM->pScene->mMeshes[0]->mVertices[vertIndex].z;

		// Normals... 
		theSMDrawInfo.pMeshData->pVertices[vertIndex].nx
			= pTheAssimpSM->pScene->mMeshes[0]->mNormals[vertIndex].x;
		theSMDrawInfo.pMeshData->pVertices[vertIndex].ny
			= pTheAssimpSM->pScene->mMeshes[0]->mNormals[vertIndex].y;
		theSMDrawInfo.pMeshData->pVertices[vertIndex].nz
			= pTheAssimpSM->pScene->mMeshes[0]->mNormals[vertIndex].z;

		// Colours... 
		// (If there are no colours, make it hit pink)
		// Note: the method is because you could have more than one set of 
		//	vertex colours in the model (Why? Who the heck knows?) 
		if (pTheAssimpSM->pScene->mMeshes[0]->HasVertexColors(0))
		{
			theSMDrawInfo.pMeshData->pVertices[vertIndex].r
				= pTheAssimpSM->pScene->mMeshes[0]->mColors[vertIndex]->r;
			theSMDrawInfo.pMeshData->pVertices[vertIndex].g
				= pTheAssimpSM->pScene->mMeshes[0]->mColors[vertIndex]->g;
			theSMDrawInfo.pMeshData->pVertices[vertIndex].b
				= pTheAssimpSM->pScene->mMeshes[0]->mColors[vertIndex]->b;
			theSMDrawInfo.pMeshData->pVertices[vertIndex].a
				= pTheAssimpSM->pScene->mMeshes[0]->mColors[vertIndex]->a;
		}
		else
		{	// hotpink	#FF69B4	rgb(255,105,180)
			theSMDrawInfo.pMeshData->pVertices[vertIndex].r = 1.0f;
			theSMDrawInfo.pMeshData->pVertices[vertIndex].g = 105.0f / 255.0f;
			theSMDrawInfo.pMeshData->pVertices[vertIndex].b = 180.0f / 255.0f;
			theSMDrawInfo.pMeshData->pVertices[vertIndex].a = 1.0f;
		}

		//bi-normal  (or bi-tangent)
		theSMDrawInfo.pMeshData->pVertices[vertIndex].bx
			= pTheAssimpSM->pScene->mMeshes[0]->mBitangents[vertIndex].x;
		theSMDrawInfo.pMeshData->pVertices[vertIndex].by
			= pTheAssimpSM->pScene->mMeshes[0]->mBitangents[vertIndex].y;
		theSMDrawInfo.pMeshData->pVertices[vertIndex].bz
			= pTheAssimpSM->pScene->mMeshes[0]->mBitangents[vertIndex].z;

		// Tangent
		theSMDrawInfo.pMeshData->pVertices[vertIndex].tx
			= pTheAssimpSM->pScene->mMeshes[0]->mTangents[vertIndex].x;
		theSMDrawInfo.pMeshData->pVertices[vertIndex].ty
			= pTheAssimpSM->pScene->mMeshes[0]->mTangents[vertIndex].y;
		theSMDrawInfo.pMeshData->pVertices[vertIndex].tz
			= pTheAssimpSM->pScene->mMeshes[0]->mTangents[vertIndex].z;

		// uv2 (which are odd in assimp)
		// Note that there is an array of texture coordinates, 
		// up to 8 (actually). Usually, there's only 1
		if (pTheAssimpSM->pScene->mMeshes[0]->HasTextureCoords(0))	// 1st UV coords
		{
			// Assume there's 1... (at least)
			theSMDrawInfo.pMeshData->pVertices[vertIndex].u0
				= pTheAssimpSM->pScene->mMeshes[0]->mTextureCoords[0][vertIndex].x;
			theSMDrawInfo.pMeshData->pVertices[vertIndex].v0
				= pTheAssimpSM->pScene->mMeshes[0]->mTextureCoords[0][vertIndex].y;
		}
		if (pTheAssimpSM->pScene->mMeshes[0]->HasTextureCoords(1))	// 2nd UV coords
		{
			theSMDrawInfo.pMeshData->pVertices[vertIndex].u0
				= pTheAssimpSM->pScene->mMeshes[0]->mTextureCoords[1]->x;
			theSMDrawInfo.pMeshData->pVertices[vertIndex].v0
				= pTheAssimpSM->pScene->mMeshes[0]->mTextureCoords[1]->y;
		}
		// TODO: add additional texture coordinates (mTextureCoords[1], etc.)


		// 4Bones: ids and weights
		theSMDrawInfo.pMeshData->pVertices[vertIndex].boneID[0] = pTheAssimpSM->vecVertexBoneData[vertIndex].ids[0];
		theSMDrawInfo.pMeshData->pVertices[vertIndex].boneID[1] = pTheAssimpSM->vecVertexBoneData[vertIndex].ids[1];
		theSMDrawInfo.pMeshData->pVertices[vertIndex].boneID[2] = pTheAssimpSM->vecVertexBoneData[vertIndex].ids[2];
		theSMDrawInfo.pMeshData->pVertices[vertIndex].boneID[3] = pTheAssimpSM->vecVertexBoneData[vertIndex].ids[3];

		theSMDrawInfo.pMeshData->pVertices[vertIndex].boneWeights[0] = pTheAssimpSM->vecVertexBoneData[vertIndex].weights[0];
		theSMDrawInfo.pMeshData->pVertices[vertIndex].boneWeights[1] = pTheAssimpSM->vecVertexBoneData[vertIndex].weights[1];
		theSMDrawInfo.pMeshData->pVertices[vertIndex].boneWeights[2] = pTheAssimpSM->vecVertexBoneData[vertIndex].weights[2];
		theSMDrawInfo.pMeshData->pVertices[vertIndex].boneWeights[3] = pTheAssimpSM->vecVertexBoneData[vertIndex].weights[3];


	}//for ( unsigned int vertIndex = 0;

	// And the triangles

	// Allocate the array to hold the indices (triangle) info


	// Allocate the array for that (indices NOT triangles)
	theSMDrawInfo.pMeshData->pIndices = new unsigned int[theSMDrawInfo.numberOfIndices];

	// Danger Will Robinson! 
	// You don't really need to do this, but at least it will clear it to zero.
	// (compiler will complain that it's 'not safe', etc.)
	memset(theSMDrawInfo.pMeshData->pIndices, 0,
		sizeof(unsigned int) * theSMDrawInfo.numberOfIndices);


	unsigned int numTriangles = pTheAssimpSM->pScene->mMeshes[0]->mNumFaces;
	unsigned int triIndex = 0;		// Steps through the triangles.
	unsigned int indexIndex = 0;	// Setps through the indices (index buffer)
	for (; triIndex != theSMDrawInfo.numberOfTriangles;
		triIndex++, indexIndex += 3)	// Note, every 1 triangle = 3 index steps
	{
		// Get the triangle at this triangle index...
		aiFace* pAIFace = &(pTheAssimpSM->pScene->mMeshes[0]->mFaces[triIndex]);

		theSMDrawInfo.pMeshData->pIndices[indexIndex + 0]				// Offset by 0 (zero)
			= pAIFace->mIndices[0];	// vertex 0

		theSMDrawInfo.pMeshData->pIndices[indexIndex + 1]				// Offset by 1
			= pAIFace->mIndices[1];	// vertex 1

		theSMDrawInfo.pMeshData->pIndices[indexIndex + 2]				// Offset by 2
			= pAIFace->mIndices[2];	// vertex 1
	}//for ( ; triIndex != numVertices; 


	// Calculate the extents on the mesh
	// (Note, because I'm a bone head, this is dupicated...)
	theSMDrawInfo.pMeshData->name = theSMDrawInfo.friendlyName;
	theSMDrawInfo.pMeshData->numberOfIndices = theSMDrawInfo.numberOfIndices;
	theSMDrawInfo.pMeshData->numberOfTriangles = theSMDrawInfo.numberOfTriangles;
	theSMDrawInfo.pMeshData->numberOfVertices = theSMDrawInfo.numberOfVertices;
	theSMDrawInfo.pMeshData->CalculateExtents();

	// ...then pass it to the mesh manager

	::g_VAOMeshManager->LoadModelIntoVAO(theSMDrawInfo, shaderProgramID);
	return true;
}

void LoadPlayerMeshModel(const nLoad::sConfig& config, int c, std::vector<cEntity*> &vec_pObjectsToDraw,
	GLuint shaderProgramID)
{
	//player = new cEntity();
	::g_pRPGSkinnedMesh = new cSimpleAssimpSkinnedMesh();

	//if ( ! ::g_pRPGSkinnedMesh->LoadMeshFromFile( "assets/modelsMD5monsters/hellknight/attack2.md5anim" ) ) 
	//{
	//	std::cout << "Didn't load the army pilot" << std::endl;
	//}

	//if ( ! ::g_pRPGSkinnedMesh->LoadMeshFromFile( "assets/modelsFBX/ArmyPilot(FBX2013).fbx" ) ) 
	//{
	//	std::cout << "Didn't load the army pilot" << std::endl;
	//}
	//	if ( ! ::g_pSkinnedMesh01->LoadMeshFromFile( "assets/modelsFBX/RPG-Character(FBX2013).FBX" ) ) 
//	if ( ! ::g_pSkinnedMesh01->LoadMeshFromFile( "assets/modelsFBX/RPG-Character_Unarmed-Attack-Kick-L1(FBX2013).FBX" ) ) 
//	if ( ! ::g_pSkinnedMesh01->LoadMeshFromFile( "assets/modelsFBX/RPG-Character_Unarmed-Walk(FBX2013).FBX" ) ) 
//	if ( ! ::g_pSkinnedMesh01->LoadMeshFromFile( "assets/modelsFBX/RPG-Character_Unarmed-Idle(FBX2013).fbx" ) ) 
//	if ( ! ::g_pSkinnedMesh01->LoadMeshFromFile( "assets/modelsMD5/hellknight/attack2.md5anim" ) ) 
//	if ( ! ::g_pSkinnedMesh01->LoadMeshFromFile( "assets/modelsFBX/RPG-Character_Unarmed-Fall(FBX2013).fbx" ) ) 
//	if ( ! ::g_pRPGSkinnedMesh->LoadMeshFromFile( "RPG-Character", "assets/modelsFBX/RPG-Character(FBX2013).FBX" ) )
	//if ( ! ::g_pRPGSkinnedMesh->LoadMeshFromFile( "RPG-Character", "assets/modelsFBX/rockgolem(FBX2013).fbx" ) )
	if (!::g_pRPGSkinnedMesh->LoadMeshFromFile("RPG-Character", "assets/modelsFBX/kachujin(2013).fbx"))
	{
		std::cout << "Error: problem loading the skinned mesh" << std::endl;
	}
	std::vector<std::string> vecBoneNames;
	::g_pRPGSkinnedMesh->GetListOfBoneIDandNames(vecBoneNames);

	// Now load another animation file... 
	//::g_pRPGSkinnedMesh->LoadMeshAnimation( "Idle", "assets/modelsFBX/RPG-Character_Unarmed-Idle(FBX2013).fbx" );
	::g_pRPGSkinnedMesh->LoadMeshAnimation("Idle", "assets/modelsFBX/kachujin_Idle(2013).fbx");
	::g_pRPGSkinnedMesh->LoadMeshAnimation("Unarmed-Attack-Kick-L1", "assets/modelsFBX/RPG-Character_Unarmed-Attack-Kick-L1(FBX2013).FBX");
	::g_pRPGSkinnedMesh->LoadMeshAnimation("Jump", "assets/modelsFBX/kachujin_JumpNew(2013).fbx");
	::g_pRPGSkinnedMesh->LoadMeshAnimation("Unarmed-Walk", "assets/modelsFBX/kachujin_Walking(2013).FBX");
	::g_pRPGSkinnedMesh->LoadMeshAnimation("Unarmed-Run", "assets/modelsFBX/kachujin_Running(2013).FBX");
	::g_pRPGSkinnedMesh->LoadMeshAnimation("Attack-L3", "assets/modelsFBX/kachujin_Punching(2013).fbx");
	::g_pRPGSkinnedMesh->LoadMeshAnimation("Attack-R3", "assets/modelsFBX/RPG-Character_Unarmed-Attack-R3(FBX2013).FBX");
	::g_pRPGSkinnedMesh->LoadMeshAnimation("Strafe-R", "assets/modelsFBX/kachujin_Left_Strafe(2013).FBX");
	::g_pRPGSkinnedMesh->LoadMeshAnimation("Strafe-L", "assets/modelsFBX/kachujin_Right_Strafe(2013).FBX");
	::g_pRPGSkinnedMesh->LoadMeshAnimation("Roll", "assets/modelsFBX/kachujin_Roll(2013).FBX");
	//::g_pRPGSkinnedMesh->LoadMeshAnimation( "Unarmed-Fall", "assets/modelsFBX/RPG-Character_Unarmed-Fall(FBX2013).fbx" );
	//::g_pRPGSkinnedMesh->LoadMeshAnimation( "Roll-Backward", "assets/modelsFBX/RPG-Character_Unarmed-Roll-Backward(FBX2013).fbx" );
	//::g_pRPGSkinnedMesh->LoadMeshAnimation( "Roll-Forwards", "assets/modelsFBX/RPG-Character_Unarmed-Roll-Forward(FBX2013).FBX" );
	//4::g_pRPGSkinnedMesh->LoadMeshAnimation( "Roll-Left", "assets/modelsFBX/RPG-Character_Unarmed-Roll-Left(FBX2013).FBX" );
	//::g_pRPGSkinnedMesh->LoadMeshAnimation( "Roll-Right", "assets/modelsFBX/RPG-Character_Unarmed-Roll-Right(FBX2013).FBX" );
	//::g_pRPGSkinnedMesh->LoadMeshAnimation( "Unarmed-Jump", "assets/modelsFBX/RPG-Character_Unarmed-Jump(FBX2013).FBX" );

	::g_pRPGSkinnedMesh->friendlyName = "RPG-Character";

	cMesh* pTheMesh = ::g_pRPGSkinnedMesh->CreateMeshObjectFromCurrentModel();

	if (pTheMesh)
	{
		std::cout << "Mesh got loaded" << std::endl;
	}
	else
	{
		std::cout << "Didn't load the skinned mesh model" << std::endl;
	}

	// Copy the mesh information from assimp into our cMesh object, 
	// then into the sModelDrawInfo thing, and pass to the VAOManager

	if (!AssimpSM_to_VAO_Converter(::g_pRPGSkinnedMesh, shaderProgramID))
	{
		std::cout << "Error: Didn't copy the skinned mesh into the VAO format." << std::endl;
	}
	else
	{
		std::cout << "Copied the skinned mesh into the VAO format" << std::endl;

		// Add this mesh model into the "models to draw" vector

		{	// Bind pose Skinned Mesh object
			cEntity* pTestSM = new cEntity();
			pTestSM->m_EntityMesh->setDiffuseColour(glm::vec3(1.0f, 0.0f, 0.0f));	// Yellow
			pTestSM->m_EntityMesh->setAlphaTransparency(1.0f);
			pTestSM->friendlyName = "Player";
			//pTestSM->m_EntityPhysics->position = glm::vec3(5.0f, 0.0f, 5.0f);
			pTestSM->m_EntityPhysics->position = config.RigidBodyDefs[c].Position;
			float scale = (0.05f);
			pTestSM->m_EntityPhysics->nonUniformScale = glm::vec3(scale, scale, scale);
			pTestSM->m_EntityMesh->vecLODMeshs.push_back(sLODInfo("RPG-Character.ply"));
			pTestSM->m_EntityMesh->bIsVisible = true;
			pTestSM->m_EntityPhysics->bIsUpdatedByPhysics = true;

			// Wireframe, to make it easier to see (for now)
//			pTestSM->bIsWireFrame = true;
//			pTestSM->bDontLight = true;

			// Set this mesh to the skinned mesh object
			pTestSM->m_EntityMesh->pSimpleSkinnedMesh = ::g_pRPGSkinnedMesh;
			// HACK
			//pTestSM->currentAnimation = "assets/modelsFBX/RPG-Character_Unarmed-Idle(FBX2013).fbx";
//			pTestSM->currentAnimation = "Idle";

			cAnimationState* pAniState;
			pTestSM->m_EntityMesh->pAniState = new cAnimationState();

			pTestSM->m_EntityMesh->pAniState->defaultAnimation.name = "Idle";
			pTestSM->m_EntityMesh->currentAnimation = pTestSM->m_EntityMesh->pAniState->defaultAnimation.name;



			sTextureInfo testObjectTexture;
			testObjectTexture.name = "grass.bmp";
			testObjectTexture.strength = 1.0f;

			pTestSM->m_EntityMesh->vecTextures.push_back(sTextureInfo(testObjectTexture));

			pTestSM->m_EntityPhysics->physObjType = cEntityPhysics::ePhysicsObjType::RIGID_BODY;
			//glm::vec3 extents = glm::vec3(1.92339, 2.26989, 0.28641) * scale;
			//makeBox(pTestSM, config.RigidBodyDefs[c], extents);
			makeCapsule(pTestSM, config.RigidBodyDefs[c], 1.0f * 10.0f * scale, 2.26989f * 10.0f * scale);

			player = pTestSM;

			vec_pObjectsToDraw.push_back(pTestSM);			
		}
	}//if ( ! AssimpSM_to_VAO_Converter(


	return;
}

//Function for loading models using threads
void LoadModelsToVAO_ASYNC(cVAOMeshManager* pTheVAOMeshManager, GLuint shaderProgramID)
{
	// Load the cube map
	std::string errorString;
	cBasicTextureManager::sCubeMapTextureLoadParams cubeLoadParams;
	cubeLoadParams.cubeMapName = "CityCubeMap";
	cubeLoadParams.posX_fileName = "SpaceBox_right1_posX.bmp";
	cubeLoadParams.negX_fileName = "SpaceBox_left2_negX.bmp";
	cubeLoadParams.posY_fileName = "SpaceBox_top3_posY.bmp";
	cubeLoadParams.negY_fileName = "SpaceBox_bottom4_negY.bmp";
	cubeLoadParams.posZ_fileName = "SpaceBox_front5_posZ.bmp";
	cubeLoadParams.negZ_fileName = "SpaceBox_back6_negZ.bmp";
	cubeLoadParams.basePath = "assets/textures/cubemaps";
	cubeLoadParams.bIsSeamless = true;
	if (::g_TheTextureManager->CreateCubeTextureFromBMPFiles(cubeLoadParams, errorString))
	{
		//std::cout << "Loaded the space cube map OK" << std::endl;
	}
	else
	{
		std::cout << "Error: space cube map DIDN't load. On no!" << std::endl;
	}

	pTheVAOMeshManager->changeLoaderToPly5n();

	//pTheVAOMeshManager->SetBasePath("../assets/models");
	pTheVAOMeshManager->SetBasePath("assets/models");

	//sModelDrawInfo cityInfo;
	//cityInfo.meshFileName = "ScifiCityRuins.ply";	// "MeshLab_Fractal_Terrain_xyz.ply";
	//// Will alow me to update the vertex data in the mesh
	//cityInfo.bVertexBufferIsDynamic = true;
	//pTheVAOMeshManager->LoadModelIntoVAO_ASYNC(cityInfo, shaderProgramID);

	sModelDrawInfo sphereInfo;
	sphereInfo.meshFileName = "Sphere_320_faces_xyz_n_GARBAGE_uv.ply";			// "Sphere_320_faces_xyz.ply";
	pTheVAOMeshManager->LoadModelIntoVAO_ASYNC(sphereInfo, shaderProgramID);

	sModelDrawInfo sphereInvertedNormalsInfo;
	sphereInvertedNormalsInfo.meshFileName = "Sphere_320_faces_xyz_n_GARBAGE_uv_INVERTED_NORMALS.ply";			// "Sphere_320_faces_xyz.ply";
	pTheVAOMeshManager->LoadModelIntoVAO_ASYNC(sphereInvertedNormalsInfo, shaderProgramID);

	sModelDrawInfo planeInfo;
	planeInfo.meshFileName = "simpleplane0.ply";
	pTheVAOMeshManager->LoadModelIntoVAO_ASYNC(planeInfo, shaderProgramID);

	sModelDrawInfo plane1Info;
	plane1Info.meshFileName = "simpleplane1.ply";
	pTheVAOMeshManager->LoadModelIntoVAO_ASYNC(plane1Info, shaderProgramID);

	sModelDrawInfo plane2Info;
	plane2Info.meshFileName = "simpleplane2.ply";
	pTheVAOMeshManager->LoadModelIntoVAO_ASYNC(plane2Info, shaderProgramID);

	sModelDrawInfo plane3Info;
	plane3Info.meshFileName = "simpleplane3.ply";
	pTheVAOMeshManager->LoadModelIntoVAO_ASYNC(plane3Info, shaderProgramID);

	sModelDrawInfo plane4Info;
	plane4Info.meshFileName = "simpleplane4.ply";
	pTheVAOMeshManager->LoadModelIntoVAO_ASYNC(plane4Info, shaderProgramID);

	sModelDrawInfo cube1x1x1;
	cube1x1x1.meshFileName = "cube_flat_shaded_xyz_n_uv.ply";			// "cube_flat_shaded_xyz.ply";
	pTheVAOMeshManager->LoadModelIntoVAO_ASYNC(cube1x1x1, shaderProgramID);

	sModelDrawInfo cylinder;
	cylinder.meshFileName = "cylinderUV.ply";
	pTheVAOMeshManager->LoadModelIntoVAO_ASYNC(cylinder, shaderProgramID);

	sModelDrawInfo cone;
	cone.meshFileName = "coneUV.ply";
	pTheVAOMeshManager->LoadModelIntoVAO_ASYNC(cone, shaderProgramID);

	sModelDrawInfo katana;
	katana.meshFileName = "katana.ply";
	pTheVAOMeshManager->LoadModelIntoVAO_ASYNC(katana, shaderProgramID);

	//Quad for FBOs
	sModelDrawInfo p2SidedQuad;
	p2SidedQuad.meshFileName = "1x1_2Tri_Quad_2_Sided_xyz_n_uv.ply";
	pTheVAOMeshManager->LoadModelIntoVAO_ASYNC(p2SidedQuad, shaderProgramID);

	pTheVAOMeshManager->m_LoadModelFromFile_ASYNC();
}

void LoadTextures_ASYNC()
{
	// Async load
	::g_TheTextureManager->Create2DTextureFromBMPFile_ASYNC(cBasicTextureManager::s2DTextureLoadParams("Justin.bmp", "assets/textures"), true);
	::g_TheTextureManager->Create2DTextureFromBMPFile_ASYNC(cBasicTextureManager::s2DTextureLoadParams("grass.bmp", "assets/textures"), true);
	::g_TheTextureManager->Create2DTextureFromBMPFile_ASYNC(cBasicTextureManager::s2DTextureLoadParams("water-1514818_960_720.bmp", "assets/textures"), true);
	::g_TheTextureManager->Create2DTextureFromBMPFile_ASYNC(cBasicTextureManager::s2DTextureLoadParams("brick-wall.bmp", "assets/textures"), true);
	::g_TheTextureManager->Create2DTextureFromBMPFile_ASYNC(cBasicTextureManager::s2DTextureLoadParams("220px-Emma_Watson_2013.bmp", "assets/textures"), true);
	::g_TheTextureManager->Create2DTextureFromBMPFile_ASYNC(cBasicTextureManager::s2DTextureLoadParams("Flowers.bmp", "assets/textures"), true);
	::g_TheTextureManager->Create2DTextureFromBMPFile_ASYNC(cBasicTextureManager::s2DTextureLoadParams("Smoke_1.bmp", "assets/textures"), true);
	::g_TheTextureManager->Create2DTextureFromBMPFile_ASYNC(cBasicTextureManager::s2DTextureLoadParams("Plasma_Ring.bmp", "assets/textures"), true);
	::g_TheTextureManager->Create2DTextureFromBMPFile_ASYNC(cBasicTextureManager::s2DTextureLoadParams("rock.bmp", "assets/textures"), true);
}

void LoadModelTypes_ASYNC(cVAOMeshManager* pTheVAOMeshManager, GLuint shaderProgramID)
{
	LoadTextures_ASYNC();

	//LoadSkinnedMeshModelTypes(pTheVAOMeshManager, shaderProgramID);
	LoadModelsToVAO_ASYNC(pTheVAOMeshManager, shaderProgramID);
}

void LoadModelsIntoScene(std::vector<cEntity*> &vec_pObjectsToDraw)
{
	{	// This will be our "skybox" object.
	// (could also be a cube, or whatever)
		cEntity* pSkyBoxObject = new cEntity();
		pSkyBoxObject->m_EntityMesh->setDiffuseColour(glm::vec3(1.0f, 105.0f / 255.0f, 180.0f / 255.0f));
		pSkyBoxObject->m_EntityMesh->bUseVertexColour = false;
		pSkyBoxObject->friendlyName = "SkyBoxObject";
		pSkyBoxObject->m_EntityPhysics->setUniformScale(5000.0f);
		//		pSkyBoxObject->meshName = "Sphere_320_faces_xyz_n_GARBAGE_uv_INVERTED_NORMALS.ply";			// "Sphere_320_faces_xyz.ply";
		pSkyBoxObject->m_EntityMesh->vecLODMeshs.push_back(sLODInfo("Sphere_320_faces_xyz_n_GARBAGE_uv_INVERTED_NORMALS.ply"));
		//		pSkyBoxObject->bIsWireFrame = true;
		sTextureInfo skyboxTextureInfo;
		skyboxTextureInfo.name = "CityCubeMap";
		skyboxTextureInfo.strength = 1.0f;
		pSkyBoxObject->m_EntityMesh->vecTextures.push_back(skyboxTextureInfo);

		pSkyBoxObject->m_EntityPhysics->physObjType = cEntityPhysics::ePhysicsObjType::NONE;

		// Invisible until I need to draw it
		pSkyBoxObject->m_EntityMesh->bIsVisible = false;
		//pSkyBoxObject->m_EntityMesh->bIsVisible = true;

		vec_pObjectsToDraw.push_back(pSkyBoxObject);
	}

	{	// This sphere is the tiny little debug sphere
		cEntity* pDebugSphere = new cEntity();
		pDebugSphere->m_EntityPhysics->position = glm::vec3(0.0f, 0.0f, 10.0f);
		//		pDebugSphere->objColour = glm::vec3( 0.0f, 1.0f, 0.0f );
		pDebugSphere->m_EntityMesh->setDiffuseColour(glm::vec3(0.0f, 0.0f, 0.0f));
		pDebugSphere->m_EntityMesh->setSpecularPower(100.0f);
		pDebugSphere->m_EntityMesh->setSpecularColour(glm::vec3(1.000f, 0.766f, 0.336f));


		pDebugSphere->friendlyName = "DebugSphere";
		pDebugSphere->m_EntityPhysics->setUniformScale(3.0f);
		//		pDebugSphere->meshName = "Sphere_320_faces_xyz_n_GARBAGE_uv.ply";		// "Sphere_320_faces_xyz.ply";
		pDebugSphere->m_EntityMesh->vecLODMeshs.push_back(sLODInfo("Sphere_320_faces_xyz_n_GARBAGE_uv.ply"));
		pDebugSphere->m_EntityMesh->bIsWireFrame = false;
		pDebugSphere->m_EntityMesh->bDontLight = false;
		pDebugSphere->m_EntityMesh->bIsVisible = true;
		pDebugSphere->m_EntityMesh->bUseVertexColour = false;

		sTextureInfo stone;
		stone.name = "rock.bmp";
		stone.strength = 1.0f;
		pDebugSphere->m_EntityMesh->vecTextures.push_back(stone);


		//pTerrain->nonUniformScale = glm::vec3(0.1f,0.1f,0.1f);
		//vec_pObjectsToDraw.push_back(pDebugSphere);
	}

	{	// This sphere is the tiny little debug sphere
		cEntity* p2SidedQuad = new cEntity();
		p2SidedQuad->m_EntityPhysics->position = glm::vec3(0.0f, 0.0f, 0.0f);
		p2SidedQuad->m_EntityMesh->setDiffuseColour(glm::vec3(0.0f, 0.0f, 0.0f));
		p2SidedQuad->m_EntityMesh->setSpecularPower(100.0f);
		p2SidedQuad->m_EntityMesh->setSpecularColour(glm::vec3(1.000f, 0.766f, 0.336f));

		p2SidedQuad->friendlyName = "2SidedQuad";
		p2SidedQuad->m_EntityPhysics->setUniformScale(10.0f);
		p2SidedQuad->m_EntityMesh->vecLODMeshs.push_back(sLODInfo("1x1_2Tri_Quad_2_Sided_xyz_n_uv.ply"));
		p2SidedQuad->m_EntityMesh->bIsWireFrame = false;
		//pDebugSphere->m_EntityMesh->bDontLight = false;
		p2SidedQuad->m_EntityMesh->bIsVisible = false;
		p2SidedQuad->m_EntityMesh->bUseVertexColour = false;

		vec_pObjectsToDraw.push_back(p2SidedQuad);
	}

	{	// This sphere is the tiny little debug sphere
		cEntity* pKatana = new cEntity();
		pKatana->m_EntityPhysics->position = glm::vec3(0.0f, 10.0f, 0.0f);
		pKatana->m_EntityMesh->setDiffuseColour(glm::vec3(0.0f, 1.0f, 1.0f));
		pKatana->m_EntityMesh->setSpecularPower(100.0f);
		pKatana->m_EntityMesh->setSpecularColour(glm::vec3(1.000f, 0.766f, 0.336f));

		pKatana->friendlyName = "Katana";
		float scale = 0.03f;
		pKatana->m_EntityPhysics->setUniformScale(scale);
		pKatana->m_EntityMesh->vecLODMeshs.push_back(sLODInfo("katana.ply"));
		pKatana->m_EntityMesh->bIsWireFrame = false;
		pKatana->m_EntityMesh->bIsVisible = true;
		pKatana->m_EntityMesh->bUseVertexColour = false;
		pKatana->m_EntityPhysics->bIsUpdatedByPhysics = true;

		sTextureInfo stone;
		stone.name = "rock.bmp";
		stone.strength = 1.0f;
		pKatana->m_EntityMesh->vecTextures.push_back(stone);

		pKatana->m_EntityPhysics->physObjType = cEntityPhysics::ePhysicsObjType::RIGID_BODY;

		nPhysics::sRigidBodyDef katanaDef;
		katanaDef.Mass = 5.0f;
		katanaDef.Position = pKatana->m_EntityPhysics->position;
		katanaDef.Orientation = glm::vec3(0.0f, 0.0f, 0.0f);

		makeCapsule(pKatana, katanaDef, 1.0f * scale, 4.0f * scale);
		makePointPointConstraint(pKatana, player, glm::vec3(0.0f), glm::vec3(0.0f));

		vec_pObjectsToDraw.push_back(pKatana);
	}
}