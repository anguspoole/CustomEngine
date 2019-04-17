#include <cBulletRigidBody.h>
#include <eShapeType.h>
#include "nConvert.h"
#include <glm/gtx/quaternion.hpp>
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
			rbInfo.m_restitution = 0.3f;
			rbInfo.m_friction = 10.0f;

			mBody = new btRigidBody(rbInfo);
			mBody->setLinearVelocity(nConvert::ToBullet(def.Velocity));

			mBody->setCollisionFlags(mBody->getCollisionFlags() |
				btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

			mBody->setUserPointer(this);
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

			mBody->setSleepingThresholds(0, 0);

			mBody->setCollisionFlags(mBody->getCollisionFlags() |
				btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

			this->mBody->setUserPointer(this);
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
		if (shapeType == nPhysics::eShapeType::SHAPE_TYPE_CAPSULE)
		{
			btCollisionShape* colShape = dynamic_cast<cBulletCapsuleShape*>(shape)->GetBulletShape();

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
			startTransform.setOrigin(nConvert::ToBullet(def.Position));
			//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
			mMotionState = new btDefaultMotionState(startTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, mMotionState, colShape, localInertia);
			
			rbInfo.m_restitution = 0.0f;
			//rbInfo.m_friction = 0.0f;

			mBody = new btRigidBody(rbInfo);
			mBody->setLinearVelocity(nConvert::ToBullet(def.Velocity));

			//mBody->setAngularFactor(btVector3(0.0f, 1.0f, 0.0f));
			mBody->setAngularFactor(btVector3(0.0f, 0.0f, 0.0f));

			mBody->setCollisionFlags(mBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

			mBody->setSleepingThresholds(0, 0);

			this->mBody->setUserPointer(this);
		}

		this->mIsHit = false;
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

	btTransform cBulletRigidBody::GetBtTransform()
	{
		btTransform transform;
		mMotionState->getWorldTransform(transform);
		return transform;
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

	void cBulletRigidBody::GetOrientation(glm::mat4& orientationOut)
	{
		btQuaternion btQuat = this->mBody->getOrientation();
		glm::quat glmQuat(btQuat.getW(), btQuat.getX(), btQuat.getY(), btQuat.getZ());
		orientationOut = glm::mat4(glmQuat);
	}

	bool cBulletRigidBody::GetHitStatus()
	{
		return this->mIsHit;
	}

	std::string cBulletRigidBody::GetName()
	{
		return this->mName;
	}

	glm::vec3 cBulletRigidBody::GetColPos()
	{
		return this->colPos;
	}

	glm::vec3 cBulletRigidBody::GetColNorm()
	{
		return this->colNorm;
	}

	eEntityType cBulletRigidBody::GetEntityType()
	{
		return this->entityType;
	}

	void cBulletRigidBody::SetTransform(glm::mat4& transformIn)
	{

	}

	void cBulletRigidBody::SetPosition(glm::vec3 p)
	{
		this->mBody->activate(true);
		btTransform transform;
		//mMotionState->getWorldTransform(transform);
		transform = this->mBody->getWorldTransform();

		btVector3 btPos = nConvert::ToBullet(p);
		this->mBody->translate(btPos);
		transform.setOrigin(btPos);

		//mMotionState->setWorldTransform(transform);
		this->mBody->setWorldTransform(transform);
	}

	void cBulletRigidBody::SetVelocity(glm::vec3 v)
	{
		glm::mat4 currentOrientation;
		this->GetOrientation(currentOrientation);

		//Set velocity after rotating
		glm::vec3 currentX = (currentOrientation * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f)) * v.x;
		glm::vec3 currentY = (currentOrientation * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)) * v.y;
		glm::vec3 currentZ = (currentOrientation * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f)) * v.z;

		glm::vec3 newGlmVel = currentX + currentY + currentZ;

		btVector3 newBtVel = nConvert::ToBullet(newGlmVel);
		//glm::vec3 currentV;
		//GetVelocity(currentV);
		//glm::vec3 velDiff = v - currentV;
		//ApplyForce(velDiff);
		this->mBody->activate();
		this->mBody->setLinearVelocity(newBtVel);
	}

	void cBulletRigidBody::SetAcceleration(glm::vec3 a)
	{

	}

	void cBulletRigidBody::SetOrientation(glm::mat4 o)
	{
		glm::quat q = glm::toQuat(o);
		btQuaternion btQ = nConvert::ToBullet(q);
		btTransform btT = this->mBody->getWorldTransform();
		btT.setRotation(btQ);
		this->mBody->setWorldTransform(btT);
	}

	void cBulletRigidBody::SetMass(float m)
	{

	}
	
	void cBulletRigidBody::SetHitStatus(bool h)
	{
		this->mIsHit = h;
	}

	void cBulletRigidBody::SetName(std::string name)
	{
		this->mName = name;
	}
	void cBulletRigidBody::SetEntityType(eEntityType entityType)
	{
		this->entityType = entityType;
	}
	void cBulletRigidBody::SetColPos(glm::vec3 pos)
	{
		this->colPos = pos;
	}
	void cBulletRigidBody::SetColNorm(glm::vec3 pos)
	{
		this->colNorm = pos;
	}
}