#ifndef _GLOBAL_HG_
#define _GLOBAL_HG_

/*
	This file declares various global functions and variables
	It is also used for various includes that will be common throughout the program
*/

#include <string.h>
#include <iostream>

#include <glm/glm.hpp>

#include "gOpenGL.h"

#include "../Camera/cCamera.h"
#include "../Meshes/sModelDrawInfo.h"
#include "../Meshes/cVAOMeshManager.h"
#include "../Meshes/AssimpFBX/cSimpleAssimpSkinnedMeshLoader_OneMesh.h"

#include "../TextureManager/cBasicTextureManager.h"
#include "../Lighting/cLightManager.h"

#include "../Entity/cEntity.h"

extern cCamera* g_Camera;
extern cVAOMeshManager* g_VAOMeshManager;
extern cBasicTextureManager* g_TheTextureManager;
extern cLightManager* g_LightManager;

cEntity* findObjectByFriendlyName(std::string theNameToFind);

void LoadModelTypes_ASYNC(cVAOMeshManager* pTheVAOMeshManager, GLuint shaderProgramID);
void LoadModelsIntoScene(std::vector<cEntity*> &vec_pObjectsToDraw);
void DrawObject(cEntity* pCurrentEntity, glm::mat4x4 &matModel, GLuint shaderProgramID, unsigned int RenderPassNumber, cFBO* fbo);
void DrawScene_Simple(std::vector<cEntity*> vec_pEntities, GLuint shaderProgramID, unsigned int passNumber, cFBO* fbo);

#endif