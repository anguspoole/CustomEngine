#pragma once

#include <iRigidBody.h>
#include <sRigidBodyDef.h>
#include "bullet_shapes.h"
#include "btBulletDynamicsCommon.h"

namespace nPhysics
{
	class cBulletRigidBody : public iRigidBody
	{
	public:
		cBulletRigidBody(const sRigidBodyDef& def, iShape* shape);
		virtual ~cBulletRigidBody();

		virtual void GetTransform(glm::mat4& transformOut);
		virtual void ApplyForce(const glm::vec3& force);

		inline btRigidBody* GetBulletBody() 
		{
			return mBody;
		}

		virtual void GetVelocity(glm::vec3& velocityOut);
		void GetOrientation(glm::mat4& orientationOut);

		virtual void SetTransform(glm::mat4& transformIn);

		virtual void SetPosition(glm::vec3 p);
		virtual void SetVelocity(glm::vec3 v);
		virtual void SetAcceleration(glm::vec3 a);
		virtual void SetOrientation(glm::vec3 o);
		virtual void SetMass(float m);

	protected:
		cBulletRigidBody(const cBulletRigidBody& other) {}
		cBulletRigidBody& operator=(const cBulletRigidBody& other) { return *this; }

	private:
		btDefaultMotionState* mMotionState;
		btRigidBody* mBody;
		btCollisionShape* mShape;
	};
}