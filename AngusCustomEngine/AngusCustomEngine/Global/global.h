#ifndef _GLOBAL_HG_
#define _GLOBAL_HG_

/*
	This file declares various global functions and variables
	It is also used for various includes that will be common throughout the program
*/

#include <string.h>
#include <iostream>

#include "../Camera/cCamera.h"
#include "../Meshes/sModelDrawInfo.h"
#include "../Meshes/cVAOMeshManager.h"

#include "../Meshes/AssimpFBX/cSimpleAssimpSkinnedMeshLoader_OneMesh.h"

extern cCamera* g_Camera;
extern cVAOMeshManager* g_VAOMeshManager;

#endif