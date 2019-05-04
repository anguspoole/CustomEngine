#pragma once
#include <iPhysicsWorld.h>
#include "btBulletDynamicsCommon.h"
#include <iConstraint.h>
#include <vector>

namespace nPhysics
{
	class cBulletPhysicsWorld : public iPhysicsWorld
	{
	public:
		cBulletPhysicsWorld();
		virtual ~cBulletPhysicsWorld();

		virtual void SetDebugRenderer(iDebugRenderer* debugRenderer);
		virtual void DrawDebug();

		virtual void SetGravity(const glm::vec3 gravity);

		//Rigidbodies
		virtual bool AddBody(iRigidBody* body);
		virtual bool AddBody(iRigidBody* body, int group, int mask);
		virtual bool RemoveBody(iRigidBody* body);

		//Softbodies
		virtual bool AddBody(iSoftBody* body);
		virtual bool RemoveBody(iSoftBody* body);

		//Constraints
		virtual bool AddConstraint(iConstraint* constraint);
		virtual bool RemoveConstraint(iConstraint* constraint);

		void DisableCollision(iRigidBody* rbA, iRigidBody* rbB);

		//virtual void AddNamePair(std::string name1, std::string name2);
		//virtual std::vector<std::pair<std::string, std::string>> GetNamePairs();

		virtual void Update(float dt);

	protected:

	private:
		//all the stuff!
		///collision configuration contains default setup for memory, collision setup. 
		//Advanced users can create their own configuration.
		btDefaultCollisionConfiguration* mCollisionConfiguration;

		// Use the default collision dispatcher. 
		// For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
		btCollisionDispatcher* mDispatcher;

		///btDbvtBroadphase is a good general purpose broadphase. 
		// You can also try out btAxis3Sweep.
		btBroadphaseInterface* mOverlappingPairCache;

		///the default constraint solver. 
		// For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
		btSequentialImpulseConstraintSolver* mSolver;

		// The world!
		btDiscreteDynamicsWorld* mDynamicsWorld;

		std::vector<std::pair<std::string, std::string>> mNamePairList;

	};
}