#ifndef _C_CAMERA_HG_
#define _C_CAMERA_HG_

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL		// To get glm quaternion stuff to compile
#include <glm/gtx/quaternion.hpp>

class cCamera
{
public:
	cCamera();
	~cCamera();

	enum eCamType
	{
		FREE = 0,
		FIRST = 1,
		THIRD = 2
	};

	enum eCamMovement
	{
		FORWARD = 0,
		BACKWARD = 1,
		UP = 2,
		DOWN = 3,
		LEFT = 4,
		RIGHT = 5,
	};

	glm::vec3 eye;
	glm::vec3 targetPos;
	glm::vec3 m_up;
	glm::vec3 m_forward;
	glm::vec3 m_right;

	bool bKeepCameraFacingUp;

	void MoveForward_Z(float amount);
	void MoveLeftRight_X(float amount);
	void MoveUpDown_Y(float amount);

	glm::vec3 getAtInWorldSpace(void);
	glm::vec3 getCameraDirection(void);
	glm::vec3 getUpVector(void);

	void Pitch_UpDown(float angleDegrees);	// around X
	void Yaw_LeftRight(float angleDegrees);	// around y
	void Roll_CW_CCW(float angleDegrees);	// around z

	glm::quat getQOrientation(void) { return this->qOrientation; };
	void setMeshOrientationEulerAngles(glm::vec3 newAnglesEuler, bool bIsDegrees = false);
	void setMeshOrientationEulerAngles(float x, float y, float z, bool bIsDegrees = false);
	void adjMeshOrientationEulerAngles(glm::vec3 adjAngleEuler, bool bIsDegrees = false);
	void adjMeshOrientationEulerAngles(float x, float y, float z, bool bIsDegrees = false);
	void adjMeshOrientationQ(glm::quat adjOrientQ);
	void m_UpdateAtFromOrientation(void);
	void m_UpdateUpFromOrientation(void);

private:
	glm::quat qOrientation;
	float mYaw;
	float mPitch;

};

#endif
