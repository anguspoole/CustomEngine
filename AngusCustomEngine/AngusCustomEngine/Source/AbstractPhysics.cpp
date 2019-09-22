#include "../Global/global.h"
#include <glm/glm.hpp>
#include <vector>
#include "../Meshes/sModelDrawInfo.h"
#include <iostream>
#include <../load/sConfig.h>
#include <eShapeType.h>
#include <eCollisionGroup.h>
#include <cBtFactory.h>

typedef nPhysics::iPhysicsFactory*(*f_CreateFactory)();

//HINSTANCE hGetProckDll = 0;

nPhysics::iPhysicsFactory* gPhysicsFactory = NULL;
nPhysics::iPhysicsWorld* gPhysicsWorld = NULL;

//	InitPhysicsStuff is used to call initial physics stuff (load dll, start factory)
bool InitPhysicsStuff(const nLoad::sConfig& config)
{
	if (gPhysicsFactory)
	{
		std::cout << "Factory already exists" << std::endl;
		return false;
	}

	/*hGetProckDll = LoadLibraryA(config.PhysicsLibrary.c_str());
	if (!hGetProckDll)
	{
		std::cout << "Library not found" << std::endl;
		return false;
	}*/

	/*f_CreateFactory CreateFactory = (f_CreateFactory)GetProcAddress(hGetProckDll, "CreateFactory");
	if (!CreateFactory)
	{
		std::cout << "CreateFactory not found" << std::endl;
		return false;
	}*/

	//gPhysicsFactory = CreateFactory();
	gPhysicsFactory = new nPhysics::cBulletPhysicsFactory();

	return true;
}

// The buildPhysicsWorld function is used to create the initial physics world and set the gravity
bool buildPhysicsWorld()
{
	gPhysicsWorld = gPhysicsFactory->CreatePhysicsWorld(); //create the physics world
	gPhysicsWorld->SetGravity(glm::vec3(0.0f, -9.8f, 0.0f));
	return true;
}

// The addClothNode function is used for adding a "cloth node" for physics softbodies
void addClothNode(std::vector< cEntity* > &vec_pObjectsToDraw)
{
	cEntity * testObj = new cEntity();
	float scale = 1.5f;
	testObj->m_EntityPhysics->setUniformScale(scale);
	testObj->m_EntityMesh->vecLODMeshs.push_back(sLODInfo("Sphere_320_faces_xyz_n_GARBAGE_uv.ply"));
	testObj->friendlyName = "ClothNode";
	testObj->m_EntityMesh->setDiffuseColour(glm::vec3(1.0f, 1.0f, 0.0f));
	testObj->m_EntityMesh->setSpecularPower(0.1f);
	testObj->m_EntityMesh->bIsWireFrame = false;
	testObj->m_EntityPhysics->physObjType = cEntityPhysics::ePhysicsObjType::SOFT_BODY_NODE;
	//testObj->bIsWireFrame = true;

	testObj->m_EntityMesh->bUseVertexColour = false;
	testObj->m_EntityMesh->setDiffuseColour(glm::vec3(0.0f, 0.0f, 0.0f));
	testObj->m_EntityMesh->setSpecularPower(100.0f);
	testObj->m_EntityMesh->setSpecularColour(glm::vec3(1.000f, 0.766f, 0.336f));

	sTextureInfo stone;
	stone.name = "canvas.bmp";
	stone.strength = 1.0f;
	testObj->m_EntityMesh->vecTextures.push_back(stone);

	testObj->m_EntityMesh->bIsVisible = true;

	vec_pObjectsToDraw.push_back(testObj);
}

// The buildPhysicsObjects function is used to create the actual physics objects
void buildPhysicsObjects(const nLoad::sConfig& config, std::vector< cEntity* > &vec_pObjectsToDraw, GLuint shaderProgramID)
{

	size_t numRigidShapes = config.RigidShapeTypes.size();
	size_t numSoftShapes = config.SoftShapeTypes.size();
	int sphereCount = 0;
	int planeCount = 0;
	int boxCount = 0;
	int cylinderCount = 0;
	int coneCount = 0;
	int softBodyCount = 0;

	cSimpleAssimpSkinnedMesh* enemySkinnedMesh = LoadEnemyMeshModel(config, 0, vec_pObjectsToDraw, shaderProgramID);

	if (numSoftShapes > 0)
	{
		addClothNode(vec_pObjectsToDraw);
	}

	for (size_t c = 0; c < numSoftShapes; c++)
	{
		if (std::strcmp(config.SoftObjTypes[c].c_str(), "Cloth") == 0)
		{
			if (std::strcmp(config.SoftShapeTypes[c].c_str(), "Cloth") == 0)
			{
				cEntity * softBodyObj = new cEntity();
				//soft body has no mesh on its own(?)
				softBodyObj->m_EntityMesh->bIsVisible = true;

				softBodyObj->m_EntityMesh->bUseVertexColour = false;
				softBodyObj->m_EntityMesh->setDiffuseColour(glm::vec3(0.0f, 0.0f, 0.0f));
				softBodyObj->m_EntityMesh->setSpecularPower(100.0f);
				softBodyObj->m_EntityMesh->setSpecularColour(glm::vec3(1.000f, 0.766f, 0.336f));

				sTextureInfo stone;
				stone.name = "canvas.bmp";
				stone.strength = 1.0f;
				softBodyObj->m_EntityMesh->vecTextures.push_back(stone);

				softBodyObj->m_EntityPhysics->physObjType = cEntityPhysics::ePhysicsObjType::SOFT_BODY;
				makeSoftBody(softBodyObj, config.SoftBodyDefs[softBodyCount]);
				vec_pObjectsToDraw.push_back(softBodyObj);
				softBodyCount++;
			}
		}
	}

	for (size_t c = 0; c < numRigidShapes; c++)
	{
		if (std::strcmp(config.RigidObjTypes[c].c_str(), "Simple") == 0)
		{
			if (std::strcmp(config.RigidShapeTypes[c].c_str(), "Player") == 0)
			{
				LoadPlayerMeshModel(config, c, vec_pObjectsToDraw, shaderProgramID);
			}
			else if (std::strcmp(config.RigidShapeTypes[c].c_str(), "Enemy") == 0)
			{
				CreateAndAssignAnimatedEnemy(config, c, vec_pObjectsToDraw, shaderProgramID, enemySkinnedMesh);
			}
			else //Regular object
			{
				cEntity * entity = new cEntity();
				entity->m_EntityMesh->vecLODMeshs.push_back(sLODInfo(config.RigidBodyDefs[c].Model));
				entity->friendlyName = config.RigidBodyDefs[c].Name;
				entity->m_EntityMesh->bIsWireFrame = false;
				entity->m_EntityPhysics->physObjType = cEntityPhysics::ePhysicsObjType::RIGID_BODY;
				entity->m_EntityMesh->setDiffuseColour(config.RigidBodyDefs[c].Diffuse);
				entity->m_EntityMesh->bUseVertexColour = false;
				entity->m_EntityMesh->setSpecularPower(100.0f);
				entity->m_EntityMesh->setSpecularColour(config.RigidBodyDefs[c].Specular);

				if (std::strcmp(config.RigidShapeTypes[c].c_str(), "Sphere") == 0)
				{
					if (config.RigidBodyDefs[c].Mass > 0.001f)
					{
						entity->m_EntityPhysics->setUniformScale(config.RigidBodyDefs[c].Mass);
					}
					else
					{
						entity->m_EntityPhysics->setUniformScale(config.RigidBodyDefs[c].Extents.x);
					}

					sTextureInfo stone;
					stone.name = "rock.bmp";
					stone.strength = 1.0f;
					entity->m_EntityMesh->vecTextures.push_back(stone);

					vec_pObjectsToDraw.push_back(entity);

					makeSphere(entity, config.RigidBodyDefs[c]);

					sphereCount++;
				}

				else if (std::strcmp(config.RigidShapeTypes[c].c_str(), "Plane0") == 0)
				{
					entity->m_EntityPhysics->setUniformScale(5.0f);
					entity->m_EntityMesh->bHasNormalMap = true;

					//sBox* pBox = new sBox();
					sModelDrawInfo cube;
					cube.meshFileName = entity->m_EntityMesh->vecLODMeshs[0].meshName;
					g_VAOMeshManager->FindDrawInfoByModelName(cube);

					//for (int i = 0; i < cube.numberOfTriangles; i++)
					//{
					sTriangle* pTri = new sTriangle();
					pTri->v[0] = glm::vec3(cube.pMeshData->pVertices[cube.pMeshData->pTriangles[0].vertex_ID_0].x, cube.pMeshData->pVertices[cube.pMeshData->pTriangles[0].vertex_ID_0].y, cube.pMeshData->pVertices[cube.pMeshData->pTriangles[0].vertex_ID_0].z);
					pTri->v[1] = glm::vec3(cube.pMeshData->pVertices[cube.pMeshData->pTriangles[0].vertex_ID_1].x, cube.pMeshData->pVertices[cube.pMeshData->pTriangles[0].vertex_ID_1].y, cube.pMeshData->pVertices[cube.pMeshData->pTriangles[0].vertex_ID_1].z);
					pTri->v[2] = glm::vec3(cube.pMeshData->pVertices[cube.pMeshData->pTriangles[0].vertex_ID_2].x, cube.pMeshData->pVertices[cube.pMeshData->pTriangles[0].vertex_ID_2].y, cube.pMeshData->pVertices[cube.pMeshData->pTriangles[0].vertex_ID_2].z);

					glm::vec3 A = pTri->v[1] - pTri->v[0];
					glm::vec3 B = pTri->v[2] - pTri->v[0];

					glm::vec3 sn = glm::cross(A, B);

					pTri->n = glm::normalize(sn);

					sTextureInfo stone;
					stone.name = "metalplateColor.bmp";
					stone.strength = 0.7f;
					entity->m_EntityMesh->vecTextures.push_back(stone);

					sTextureInfo plate;
					plate.name = "metallplates.bmp";
					plate.strength = 0.3f;
					entity->m_EntityMesh->vecTextures.push_back(plate);

					sTextureInfo plateN;
					plateN.name = "metalplateNormal.bmp";
					plateN.strength = 0.0f;
					entity->m_EntityMesh->vecTextures.push_back(plateN);

					vec_pObjectsToDraw.push_back(entity);

					glm::vec3 n = pTri->n;
					float dx = (n.x * config.RigidBodyDefs[c].Position.x);
					float dy = (n.y * config.RigidBodyDefs[c].Position.y);
					float dz = (n.z * config.RigidBodyDefs[c].Position.z);
					float d = dx + dy + dz;

					int colGroup = eCollisionGroup::COL_ENVIRONMENT;
					int colMask = (eCollisionGroup::COL_ENVIRONMENT | eCollisionGroup::COL_PLAYER | eCollisionGroup::COL_ENEMY | eCollisionGroup::COL_PAINTGLOB);

					makePlane(entity, n, config.RigidBodyDefs[c], d, colGroup, colMask);
					entity->m_EntityPhysics->rigidBody->SetEntityType(eEntityType::ENVIRONMENT);
					planeCount++;
				}
				else if (std::strcmp(config.RigidShapeTypes[c].c_str(), "Plane1") == 0)
				{
					entity->m_EntityPhysics->setUniformScale(5.0f);
					entity->m_EntityMesh->bIsVisible = false;

					//sBox* pBox = new sBox();
					sModelDrawInfo cube;
					cube.meshFileName = entity->m_EntityMesh->vecLODMeshs[0].meshName;
					g_VAOMeshManager->FindDrawInfoByModelName(cube);

					//for (int i = 0; i < cube.numberOfTriangles; i++)
					//{
					sTriangle* pTri = new sTriangle();
					pTri->v[0] = glm::vec3(cube.pMeshData->pVertices[cube.pMeshData->pTriangles[0].vertex_ID_0].x, cube.pMeshData->pVertices[cube.pMeshData->pTriangles[0].vertex_ID_0].y, cube.pMeshData->pVertices[cube.pMeshData->pTriangles[0].vertex_ID_0].z);
					pTri->v[1] = glm::vec3(cube.pMeshData->pVertices[cube.pMeshData->pTriangles[0].vertex_ID_1].x, cube.pMeshData->pVertices[cube.pMeshData->pTriangles[0].vertex_ID_1].y, cube.pMeshData->pVertices[cube.pMeshData->pTriangles[0].vertex_ID_1].z);
					pTri->v[2] = glm::vec3(cube.pMeshData->pVertices[cube.pMeshData->pTriangles[0].vertex_ID_2].x, cube.pMeshData->pVertices[cube.pMeshData->pTriangles[0].vertex_ID_2].y, cube.pMeshData->pVertices[cube.pMeshData->pTriangles[0].vertex_ID_2].z);

					glm::vec3 A = pTri->v[1] - pTri->v[0];
					glm::vec3 B = pTri->v[2] - pTri->v[0];

					glm::vec3 sn = glm::cross(A, B);

					pTri->n = glm::normalize(sn);

					sTextureInfo stone;
					stone.name = "grass.bmp";
					stone.strength = 1.0f;
					entity->m_EntityMesh->vecTextures.push_back(stone);

					vec_pObjectsToDraw.push_back(entity);

					glm::vec3 n = pTri->n;
					float dx = (n.x * config.RigidBodyDefs[c].Position.x);
					float dy = (n.y * config.RigidBodyDefs[c].Position.y);
					float dz = (n.z * config.RigidBodyDefs[c].Position.z);
					float d = dx + dy + dz;

					int colGroup = eCollisionGroup::COL_ENVIRONMENT;
					int colMask = (eCollisionGroup::COL_ENVIRONMENT | eCollisionGroup::COL_PLAYER | eCollisionGroup::COL_ENEMY | eCollisionGroup::COL_PAINTGLOB);

					makePlane(entity, n, config.RigidBodyDefs[c], d, colGroup, colMask);
					planeCount++;
				}
				else if (std::strcmp(config.RigidShapeTypes[c].c_str(), "Plane2") == 0)
				{
					entity->m_EntityPhysics->setUniformScale(5.0f);
					entity->m_EntityMesh->bIsVisible = false;

					//sBox* pBox = new sBox();
					sModelDrawInfo cube;
					cube.meshFileName = entity->m_EntityMesh->vecLODMeshs[0].meshName;
					g_VAOMeshManager->FindDrawInfoByModelName(cube);

					//for (int i = 0; i < cube.numberOfTriangles; i++)
					//{
					sTriangle* pTri = new sTriangle();
					pTri->v[0] = glm::vec3(cube.pMeshData->pVertices[cube.pMeshData->pTriangles[0].vertex_ID_0].x, cube.pMeshData->pVertices[cube.pMeshData->pTriangles[0].vertex_ID_0].y, cube.pMeshData->pVertices[cube.pMeshData->pTriangles[0].vertex_ID_0].z);
					pTri->v[1] = glm::vec3(cube.pMeshData->pVertices[cube.pMeshData->pTriangles[0].vertex_ID_1].x, cube.pMeshData->pVertices[cube.pMeshData->pTriangles[0].vertex_ID_1].y, cube.pMeshData->pVertices[cube.pMeshData->pTriangles[0].vertex_ID_1].z);
					pTri->v[2] = glm::vec3(cube.pMeshData->pVertices[cube.pMeshData->pTriangles[0].vertex_ID_2].x, cube.pMeshData->pVertices[cube.pMeshData->pTriangles[0].vertex_ID_2].y, cube.pMeshData->pVertices[cube.pMeshData->pTriangles[0].vertex_ID_2].z);

					glm::vec3 A = pTri->v[1] - pTri->v[0];
					glm::vec3 B = pTri->v[2] - pTri->v[0];

					glm::vec3 sn = glm::cross(A, B);

					pTri->n = glm::normalize(sn);

					sTextureInfo stone;
					stone.name = "grass.bmp";
					stone.strength = 1.0f;
					entity->m_EntityMesh->vecTextures.push_back(stone);

					vec_pObjectsToDraw.push_back(entity);

					glm::vec3 n = pTri->n;
					float dx = (n.x * config.RigidBodyDefs[c].Position.x);
					float dy = (n.y * config.RigidBodyDefs[c].Position.y);
					float dz = (n.z * config.RigidBodyDefs[c].Position.z);
					float d = dx + dy + dz;

					int colGroup = eCollisionGroup::COL_ENVIRONMENT;
					int colMask = (eCollisionGroup::COL_ENVIRONMENT | eCollisionGroup::COL_PLAYER | eCollisionGroup::COL_ENEMY | eCollisionGroup::COL_PAINTGLOB);

					makePlane(entity, n, config.RigidBodyDefs[c], d, colGroup, colMask);
					planeCount++;
				}

				else if (std::strcmp(config.RigidShapeTypes[c].c_str(), "Plane3") == 0)
				{
					entity->m_EntityPhysics->setUniformScale(5.0f);
					entity->m_EntityMesh->bIsVisible = false;

					//sBox* pBox = new sBox();
					sModelDrawInfo cube;
					cube.meshFileName = entity->m_EntityMesh->vecLODMeshs[0].meshName;
					g_VAOMeshManager->FindDrawInfoByModelName(cube);

					//for (int i = 0; i < cube.numberOfTriangles; i++)
					//{
					sTriangle* pTri = new sTriangle();
					pTri->v[0] = glm::vec3(cube.pMeshData->pVertices[cube.pMeshData->pTriangles[0].vertex_ID_0].x, cube.pMeshData->pVertices[cube.pMeshData->pTriangles[0].vertex_ID_0].y, cube.pMeshData->pVertices[cube.pMeshData->pTriangles[0].vertex_ID_0].z);
					pTri->v[1] = glm::vec3(cube.pMeshData->pVertices[cube.pMeshData->pTriangles[0].vertex_ID_1].x, cube.pMeshData->pVertices[cube.pMeshData->pTriangles[0].vertex_ID_1].y, cube.pMeshData->pVertices[cube.pMeshData->pTriangles[0].vertex_ID_1].z);
					pTri->v[2] = glm::vec3(cube.pMeshData->pVertices[cube.pMeshData->pTriangles[0].vertex_ID_2].x, cube.pMeshData->pVertices[cube.pMeshData->pTriangles[0].vertex_ID_2].y, cube.pMeshData->pVertices[cube.pMeshData->pTriangles[0].vertex_ID_2].z);

					glm::vec3 A = pTri->v[1] - pTri->v[0];
					glm::vec3 B = pTri->v[2] - pTri->v[0];

					glm::vec3 sn = glm::cross(A, B);

					pTri->n = glm::normalize(sn);

					sTextureInfo stone;
					stone.name = "grass.bmp";
					stone.strength = 1.0f;
					entity->m_EntityMesh->vecTextures.push_back(stone);

					vec_pObjectsToDraw.push_back(entity);

					glm::vec3 n = pTri->n;
					float dx = (n.x * config.RigidBodyDefs[c].Position.x);
					float dy = (n.y * config.RigidBodyDefs[c].Position.y);
					float dz = (n.z * config.RigidBodyDefs[c].Position.z);
					float d = dx + dy + dz;

					int colGroup = eCollisionGroup::COL_ENVIRONMENT;
					int colMask = (eCollisionGroup::COL_ENVIRONMENT | eCollisionGroup::COL_PLAYER | eCollisionGroup::COL_ENEMY | eCollisionGroup::COL_PAINTGLOB);

					makePlane(entity, n, config.RigidBodyDefs[c], d, colGroup, colMask);
					planeCount++;
				}
				else if (std::strcmp(config.RigidShapeTypes[c].c_str(), "Plane4") == 0)
				{
					entity->m_EntityPhysics->setUniformScale(5.0f);
					entity->m_EntityMesh->bIsVisible = false;

					//sBox* pBox = new sBox();
					sModelDrawInfo cube;
					cube.meshFileName = entity->m_EntityMesh->vecLODMeshs[0].meshName;
					g_VAOMeshManager->FindDrawInfoByModelName(cube);

					//for (int i = 0; i < cube.numberOfTriangles; i++)
					//{
					sTriangle* pTri = new sTriangle();
					pTri->v[0] = glm::vec3(cube.pMeshData->pVertices[cube.pMeshData->pTriangles[0].vertex_ID_0].x, cube.pMeshData->pVertices[cube.pMeshData->pTriangles[0].vertex_ID_0].y, cube.pMeshData->pVertices[cube.pMeshData->pTriangles[0].vertex_ID_0].z);
					pTri->v[1] = glm::vec3(cube.pMeshData->pVertices[cube.pMeshData->pTriangles[0].vertex_ID_1].x, cube.pMeshData->pVertices[cube.pMeshData->pTriangles[0].vertex_ID_1].y, cube.pMeshData->pVertices[cube.pMeshData->pTriangles[0].vertex_ID_1].z);
					pTri->v[2] = glm::vec3(cube.pMeshData->pVertices[cube.pMeshData->pTriangles[0].vertex_ID_2].x, cube.pMeshData->pVertices[cube.pMeshData->pTriangles[0].vertex_ID_2].y, cube.pMeshData->pVertices[cube.pMeshData->pTriangles[0].vertex_ID_2].z);

					glm::vec3 A = pTri->v[1] - pTri->v[0];
					glm::vec3 B = pTri->v[2] - pTri->v[0];

					glm::vec3 sn = glm::cross(A, B);

					pTri->n = glm::normalize(sn);

					sTextureInfo stone;
					stone.name = "grass.bmp";
					stone.strength = 1.0f;
					entity->m_EntityMesh->vecTextures.push_back(stone);

					vec_pObjectsToDraw.push_back(entity);

					glm::vec3 n = pTri->n;
					float dx = (n.x * config.RigidBodyDefs[c].Position.x);
					float dy = (n.y * config.RigidBodyDefs[c].Position.y);
					float dz = (n.z * config.RigidBodyDefs[c].Position.z);
					float d = dx + dy + dz;

					int colGroup = eCollisionGroup::COL_ENVIRONMENT;
					int colMask = (eCollisionGroup::COL_ENVIRONMENT | eCollisionGroup::COL_PLAYER | eCollisionGroup::COL_ENEMY | eCollisionGroup::COL_PAINTGLOB);

					makePlane(entity, n, config.RigidBodyDefs[c], d, colGroup, colMask);
					planeCount++;
				}
				else if (std::strcmp(config.RigidShapeTypes[c].c_str(), "Box") == 0)
				{
					glm::vec3 rotationValues = config.RigidBodyDefs[c].Orientation;
					glm::quat qRotation = glm::quat(glm::vec3(rotationValues.x, rotationValues.y, rotationValues.z));
					glm::mat4 rotation = glm::toMat4(qRotation);

					sTextureInfo stone;
					stone.name = "rock.bmp";
					stone.strength = 1.0f;
					entity->m_EntityMesh->vecTextures.push_back(stone);

					vec_pObjectsToDraw.push_back(entity);

					glm::vec3 extents = glm::vec3(config.RigidBodyDefs[c].Extents);
					entity->m_EntityPhysics->nonUniformScale = extents;

					int colGroup = eCollisionGroup::COL_ENVIRONMENT;
					int colMask = (eCollisionGroup::COL_PLAYER | eCollisionGroup::COL_ENEMY);
					makeBox(entity, config.RigidBodyDefs[c], extents);
					boxCount++;
				}
				else if (std::strcmp(config.RigidShapeTypes[c].c_str(), "Cylinder") == 0)
				{
					sTextureInfo stone;
					stone.name = "rock.bmp";
					stone.strength = 1.0f;
					entity->m_EntityMesh->vecTextures.push_back(stone);

					//testObj->pDebugRenderer = ::g_pDebugRenderer;

					vec_pObjectsToDraw.push_back(entity);
					//sphereList.push_back(testObj);

					float scale = 0.03f;
					entity->m_EntityPhysics->nonUniformScale = glm::vec3(13.0f * scale, 14.0f * scale, 130.0f * scale);
					glm::vec3 extents = glm::vec3(13.0f * scale, 14.0f * scale, 130.0f * scale);

					makeCylinder(entity, config.RigidBodyDefs[c], extents);
					cylinderCount++;

					//if (cylinderCount == 1)
					//{
					//	cEntity* sphereObj = findObjectByFriendlyName("sphere4");

					//	if (sphereObj != NULL)
					//	{
					//		makeSliderConstraint(testObj, sphereObj);
					//	}
					//}
				}
				else if (std::strcmp(config.RigidShapeTypes[c].c_str(), "Cone") == 0)
				{
					sTextureInfo stone;
					stone.name = "rock.bmp";
					stone.strength = 1.0f;
					entity->m_EntityMesh->vecTextures.push_back(stone);

					vec_pObjectsToDraw.push_back(entity);
					makeCone(entity, config.RigidBodyDefs[c]);
					coneCount++;

					if (coneCount == 1)
					{
						cEntity* sphereObj = findObjectByFriendlyName("sphere3");

						if (sphereObj != NULL)
						{
							makeConeTwistConstraint(entity, sphereObj);
						}
					}
				}
			}
		}
	}
}