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

	float minDist = 42.0f;
	float maxDist = 50.0f;

	bool tooFar = false;
	bool tooClose = false;
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
	void setMeshOrientationQ(glm::quat newOrientQ);
	void adjMeshOrientationEulerAngles(glm::vec3 adjAngleEuler, bool bIsDegrees = false);
	void adjMeshOrientationEulerAngles(float x, float y, float z, bool bIsDegrees = false);
	void adjMeshOrientationQ(glm::quat adjOrientQ);
	void m_UpdateAtFromOrientation(void);
	void m_UpdateUpFromOrientation(void);
	
	// These set methods will also clear the "last" mouse positions
	void setMouseXY(double newX, double newY);
	// The mouse wheel (on windows) only returns the delta, or the amount you scrolled
	void setMouseWheelDelta(double deltaWheel);
	void resetMouseWheel();

	float getMouseX(void);				// ...but we only need floats
	float getMouseY(void);

	float getDeltaMouseX(void);
	float getDeltaMouseY(void);
	float getMouseWheel(void);

	float zoomAmount = 0.0f;

private:
	glm::quat qOrientation;
	float mYaw;
	float mPitch;

	float m_lastMouse_X;
	float m_lastMouse_Y;
	float m_lastMouse_Wheel;
	float m_Mouse_X;
	float m_Mouse_Y;
	float m_MouseWheel;
	// We don't know where the mouse is at the start, 
	//	so things like getDeltaMouse() methods will return 
	//	crazy things... this flag indicates the values are OK
	bool m_Mouse_Initial_State_Is_Set;

};

#endif
