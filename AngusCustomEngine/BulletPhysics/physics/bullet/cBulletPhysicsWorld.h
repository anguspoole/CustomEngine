#pragma once
#include <iPhysicsWorld.h>
#include "btBulletDynamicsCommon.h"
#include <iConstraint.h>

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
		virtual bool RemoveBody(iRigidBody* body);

		//Softbodies
		virtual bool AddBody(iSoftBody* body);
		virtual bool RemoveBody(iSoftBody* body);

		//Constraints
		virtual bool AddConstraint(iConstraint* constraint);
		virtual bool RemoveConstraint(iConstraint* constraint);

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

	};
}