#ifndef _C_ENTITY_MESH_HG_
#define _C_ENTITY_MESH_HG_

//This class holds mesh/graphics-related info for cEntity objects

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#define GLM_ENABLE_EXPERIMENTAL		// To get glm quaternion stuff to compile
#include <glm/gtx/quaternion.hpp>	// Note strange folder
#include <string>
#include <vector>

// Add the ability to add debug shapes to the debug renderer
// **NOTE** it's the INTERFACE that I have, NOT the class
//#include "DebugRenderer/iDebugRenderer.h"


// This is for the skinned mesh and animation
class cSimpleAssimpSkinnedMesh;	// Forward declare
class cAnimationState;			// Forward declare 

struct sTextureInfo
{
	std::string name;
	int cachedTextureID; 	// Set to -1 by default
	float strength;		// Set to 0.0f by default
	// Added: specific FBOs
};

struct sLODInfo
{
	sLODInfo() :
		minDistance(FLT_MAX) {}
	std::string meshName;	// Model to draw
	sLODInfo(std::string meshName_)
	{
		this->meshName = meshName_;
		this->minDistance = FLT_MAX;
	};
	sLODInfo(std::string meshName_, float minDistanceToDraw)
	{
		this->meshName = meshName_;
		this->minDistance = minDistanceToDraw;
	};

	// If it's at or closer than this, draw this model
	float minDistance;
};

class cEntityMesh
{
public:
	cEntityMesh();
	~cEntityMesh();

	bool bIsWireFrame;
	bool bIsVisible;
	bool bHasNormalMap = false;

	// If there's only 1, then we AREN'T using LOD
	std::vector<sLODInfo> vecLODMeshs;

	bool b_HACK_UsesOffscreenFBO = false;

	//For FBOs
	unsigned int renderPassNumber;

	glm::vec4 materialDiffuse;		// RGB+Alpha
	void setDiffuseColour(glm::vec3 newDiffuse);
	void setAlphaTransparency(float newAlpha);
	glm::vec4 materialSpecular;		// RGB+specular power
	void setSpecularColour(glm::vec3 colourRGB);
	void setSpecularPower(float specPower);

	bool bUseVertexColour;		// = false (default)
	bool bDontLight;		// If true, just object colour is used

	// If true, then will draw both front and back.
	bool bIsALightVolume;

	//Textures
	std::vector<sTextureInfo> vecTextures;

	// Skinned mesh and animations:
	// If NULL, then object ISN'T a skinned mesh
	cSimpleAssimpSkinnedMesh*	pSimpleSkinnedMesh;

	// HACK
	std::string currentAnimation; //the current animation of the mesh
	cAnimationState*	pAniState;

	// Extent Values for skinned mesh
	// These can be updated per frame, from the "update skinned mesh" call
	glm::vec3 minXYZ_from_SM_Bones;
	glm::vec3 maxXYZ_from_SM_Bones;
	// Store all the bones for this model, being updated per frame
	std::vector< glm::mat4x4 > vecObjectBoneTransformation;
};

#endif // !_C_ENTITY_MESH_HG_
