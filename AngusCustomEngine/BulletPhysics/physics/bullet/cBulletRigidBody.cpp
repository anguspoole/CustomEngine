#include <cBulletRigidBody.h>
#include <eShapeType.h>
#include "nConvert.h"
#include <iostream>

namespace nPhysics
{
	cBulletRigidBody::cBulletRigidBody(const sRigidBodyDef & def, iShape* shape)
	{
		int shapeType = shape->GetShapeType();
		if (shapeType == nPhysics::eShapeType::SHAPE_TYPE_SPHERE)
		{
			btCollisionShape* colShape = dynamic_cast<cBulletSphereShape*>(shape)->GetBulletShape();
			
			/// Create Dynamic Objects
			btTransform startTransform;
			startTransform.setIdentity();

			btScalar mass(def.Mass);

			//rigidbody is dynamic if and only if mass is non zero, otherwise static
			bool isDynamic = (mass != 0.f);
			btVector3 localInertia(0, 0, 0);
			if (isDynamic)
			{
				colShape->calculateLocalInertia(mass, localInertia);
			}
			startTransform.setOrigin(nConvert::ToBullet(def.Position));
			//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
			mMotionState = new btDefaultMotionState(startTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, mMotionState, colShape, localInertia);
			rbInfo.m_restitution = 0.5f;
			rbInfo.m_friction = 1.0f;

			mBody = new btRigidBody(rbInfo);
			mBody->setLinearVelocity(nConvert::ToBullet(def.Velocity));
			mBody->setSleepingThresholds(0, 0);
		}

		else if (shapeType == nPhysics::eShapeType::SHAPE_TYPE_PLANE)
		{
			//TODO: ACTUALLY USE A SHAPE FROM THE PARAMETER

			btCollisionShape* colShape = dynamic_cast<cBulletPlaneShape*>(shape)->GetBulletShape();

			/// Create Dynamic Objects
			btTransform startTransform;
			startTransform.setIdentity();

			btScalar mass(def.Mass);

			//rigidbody is dynamic if and only if mass is non zero, otherwise static
			bool isDynamic = (mass != 0.f);
			btVector3 localInertia(0, 0, 0);
			if (isDynamic)
			{
				colShape->calculateLocalInertia(mass, localInertia);
			}
			startTransform.setOrigin(nConvert::ToBullet(def.Position));
			//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
			mMotionState = new btDefaultMotionState(startTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(0, 0, colShape, localInertia);
			rbInfo.m_restitution = 0.9f;
			rbInfo.m_friction = 10.0f;

			mBody = new btRigidBody(rbInfo);
			mBody->setLinearVelocity(nConvert::ToBullet(def.Velocity));
		}
		if (shapeType == nPhysics::eShapeType::SHAPE_TYPE_BOX)
		{
			btCollisionShape* colShape = dynamic_cast<cBulletBoxShape*>(shape)->GetBulletShape();

			/// Create Dynamic Objects
			btTransform startTransform;
			startTransform.setIdentity();

			btScalar mass(def.Mass);

			//rigidbody is dynamic if and only if mass is non zero, otherwise static
			bool isDynamic = (mass != 0.f);
			btVector3 localInertia(0, 0, 0);
			if (isDynamic)
			{
				colShape->calculateLocalInertia(mass, localInertia);
			}
			startTransform.setOrigin(nConvert::ToBullet(def.Position));
			//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
			mMotionState = new btDefaultMotionState(startTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, mMotionState, colShape, localInertia);
			rbInfo.m_restitution = 0.9f;
			rbInfo.m_friction = 10.0f;

			mBody = new btRigidBody(rbInfo);
			mBody->setLinearVelocity(nConvert::ToBullet(def.Velocity));
		}
		if (shapeType == nPhysics::eShapeType::SHAPE_TYPE_CYLINDER)
		{
			btCollisionShape* colShape = dynamic_cast<cBulletCylinderShape*>(shape)->GetBulletShape();

			/// Create Dynamic Objects
			btTransform startTransform;
			startTransform.setIdentity();

			btScalar mass(def.Mass);

			//rigidbody is dynamic if and only if mass is non zero, otherwise static
			bool isDynamic = (mass != 0.f);
			btVector3 localInertia(0, 0, 0);
			if (isDynamic)
			{
				colShape->calculateLocalInertia(mass, localInertia);
			}
			startTransform.setOrigin(nConvert::ToBullet(def.Position));
			//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
			mMotionState = new btDefaultMotionState(startTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, mMotionState, colShape, localInertia);
			rbInfo.m_restitution = 0.9f;
			rbInfo.m_friction = 10.0f;

			mBody = new btRigidBody(rbInfo);
			mBody->setLinearVelocity(nConvert::ToBullet(def.Velocity));
		}
		if (shapeType == nPhysics::eShapeType::SHAPE_TYPE_CONE)
		{
			btCollisionShape* colShape = dynamic_cast<cBulletConeShape*>(shape)->GetBulletShape();

			/// Create Dynamic Objects
			btTransform startTransform;
			startTransform.setIdentity();

			btScalar mass(def.Mass);

			//rigidbody is dynamic if and only if mass is non zero, otherwise static
			bool isDynamic = (mass != 0.f);
			btVector3 localInertia(0, 0, 0);
			if (isDynamic)
			{
				colShape->calculateLocalInertia(mass, localInertia);
			}
			startTransform.setOrigin(nConvert::ToBullet(def.Position));
			//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
			mMotionState = new btDefaultMotionState(startTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, mMotionState, colShape, localInertia);
			rbInfo.m_restitution = 0.9f;
			rbInfo.m_friction = 10.0f;

			mBody = new btRigidBody(rbInfo);
			mBody->setLinearVelocity(nConvert::ToBullet(def.Velocity));
		}
	}

	cBulletRigidBody::~cBulletRigidBody()
	{
		delete mBody;
		mBody = 0;
		delete mMotionState;
		mMotionState = 0;
	}

	void cBulletRigidBody::GetTransform(glm::mat4 & transformOut)
	{
		btTransform transform;
		mMotionState->getWorldTransform(transform);
		nConvert::ToSimple(transform, transformOut);
	}

	void cBulletRigidBody::ApplyForce(const glm::vec3& force)
	{
		btVector3 newForce = nConvert::ToBullet(force);
		mBody->applyCentralForce(newForce);

		btTransform transform;
		mMotionState->getWorldTransform(transform);
		glm::mat4 transformOut;
		nConvert::ToSimple(transform, transformOut);
		std::cout << "v: " << transformOut[3].x << "~~~" << transformOut[3].y << "~~~" << transformOut[3].z << std::endl;
	}

	void cBulletRigidBody::GetVelocity(glm::vec3& velocityOut)
	{
		btTransform transform;
		mMotionState->getWorldTransform(transform);

		//btVector3 vel = mBody->getVelocityInLocalPoint(mBody->getCenterOfMassPosition());
		btVector3 vel = mBody->getLinearVelocity();
		//btVector3 vel = mBody->getAngularVelocity();
		velocityOut = nConvert::ToSimple(vel);
	}

	void cBulletRigidBody::SetTransform(glm::mat4& transformIn)
	{

	}

	void cBulletRigidBody::SetPosition(glm::vec3 p)
	{
		btTransform transform;
		mMotionState->getWorldTransform(transform);

		btVector3 btPos = nConvert::ToBullet(p);
		transform.setOrigin(btPos);

		mMotionState->setWorldTransform(transform);
	}

	void cBulletRigidBody::SetVelocity(glm::vec3 v)
	{
		glm::vec3 currentV;
		GetVelocity(currentV);
		glm::vec3 velDiff = v - currentV;
		ApplyForce(velDiff);
	}

	void cBulletRigidBody::SetAcceleration(glm::vec3 a)
	{

	}

	void cBulletRigidBody::SetOrientation(glm::vec3 o)
	{

	}

	void cBulletRigidBody::SetMass(float m)
	{

	}
}