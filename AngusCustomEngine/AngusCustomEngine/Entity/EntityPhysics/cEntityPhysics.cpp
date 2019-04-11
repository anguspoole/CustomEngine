#include "cEntityPhysics.h"



cEntityPhysics::cEntityPhysics()
{
	this->bIsUpdatedByPhysics = false;	// physics ignores by default
	this->position = glm::vec3(0.0f);
	this->velocity = glm::vec3(0.0f);
	this->accel = glm::vec3(0.0f);
	this->setMeshOrientationEulerAngles(glm::vec3(0.0f, 0.0f, 0.0f));
	this->uniformScale = 1.0f;
}


cEntityPhysics::~cEntityPhysics()
{
}

void cEntityPhysics::setMeshOrientationEulerAngles(glm::vec3 newAnglesEuler, bool bIsDegrees /*=false*/)
{
	if (bIsDegrees)
	{
		newAnglesEuler = glm::vec3(glm::radians(newAnglesEuler.x),
			glm::radians(newAnglesEuler.y),
			glm::radians(newAnglesEuler.z));
	}

	this->m_meshQOrientation = glm::quat(glm::vec3(newAnglesEuler.x, newAnglesEuler.y, newAnglesEuler.z));
	return;
}

void cEntityPhysics::setMeshOrientationEulerAngles(float x, float y, float z, bool bIsDegrees /*=false*/)
{
	return this->setMeshOrientationEulerAngles(glm::vec3(x, y, z), bIsDegrees);
}

void cEntityPhysics::adjMeshOrientationEulerAngles(glm::vec3 adjAngleEuler, bool bIsDegrees /*=false*/)
{
	if (bIsDegrees)
	{
		adjAngleEuler = glm::vec3(glm::radians(adjAngleEuler.x),
			glm::radians(adjAngleEuler.y),
			glm::radians(adjAngleEuler.z));
	}

	// Step 1: make a quaternion that represents the angle we want to rotate
	glm::quat rotationAdjust(adjAngleEuler);

	// Step 2: Multiply this quaternion by the existing quaternion. This "adds" the angle we want.
	this->m_meshQOrientation *= rotationAdjust;
	this->m_meshQOrientation = this->m_meshQOrientation * rotationAdjust;

	return;
}

void cEntityPhysics::adjMeshOrientationEulerAngles(float x, float y, float z, bool bIsDegrees /*=false*/)
{
	return this->adjMeshOrientationEulerAngles(glm::vec3(x, y, z), bIsDegrees);
}

void cEntityPhysics::adjMeshOrientationQ(glm::quat adjOrientQ)
{
	this->m_meshQOrientation *= adjOrientQ;
	return;
}

void cEntityPhysics::Update(double deltaTime)
{
	if (this->bIsUpdatedByPhysics)
	{

	}//if ( this->bIsUpdatedByPhysics )

	return;
}
