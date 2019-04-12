#include "../Global/gOpenGL.h"
#include "../Global/global.h"

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

void LoadSkinnedMeshModelTypes(cVAOMeshManager* pTheVAOMeshManager, GLuint shaderProgramID)
{
	// RPG character

	cSimpleAssimpSkinnedMesh* pRPG = new cSimpleAssimpSkinnedMesh();

	if (!pRPG->LoadMeshFromFile("RPG-Character", "assets/modelsFBX/RPG-Character(FBX2013).FBX"))
	{
		std::cout << "Error: problem loading the skinned mesh" << std::endl;
	}

	// Now load another animation file... 
	pRPG->LoadMeshAnimation("Idle", "assets/modelsFBX/RPG-Character_Unarmed-Idle(FBX2013).fbx");
	pRPG->LoadMeshAnimation("Unarmed-Attack-Kick-L1", "assets/modelsFBX/RPG-Character_Unarmed-Attack-Kick-L1(FBX2013).FBX");
	pRPG->LoadMeshAnimation("Unarmed-Walk", "assets/modelsFBX/RPG-Character_Unarmed-Walk(FBX2013).FBX");
	pRPG->LoadMeshAnimation("Unarmed-Fall", "assets/modelsFBX/RPG-Character_Unarmed-Fall(FBX2013).fbx");
	pRPG->LoadMeshAnimation("Roll-Backward", "assets/modelsFBX/RPG-Character_Unarmed-Roll-Backward(FBX2013).fbx");
	pRPG->LoadMeshAnimation("Roll-Forwards", "assets/modelsFBX/RPG-Character_Unarmed-Roll-Forward(FBX2013).FBX");
	pRPG->LoadMeshAnimation("Roll-Left", "assets/modelsFBX/RPG-Character_Unarmed-Roll-Left(FBX2013).FBX");
	pRPG->LoadMeshAnimation("Roll-Right", "assets/modelsFBX/RPG-Character_Unarmed-Roll-Right(FBX2013).FBX");
	pRPG->LoadMeshAnimation("Unarmed-Jump", "assets/modelsFBX/RPG-Character_Unarmed-Jump(FBX2013).FBX");
	pRPG->LoadMeshAnimation("Unarmed-Attack-L3", "assets/modelsFBX/RPG-Character_Unarmed-Attack-L3(FBX2013).FBX");
	pRPG->LoadMeshAnimation("Unarmed-Attack-R3", "assets/modelsFBX/RPG-Character_Unarmed-Attack-R3(FBX2013).FBX");

	pRPG->friendlyName = "RPG-Character";

	if (!pTheVAOMeshManager->LoadSkinnedMeshModelIntoVAO(pRPG, shaderProgramID))
	{
		std::cout << "ERROR: Failed to load RPG Skinned Mesh character" << std::endl;
	}
	else
	{
		std::cout << "RPG SM character + animations are loaded OK" << std::endl;
	}

	std::vector<std::string> vecBoneList;
	pRPG->GetListOfBoneIDandNames(vecBoneList);
	for (int index = 0; index != vecBoneList.size(); index++)
	{
		//std::cout << index << " : " << vecBoneList[index] << std::endl;
	}

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

	pTheVAOMeshManager->SetBasePath("../assets/models");

	sModelDrawInfo cityInfo;
	cityInfo.meshFileName = "ScifiCityRuins.ply";	// "MeshLab_Fractal_Terrain_xyz.ply";
	// Will alow me to update the vertex data in the mesh
	cityInfo.bVertexBufferIsDynamic = true;
	pTheVAOMeshManager->LoadModelIntoVAO_ASYNC(cityInfo, shaderProgramID);

	sModelDrawInfo sphereInfo;
	sphereInfo.meshFileName = "Sphere_320_faces_xyz_n_GARBAGE_uv.ply";			// "Sphere_320_faces_xyz.ply";
	pTheVAOMeshManager->LoadModelIntoVAO_ASYNC(sphereInfo, shaderProgramID);

	sModelDrawInfo sphereInvertedNormalsInfo;
	sphereInvertedNormalsInfo.meshFileName = "Sphere_320_faces_xyz_n_GARBAGE_uv_INVERTED_NORMALS.ply";			// "Sphere_320_faces_xyz.ply";
	pTheVAOMeshManager->LoadModelIntoVAO_ASYNC(sphereInvertedNormalsInfo, shaderProgramID);

	pTheVAOMeshManager->m_LoadModelFromFile_ASYNC();
}

void LoadTextures_ASYNC()
{
	// Async load
	::g_TheTextureManager->Create2DTextureFromBMPFile_ASYNC(cBasicTextureManager::s2DTextureLoadParams("Justin.bmp", "../assets/textures"), true);
	::g_TheTextureManager->Create2DTextureFromBMPFile_ASYNC(cBasicTextureManager::s2DTextureLoadParams("grass.bmp", "../assets/textures"), true);
	::g_TheTextureManager->Create2DTextureFromBMPFile_ASYNC(cBasicTextureManager::s2DTextureLoadParams("water-1514818_960_720.bmp", "assets/textures"), true);
	::g_TheTextureManager->Create2DTextureFromBMPFile_ASYNC(cBasicTextureManager::s2DTextureLoadParams("brick-wall.bmp", "../assets/textures"), true);
	::g_TheTextureManager->Create2DTextureFromBMPFile_ASYNC(cBasicTextureManager::s2DTextureLoadParams("220px-Emma_Watson_2013.bmp", "../assets/textures"), true);
	::g_TheTextureManager->Create2DTextureFromBMPFile_ASYNC(cBasicTextureManager::s2DTextureLoadParams("Flowers.bmp", "../assets/textures"), true);
	::g_TheTextureManager->Create2DTextureFromBMPFile_ASYNC(cBasicTextureManager::s2DTextureLoadParams("Smoke_1.bmp", "../assets/textures"), true);
	::g_TheTextureManager->Create2DTextureFromBMPFile_ASYNC(cBasicTextureManager::s2DTextureLoadParams("Plasma_Ring.bmp", "../assets/textures"), true);
	::g_TheTextureManager->Create2DTextureFromBMPFile_ASYNC(cBasicTextureManager::s2DTextureLoadParams("rock_cave_stylized_height.bmp", "../assets/textures"), true);
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
		pSkyBoxObject->m_EntityPhysics->uniformScale = 5000.0f;
		//		pSkyBoxObject->meshName = "Sphere_320_faces_xyz_n_GARBAGE_uv_INVERTED_NORMALS.ply";			// "Sphere_320_faces_xyz.ply";
		pSkyBoxObject->m_EntityMesh->vecLODMeshs.push_back(sLODInfo("Sphere_320_faces_xyz_n_GARBAGE_uv_INVERTED_NORMALS.ply"));
		//		pSkyBoxObject->bIsWireFrame = true;
		sTextureInfo skyboxTextureInfo;
		skyboxTextureInfo.name = "CityCubeMap";
		skyboxTextureInfo.strength = 1.0f;
		pSkyBoxObject->m_EntityMesh->vecTextures.push_back(skyboxTextureInfo);

		// Invisible until I need to draw it
		pSkyBoxObject->m_EntityMesh->bIsVisible = false;
		//pSkyBoxObject->m_EntityMesh->bIsVisible = true;

		vec_pObjectsToDraw.push_back(pSkyBoxObject);
	}

	{	// This sphere is the tiny little debug sphere
		cEntity* pDebugSphere = new cEntity();
		pDebugSphere->m_EntityPhysics->position = glm::vec3(0.0f, 0.0f, 10.0f);
		//		pDebugSphere->objColour = glm::vec3( 0.0f, 1.0f, 0.0f );
		pDebugSphere->m_EntityMesh->setDiffuseColour(glm::vec3(0.0f, 1.0f, 0.0f));
		pDebugSphere->m_EntityMesh->setSpecularPower(100.0f);
		pDebugSphere->m_EntityMesh->setSpecularColour(glm::vec3(1.000f, 0.766f, 0.336f));


		pDebugSphere->friendlyName = "DebugSphere";
		pDebugSphere->m_EntityPhysics->uniformScale = 3.0f;
		//		pDebugSphere->meshName = "Sphere_320_faces_xyz_n_GARBAGE_uv.ply";		// "Sphere_320_faces_xyz.ply";
		pDebugSphere->m_EntityMesh->vecLODMeshs.push_back(sLODInfo("Sphere_320_faces_xyz_n_GARBAGE_uv.ply"));
		pDebugSphere->m_EntityMesh->bIsWireFrame = true;
		pDebugSphere->m_EntityMesh->bDontLight = true;
		pDebugSphere->m_EntityMesh->bIsVisible = true;
		pDebugSphere->m_EntityMesh->bUseVertexColour = false;


		//pTerrain->nonUniformScale = glm::vec3(0.1f,0.1f,0.1f);
		vec_pObjectsToDraw.push_back(pDebugSphere);
	}
}