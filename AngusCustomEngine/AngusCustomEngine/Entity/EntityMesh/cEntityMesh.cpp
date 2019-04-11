#include "cEntityMesh.h"



cEntityMesh::cEntityMesh()
{
	this->uniformScale = 1.0f;

	this->bIsVisible = true;
	this->bIsWireFrame = false;
	this->bIsALightVolume = false;

	this->renderPassNumber = 0;

	this->materialDiffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	this->materialSpecular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	this->bUseVertexColour = false;
	this->bDontLight = false;

	// Skinned mesh and animations:
	this->pSimpleSkinnedMesh = NULL;
	this->pAniState = NULL;
}


cEntityMesh::~cEntityMesh()
{
}

void cEntityMesh::setDiffuseColour(glm::vec3 newDiffuse)
{
	this->materialDiffuse = glm::vec4(newDiffuse, this->materialDiffuse.a);
	return;
}

void cEntityMesh::setAlphaTransparency(float newAlpha)
{
	// RGB A
	this->materialDiffuse.a = newAlpha;
	return;
}

void cEntityMesh::setSpecularColour(glm::vec3 colourRGB)
{
	this->materialSpecular.r = colourRGB.r;
	this->materialSpecular.g = colourRGB.g;
	this->materialSpecular.b = colourRGB.b;
	return;
}

void cEntityMesh::setSpecularPower(float specPower)
{
	this->materialSpecular.a = specPower;
	return;
}
