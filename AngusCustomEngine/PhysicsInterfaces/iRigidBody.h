#pragma once

#include <game_math.h>
#include <string>
#include "eEntityType.h"
#include "iShape.h"

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
		virtual void GetPosition(glm::vec3& posOut) = 0;
		virtual void GetVelocity(glm::vec3& velocityOut) = 0;
		virtual void GetOrientation(glm::mat4& orientationOut) = 0;
		virtual bool GetHitStatus() = 0;
		virtual std::string GetName() = 0;
		virtual eEntityType GetEntityType() = 0;
		virtual glm::vec3 GetColPos() = 0;
		virtual glm::vec3 GetColNorm() = 0;
		virtual iShape* GetIShape() = 0;


		virtual void SetTransform(glm::mat4& transformIn) = 0;

		virtual void SetPosition(glm::vec3 p) = 0;
		virtual void SetVelocity(glm::vec3 v) = 0;
		virtual void SetAcceleration(glm::vec3 a) = 0;
		virtual void SetOrientation(glm::mat4 o) = 0;
		virtual void SetMass(float m) = 0;
		virtual void SetHitStatus(bool h) = 0;
		virtual void SetName(std::string name) = 0;
		virtual void SetEntityType(eEntityType entityType) = 0;
		virtual void SetColPos(glm::vec3 pos) = 0;
		virtual void SetColNorm(glm::vec3 pos) = 0;

	protected:

		iRigidBody() {}
		iRigidBody(const iRigidBody& other) {}
		iRigidBody& operator=(const iRigidBody& other) { return *this; }
	};
}