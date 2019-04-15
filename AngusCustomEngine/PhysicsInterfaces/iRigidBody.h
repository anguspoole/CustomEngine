#pragma once

#include <game_math.h>

namespace nPhysics
{
	class iRigidBody
	{
	public:
		virtual ~iRigidBody() {}

		virtual void GetTransform(glm::mat4& transformOut) = 0;

		virtual void ApplyForce(const glm::vec3& force) = 0;
		
		// include these if we need them
		/*virtual void GetPosition(glm::vec3& positionOut) = 0;
		virtual void GetOrientation(glm::quat& orientationOut) = 0;*/

		virtual void GetVelocity(glm::vec3& velocityOut) = 0;

		virtual void GetOrientation(glm::mat4& orientationOut) = 0;

		virtual void SetTransform(glm::mat4& transformIn) = 0;

		virtual void SetPosition(glm::vec3 p) = 0;
		virtual void SetVelocity(glm::vec3 v) = 0;
		virtual void SetAcceleration(glm::vec3 a) = 0;
		virtual void SetOrientation(glm::mat4 o) = 0;
		virtual void SetMass(float m) = 0;

	protected:

		iRigidBody() {}
		iRigidBody(const iRigidBody& other) {}
		iRigidBody& operator=(const iRigidBody& other) { return *this; }
	};
}