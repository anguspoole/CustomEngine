#include "../Global/gOpenGL.h"
#include "../Global/global.h"

#include "../Meshes/AssimpFBX/cAnimationState.h"
#include <eCollisionGroup.h>

extern cSimpleAssimpSkinnedMesh* g_pRPGSkinnedMesh = NULL;

std::map<std::string, float> mapAnimToDuration;
std::map<std::string, float> mapAnimToSpeed;

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
				= pTheAssimpSM->pScene->mMeshes[0]->mTextureCoords[1][vertIndex].x;
			theSMDrawInfo.pMeshData->pVertices[vertIndex].v0
				= pTheAssimpSM->pScene->mMeshes[0]->mTextureCoords[1][vertIndex].y;
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

	if (!::g_pRPGSkinnedMesh->LoadMeshFromFile("RPG-Character", "assets/modelsFBX/kachujin(2013).fbx"))
	{
		std::cout << "Error: problem loading the skinned mesh" << std::endl;
	}
	std::vector<std::string> vecBoneNames;
	::g_pRPGSkinnedMesh->GetListOfBoneIDandNames(vecBoneNames);

	// Now load another animation file... 
	//::g_pRPGSkinnedMesh->LoadMeshAnimation( "Idle", "assets/modelsFBX/RPG-Character_Unarmed-Idle(FBX2013).fbx" );
	::g_pRPGSkinnedMesh->LoadMeshAnimation("Idle", "assets/modelsFBX/kachujin_Idle(2013).fbx");
	::g_pRPGSkinnedMesh->LoadMeshAnimation("TwoHand-Attack", "assets/modelsFBX/kachujin_Heavy_Weapon_Swing(2013).fbx");
	::g_pRPGSkinnedMesh->LoadMeshAnimation("Jump", "assets/modelsFBX/kachujin_JumpNew(2013).fbx");
	::g_pRPGSkinnedMesh->LoadMeshAnimation("Walk", "assets/modelsFBX/kachujin_Walking(2013).FBX");
	::g_pRPGSkinnedMesh->LoadMeshAnimation("Run", "assets/modelsFBX/kachujin_Running(2013).FBX");
	::g_pRPGSkinnedMesh->LoadMeshAnimation("Stab-Attack", "assets/modelsFBX/kachujin_StabbingM(2013).fbx");
	::g_pRPGSkinnedMesh->LoadMeshAnimation("Hit", "assets/modelsFBX/kachujin_Hit(2013).fbx");
	::g_pRPGSkinnedMesh->LoadMeshAnimation("Death", "assets/modelsFBX/kachujin_Death(2013).fbx");
	::g_pRPGSkinnedMesh->LoadMeshAnimation("Backflip", "assets/modelsFBX/kachujin_Backflip(2013).fbx");
	::g_pRPGSkinnedMesh->LoadMeshAnimation("Frontflip", "assets/modelsFBX/kachujin_Running_Forward_Flip(2013).fbx");
	::g_pRPGSkinnedMesh->LoadMeshAnimation("Strafe-R", "assets/modelsFBX/kachujin_Left_Strafe(2013).FBX");
	::g_pRPGSkinnedMesh->LoadMeshAnimation("Strafe-L", "assets/modelsFBX/kachujin_Right_Strafe(2013).FBX");
	::g_pRPGSkinnedMesh->LoadMeshAnimation("Roll", "assets/modelsFBX/kachujin_Roll(2013).FBX");

	::g_pRPGSkinnedMesh->friendlyName = "RPG-Character";

	mapAnimToDuration.insert(std::pair<std::string, float>("Idle", 2.0f));
	mapAnimToDuration.insert(std::pair<std::string, float>("Run", 0.3f));
	mapAnimToDuration.insert(std::pair<std::string, float>("TwoHand-Attack", 1.0f));
	mapAnimToDuration.insert(std::pair<std::string, float>("Stab-Attack", 2.0f));
	mapAnimToDuration.insert(std::pair<std::string, float>("Backflip", 1.0f));
	mapAnimToDuration.insert(std::pair<std::string, float>("Frontflip", 1.0f));

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
			pTestSM->m_EntityMesh->setDiffuseColour(glm::vec3(0.0f, 0.0f, 0.0f));	// Yellow
			pTestSM->m_EntityMesh->setAlphaTransparency(1.0f);
			pTestSM->m_EntityMesh->setSpecularPower(50.0f);
			pTestSM->m_EntityMesh->setSpecularColour(glm::vec3(0.2f, 0.2f, 0.2f));
			pTestSM->m_EntityMesh->bUseVertexColour = false;
			pTestSM->friendlyName = "Player";
			pTestSM->health = 50.0f;
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
			testObjectTexture.name = "kachujin.bmp";
			testObjectTexture.strength = 1.0f;
			pTestSM->m_EntityMesh->vecTextures.push_back(sTextureInfo(testObjectTexture));

			pTestSM->m_EntityPhysics->physObjType = cEntityPhysics::ePhysicsObjType::RIGID_BODY;
			//glm::vec3 extents = glm::vec3(1.92339, 2.26989, 0.28641) * scale;
			//makeBox(pTestSM, config.RigidBodyDefs[c], extents);
			//makeCapsule(pTestSM, config.RigidBodyDefs[c], 1.0f * 10.0f * scale, 2.26989f * 10.0f * scale);

			int colGroup = eCollisionGroup::COL_PLAYER;
			int colMask = (eCollisionGroup::COL_ENVIRONMENT | eCollisionGroup::COL_ENEMY | eCollisionGroup::COL_ENEMYWEAPON);

			makeCapsule(pTestSM, config.RigidBodyDefs[c], scale * config.RigidBodyDefs[c].Mass/2, scale * config.RigidBodyDefs[c].Mass * 2.0f, colGroup, colMask);
			//makeCapsule(pTestSM, config.RigidBodyDefs[c], 5.0f * scale, 10.0f * scale, colGroup, colMask);

			pTestSM->m_EntityPhysics->rigidBody->GetPosition(pTestSM->m_EntityPhysics->position);

			pTestSM->m_EntityPhysics->rigidBody->SetEntityType(eEntityType::PLAYER);
			pTestSM->m_EntityPhysics->rigidBody->SetName(pTestSM->friendlyName);

			player = pTestSM;

			vec_pObjectsToDraw.push_back(pTestSM);			
		}
	}//if ( ! AssimpSM_to_VAO_Converter(
	return;
}

cSimpleAssimpSkinnedMesh* LoadEnemyMeshModel(const nLoad::sConfig& config, int c, std::vector<cEntity*> &vec_pObjectsToDraw,
	GLuint shaderProgramID)
{
	//player = new cEntity();
	cSimpleAssimpSkinnedMesh* enemySkinnedMesh = new cSimpleAssimpSkinnedMesh();

	if (!enemySkinnedMesh->LoadMeshFromFile("RPG-Character2", "assets/modelsFBX/heraklios(2013).fbx"))
	//if (!enemySkinnedMesh->LoadMeshFromFile("RPG-Character2", "assets/modelsFBX/heraklios2(2013).fbx"))
	//if (!enemySkinnedMesh->LoadMeshFromFile("RPG-Character2", "assets/modelsFBX/kachujin(2013).fbx"))
	{
		std::cout << "Error: problem loading the skinned mesh" << std::endl;
	}
	std::vector<std::string> vecBoneNames;
	enemySkinnedMesh->GetListOfBoneIDandNames(vecBoneNames);

	// Now load another animation file... 
	//::enemySkinnedMesh->LoadMeshAnimation( "Idle", "assets/modelsFBX/RPG-Character_Unarmed-Idle(FBX2013).fbx" );
	enemySkinnedMesh->LoadMeshAnimation("EnemyIdle", "assets/modelsFBX/heraklios_Idle(2013).fbx");
	enemySkinnedMesh->LoadMeshAnimation("EnemyRun", "assets/modelsFBX/heraklios_Running(2013).FBX");
	enemySkinnedMesh->LoadMeshAnimation("EnemyAttack", "assets/modelsFBX/heraklios_Horizontal_Attack(2013).fbx");
	enemySkinnedMesh->LoadMeshAnimation("EnemyHit", "assets/modelsFBX/heraklios_Hit_Reaction(2013).fbx");
	enemySkinnedMesh->LoadMeshAnimation("EnemyDeath", "assets/modelsFBX/heraklios_Standing_Death(2013).fbx");

	enemySkinnedMesh->friendlyName = "Enemy" + std::to_string(enemyList.size());

	mapAnimToDuration.insert(std::pair<std::string, float>("EnemyIdle", 2.0f));
	mapAnimToDuration.insert(std::pair<std::string, float>("EnemyRun", 0.5f));
	mapAnimToDuration.insert(std::pair<std::string, float>("EnemyAttack", 2.0f));
	mapAnimToDuration.insert(std::pair<std::string, float>("EnemyHit", 2.0f));
	mapAnimToDuration.insert(std::pair<std::string, float>("EnemyDeath", 1.6f));

	cMesh* pTheMesh = enemySkinnedMesh->CreateMeshObjectFromCurrentModel();

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
	return enemySkinnedMesh;
}

void CreateAndAssignAnimatedEnemy(const nLoad::sConfig& config, int c, std::vector<cEntity*> &vec_pObjectsToDraw,
	GLuint shaderProgramID, cSimpleAssimpSkinnedMesh* enemySkinnedMesh)
{
	if (!AssimpSM_to_VAO_Converter(enemySkinnedMesh, shaderProgramID))
	{
		std::cout << "Error: Didn't copy the skinned mesh into the VAO format." << std::endl;
	}
	else
	{
		std::cout << "Copied the skinned mesh into the VAO format" << std::endl;

		// Add this mesh model into the "models to draw" vector

		{	// Bind pose Skinned Mesh object
			cEntity* pTestSM = new cEntity();
			pTestSM->m_EntityMesh->setDiffuseColour(glm::vec3(0.0f, 0.0f, 0.0f));	// Yellow
			pTestSM->m_EntityMesh->setAlphaTransparency(1.0f);
			pTestSM->m_EntityMesh->setSpecularPower(50.0f);
			pTestSM->m_EntityMesh->setSpecularColour(glm::vec3(0.0f, 0.0f, 0.0f));
			pTestSM->friendlyName = "Enemy" + std::to_string(enemyList.size());
			//pTestSM->m_EntityPhysics->position = glm::vec3(5.0f, 0.0f, 5.0f);
			pTestSM->m_EntityPhysics->position = config.RigidBodyDefs[c].Position;
			float scale = (0.05f);
			pTestSM->m_EntityPhysics->nonUniformScale = glm::vec3(scale, scale, scale);
			pTestSM->m_EntityMesh->vecLODMeshs.push_back(sLODInfo("RPG-Character.ply"));
			pTestSM->m_EntityMesh->bIsVisible = true;
			pTestSM->m_EntityPhysics->bIsUpdatedByPhysics = true;
			pTestSM->m_EntityMesh->bUseVertexColour = false;

			// Wireframe, to make it easier to see (for now)
//			pTestSM->bIsWireFrame = true;
//			pTestSM->bDontLight = true;

			// Set this mesh to the skinned mesh object
			pTestSM->m_EntityMesh->pSimpleSkinnedMesh = enemySkinnedMesh;
			// HACK
			//pTestSM->currentAnimation = "assets/modelsFBX/RPG-Character_Unarmed-Idle(FBX2013).fbx";
//			pTestSM->currentAnimation = "Idle";

			cAnimationState* pAniState;
			pTestSM->m_EntityMesh->pAniState = new cAnimationState();

			pTestSM->m_EntityMesh->pAniState->defaultAnimation.name = "EnemyIdle";
			pTestSM->m_EntityMesh->currentAnimation = pTestSM->m_EntityMesh->pAniState->defaultAnimation.name;



			sTextureInfo testObjectTexture;
			testObjectTexture.name = "heraklios2.bmp";
			//testObjectTexture.name = "kachujin.bmp";
			testObjectTexture.strength = 1.0f;
			pTestSM->m_EntityMesh->vecTextures.push_back(sTextureInfo(testObjectTexture));

			//sTextureInfo secondTexture;
			//secondTexture.name = "metal_halomap.bmp";
			//secondTexture.strength = 0.5f;
			//pTestSM->m_EntityMesh->vecTextures.push_back(sTextureInfo(secondTexture));


			pTestSM->m_EntityPhysics->physObjType = cEntityPhysics::ePhysicsObjType::RIGID_BODY;
			//glm::vec3 extents = glm::vec3(1.92339, 2.26989, 0.28641) * scale;
			//makeBox(pTestSM, config.RigidBodyDefs[c], extents);

			int colGroup = eCollisionGroup::COL_ENEMY;
			int colMask = (eCollisionGroup::COL_ENVIRONMENT | eCollisionGroup::COL_PLAYER | eCollisionGroup::COL_PLAYERWEAPON);

			//makeCapsule(pTestSM, config.RigidBodyDefs[c], 20.0f * scale, 2.0f * 30.0f * scale, colGroup, colMask);
			makeCapsule(pTestSM, config.RigidBodyDefs[c], scale * config.RigidBodyDefs[c].Mass / 2, scale * config.RigidBodyDefs[c].Mass * 2.0f, colGroup, colMask);

			pTestSM->m_EntityPhysics->rigidBody->SetEntityType(eEntityType::ENEMY);
			pTestSM->m_EntityPhysics->rigidBody->SetName(pTestSM->friendlyName);

			vec_pObjectsToDraw.push_back(pTestSM);
			enemyList.push_back(pTestSM);
		
			// This sphere is the tiny little debug sphere
			cEntity* pKatana = new cEntity();
			pKatana->m_EntityMesh->setDiffuseColour(glm::vec3(0.0f, 0.0f, 1.0f));
			pKatana->m_EntityMesh->setSpecularPower(0.1f);
			pKatana->m_EntityMesh->setSpecularColour(glm::vec3(0.766f, 0.336f, 1.000f));

			pKatana->friendlyName = "EnemyKatana" + std::to_string(enemyList.size() - 1);
			float weaponScale = 0.03f;
			pKatana->m_EntityPhysics->position = glm::vec3(0.0f, 0.0f, 0.0f);
			pKatana->m_EntityPhysics->offset = glm::vec3(0.0f, 0.0f, 50.0f);
			pKatana->m_EntityPhysics->setUniformScale(weaponScale);
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

			sModelDrawInfo modelInfo;
			modelInfo.meshFileName = "katana.ply";
			g_VAOMeshManager->FindDrawInfoByModelName(modelInfo);

			nPhysics::sModelPoint* modelPoints = new nPhysics::sModelPoint[modelInfo.numberOfVertices]();
			for (size_t i = 0; i < modelInfo.numberOfVertices; i++)
			{
				modelPoints[i].vert = glm::vec4(modelInfo.pMeshData->pVertices->x, modelInfo.pMeshData->pVertices->y,
					modelInfo.pMeshData->pVertices->z, modelInfo.pMeshData->pVertices->w);
			}

			nPhysics::sRigidBodyDef katanaDef;
			katanaDef.Mass = 5.0f;
			katanaDef.Mass = 0.0f; //this will keep the object static while equipped
			katanaDef.Position = glm::vec3(0.0f, 0.0f, 0.0f);
			//katanaDef.Position = glm::vec3(0.0f, 0.0f, 0.0f);
			katanaDef.Orientation = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 extents = glm::vec3(1.0f, 1.0f, 130.0f * weaponScale);
			//glm::vec3 extents = glm::vec3(1.0f, 1.0f, 7.0f);

			//makeCapsule(pKatana, katanaDef, 1.0f * weaponScale, 4.0f * weaponScale);
			colGroup = eCollisionGroup::COL_ENEMYWEAPON;
			colMask = (eCollisionGroup::COL_PLAYER);

			makeCylinder(pKatana, katanaDef, extents, colGroup, colMask);

			//makeConvexHull(pKatana, katanaDef, modelPoints, modelInfo.numberOfVertices);
			//makePointPointConstraint(pKatana, player, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
			//makeFixedConstraint(pKatana, player);

			pKatana->m_EntityPhysics->rigidBody->SetEntityType(eEntityType::ENEMYWEAPON);
			pKatana->m_EntityPhysics->rigidBody->SetName(pKatana->friendlyName);

			pTestSM->vec_pChildrenEntities.push_back(pKatana);
			pTestSM->Equip(35, pKatana);

			gPhysicsWorld->DisableCollision(pTestSM->m_EntityPhysics->rigidBody, pKatana->m_EntityPhysics->rigidBody);

			for (int i = 0; i < enemyList.size(); i++)
			{
				gPhysicsWorld->DisableCollision(enemyList[i]->vec_pChildrenEntities[0]->m_EntityPhysics->rigidBody, pKatana->m_EntityPhysics->rigidBody);
			}

			//removeRB(pKatana);

			//vec_pObjectsToDraw.push_back(pKatana);
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
	//cubeLoadParams.posX_fileName = "SpaceBox_right1_posX.bmp";
	cubeLoadParams.posX_fileName = "TropicalSunnyDayRight2048.bmp";
	//cubeLoadParams.negX_fileName = "SpaceBox_left2_negX.bmp";
	cubeLoadParams.negX_fileName = "TropicalSunnyDayLeft2048.bmp";
	//cubeLoadParams.posY_fileName = "SpaceBox_top3_posY.bmp";
	cubeLoadParams.posY_fileName = "TropicalSunnyDayUp2048.bmp";
	//cubeLoadParams.negY_fileName = "SpaceBox_bottom4_negY.bmp";
	cubeLoadParams.negY_fileName = "TropicalSunnyDayDown2048.bmp";
	//cubeLoadParams.posZ_fileName = "SpaceBox_front5_posZ.bmp";
	cubeLoadParams.posZ_fileName = "TropicalSunnyDayFront2048.bmp";
	//cubeLoadParams.negZ_fileName = "SpaceBox_back6_negZ.bmp";
	cubeLoadParams.negZ_fileName = "TropicalSunnyDayBack2048.bmp";
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

	sModelDrawInfo twoSidedQuad;
	twoSidedQuad.meshFileName = "1x1_2Tri_Quad_2_Sided_xyz_n_uv.ply";			// "cube_flat_shaded_xyz.ply";
	pTheVAOMeshManager->LoadModelIntoVAO_ASYNC(twoSidedQuad, shaderProgramID);

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
	sModelDrawInfo starParticle;
	starParticle.meshFileName = "1x1_Star_1_Quad_2_Sided_xyz_n_uv.ply";
	pTheVAOMeshManager->LoadModelIntoVAO_ASYNC(starParticle, shaderProgramID);

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
	::g_TheTextureManager->Create2DTextureFromBMPFile_ASYNC(cBasicTextureManager::s2DTextureLoadParams("kachujin.bmp", "assets/textures"), true);
	::g_TheTextureManager->Create2DTextureFromBMPFile_ASYNC(cBasicTextureManager::s2DTextureLoadParams("metal_halomap.bmp", "assets/textures"), true);
	::g_TheTextureManager->Create2DTextureFromBMPFile_ASYNC(cBasicTextureManager::s2DTextureLoadParams("metallplates.bmp", "assets/textures"), true);
	::g_TheTextureManager->Create2DTextureFromBMPFile_ASYNC(cBasicTextureManager::s2DTextureLoadParams("metalplateColor.bmp", "assets/textures"), true);
	::g_TheTextureManager->Create2DTextureFromBMPFile_ASYNC(cBasicTextureManager::s2DTextureLoadParams("metalplateNormal.bmp", "assets/textures"), true);
	::g_TheTextureManager->Create2DTextureFromBMPFile_ASYNC(cBasicTextureManager::s2DTextureLoadParams("heraklios2.bmp", "assets/textures"), true);
	::g_TheTextureManager->Create2DTextureFromBMPFile_ASYNC(cBasicTextureManager::s2DTextureLoadParams("splatter1.bmp", "assets/textures"), true);
	::g_TheTextureManager->Create2DTextureFromBMPFile_ASYNC(cBasicTextureManager::s2DTextureLoadParams("splatter2.bmp", "assets/textures"), true);
	::g_TheTextureManager->Create2DTextureFromBMPFile_ASYNC(cBasicTextureManager::s2DTextureLoadParams("flames.bmp", "assets/textures"), true);
}

void LoadPaintGlob(std::vector<cEntity*> &vec_pObjectsToDraw, GLuint shaderProgramID, glm::vec3 startPos, glm::vec3 startVel)
{
	{	// This sphere is the tiny little debug sphere
		cEntity* pGlob = new cEntity();
		pGlob->m_EntityMesh->setDiffuseColour(glm::vec3(0.0f, 1.0f, 1.0f));
		pGlob->m_EntityMesh->setSpecularPower(50.0f);
		pGlob->m_EntityMesh->setSpecularColour(glm::vec3(1.000f, 0.766f, 0.336f));

		pGlob->friendlyName = "Glob" + std::to_string(globList.size());
		float scale = 1.0f;
		pGlob->m_EntityPhysics->position = startPos;
		pGlob->m_EntityPhysics->setUniformScale(scale);
		pGlob->m_EntityMesh->vecLODMeshs.push_back(sLODInfo("Sphere_320_faces_xyz_n_GARBAGE_uv.ply"));
		pGlob->m_EntityMesh->bIsWireFrame = false;
		pGlob->m_EntityMesh->bIsVisible = true;
		pGlob->m_EntityMesh->bUseVertexColour = false;
		pGlob->m_EntityPhysics->bIsUpdatedByPhysics = true;

		pGlob->m_EntityPhysics->physObjType = cEntityPhysics::ePhysicsObjType::RIGID_BODY;

		sTextureInfo stone;
		stone.name = "rock.bmp";
		stone.strength = 1.0f;
		pGlob->m_EntityMesh->vecTextures.push_back(stone);

		nPhysics::sRigidBodyDef globDef;
		globDef.Mass = 1.0f;
		globDef.Position = pGlob->m_EntityPhysics->position;
		//globDef.Position = glm::vec3(0.0f, 0.0f, 0.0f);
		globDef.Orientation = glm::vec3(0.0f, 0.0f, 0.0f);
		globDef.Velocity = startVel;

		int colGroup = eCollisionGroup::COL_PAINTGLOB;
		int colMask = (eCollisionGroup::COL_ENVIRONMENT);

		makeSphere(pGlob, globDef, colGroup, colMask);
		//makePointPointConstraint(pGlob, player, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		//makeFixedConstraint(pGlob, player);

		pGlob->m_EntityPhysics->rigidBody->SetEntityType(eEntityType::PAINTGLOB);
		pGlob->m_EntityPhysics->rigidBody->SetName(pGlob->friendlyName);

		//player->vec_pChildrenEntities.push_back(pGlob);
		globList.push_back(pGlob);

		gPhysicsWorld->DisableCollision(pGlob->m_EntityPhysics->rigidBody, player->m_EntityPhysics->rigidBody);
		gPhysicsWorld->DisableCollision(pGlob->m_EntityPhysics->rigidBody, player->vec_pChildrenEntities[0]->m_EntityPhysics->rigidBody);
		for (int i = 0; i < enemyList.size(); i++)
		{
			gPhysicsWorld->DisableCollision(pGlob->m_EntityPhysics->rigidBody, enemyList[i]->m_EntityPhysics->rigidBody);
			gPhysicsWorld->DisableCollision(pGlob->m_EntityPhysics->rigidBody, enemyList[i]->vec_pChildrenEntities[0]->m_EntityPhysics->rigidBody);
		}

		vec_pObjectsToDraw.push_back(pGlob);
	}
}

int splatterCount = 1;

void LoadPaintCube(std::vector<cEntity*> &vec_pObjectsToDraw, glm::vec3 startPos)
{
	{	// This sphere is the tiny little debug sphere
		cEntity* pGlob = new cEntity();
		pGlob->m_EntityMesh->setDiffuseColour(glm::vec3(0.0f, 0.0f, 0.0f));
		pGlob->m_EntityMesh->setSpecularPower(40.0f);
		pGlob->m_EntityMesh->setSpecularColour(glm::vec3(1.000f, 0.766f, 0.336f));

		pGlob->friendlyName = "Splatter" + std::to_string(splatterCount);
		float scale = 1.0f;
		pGlob->m_EntityPhysics->position = startPos + glm::vec3(0.0f, 0.01f * splatterCount, 0.0f);
		pGlob->m_EntityPhysics->nonUniformScale = glm::vec3(scale, 0.01f, scale);
		pGlob->m_EntityMesh->vecLODMeshs.push_back(sLODInfo("simpleplane0.ply"));
		//pGlob->m_EntityMesh->vecLODMeshs.push_back(sLODInfo("cube_flat_shaded_xyz_n_uv.ply"));
		pGlob->m_EntityMesh->bIsWireFrame = false;
		pGlob->m_EntityMesh->bIsVisible = true;
		pGlob->m_EntityMesh->bUseVertexColour = false;
		pGlob->m_EntityPhysics->bIsUpdatedByPhysics = true;

		pGlob->m_EntityPhysics->physObjType = cEntityPhysics::ePhysicsObjType::NONE;

		int tex = rand() % 2 + 1; //either 1 or 2
		if (tex == 1)
		{
			sTextureInfo sPaintInfo;
			sPaintInfo.name = "splatter1.bmp";
			sPaintInfo.strength = 1.0f;
			pGlob->m_EntityMesh->vecTextures.push_back(sPaintInfo);
			pGlob->m_EntityMesh->setDiffuseColour(glm::vec3(1.0f, 1.0f, 0.0f));
		}
		else if (tex == 2)
		{
			sTextureInfo sPaintInfo;
			sPaintInfo.name = "splatter2.bmp";
			sPaintInfo.strength = 1.0f;
			pGlob->m_EntityMesh->vecTextures.push_back(sPaintInfo);
			pGlob->m_EntityMesh->setDiffuseColour(glm::vec3(0.0f, 0.0f, 1.0f));
		}

		vec_pObjectsToDraw.push_back(pGlob);
		splatterCount++;
	}
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

		p2SidedQuad->friendlyName = "2SidedQuadBase";
		p2SidedQuad->m_EntityPhysics->setUniformScale(10.0f);
		p2SidedQuad->m_EntityMesh->vecLODMeshs.push_back(sLODInfo("1x1_2Tri_Quad_2_Sided_xyz_n_uv.ply"));
		p2SidedQuad->m_EntityMesh->bIsWireFrame = false;
		//pDebugSphere->m_EntityMesh->bDontLight = false;
		p2SidedQuad->m_EntityMesh->bIsVisible = false;
		p2SidedQuad->m_EntityMesh->bUseVertexColour = false;

		vec_pObjectsToDraw.push_back(p2SidedQuad);
	}

	{	// This sphere is the tiny little debug sphere
		cEntity* p2SidedQuad = new cEntity();
		p2SidedQuad->m_EntityPhysics->position = glm::vec3(0.0f, 0.0f, 0.0f);
		p2SidedQuad->m_EntityMesh->setDiffuseColour(glm::vec3(0.0f, 0.0f, 0.0f));
		p2SidedQuad->m_EntityMesh->setSpecularPower(100.0f);
		p2SidedQuad->m_EntityMesh->setSpecularColour(glm::vec3(1.000f, 0.766f, 0.336f));

		p2SidedQuad->friendlyName = "2SidedQuadFinal";
		p2SidedQuad->m_EntityPhysics->setUniformScale(10.0f);
		p2SidedQuad->m_EntityMesh->vecLODMeshs.push_back(sLODInfo("1x1_2Tri_Quad_2_Sided_xyz_n_uv.ply"));
		p2SidedQuad->m_EntityMesh->bIsWireFrame = false;
		//pDebugSphere->m_EntityMesh->bDontLight = false;
		p2SidedQuad->m_EntityMesh->bIsVisible = false;
		p2SidedQuad->m_EntityMesh->bUseVertexColour = false;

		vec_pObjectsToDraw.push_back(p2SidedQuad);
	}

	{	// This sphere is the tiny little debug sphere
		cEntity* p2SidedQuad = new cEntity();
		p2SidedQuad->m_EntityPhysics->position = glm::vec3(0.0f, 0.0f, 0.0f);
		p2SidedQuad->m_EntityMesh->setDiffuseColour(glm::vec3(0.0f, 0.0f, 0.0f));
		p2SidedQuad->m_EntityMesh->setSpecularPower(100.0f);
		p2SidedQuad->m_EntityMesh->setSpecularColour(glm::vec3(1.000f, 0.766f, 0.336f));

		p2SidedQuad->friendlyName = "2SidedQuadBloomA";
		p2SidedQuad->m_EntityPhysics->setUniformScale(10.0f);
		p2SidedQuad->m_EntityMesh->vecLODMeshs.push_back(sLODInfo("1x1_2Tri_Quad_2_Sided_xyz_n_uv.ply"));
		p2SidedQuad->m_EntityMesh->bIsWireFrame = false;
		//pDebugSphere->m_EntityMesh->bDontLight = false;
		p2SidedQuad->m_EntityMesh->bIsVisible = false;
		p2SidedQuad->m_EntityMesh->bUseVertexColour = false;

		vec_pObjectsToDraw.push_back(p2SidedQuad);
	}

	{	// This sphere is the tiny little debug sphere
		cEntity* p2SidedQuad = new cEntity();
		p2SidedQuad->m_EntityPhysics->position = glm::vec3(0.0f, 0.0f, 0.0f);
		p2SidedQuad->m_EntityMesh->setDiffuseColour(glm::vec3(0.0f, 0.0f, 0.0f));
		p2SidedQuad->m_EntityMesh->setSpecularPower(100.0f);
		p2SidedQuad->m_EntityMesh->setSpecularColour(glm::vec3(1.000f, 0.766f, 0.336f));

		p2SidedQuad->friendlyName = "2SidedQuadBloomB";
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
		pKatana->m_EntityMesh->setDiffuseColour(glm::vec3(1.0f, 0.0f, 0.0f));
		pKatana->m_EntityMesh->setSpecularPower(0.1f);
		pKatana->m_EntityMesh->setSpecularColour(glm::vec3(1.000f, 0.766f, 0.336f));

		pKatana->friendlyName = "Katana";
		float scale = 0.03f;
		pKatana->m_EntityPhysics->position = glm::vec3(0.0f, 0.0f, 0.0f);
		pKatana->m_EntityPhysics->offset = glm::vec3(0.0f, 0.0f, 50.0f);
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

		sModelDrawInfo modelInfo;
		modelInfo.meshFileName = "katana.ply";
		g_VAOMeshManager->FindDrawInfoByModelName(modelInfo);
		nPhysics::sModelPoint* modelPoints = new nPhysics::sModelPoint[modelInfo.numberOfVertices]();
		for (size_t i = 0; i < modelInfo.numberOfVertices; i++)
		{
			modelPoints[i].vert = glm::vec4(modelInfo.pMeshData->pVertices->x, modelInfo.pMeshData->pVertices->y, 
				modelInfo.pMeshData->pVertices->z, modelInfo.pMeshData->pVertices->w);
		}

		nPhysics::sRigidBodyDef katanaDef;
		//katanaDef.Mass = 5.0f;
		katanaDef.Mass = 0.0f; //this will keep the object static while equipped
		katanaDef.Position = glm::vec3(0.0f, 0.0f, 0.0f);
		//katanaDef.Position = glm::vec3(0.0f, 0.0f, 0.0f);
		katanaDef.Orientation = glm::vec3(0.0f, 0.0f, 0.0f);
		katanaDef.Velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 extents = glm::vec3(1.0f, 1.0f, 130.0f * scale);
		//glm::vec3 extents = glm::vec3(1.0f, 1.0f, 13.0f);

		int colGroup = eCollisionGroup::COL_PLAYERWEAPON;
		int colMask = (eCollisionGroup::COL_ENEMY);

		//makeCapsule(pKatana, katanaDef, 1.0f * scale, 4.0f * scale);
		makeCylinder(pKatana, katanaDef, extents);
		//makeConvexHull(pKatana, katanaDef, modelPoints, modelInfo.numberOfVertices);
		//makePointPointConstraint(pKatana, player, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		//makeFixedConstraint(pKatana, player);

		pKatana->m_EntityPhysics->rigidBody->SetEntityType(eEntityType::PLAYERWEAPON);
		pKatana->m_EntityPhysics->rigidBody->SetName(pKatana->friendlyName);

		player->vec_pChildrenEntities.push_back(pKatana);
		player->Equip(25, pKatana);

		gPhysicsWorld->DisableCollision(player->m_EntityPhysics->rigidBody, pKatana->m_EntityPhysics->rigidBody);
		//removeRB(pKatana);

		//vec_pObjectsToDraw.push_back(pKatana);
	}

	{	// One of the "star shaped" imposter object
		cEntity* pSmoke = new cEntity();
		pSmoke->m_EntityMesh->setDiffuseColour(glm::vec3(1.0f, 1.0f, 0.0f));
		pSmoke->m_EntityMesh->setAlphaTransparency(1.0f);
		pSmoke->m_EntityPhysics->nonUniformScale = glm::vec3(0.05f);
		//pSmoke->friendlyName = "SmokeObject";
		pSmoke->friendlyName = "SmokeObjectStar";
		// ***********************************************************
		// STAR SHAPED IMPOSTER
		// ***********************************************************
//		pSmoke->meshName = "1x1_2Tri_Quad_2_Sided_xyz_n_uv.ply";		// Flat, classic imposter
		pSmoke->m_EntityMesh->vecLODMeshs.push_back(sLODInfo("1x1_Star_1_Quad_2_Sided_xyz_n_uv.ply"));		// Multi-faced imposter
//		pSmoke->meshName = "Sphere_UV_xyz_n_uv.ply";					// Imposter can be any shape, really

		sTextureInfo smokeTex;
		smokeTex.name = "Smoke_1.bmp";
		smokeTex.strength = 1.0f;

		pSmoke->m_EntityMesh->vecTextures.push_back(sTextureInfo(smokeTex));
		pSmoke->m_EntityPhysics->setUniformScale(1.0f);
		pSmoke->m_EntityMesh->bIsVisible = false;
		vec_pObjectsToDraw.push_back(pSmoke);
	}

	{	// One of the "star shaped" imposter object
		cEntity* pSmoke = new cEntity();
		pSmoke->m_EntityMesh->setDiffuseColour(glm::vec3(1.0f, 1.0f, 0.0f));
		pSmoke->m_EntityMesh->setAlphaTransparency(1.0f);
		pSmoke->m_EntityPhysics->nonUniformScale = glm::vec3(0.05f);
		//pSmoke->friendlyName = "SmokeObject";
		pSmoke->friendlyName = "FireObjectStar";
		// ***********************************************************
		// STAR SHAPED IMPOSTER
		// ***********************************************************
//		pSmoke->meshName = "1x1_2Tri_Quad_2_Sided_xyz_n_uv.ply";		// Flat, classic imposter
		pSmoke->m_EntityMesh->vecLODMeshs.push_back(sLODInfo("1x1_Star_1_Quad_2_Sided_xyz_n_uv.ply"));		// Multi-faced imposter
//		pSmoke->meshName = "Sphere_UV_xyz_n_uv.ply";					// Imposter can be any shape, really

		sTextureInfo smokeTex;
		smokeTex.name = "flames.bmp";
		smokeTex.strength = 1.0f;

		pSmoke->m_EntityMesh->vecTextures.push_back(sTextureInfo(smokeTex));
		pSmoke->m_EntityPhysics->setUniformScale(1.0f);
		pSmoke->m_EntityMesh->bIsVisible = false;
		vec_pObjectsToDraw.push_back(pSmoke);
	}
}

void setUpParticleEmitters(void)
{
	{	//STARTOF: Star shaped smoke emitter
		cParticleEmitter* pPE_Smoke_01 = new cParticleEmitter();

		// This takes a while
		cParticleEmitter::sParticleCreationParams particleParams;
		particleParams.totalNumberOfParticles = 500;	// Max # of particles ALIVE at ONE TIME
		particleParams.minLifeTime = 5.0f;
		particleParams.maxLifeTime = 10.0f;
		// Will fade to fully transparent in the last 1 second of "life"
		particleParams.deathTransparencyFadeTimeSeconds = 2.0f;

		//particleParams.position = glm::vec3(10.0f, -10.0f, 0.0f);
		player->vec_pChildrenEntities[0]->m_EntityPhysics->rigidBody->GetPosition(particleParams.position);
		particleParams.minInitialVelocity = glm::vec3(-0.5, 0.5, -0.5);
		particleParams.maxInitialVelocity = glm::vec3(0.5, 1.0, 0.5);
		particleParams.acceleration = glm::vec3(0.0f, 1.0f, 0.0f);
		particleParams.minNumberNewParticles = 2;
		particleParams.maxNumberNewParticles = 5;
		particleParams.minTimeBetweenParticleGenerationSeconds = 0.1f;
		particleParams.maxTimeBetweenParticleGenerationSeconds = 0.5f;
		particleParams.minInitialScale = 0.1f;
		particleParams.maxInitialScale = 0.15f;
		particleParams.minScaleChange = 0.0f;
		particleParams.maxScaleChange = 0.1f;	// This is per frame change

		// Degrees per frame
		particleParams.minOrientationChangeAngleEuler = glm::vec3(-0.25f, -0.25f, -0.25f);
		particleParams.maxOrientationChangeAngleEuler = glm::vec3(+0.25f, +0.25f, +0.25f);

		pPE_Smoke_01->Init(particleParams);

		::g_map_pParticleEmitters["Smoke01"] = pPE_Smoke_01;

	}//ENDOF: Star shaped smoke emitter

	{	//STARTOF: Star shaped smoke emitter
		cParticleEmitter* pPE_Fire_01 = new cParticleEmitter();

		// This takes a while
		cParticleEmitter::sParticleCreationParams particleParams;
		particleParams.totalNumberOfParticles = 500;	// Max # of particles ALIVE at ONE TIME
		particleParams.minLifeTime = 5.0f;
		particleParams.maxLifeTime = 10.0f;
		// Will fade to fully transparent in the last 1 second of "life"
		particleParams.deathTransparencyFadeTimeSeconds = 2.0f;

		//particleParams.position = glm::vec3(10.0f, -10.0f, 0.0f);
		player->vec_pChildrenEntities[0]->m_EntityPhysics->rigidBody->GetPosition(particleParams.position);
		particleParams.minInitialVelocity = glm::vec3(-0.5, 0.5, -0.5);
		particleParams.maxInitialVelocity = glm::vec3(0.5, 1.0, 0.5);
		particleParams.acceleration = glm::vec3(0.0f, 1.0f, 0.0f);
		particleParams.minNumberNewParticles = 2;
		particleParams.maxNumberNewParticles = 5;
		particleParams.minTimeBetweenParticleGenerationSeconds = 0.1f;
		particleParams.maxTimeBetweenParticleGenerationSeconds = 0.5f;
		particleParams.minInitialScale = 0.1f;
		particleParams.maxInitialScale = 0.15f;
		particleParams.minScaleChange = 0.0f;
		particleParams.maxScaleChange = 0.1f;	// This is per frame change

		// Degrees per frame
		particleParams.minOrientationChangeAngleEuler = glm::vec3(-0.25f, -0.25f, -0.25f);
		particleParams.maxOrientationChangeAngleEuler = glm::vec3(+0.25f, +0.25f, +0.25f);

		pPE_Fire_01->Init(particleParams);

		::g_map_pParticleEmitters["Fire01"] = pPE_Fire_01;

	}//ENDOF: Star shaped smoke emitter
}