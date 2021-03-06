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
#include <sModelPoint.h>

#include "gOpenGL.h"
#include "globalParticle.h"

#include "../Camera/cCamera.h"
#include "../Meshes/sModelDrawInfo.h"
#include "../Meshes/cVAOMeshManager.h"
#include "../Meshes/AssimpFBX/cSimpleAssimpSkinnedMeshLoader_OneMesh.h"

#include "../TextureManager/cBasicTextureManager.h"
#include "../Lighting/cLightManager.h"
#include "../Shading/cShaderManager.h"

#include "../Entity/cEntity.h"
#include "../Meshes/AssimpFBX/cAnimationState.h"
#include "../Particles/cParticleEmitter.h"

extern cCamera* g_Camera;
extern cCamera* player_Camera;
extern cCamera* pass_Camera;
extern cCamera* currentCamera;
extern cVAOMeshManager* g_VAOMeshManager;
extern cBasicTextureManager* g_TheTextureManager;
extern cLightManager* g_LightManager;
extern cShaderManager* pTheShaderManager;

extern cEntity* player;
extern std::vector<cEntity*> enemyList;
extern std::vector<cEntity*> globList;
extern std::vector<cEntity*> paintList;

//extern std::vector<cEntity*> vec_pObjectsToDraw;

extern float timeRemaining;

// Skinned mesh object 
extern cSimpleAssimpSkinnedMesh* g_pRPGSkinnedMesh; //= NULL;
extern std::vector<cSimpleAssimpSkinnedMesh*> skinnedMeshes;

cEntity* findObjectByFriendlyName(std::string theNameToFind);
void UpdateWindowTitle(GLFWwindow* window, std::string title);

void LoadModelTypes_ASYNC(cVAOMeshManager* pTheVAOMeshManager, GLuint shaderProgramID);
void LoadModelsIntoScene(std::vector<cEntity*> &vec_pObjectsToDraw);
// Player skinned mesh model
void LoadPlayerMeshModel(const nLoad::sConfig& config, int c, std::vector<cEntity*> &vec_pObjectsToDraw,
	GLuint shaderProgramID);
// Enemy skinned mesh model
cSimpleAssimpSkinnedMesh* LoadEnemyMeshModel(const nLoad::sConfig& config, int c, std::vector<cEntity*> &vec_pObjectsToDraw,
	GLuint shaderProgramID);
void CreateAndAssignAnimatedEnemy(const nLoad::sConfig& config, int c, std::vector<cEntity*> &vec_pObjectsToDraw,
	GLuint shaderProgramID, cSimpleAssimpSkinnedMesh* enemySkinnedMesh);
void LoadPaintGlob(std::vector<cEntity*> &vec_pObjectsToDraw, GLuint shaderProgramID, glm::vec3 startPos, glm::vec3 startVel);
void LoadPaintCube(std::vector<cEntity*> &vec_pObjectsToDraw, glm::vec3 startPos);
void setUpParticleEmitters(void);

void DrawObject(cEntity* pCurrentEntity, glm::mat4x4 &matModel, GLuint shaderProgramID, unsigned int RenderPassNumber, cFBO* fbo);
void DrawScene_Simple(std::vector<cEntity*> vec_pEntities, GLuint shaderProgramID, unsigned int passNumber, cFBO* fbo);



//Term 2 Physics Stuff
extern nPhysics::iPhysicsFactory* gPhysicsFactory; //global physics factory
extern nPhysics::iPhysicsWorld* gPhysicsWorld;
bool InitPhysicsStuff(const nLoad::sConfig& config);
bool buildPhysicsWorld();
void buildPhysicsObjects(const nLoad::sConfig& config, std::vector< cEntity* > &vec_pObjectsToDraw, GLuint shaderProgramID);
void removeRB(cEntity* entity);

//Physics Constructors
void makeSphere(cEntity * obj, nPhysics::sRigidBodyDef bodyDef0);
void makeSphere(cEntity * obj, nPhysics::sRigidBodyDef bodyDef0, int group, int mask);
void makePlane(cEntity * obj, glm::vec3 n, nPhysics::sRigidBodyDef bodyDef0, float d);
void makePlane(cEntity * obj, glm::vec3 n, nPhysics::sRigidBodyDef bodyDef0, float d, int group, int mask);
void makeSoftBody(cEntity * obj, nPhysics::sSoftBodyDef bodyDef0);
void makeBox(cEntity * obj, nPhysics::sRigidBodyDef bodyDef0, glm::vec3 extents);
void makeBox(cEntity * obj, nPhysics::sRigidBodyDef bodyDef0, glm::vec3 extents, int group, int mask);
void makeCylinder(cEntity * obj, nPhysics::sRigidBodyDef bodyDef0);
void makeCylinder(cEntity * obj, nPhysics::sRigidBodyDef bodyDef0, glm::vec3 extents);
void makeCylinder(cEntity * obj, nPhysics::sRigidBodyDef bodyDef0, glm::vec3 extents, int group, int mask);
void makeCone(cEntity * obj, nPhysics::sRigidBodyDef bodyDef0);
void makeCone(cEntity * obj, nPhysics::sRigidBodyDef bodyDef0, int group, int mask);
void makeCapsule(cEntity * obj, nPhysics::sRigidBodyDef bodyDef0, float radius, float height);
void makeCapsule(cEntity * obj, nPhysics::sRigidBodyDef bodyDef0, float radius, float height, int group, int mask);
void makeConvexHull(cEntity * obj, nPhysics::sRigidBodyDef bodyDef0, nPhysics::sModelPoint* point, size_t numPoints);

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
//extern cFBO* g_pFBOMid;
extern cFBO* g_pFBOBlurA;
extern cFBO* g_pFBOBlurB;
extern cFBO* g_pFBOFinal;


extern double deltaTime;
extern std::map<std::string, float> mapAnimToDuration;
extern std::map<std::string, float> mapAnimToSpeed;
void resetHackTime(cEntity* entity);

//Update entities that are taking damage
void CheckForHits();
bool CheckForHits(cEntity* enemy, cEntity* player, cEntity* weapon);
bool CheckForPlayerHit(cEntity* player, cEntity* weapon, cEntity* enemy);
void SpawnGlob(cEntity* obj, std::vector<cEntity*>& vec_pObjectsToDraw, GLuint program);
void CheckForGlobHits();

void makeBlur(GLuint programID, cFBO* fboA, cFBO* fboB, GLuint program);

#endif