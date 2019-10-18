#include <cBulletPhysicsWorld.h>
#include "cBulletRigidBody.h"
#include "Debug/cPhysicsDebugDrawer.h"
#include "nConvert.h"
#include "cBtConstraint.h"
#include <game_math.h>
#include <iostream>
#include <algorithm>

extern ContactAddedCallback gContactAddedCallback;

namespace nPhysics
{
	bool collisionCallback(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap,
		int partId0, int index0, const btCollisionObjectWrapper * colObj1Wrap,
		int partId1, int index1)
	{
		btRigidBody* rb0 = (btRigidBody*)(colObj0Wrap->getCollisionObject());
		btRigidBody* rb1 = (btRigidBody*)(colObj1Wrap->getCollisionObject());

		if (!(rb0 && rb1))
		{
			return false;
		}

		cBulletRigidBody* body0 = (cBulletRigidBody*)(rb0->getUserPointer());
		cBulletRigidBody* body1 = (cBulletRigidBody*)(rb1->getUserPointer());

		if (!(body0 && body1))
		{
			return false;
		}

		if (body0->GetHitStatus() || body1->GetHitStatus())
		{
			return false;
		}

		if (body0->GetEntityType() == eEntityType::PLAYERWEAPON && body1->GetEntityType() == eEntityType::ENEMY)
		{
			//std::cout << "hit1" << std::endl;
			body0->SetHitStatus(true);
			body1->SetHitStatus(true);
		}
		else if (body1->GetEntityType() == eEntityType::PLAYERWEAPON && body0->GetEntityType() == eEntityType::ENEMY)
		{
			//std::cout << "hit0" << std::endl;
			body0->SetHitStatus(true);
			body1->SetHitStatus(true);
		}
		else if (body0->GetEntityType() == eEntityType::PLAYER && body1->GetEntityType() == eEntityType::ENEMYWEAPON)
		{
			//std::cout << "hit1" << std::endl;
			body0->SetHitStatus(true);
			body1->SetHitStatus(true);
		}
		else if (body1->GetEntityType() == eEntityType::PLAYER && body0->GetEntityType() == eEntityType::ENEMYWEAPON)
		{
			//std::cout << "hit0" << std::endl;
			body0->SetHitStatus(true);
			body1->SetHitStatus(true);
		}
		else if (body0->GetEntityType() == eEntityType::PAINTGLOB && body1->GetEntityType() == eEntityType::ENVIRONMENT)
		{
			body0->SetHitStatus(true);
			//body1->SetHitStatus(true);
			btVector3 posA = cp.getPositionWorldOnA();
			btVector3 posB = cp.getPositionWorldOnB();
			btVector3 normB = cp.m_normalWorldOnB;

			body0->SetColPos(nConvert::ToSimple(posB));
			body0->SetColNorm(nConvert::ToSimple(normB));
		}
		else if (body1->GetEntityType() == eEntityType::PAINTGLOB && body0->GetEntityType() == eEntityType::ENVIRONMENT)
		{
			body0->SetHitStatus(true);
			body1->SetHitStatus(true);
			btVector3 posB = cp.getPositionWorldOnA();
			btVector3 posA = cp.getPositionWorldOnB();
			btVector3 normB = cp.m_normalWorldOnB;

			body1->SetColPos(nConvert::ToSimple(posB));
			body1->SetColNorm(nConvert::ToSimple(normB));
		}

		return true;
	}

	nPhysics::cBulletPhysicsWorld::cBulletPhysicsWorld()
	{
		mCollisionConfiguration = new btDefaultCollisionConfiguration();
		mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);
		mOverlappingPairCache = new btDbvtBroadphase();
		mSolver = new btSequentialImpulseConstraintSolver;
		mDynamicsWorld = new btDiscreteDynamicsWorld(mDispatcher, mOverlappingPairCache, mSolver, mCollisionConfiguration);

		gContactAddedCallback = collisionCallback;
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
			mRigidBodyList.push_back(bulletBody);
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

	bool nPhysics::cBulletPhysicsWorld::AddBody(iRigidBody * body, int group, int mask)
	{
		//check type
		// if type is BODY_TYPE_RIGID_BODY
		{
			cBulletRigidBody* bulletBody = dynamic_cast<cBulletRigidBody*>(body);
			if (!bulletBody)
			{
				return false;
			}
			mDynamicsWorld->addRigidBody(bulletBody->GetBulletBody(), group, mask);
			mRigidBodyList.push_back(bulletBody);
			return true;
		}
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

		//std::vector<iRigidBody*>::iterator itRBList;

		mRigidBodyList.erase(std::remove(mRigidBodyList.begin(), mRigidBodyList.end(), bulletBody), mRigidBodyList.end());

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
		cPhysicsDebugDrawer* dd = dynamic_cast<cPhysicsDebugDrawer*>(debugRenderer);
		if (!dd)
		{
			return;
		}

		this->mDynamicsWorld->setDebugDrawer(dd->getBulletDebugDrawer());
	}
	
	void nPhysics::cBulletPhysicsWorld::DrawDebug(int shaderID)
	{
		glm::mat4 matModel = glm::mat4(1.0f);
		glUniformMatrix4fv(glGetUniformLocation(shaderID, "matModel"), 1, GL_FALSE, &matModel[0][0]);

		this->mDynamicsWorld->debugDrawWorld();
	}

	void nPhysics::cBulletPhysicsWorld::DisableCollision(iRigidBody* rbA, iRigidBody* rbB)
	{
		cBulletRigidBody* bodyA = dynamic_cast<cBulletRigidBody*>(rbA);
		cBulletRigidBody* bodyB = dynamic_cast<cBulletRigidBody*>(rbB);
		bodyA->GetBulletBody()->setIgnoreCollisionCheck(bodyB->GetBulletBody(), true);
		bodyB->GetBulletBody()->setIgnoreCollisionCheck(bodyA->GetBulletBody(), true);
	}

	void nPhysics::cBulletPhysicsWorld::Update(float dt)
	{
		for (int i = 0; i < mRigidBodyList.size(); i++)
		{
			(dynamic_cast<cBulletRigidBody*>(mRigidBodyList[i]))->SetHitStatus(false); //reset hit status
		}

		mDynamicsWorld->stepSimulation(dt, 10); //advance simulation
		//mDynamicsWorld->stepSimulation(1 / 60.0f, 1, 1 / 60.0f);
	}


}