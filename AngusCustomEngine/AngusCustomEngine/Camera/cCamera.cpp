#include "cCamera.h"

cCamera::cCamera()
	: eye(0.0f, 0.0f, 0.0f),
	targetPos(0.0f, 0.0f, 1.0f),
	m_up(0.0f, 1.0f, 0.0f),
	m_forward(0.0f, 0.0f, 1.0f),
	m_right(1.0f, 0.0f, 0.0f)
{
	//this->mYaw = 0.0f;
	//this->mPitch = 0.0f;

	// Set initial orientation (all zero on Euler axes)
	this->qOrientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));

	this->m_UpdateAtFromOrientation();
	this->m_UpdateUpFromOrientation();

	// If we are using the mouse
	this->setMouseXY(0.0, 0.0);
	this->m_MouseWheel = 0.0f;

	this->m_Mouse_Initial_State_Is_Set = false;

	this->bKeepCameraFacingUp = true;

}


cCamera::~cCamera()
{
}

glm::vec3 cCamera::getAtInWorldSpace()
{
	// The "At" is relative to the where the camera is...
	return this->eye + this->targetPos;
}

glm::vec3 cCamera::getCameraDirection(void)
{
	this->m_UpdateAtFromOrientation();

	return this->targetPos;
}

void cCamera::MoveForward_Z(float amount)
{
	// We take the vector from the at to eye (i.e. direction)
	// Add this forward velocity along this direction.

	glm::vec3 direction = this->getAtInWorldSpace() - this->eye;

	// Make direction a "unit length"
	direction = glm::normalize(direction);

	// Generate a "forward" adjustment value 
	glm::vec3 amountToMove = direction * amount;

	// Add this to the eye
	this->eye += amountToMove;

	return;
}

void cCamera::MoveUpDown_Y(float amount)
{
	// We use the Up vector to determine which direction (in world space) we need to go

	glm::vec3 vecAmountToMove = glm::normalize(this->getUpVector()) * amount;

	this->eye += vecAmountToMove;

	return;
}

void cCamera::MoveLeftRight_X(float amount)
{
	// left and right is a little tricky, as we can "roll", 
	// because left and right change in world space.
	// 
	// If you perform a cross product between the "forward" and "up" vectors, you'll
	//	get a vector perpendicular to both of these, so left and right.
	// Example: Assuming "forward" is the Z axis, and "up" is the Y axis, then 
	//	performing the cross product of these gives you the X axis. 

	glm::vec3 vecLeft = glm::cross(this->getCameraDirection(), this->getUpVector());

	glm::vec3 vecAmountToMove = glm::normalize(vecLeft) * amount;

	this->eye += vecAmountToMove;

	return;
}

void cCamera::Pitch_UpDown(float angleDegrees)	// around X
{
	// Adjust the orientation of the camera by the number of degrees
	this->adjMeshOrientationEulerAngles(glm::vec3(angleDegrees, 0.0f, 0.0f), true);

	this->m_UpdateAtFromOrientation();
	this->m_UpdateUpFromOrientation();

	return;
}

void cCamera::Yaw_LeftRight(float angleDegrees)	// around y
{
	// Adjust the orientation of the camera by the number of degrees
	this->adjMeshOrientationEulerAngles(glm::vec3(0.0f, angleDegrees, 0.0f), true);

	this->m_UpdateAtFromOrientation();
	this->m_UpdateUpFromOrientation();

	return;
}

void cCamera::setMeshOrientationEulerAngles(glm::vec3 newAnglesEuler, bool bIsDegrees /*=false*/)
{
	if (bIsDegrees)
	{
		newAnglesEuler = glm::vec3(glm::radians(newAnglesEuler.x),
			glm::radians(newAnglesEuler.y),
			glm::radians(newAnglesEuler.z));
	}

	this->qOrientation = glm::quat(glm::vec3(newAnglesEuler.x, newAnglesEuler.y, newAnglesEuler.z));
	return;
}

void cCamera::setMeshOrientationEulerAngles(float x, float y, float z, bool bIsDegrees /*=false*/)
{
	return this->setMeshOrientationEulerAngles(glm::vec3(x, y, z), bIsDegrees);
}

void cCamera::setMeshOrientationQ(glm::quat newOrientQ)
{
	this->qOrientation = newOrientQ;
}

void cCamera::adjMeshOrientationEulerAngles(glm::vec3 adjAngleEuler, bool bIsDegrees /*=false*/)
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
	this->qOrientation *= rotationAdjust;
	this->qOrientation = this->qOrientation * rotationAdjust;

	return;
}

void cCamera::adjMeshOrientationEulerAngles(float x, float y, float z, bool bIsDegrees /*=false*/)
{
	return this->adjMeshOrientationEulerAngles(glm::vec3(x, y, z), bIsDegrees);
}

void cCamera::adjMeshOrientationQ(glm::quat adjOrientQ)
{
	this->qOrientation *= adjOrientQ;
	return;
}

void cCamera::m_UpdateAtFromOrientation(void)
{
	// Have a rotation around the origin (eye)

	// vec4 new = mat4Transform * (vec4)Location

	// c'tor of mat4 can take a quaternion and generate 
	//	a rotation matrix from it... 
	glm::mat4 matRotation = glm::mat4(this->qOrientation);

	// Need to make these 4x4
	// NOTE: You could just keep these as 3x3 matrix values
	// (so you could keep them as vec3's....)

	glm::vec4 frontOfCamera = glm::vec4(this->m_forward, 1.0f);

	glm::vec4 newAt = matRotation * frontOfCamera;

	// Update the "At"
	this->targetPos = glm::vec3(newAt);


	return;
}

void cCamera::m_UpdateUpFromOrientation(void)
{
	if (this->bKeepCameraFacingUp)
	{
		this->m_up = glm::vec3(0.0f, 1.0f, 0.0f);
	}
	else
	{
		glm::mat4 matRotation = glm::mat4(this->qOrientation);

		glm::vec4 upVector = glm::vec4(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f);

		glm::vec4 newUp = matRotation * upVector;

		// Update the "At"
		this->m_up = glm::vec3(newUp);
	}

	return;
}

glm::vec3 cCamera::getUpVector(void)
{
	return this->m_up;
}

void cCamera::setMouseXY(double newX, double newY)
{
	this->m_lastMouse_X = this->m_Mouse_X;
	this->m_Mouse_X = (float)newX;

	this->m_lastMouse_Y = this->m_Mouse_Y;
	this->m_Mouse_Y = (float)newY;

	// Can return value numbers
	this->m_Mouse_Initial_State_Is_Set = true;

	return;
}

void cCamera::setMouseWheelDelta(double deltaWheel)
{
	this->m_lastMouse_Wheel = this->m_MouseWheel;
	this->m_MouseWheel += (float)deltaWheel;
	return;
}

void cCamera::resetMouseWheel()
{
	this->m_lastMouse_Wheel = 0.0f;
	this->m_MouseWheel = 0.0f;
	return;
}

float cCamera::getMouseX(void)
{
	return this->m_Mouse_X;
}

float cCamera::getMouseY(void)
{
	return this->m_Mouse_Y;
}

float cCamera::getDeltaMouseX(void)
{
	if (!this->m_Mouse_Initial_State_Is_Set)
	{
		return 0.0f;
	}
	return this->m_Mouse_X - this->m_lastMouse_X;
}

float cCamera::getDeltaMouseY(void)
{
	if (!this->m_Mouse_Initial_State_Is_Set)
	{
		return 0.0f;
	}
	return this->m_Mouse_Y - this->m_lastMouse_Y;
}

float cCamera::getMouseWheel(void)
{
	return this->m_MouseWheel;
}