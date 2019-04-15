#include <cBulletPhysicsWorld.h>
#include "cBulletRigidBody.h"
#include "nConvert.h"
#include "cBtConstraint.h"

namespace nPhysics
{
	nPhysics::cBulletPhysicsWorld::cBulletPhysicsWorld()
	{
		mCollisionConfiguration = new btDefaultCollisionConfiguration();
		mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);
		mOverlappingPairCache = new btDbvtBroadphase();
		mSolver = new btSequentialImpulseConstraintSolver;
		mDynamicsWorld = new btDiscreteDynamicsWorld(mDispatcher, mOverlappingPairCache, mSolver, mCollisionConfiguration);
	}

	nPhysics::cBulletPhysicsWorld::~cBulletPhysicsWorld()
	{
		if (mDynamicsWorld)
		{
			delete mDynamicsWorld;
			mDynamicsWorld = 0;
		}
		if (mSolver)
		{
			delete mSolver;
			mSolver = 0;
		}
		if (mOverlappingPairCache)
		{
			delete mOverlappingPairCache;
			mOverlappingPairCache = 0;
		}
		if (mDispatcher)
		{
			delete mDispatcher;
			mDispatcher = 0;
		}
		if (mCollisionConfiguration)
		{
			delete mCollisionConfiguration;
			mCollisionConfiguration = 0;
		}
	}

	void nPhysics::cBulletPhysicsWorld::SetGravity(const glm::vec3 gravity)
	{
		mDynamicsWorld->setGravity(nConvert::ToBullet(gravity));
	}

	bool nPhysics::cBulletPhysicsWorld::AddBody(iRigidBody * body)
	{
		//check type
		// if type is BODY_TYPE_RIGID_BODY
		{
			cBulletRigidBody* bulletBody = dynamic_cast<cBulletRigidBody*>(body);
			if (!bulletBody)
			{
				return false;
			}
			mDynamicsWorld->addRigidBody(bulletBody->GetBulletBody());
			return true;
		}
		////else if type is BODY_TYPE_COMPOUND
		//{
		//	cBulletCompoundBody* bulletCompoundBody = dynamic_cast<cBulletCompoundBody*>(body);
		//	bulletCompoundBody->AddToWorld(mDynamicsWorld);
		//	size_t numRigidBodies = bulletCompoundBody->GetNumRigidBodies();
		//	size_t numConstraints = bulletCompoundBody->GetNumConstraints();
		//	for (size_t c = 0; c < numRigidBodies; c++)
		//	{
		//		cBulletRigidBody* rb = bulletCompoundBody->GetRigidBody(c);
		//		mDynamicsWorld->addRigidBody(rb);
		//	}
		//	for (size_t c = 0; c < numConstraints; c++)
		//	{
		//		btTypedConstraint* constraint = bulletCompoundBody->GetConstraint(c);
		//		mDynamicsWorld->addConstraint(constraint);
		//	}

		//	mCompoundBodies.push_back(bulletCompoundBody);
		//	
		//	return true;
		//}
		return false;
	}

	bool nPhysics::cBulletPhysicsWorld::RemoveBody(iRigidBody * body)
	{
		cBulletRigidBody* bulletBody = dynamic_cast<cBulletRigidBody*>(body);
		if (!bulletBody)
		{
			return false;
		}
		btRigidBody* bulletBtBody = bulletBody->GetBulletBody();
		mDynamicsWorld->removeRigidBody(bulletBtBody);
		return true;
	}

	bool cBulletPhysicsWorld::AddBody(iSoftBody* body)
	{
		//if (body == 0) return false; //can't add nothing!
		//cSimpleSoftBody* sb = dynamic_cast<cSimpleSoftBody*>(body);
		//if (!sb) return false; //don't know what this is

		//std::vector<iSoftBody*>::iterator it = std::find(mSoftBodies.begin(), mSoftBodies.end(), body);
		//if (it != mSoftBodies.end()) return false; //not added, already there
		//mSoftBodies.push_back(body);
		return true;
	}
	bool cBulletPhysicsWorld::RemoveBody(iSoftBody* body)
	{
		//if (body == 0) return false; //can't add nothing!
		//cSimpleSoftBody* sb = dynamic_cast<cSimpleSoftBody*>(body);
		//if (!sb) return false; //don't know what this is

		//std::vector<iSoftBody*>::iterator it = std::find(mSoftBodies.begin(), mSoftBodies.end(), body);
		//if (it == mSoftBodies.end()) return false; //not removed, doesn't exist
		//mSoftBodies.erase(std::remove(mSoftBodies.begin(), mSoftBodies.end(), body));
		return true;
	}

	//Function for adding a constraint
	bool cBulletPhysicsWorld::AddConstraint(iConstraint * constraint)
	{
		eConstraintType constraintType = constraint->GetConstraintType();

		if(constraintType == nPhysics::eConstraintType::POINT_TO_POINT)
		{
			cBtPointPointConstraint* btConstraint = dynamic_cast<cBtPointPointConstraint*>(constraint);
			
			this->mDynamicsWorld->addConstraint(btConstraint->GetConstraint(), true);
		}
		if (constraintType == nPhysics::eConstraintType::HINGE)
		{
			cBtHingeConstraint* btConstraint = dynamic_cast<cBtHingeConstraint*>(constraint);

			this->mDynamicsWorld->addConstraint(btConstraint->GetConstraint());
		}
		if (constraintType == nPhysics::eConstraintType::CONETWIST)
		{
			cBtConeTwistConstraint* btConstraint = dynamic_cast<cBtConeTwistConstraint*>(constraint);

			this->mDynamicsWorld->addConstraint(btConstraint->GetConstraint());
		}
		if (constraintType == nPhysics::eConstraintType::SLIDER)
		{
			cBtSliderConstraint* btConstraint = dynamic_cast<cBtSliderConstraint*>(constraint);

			this->mDynamicsWorld->addConstraint(btConstraint->GetConstraint());
		}
		return true;
	}

	bool cBulletPhysicsWorld::RemoveConstraint(iConstraint * constraint)
	{
		eConstraintType constraintType = constraint->GetConstraintType();

		if (constraintType == nPhysics::eConstraintType::POINT_TO_POINT)
		{
			cBtPointPointConstraint* btConstraint = dynamic_cast<cBtPointPointConstraint*>(constraint);
			
			if (!btConstraint)
			{
				return false;
			}
			
			this->mDynamicsWorld->removeConstraint(btConstraint->GetConstraint());
		}
		if (constraintType == nPhysics::eConstraintType::HINGE)
		{
			cBtHingeConstraint* btConstraint = dynamic_cast<cBtHingeConstraint*>(constraint);

			if (!btConstraint)
			{
				return false;
			}

			this->mDynamicsWorld->removeConstraint(btConstraint->GetConstraint());
		}
		if (constraintType == nPhysics::eConstraintType::CONETWIST)
		{
			cBtConeTwistConstraint* btConstraint = dynamic_cast<cBtConeTwistConstraint*>(constraint);

			if (!btConstraint)
			{
				return false;
			}

			this->mDynamicsWorld->removeConstraint(btConstraint->GetConstraint());
		}
		if (constraintType == nPhysics::eConstraintType::SLIDER)
		{
			cBtSliderConstraint* btConstraint = dynamic_cast<cBtSliderConstraint*>(constraint);

			if (!btConstraint)
			{
				return false;
			}

			this->mDynamicsWorld->removeConstraint(btConstraint->GetConstraint());
		}
		return true;
	}

	void nPhysics::cBulletPhysicsWorld::SetDebugRenderer(iDebugRenderer* debugRenderer)
	{

	}
	
	void nPhysics::cBulletPhysicsWorld::DrawDebug()
	{

	}

	void nPhysics::cBulletPhysicsWorld::Update(float dt)
	{
		mDynamicsWorld->stepSimulation(dt, 10);
	}


}