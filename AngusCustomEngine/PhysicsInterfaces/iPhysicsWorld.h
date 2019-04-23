#pragma once

#include <game_math.h> //this should have the glm includes in it
#include "iRigidBody.h"
#include "iSoftBody.h"
#include "iConstraint.h"
#include "iDebugRenderer.h"

namespace nPhysics
{
	class iPhysicsWorld
	{
	public:
		virtual ~iPhysicsWorld() {}
		
		virtual void SetDebugRenderer(iDebugRenderer* debugRenderer) = 0;
		virtual void DrawDebug() = 0;

		//virtual void TimeStep(float dt) = 0;
		virtual void SetGravity(const glm::vec3 gravity) = 0;

		virtual bool AddBody(iRigidBody* body) = 0;
		virtual bool RemoveBody(iRigidBody* body) = 0;

		virtual bool AddBody(iSoftBody* body) = 0;
		virtual bool RemoveBody(iSoftBody* body) = 0;

		virtual bool AddConstraint(iConstraint* constraint) = 0;
		virtual bool RemoveConstraint(iConstraint* constraint) = 0;

		virtual void DisableCollision(iRigidBody* rbA, iRigidBody* rbB) = 0;

		virtual void Update(float dt) = 0;

	protected:
		iPhysicsWorld() {}
		iPhysicsWorld(const iPhysicsWorld& other) {}
		iPhysicsWorld& operator=(const iPhysicsWorld& other) { return *this; }
	};
}