#include "../Global/global.h"
#include <glm/glm.hpp>
#include <vector>
#include "../Meshes/sModelDrawInfo.h"
#include <iostream>
#include <../load/sConfig.h>
#include <eShapeType.h>

typedef nPhysics::iPhysicsFactory*(*f_CreateFactory)();

HINSTANCE hGetProckDll = 0;

nPhysics::iPhysicsFactory* gPhysicsFactory = NULL;
nPhysics::iPhysicsWorld* gPhysicsWorld = NULL;

void makeSphere(cEntity * obj, nPhysics::sRigidBodyDef bodyDef0);
void makePlane(cEntity * obj, glm::vec3 n, nPhysics::sRigidBodyDef bodyDef0, float d);
void makeBox(cEntity * obj, nPhysics::sRigidBodyDef bodyDef0, glm::vec3 extents);
void makeCylinder(cEntity * obj, nPhysics::sRigidBodyDef bodyDef0);
void makeCone(cEntity * obj, nPhysics::sRigidBodyDef bodyDef0);
void makeCapsule(cEntity * obj, nPhysics::sRigidBodyDef bodyDef0, float radius, float height);

bool InitPhysicsStuff(const nLoad::sConfig& config)
{
	if (gPhysicsFactory)
	{
		std::cout << "Factory already exists" << std::endl;
		return false;
	}

	hGetProckDll = LoadLibraryA(config.PhysicsLibrary.c_str());
	if (!hGetProckDll)
	{
		std::cout << "Library not found" << std::endl;
		return false;
	}

	f_CreateFactory CreateFactory = (f_CreateFactory)GetProcAddress(hGetProckDll, "CreateFactory");
	if (!CreateFactory)
	{
		std::cout << "CreateFactory not found" << std::endl;
		return false;
	}

	gPhysicsFactory = CreateFactory();
	return true;
}

bool buildPhysicsWorld()
{
	gPhysicsWorld = gPhysicsFactory->CreatePhysicsWorld(); //create the physics world
	gPhysicsWorld->SetGravity(glm::vec3(0.0f, -9.8f, 0.0f));
	return true;
}

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
			else if (std::strcmp(config.RigidShapeTypes[c].c_str(), "Sphere") == 0)
			{
				cEntity * testObj = new cEntity();
				testObj->m_EntityPhysics->setUniformScale(config.RigidBodyDefs[c].Mass);
				testObj->m_EntityMesh->vecLODMeshs.push_back(sLODInfo("Sphere_320_faces_xyz_n_GARBAGE_uv.ply"));
				testObj->friendlyName = "sphere" + std::to_string(sphereCount);
				//glm::vec3 orientation = config.RigidBodyDefs[c].Orientation;
				//testObj->setMeshOrientationEulerAngles(orientation, true);
				testObj->m_EntityMesh->bIsWireFrame = false;
				testObj->m_EntityPhysics->physObjType = cEntityPhysics::ePhysicsObjType::RIGID_BODY;

				//testObj->setDiffuseColour(glm::vec3(1.0f, 223.0f / 255.0f, 0.0f));
				testObj->m_EntityMesh->setDiffuseColour(glm::vec3(0.0f, 0.0f, 0.0f));
				testObj->m_EntityMesh->bUseVertexColour = false;
				// Taken from: https://docs.unrealengine.com/en-us/Engine/Rendering/Materials/PhysicallyBased
				// Note that the specular HIGHLIGHT colour is DIFFERENT from 
				// the diffuse... 
				testObj->m_EntityMesh->setSpecularPower(100.0f);
				testObj->m_EntityMesh->setSpecularColour(glm::vec3(1.000f, 0.766f, 0.336f));

				sTextureInfo stone;
				stone.name = "rock.bmp";
				stone.strength = 1.0f;
				testObj->m_EntityMesh->vecTextures.push_back(stone);

				//testObj->pDebugRenderer = ::g_pDebugRenderer;

				vec_pObjectsToDraw.push_back(testObj);
				//sphereList.push_back(testObj);
				makeSphere(testObj, config.RigidBodyDefs[c]);

				sphereCount++;
			}
			else if (std::strcmp(config.RigidShapeTypes[c].c_str(), "Plane0") == 0)
			{
				cEntity * testObj1 = new cEntity();
				testObj1->m_EntityPhysics->setUniformScale(2.0f);
				testObj1->m_EntityMesh->vecLODMeshs.push_back(sLODInfo("simpleplane0.ply"));
				testObj1->friendlyName = "plane" + std::to_string(planeCount);
				//glm::vec3 orientation = config.RigidBodyDefs[c].Orientation;
				//testObj1->setMeshOrientationEulerAngles(orientation, true);
				//testObj1->setDiffuseColour(glm::vec3(1.0f, 1.0f, 0.0f));
				testObj1->m_EntityMesh->setDiffuseColour(glm::vec3(0.0f, 0.0f, 0.0f));
				testObj1->m_EntityMesh->setSpecularPower(100.0f);
				testObj1->m_EntityMesh->setSpecularColour(glm::vec3(1.000f, 0.766f, 0.336f));
				testObj1->m_EntityMesh->bIsWireFrame = false;
				testObj1->m_EntityPhysics->physObjType = cEntityPhysics::ePhysicsObjType::RIGID_BODY;

				//sBox* pBox = new sBox();
				sModelDrawInfo cube;
				cube.meshFileName = testObj1->m_EntityMesh->vecLODMeshs[0].meshName;
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

				//	pBox->boxTris.push_back(pTri);
				//}
				//testObj1->pTheShape = pBox;
				//testObj1->shapeType = cEntity::BOX;

				sTextureInfo stone;
				stone.name = "grass.bmp";
				stone.strength = 1.0f;
				testObj1->m_EntityMesh->vecTextures.push_back(stone);

				vec_pObjectsToDraw.push_back(testObj1);

				glm::vec3 n = pTri->n;
				float dx = (n.x * config.RigidBodyDefs[c].Position.x);
				float dy = (n.y * config.RigidBodyDefs[c].Position.y);
				float dz = (n.z * config.RigidBodyDefs[c].Position.z);
				float d = dx + dy + dz;

				makePlane(testObj1, n, config.RigidBodyDefs[c], d);
				planeCount++;
			}
			else if (std::strcmp(config.RigidShapeTypes[c].c_str(), "Plane1") == 0)
			{
				cEntity * testObj1 = new cEntity();
				testObj1->m_EntityPhysics->setUniformScale(2.0f);
				testObj1->m_EntityMesh->vecLODMeshs.push_back(sLODInfo("simpleplane1.ply"));
				testObj1->friendlyName = "plane" + std::to_string(planeCount);
				//glm::vec3 orientation = config.RigidBodyDefs[c].Orientation;
				//testObj1->setMeshOrientationEulerAngles(orientation, true);
				//testObj1->setDiffuseColour(glm::vec3(1.0f, 1.0f, 0.0f));
				testObj1->m_EntityMesh->setDiffuseColour(glm::vec3(0.0f, 0.0f, 0.0f));
				testObj1->m_EntityMesh->setSpecularPower(100.0f);
				testObj1->m_EntityMesh->setSpecularColour(glm::vec3(1.000f, 0.766f, 0.336f));
				testObj1->m_EntityMesh->bIsWireFrame = false;
				testObj1->m_EntityMesh->bIsVisible = false;
				testObj1->m_EntityPhysics->physObjType = cEntityPhysics::ePhysicsObjType::RIGID_BODY;

				//sBox* pBox = new sBox();
				sModelDrawInfo cube;
				cube.meshFileName = testObj1->m_EntityMesh->vecLODMeshs[0].meshName;
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

				//	pBox->boxTris.push_back(pTri);
				//}
				//testObj1->pTheShape = pBox;
				//testObj1->shapeType = cEntity::BOX;

				sTextureInfo stone;
				stone.name = "grass.bmp";
				stone.strength = 1.0f;
				testObj1->m_EntityMesh->vecTextures.push_back(stone);

				vec_pObjectsToDraw.push_back(testObj1);

				glm::vec3 n = pTri->n;
				float dx = (n.x * config.RigidBodyDefs[c].Position.x);
				float dy = (n.y * config.RigidBodyDefs[c].Position.y);
				float dz = (n.z * config.RigidBodyDefs[c].Position.z);
				float d = dx + dy + dz;

				makePlane(testObj1, n, config.RigidBodyDefs[c], d);
				planeCount++;
			}
			else if (std::strcmp(config.RigidShapeTypes[c].c_str(), "Plane2") == 0)
			{
			cEntity * testObj1 = new cEntity();
			testObj1->m_EntityPhysics->setUniformScale(2.0f);
			testObj1->m_EntityMesh->vecLODMeshs.push_back(sLODInfo("simpleplane2.ply"));
			testObj1->friendlyName = "plane" + std::to_string(planeCount);
			//glm::vec3 orientation = config.RigidBodyDefs[c].Orientation;
			//testObj1->setMeshOrientationEulerAngles(orientation, true);
			//testObj1->setDiffuseColour(glm::vec3(1.0f, 1.0f, 0.0f));
			testObj1->m_EntityMesh->setDiffuseColour(glm::vec3(0.0f, 0.0f, 0.0f));
			testObj1->m_EntityMesh->setSpecularPower(100.0f);
			testObj1->m_EntityMesh->setSpecularColour(glm::vec3(1.000f, 0.766f, 0.336f));
			testObj1->m_EntityMesh->bIsWireFrame = false;
			testObj1->m_EntityMesh->bIsVisible = false;
			testObj1->m_EntityPhysics->physObjType = cEntityPhysics::ePhysicsObjType::RIGID_BODY;

			//sBox* pBox = new sBox();
			sModelDrawInfo cube;
			cube.meshFileName = testObj1->m_EntityMesh->vecLODMeshs[0].meshName;
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

			//	pBox->boxTris.push_back(pTri);
			//}
			//testObj1->pTheShape = pBox;
			//testObj1->shapeType = cEntity::BOX;

			sTextureInfo stone;
			stone.name = "grass.bmp";
			stone.strength = 1.0f;
			testObj1->m_EntityMesh->vecTextures.push_back(stone);

			vec_pObjectsToDraw.push_back(testObj1);

			glm::vec3 n = pTri->n;
			float dx = (n.x * config.RigidBodyDefs[c].Position.x);
			float dy = (n.y * config.RigidBodyDefs[c].Position.y);
			float dz = (n.z * config.RigidBodyDefs[c].Position.z);
			float d = dx + dy + dz;

			makePlane(testObj1, n, config.RigidBodyDefs[c], d);
			planeCount++;
			}
			else if (std::strcmp(config.RigidShapeTypes[c].c_str(), "Plane3") == 0)
			{
			cEntity * testObj1 = new cEntity();
			testObj1->m_EntityPhysics->setUniformScale(2.0f);
			testObj1->m_EntityMesh->vecLODMeshs.push_back(sLODInfo("simpleplane3.ply"));
			testObj1->friendlyName = "plane" + std::to_string(planeCount);
			//glm::vec3 orientation = config.RigidBodyDefs[c].Orientation;
			//testObj1->setMeshOrientationEulerAngles(orientation, true);
			//testObj1->setDiffuseColour(glm::vec3(1.0f, 1.0f, 0.0f));
			testObj1->m_EntityMesh->setDiffuseColour(glm::vec3(0.0f, 0.0f, 0.0f));
			testObj1->m_EntityMesh->setSpecularPower(100.0f);
			testObj1->m_EntityMesh->setSpecularColour(glm::vec3(1.000f, 0.766f, 0.336f));
			testObj1->m_EntityMesh->bIsWireFrame = false;
			testObj1->m_EntityMesh->bIsVisible = false;
			testObj1->m_EntityPhysics->physObjType = cEntityPhysics::ePhysicsObjType::RIGID_BODY;

			//sBox* pBox = new sBox();
			sModelDrawInfo cube;
			cube.meshFileName = testObj1->m_EntityMesh->vecLODMeshs[0].meshName;
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

			//	pBox->boxTris.push_back(pTri);
			//}
			//testObj1->pTheShape = pBox;
			//testObj1->shapeType = cEntity::BOX;

			sTextureInfo stone;
			stone.name = "grass.bmp";
			stone.strength = 1.0f;
			testObj1->m_EntityMesh->vecTextures.push_back(stone);

			vec_pObjectsToDraw.push_back(testObj1);

			glm::vec3 n = pTri->n;
			float dx = (n.x * config.RigidBodyDefs[c].Position.x);
			float dy = (n.y * config.RigidBodyDefs[c].Position.y);
			float dz = (n.z * config.RigidBodyDefs[c].Position.z);
			float d = dx + dy + dz;

			makePlane(testObj1, n, config.RigidBodyDefs[c], d);
			planeCount++;
			}
			else if (std::strcmp(config.RigidShapeTypes[c].c_str(), "Plane4") == 0)
			{
			cEntity * testObj1 = new cEntity();
			testObj1->m_EntityPhysics->setUniformScale(2.0f);
			testObj1->m_EntityMesh->vecLODMeshs.push_back(sLODInfo("simpleplane4.ply"));
			testObj1->friendlyName = "plane" + std::to_string(planeCount);
			//glm::vec3 orientation = config.RigidBodyDefs[c].Orientation;
			//testObj1->setMeshOrientationEulerAngles(orientation, true);
			//testObj1->setDiffuseColour(glm::vec3(1.0f, 1.0f, 0.0f));
			testObj1->m_EntityMesh->setDiffuseColour(glm::vec3(0.0f, 0.0f, 0.0f));
			testObj1->m_EntityMesh->setSpecularPower(100.0f);
			testObj1->m_EntityMesh->setSpecularColour(glm::vec3(1.000f, 0.766f, 0.336f));
			testObj1->m_EntityMesh->bIsWireFrame = false;
			testObj1->m_EntityMesh->bIsVisible = false;
			testObj1->m_EntityPhysics->physObjType = cEntityPhysics::ePhysicsObjType::RIGID_BODY;

			//sBox* pBox = new sBox();
			sModelDrawInfo cube;
			cube.meshFileName = testObj1->m_EntityMesh->vecLODMeshs[0].meshName;
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

			//	pBox->boxTris.push_back(pTri);
			//}
			//testObj1->pTheShape = pBox;
			//testObj1->shapeType = cEntity::BOX;

			sTextureInfo stone;
			stone.name = "grass.bmp";
			stone.strength = 1.0f;
			testObj1->m_EntityMesh->vecTextures.push_back(stone);

			vec_pObjectsToDraw.push_back(testObj1);

			glm::vec3 n = pTri->n;
			float dx = (n.x * config.RigidBodyDefs[c].Position.x);
			float dy = (n.y * config.RigidBodyDefs[c].Position.y);
			float dz = (n.z * config.RigidBodyDefs[c].Position.z);
			float d = dx + dy + dz;

			makePlane(testObj1, n, config.RigidBodyDefs[c], d);
			planeCount++;
			}
			else
			{
			continue;
			}
			if (std::strcmp(config.RigidShapeTypes[c].c_str(), "Box") == 0)
			{
				cEntity * testObj = new cEntity();
				testObj->m_EntityPhysics->setUniformScale(config.RigidBodyDefs[c].Mass);
				testObj->m_EntityMesh->vecLODMeshs.push_back(sLODInfo("cube_flat_shaded_xyz_n_uv.ply"));
				testObj->friendlyName = "box" + std::to_string(boxCount);
				//glm::vec3 orientation = config.RigidBodyDefs[c].Orientation;
				//testObj->setMeshOrientationEulerAngles(orientation, true);
				testObj->m_EntityMesh->bIsWireFrame = false;
				testObj->m_EntityPhysics->physObjType = cEntityPhysics::ePhysicsObjType::RIGID_BODY;

				//testObj->setDiffuseColour(glm::vec3(1.0f, 223.0f / 255.0f, 0.0f));
				testObj->m_EntityMesh->setDiffuseColour(glm::vec3(0.0f, 0.0f, 0.0f));
				testObj->m_EntityMesh->bUseVertexColour = false;
				// Taken from: https://docs.unrealengine.com/en-us/Engine/Rendering/Materials/PhysicallyBased
				// Note that the specular HIGHLIGHT colour is DIFFERENT from 
				// the diffuse... 
				testObj->m_EntityMesh->setSpecularPower(100.0f);
				testObj->m_EntityMesh->setSpecularColour(glm::vec3(1.000f, 0.766f, 0.336f));

				sTextureInfo stone;
				stone.name = "rock.bmp";
				stone.strength = 1.0f;
				testObj->m_EntityMesh->vecTextures.push_back(stone);

				//testObj->pDebugRenderer = ::g_pDebugRenderer;

				vec_pObjectsToDraw.push_back(testObj);
				//sphereList.push_back(testObj);
				glm::vec3 extents = glm::vec3(config.RigidBodyDefs[c].Mass);
				makeBox(testObj, config.RigidBodyDefs[c], extents);

				if (boxCount == 0)
				{
					makePointPointConstraint(testObj, glm::vec3(1.0f));
				}
				else if (boxCount == 1)
				{
					cEntity* sphereObj = findObjectByFriendlyName("sphere1");

					if (sphereObj != NULL)
					{
						glm::mat4 matA;
						glm::mat4 matB;
						testObj->m_EntityPhysics->rigidBody->GetTransform(matA);
						sphereObj->m_EntityPhysics->rigidBody->GetTransform(matB);
						glm::vec3 posA = glm::vec3(matA[3].x, matA[3].y, matA[3].z);
						glm::vec3 posB = glm::vec3(matB[3].x, matB[3].y, matB[3].z);

						//sphereObj->rigidBody->SetPosition(posA + (glm::vec3(1.0f, 0.0f, 1.0f)));

						glm::vec3 extents = glm::vec3(config.RigidBodyDefs[c].Mass);

						makeBox(testObj, config.RigidBodyDefs[c], extents);

						glm::vec3 pivotA = posB + glm::vec3(1.0f, 0.0f, 0.0f);
						//glm::vec3 pivotA = (posB - posA) + posA;// +glm::vec3(30.0f, 0.0f, 0.0f);
						glm::vec3 pivotB = posA;// +glm::vec3(-1.0f, 0.0f, 0.0f);
						
						//makePointPointConstraint(testObj, sphereObj, pivotA, pivotB);
						//makePointPointConstraint(sphereObj, testObj, pivotB, pivotA);
						//makeHingeConstraint(testObj, pivotA, glm::vec3(0.0f, 1.0f, 0.0f));
						makeHingeConstraint(testObj, sphereObj, pivotA, pivotB, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
					}
				}

				boxCount++;
			}
			else if (std::strcmp(config.RigidShapeTypes[c].c_str(), "Cylinder") == 0)
			{
				cEntity * testObj = new cEntity();
				testObj->m_EntityPhysics->setUniformScale(config.RigidBodyDefs[c].Mass);
				testObj->m_EntityMesh->vecLODMeshs.push_back(sLODInfo("cylinderUV.ply"));
				testObj->friendlyName = "cylinder" + std::to_string(cylinderCount);
				//glm::vec3 orientation = config.RigidBodyDefs[c].Orientation;
				//testObj->setMeshOrientationEulerAngles(orientation, true);
				testObj->m_EntityMesh->bIsWireFrame = false;
				testObj->m_EntityPhysics->physObjType = cEntityPhysics::ePhysicsObjType::RIGID_BODY;

				//testObj->setDiffuseColour(glm::vec3(1.0f, 223.0f / 255.0f, 0.0f));
				testObj->m_EntityMesh->setDiffuseColour(glm::vec3(0.0f, 0.0f, 0.0f));
				testObj->m_EntityMesh->bUseVertexColour = false;
				// Taken from: https://docs.unrealengine.com/en-us/Engine/Rendering/Materials/PhysicallyBased
				// Note that the specular HIGHLIGHT colour is DIFFERENT from 
				// the diffuse... 
				testObj->m_EntityMesh->setSpecularPower(100.0f);
				testObj->m_EntityMesh->setSpecularColour(glm::vec3(1.000f, 0.766f, 0.336f));

				sTextureInfo stone;
				stone.name = "rock.bmp";
				stone.strength = 1.0f;
				testObj->m_EntityMesh->vecTextures.push_back(stone);

				//testObj->pDebugRenderer = ::g_pDebugRenderer;

				vec_pObjectsToDraw.push_back(testObj);
				//sphereList.push_back(testObj);
				makeCylinder(testObj, config.RigidBodyDefs[c]);
				cylinderCount++;

				if (cylinderCount == 1)
				{
					cEntity* sphereObj = findObjectByFriendlyName("sphere4");

					if (sphereObj != NULL)
					{
						makeSliderConstraint(testObj, sphereObj);
					}
				}
			}
			else if (std::strcmp(config.RigidShapeTypes[c].c_str(), "Cone") == 0)
			{
			cEntity * testObj = new cEntity();
			testObj->m_EntityPhysics->setUniformScale(config.RigidBodyDefs[c].Mass);
			testObj->m_EntityMesh->vecLODMeshs.push_back(sLODInfo("coneUV.ply"));
			testObj->friendlyName = "cone" + std::to_string(coneCount);
			//glm::vec3 orientation = config.RigidBodyDefs[c].Orientation;
			//testObj->setMeshOrientationEulerAngles(orientation, true);
			testObj->m_EntityMesh->bIsWireFrame = false;
			testObj->m_EntityPhysics->physObjType = cEntityPhysics::ePhysicsObjType::RIGID_BODY;

			//testObj->setDiffuseColour(glm::vec3(1.0f, 223.0f / 255.0f, 0.0f));
			testObj->m_EntityMesh->setDiffuseColour(glm::vec3(0.0f, 0.0f, 0.0f));
			testObj->m_EntityMesh->bUseVertexColour = false;
			// Taken from: https://docs.unrealengine.com/en-us/Engine/Rendering/Materials/PhysicallyBased
			// Note that the specular HIGHLIGHT colour is DIFFERENT from 
			// the diffuse... 
			testObj->m_EntityMesh->setSpecularPower(100.0f);
			testObj->m_EntityMesh->setSpecularColour(glm::vec3(1.000f, 0.766f, 0.336f));

			sTextureInfo stone;
			stone.name = "rock.bmp";
			stone.strength = 1.0f;
			testObj->m_EntityMesh->vecTextures.push_back(stone);

			//testObj->pDebugRenderer = ::g_pDebugRenderer;

			vec_pObjectsToDraw.push_back(testObj);
			//sphereList.push_back(testObj);
			makeCone(testObj, config.RigidBodyDefs[c]);
			coneCount++;

			if (coneCount == 1)
				{
				cEntity* sphereObj = findObjectByFriendlyName("sphere3");

					if (sphereObj != NULL)
					{
						makeConeTwistConstraint(testObj, sphereObj);
					}
				}
			}
		}


	}
}

void makeSphere(cEntity * obj, nPhysics::sRigidBodyDef bodyDef0)
{
	nPhysics::iSphereShape* sphereShape0 = gPhysicsFactory->CreateSphereShape(bodyDef0.Mass);
	nPhysics::iRigidBody* rigidBody0 = gPhysicsFactory->CreateRigidBody(bodyDef0, sphereShape0);
	gPhysicsWorld->AddBody(rigidBody0);
	obj->m_EntityPhysics->rigidBody = rigidBody0;
}

void makePlane(cEntity * obj, glm::vec3 n, nPhysics::sRigidBodyDef bodyDef0, float d)
{
	nPhysics::iPlaneShape* planeShape0 = gPhysicsFactory->CreatePlaneShape(n, d);
	nPhysics::iRigidBody* rigidBody0 = gPhysicsFactory->CreateRigidBody(bodyDef0, planeShape0);
	gPhysicsWorld->AddBody(rigidBody0);
	obj->m_EntityPhysics->rigidBody = rigidBody0;
}

void makeBox(cEntity * obj, nPhysics::sRigidBodyDef bodyDef0, glm::vec3 extents)
{
	nPhysics::iBoxShape* boxShape0 = gPhysicsFactory->CreateBoxShape(extents);
	nPhysics::iRigidBody* rigidBody0 = gPhysicsFactory->CreateRigidBody(bodyDef0, boxShape0);
	gPhysicsWorld->AddBody(rigidBody0);
	obj->m_EntityPhysics->rigidBody = rigidBody0;
}

void makeCylinder(cEntity * obj, nPhysics::sRigidBodyDef bodyDef0)
{
	glm::vec3 extents = glm::vec3(bodyDef0.Mass);
	extents.x = 1;
	nPhysics::iCylinderShape* cylinderShape0 = gPhysicsFactory->CreateCylinderShape(extents);
	nPhysics::iRigidBody* rigidBody0 = gPhysicsFactory->CreateRigidBody(bodyDef0, cylinderShape0);
	gPhysicsWorld->AddBody(rigidBody0);
	obj->m_EntityPhysics->rigidBody = rigidBody0;
}

void makeCone(cEntity * obj, nPhysics::sRigidBodyDef bodyDef0)
{
	nPhysics::iConeShape* coneShape0 = gPhysicsFactory->CreateConeShape(bodyDef0.Mass, bodyDef0.Mass);
	nPhysics::iRigidBody* rigidBody0 = gPhysicsFactory->CreateRigidBody(bodyDef0, coneShape0);
	gPhysicsWorld->AddBody(rigidBody0);
	obj->m_EntityPhysics->rigidBody = rigidBody0;
}

void makeCapsule(cEntity * obj, nPhysics::sRigidBodyDef bodyDef0, float radius, float height)
{
	nPhysics::iConeShape* coneShape0 = gPhysicsFactory->CreateConeShape(radius, height);
	nPhysics::iRigidBody* rigidBody0 = gPhysicsFactory->CreateRigidBody(bodyDef0, coneShape0);
	gPhysicsWorld->AddBody(rigidBody0);
	obj->m_EntityPhysics->rigidBody = rigidBody0;
}

void makePointPointConstraint(cEntity * obj, glm::vec3 pivot)
{
	nPhysics::iRigidBody* rigidBody0 = obj->m_EntityPhysics->rigidBody;
	nPhysics::iRigidBody* rigidBody1 = obj->m_EntityPhysics->rigidBody;
	nPhysics::iConstraint* constraint = gPhysicsFactory->CreatePointPointConstraint(rigidBody0, pivot);
	gPhysicsWorld->AddConstraint(constraint);
}

void makePointPointConstraint(cEntity * objA, cEntity * objB, glm::vec3 pivotA, glm::vec3 pivotB)
{
	nPhysics::iRigidBody* rigidBody0 = objA->m_EntityPhysics->rigidBody;
	nPhysics::iRigidBody* rigidBody1 = objB->m_EntityPhysics->rigidBody;
	//nPhysics::iConstraint* constraint = gPhysicsFactory->CreatePointPointConstraint(rigidBody0, pivotA);
	nPhysics::iConstraint* constraint = gPhysicsFactory->CreatePointPointConstraint(rigidBody0, rigidBody1, pivotA, pivotB);
	gPhysicsWorld->AddConstraint(constraint);
}

void makeHingeConstraint(cEntity * obj, glm::vec3 pivot, glm::vec3 axis)
{
	nPhysics::iRigidBody* rigidBody0 = obj->m_EntityPhysics->rigidBody;
	nPhysics::iConstraint* constraint = gPhysicsFactory->CreateHingeConstraint(rigidBody0, pivot, axis);
	gPhysicsWorld->AddConstraint(constraint);
}

void makeHingeConstraint(cEntity * objA, cEntity* objB, glm::vec3 pivotA, glm::vec3 pivotB, glm::vec3 axisA, glm::vec3 axisB)
{
	nPhysics::iRigidBody* rigidBody0 = objA->m_EntityPhysics->rigidBody;
	nPhysics::iRigidBody* rigidBody1 = objB->m_EntityPhysics->rigidBody;
	nPhysics::iConstraint* constraint = gPhysicsFactory->CreateHingeConstraint(rigidBody0, rigidBody1, pivotA, pivotB, axisA, axisB);
	gPhysicsWorld->AddConstraint(constraint);
}

void makeConeTwistConstraint(cEntity * obj)
{
	nPhysics::iRigidBody* rigidBody0 = obj->m_EntityPhysics->rigidBody;
	nPhysics::iConstraint* constraint = gPhysicsFactory->CreateConeTwistConstraint(rigidBody0);
	gPhysicsWorld->AddConstraint(constraint);
}

void makeConeTwistConstraint(cEntity * objA, cEntity* objB)
{
	nPhysics::iRigidBody* rigidBody0 = objA->m_EntityPhysics->rigidBody;
	nPhysics::iRigidBody* rigidBody1 = objB->m_EntityPhysics->rigidBody;
	nPhysics::iConstraint* constraint = gPhysicsFactory->CreateConeTwistConstraint(rigidBody0, rigidBody1);
	gPhysicsWorld->AddConstraint(constraint);
}

void makeSliderConstraint(cEntity * obj)
{
	nPhysics::iRigidBody* rigidBody0 = obj->m_EntityPhysics->rigidBody;
	nPhysics::iConstraint* constraint = gPhysicsFactory->CreateConeTwistConstraint(rigidBody0);
	gPhysicsWorld->AddConstraint(constraint);
}

void makeSliderConstraint(cEntity * objA, cEntity* objB)
{
	nPhysics::iRigidBody* rigidBody0 = objA->m_EntityPhysics->rigidBody;
	nPhysics::iRigidBody* rigidBody1 = objB->m_EntityPhysics->rigidBody;
	nPhysics::iConstraint* constraint = gPhysicsFactory->CreateConeTwistConstraint(rigidBody0, rigidBody1);
	gPhysicsWorld->AddConstraint(constraint);
}

void makeSoftBody(cEntity * obj, nPhysics::sSoftBodyDef bodyDef0)
{
	nPhysics::iSoftBody* softBody0 = gPhysicsFactory->CreateSoftBody(bodyDef0);
	gPhysicsWorld->AddBody(softBody0);
	obj->m_EntityPhysics->softBody = softBody0;
}