#include "cCamera.h"

cCamera::cCamera()
	: eye(0.0f, 0.0f, 0.0f),
	targetPos(0.0f, 0.0f, 1.0f),
	up(0.0f, 1.0f, 0.0f),
	forward(0.0f, 0.0f, 1.0f),
	right(1.0f, 0.0f, 0.0f)
{
	this->mYaw = 0.0f;
	this->mPitch = 0.0f;
}


cCamera::~cCamera()
{
}

glm::vec3 cCamera::getAtInWorldSpace()
{
	// The "At" is relative to the where the camera is...
	return this->eye + this->targetPos;
}
