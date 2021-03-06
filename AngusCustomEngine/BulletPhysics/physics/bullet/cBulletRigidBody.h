#pragma once

#include <iRigidBody.h>
#include <sRigidBodyDef.h>
#include "bullet_shapes.h"
#include "btBulletDynamicsCommon.h"
#include <eEntityType.h>

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

		btTransform GetBtTransform();
		virtual void GetPosition(glm::vec3& posOut);
		virtual void GetVelocity(glm::vec3& velocityOut);
		void GetOrientation(glm::mat4& orientationOut);
		virtual bool GetHitStatus();
		virtual std::string GetName();
		virtual glm::vec3 GetColPos();
		virtual glm::vec3 GetColNorm();
		virtual eEntityType GetEntityType();
		virtual iShape* GetIShape();

		virtual void SetTransform(glm::mat4& transformIn);

		virtual void SetPosition(glm::vec3 p);
		virtual void SetVelocity(glm::vec3 v);
		virtual void SetAcceleration(glm::vec3 a);
		virtual void SetOrientation(glm::mat4 o);
		virtual void SetMass(float m);
		virtual void SetHitStatus(bool h);
		virtual void SetName(std::string name);
		virtual void SetEntityType(eEntityType entityType);
		virtual void SetColPos(glm::vec3 pos);
		virtual void SetColNorm(glm::vec3 pos);


	protected:
		cBulletRigidBody(const cBulletRigidBody& other) {}
		cBulletRigidBody& operator=(const cBulletRigidBody& other) { return *this; }

	private:
		btDefaultMotionState* mMotionState;
		btRigidBody* mBody;
		btCollisionShape* mShape;
		iShape* mIShape;
		bool mIsHit;
		float mMass;
		std::string mName;
		eEntityType entityType = eEntityType::NONE;
		glm::vec3 colPos;
		glm::vec3 colNorm;
	};
}