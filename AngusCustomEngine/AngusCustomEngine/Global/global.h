#ifndef _GLOBAL_HG_
#define _GLOBAL_HG_

/*
	This file declares various global functions and variables
	It is also used for various includes that will be common throughout the program
*/

#include <string.h>
#include <map>
#include <iostream>

#include <glm/glm.hpp>

#include <iPhysicsFactory.h>
#include <physics_interfaces.h>
#include <../load/sConfig.h>

#include "gOpenGL.h"

#include "../Camera/cCamera.h"
#include "../Meshes/sModelDrawInfo.h"
#include "../Meshes/cVAOMeshManager.h"
#include "../Meshes/AssimpFBX/cSimpleAssimpSkinnedMeshLoader_OneMesh.h"

#include "../TextureManager/cBasicTextureManager.h"
#include "../Lighting/cLightManager.h"

#include "../Entity/cEntity.h"
#include "../Meshes/AssimpFBX/cAnimationState.h"

extern cCamera* g_Camera;
extern cCamera* player_Camera;
extern cCamera* currentCamera;
extern cVAOMeshManager* g_VAOMeshManager;
extern cBasicTextureManager* g_TheTextureManager;
extern cLightManager* g_LightManager;

extern cEntity* player;
extern std::vector<cEntity*> enemyList;
extern std::vector<cEntity*> globList;
extern std::vector<cEntity*> paintList;
//extern std::vector<cEntity*> vec_pObjectsToDraw;

// Skinned mesh object 
extern cSimpleAssimpSkinnedMesh* g_pRPGSkinnedMesh; //= NULL;
extern std::vector<cSimpleAssimpSkinnedMesh*> skinnedMeshes;

cEntity* findObjectByFriendlyName(std::string theNameToFind);

void LoadModelTypes_ASYNC(cVAOMeshManager* pTheVAOMeshManager, GLuint shaderProgramID);
void LoadModelsIntoScene(std::vector<cEntity*> &vec_pObjectsToDraw);
// Player skinned mesh model
void LoadPlayerMeshModel(const nLoad::sConfig& config, int c, std::vector<cEntity*> &vec_pObjectsToDraw,
	GLuint shaderProgramID);
// Enemy skinned mesh model
void LoadEnemyMeshModel(const nLoad::sConfig& config, int c, std::vector<cEntity*> &vec_pObjectsToDraw,
	GLuint shaderProgramID);
void LoadPaintGlob(std::vector<cEntity*> &vec_pObjectsToDraw, GLuint shaderProgramID, glm::vec3 startPos, glm::vec3 startVel);
void LoadPaintCube(std::vector<cEntity*> &vec_pObjectsToDraw, glm::vec3 startPos);

void DrawObject(cEntity* pCurrentEntity, glm::mat4x4 &matModel, GLuint shaderProgramID, unsigned int RenderPassNumber, cFBO* fbo);
void DrawScene_Simple(std::vector<cEntity*> vec_pEntities, GLuint shaderProgramID, unsigned int passNumber, cFBO* fbo);



//Term 2 Physics Stuff
extern nPhysics::iPhysicsFactory* gPhysicsFactory; //global physics factory
extern nPhysics::iPhysicsWorld* gPhysicsWorld;
bool InitPhysicsStuff(const nLoad::sConfig& config);
bool buildPhysicsWorld();
void buildPhysicsObjects(const nLoad::sConfig& config, std::vector< cEntity* > &vec_pObjectsToDraw, GLuint shaderProgramID);
void makeSphere(cEntity * obj, nPhysics::sRigidBodyDef bodyDef0);
void makePlane(cEntity * obj, glm::vec3 n, nPhysics::sRigidBodyDef bodyDef0, float d);
void makeSoftBody(cEntity * obj, nPhysics::sSoftBodyDef bodyDef0);
void makeBox(cEntity * obj, nPhysics::sRigidBodyDef bodyDef0, glm::vec3 extents);
void makeCylinder(cEntity * obj, nPhysics::sRigidBodyDef bodyDef0);
void makeCylinder(cEntity * obj, nPhysics::sRigidBodyDef bodyDef0, glm::vec3 extents);
void makeCone(cEntity * obj, nPhysics::sRigidBodyDef bodyDef0);
void makeCapsule(cEntity * obj, nPhysics::sRigidBodyDef bodyDef0, float radius, float height);

void makePointPointConstraint(cEntity* objA, glm::vec3 pivot);
void makePointPointConstraint(cEntity* objA, cEntity* objB, glm::vec3 pivotA, glm::vec3 pivotB);
void makeHingeConstraint(cEntity * obj, glm::vec3 pivot, glm::vec3 axis);
void makeHingeConstraint(cEntity * objA, cEntity* objB, glm::vec3 pivotA, glm::vec3 pivotB, glm::vec3 axisA, glm::vec3 axisB);
void makeConeTwistConstraint(cEntity * objA);
void makeConeTwistConstraint(cEntity * objA, cEntity* objB);
void makeSliderConstraint(cEntity * objA);
void makeSliderConstraint(cEntity * objA, cEntity* objB);
void makeFixedConstraint(cEntity * objA, cEntity* objB);

extern nLoad::sConfig config;

extern bool animationComplete; //boolean for if current animation is done

// The main Frame Buffer Object (off screen rendering)
// (You can create one of these for each off screen rendered thing, if you'd like)
extern cFBO* g_pFBOMain;

extern double deltaTime;
extern std::map<std::string, float> mapAnimToDuration;
void resetHackTime(cEntity* entity);

//Update entities that are taking damage
void CheckForHits();
void SpawnGlob(cEntity* obj, std::vector<cEntity*>& vec_pObjectsToDraw, GLuint program);

#endif