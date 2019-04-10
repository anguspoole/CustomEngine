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
	glm::vec3 up;
	glm::vec3 forward;
	glm::vec3 right;

	glm::vec3 getAtInWorldSpace(void);

private:
	glm::quat qOrientation;
	float mYaw;
	float mPitch;
};

#endif
