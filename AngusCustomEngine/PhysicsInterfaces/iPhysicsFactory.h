#pragma once

#include <game_math.h>

#include "sRigidBodyDef.h"
#include "iShape.h"
#include "iPhysicsWorld.h"
#include "iRigidBody.h"
#include "iSoftBody.h"
#include "sSoftBodyDef.h"
#include "shapes.h"
#include "iWreckingBallPhysics.h"
#include "iConstraint.h"

namespace nPhysics
{
	class iPhysicsFactory
	{
	public:
		virtual ~iPhysicsFactory() {}

		virtual iPhysicsWorld* CreatePhysicsWorld() = 0;
		virtual iRigidBody* CreateRigidBody(const sRigidBodyDef& def, iShape* shape) = 0;
		virtual iSphereShape* CreateSphereShape(float radius) = 0;
		virtual iPlaneShape* CreatePlaneShape(const glm::vec3& normal, float constant) = 0;
		virtual iBoxShape* CreateBoxShape(const glm::vec3& extents) = 0;
		virtual iCylinderShape* CreateCylinderShape(const glm::vec3& extents) = 0;
		virtual iConeShape* CreateConeShape(float radius, float height) = 0;
		virtual iCapsuleShape* CreateCapsuleShape(float radius, float height) = 0;
		virtual iSoftBody* CreateSoftBody(const sSoftBodyDef& sbDef) = 0;

		virtual iConstraint* CreatePointPointConstraint(iRigidBody* rb, const glm::vec3& pivot) = 0;
		virtual iConstraint* CreatePointPointConstraint(iRigidBody* rbA, iRigidBody* rbB, const glm::vec3& pivotA, const glm::vec3& pivotB) = 0;
		virtual iConstraint* CreateHingeConstraint(iRigidBody* rb, const glm::vec3& pivot, const glm::vec3& axis) = 0;
		virtual iConstraint* CreateHingeConstraint(iRigidBody* rbA, iRigidBody* rbB, const glm::vec3& pivotA, const glm::vec3& pivotB, const glm::vec3& axisA, const glm::vec3& axisB) = 0;
		virtual iConstraint* CreateConeTwistConstraint(iRigidBody* rb) = 0;
		virtual iConstraint* CreateConeTwistConstraint(iRigidBody* rbA, iRigidBody* rbB) = 0;
		virtual iConstraint* CreateSliderConstraint(iRigidBody* rb) = 0;
		virtual iConstraint* CreateSliderConstraint(iRigidBody* rbA, iRigidBody* rbB) = 0;

		//more interesting things
		//virtual iWreckingBallPhysics* CreateWreckingBall(const std::vector<sWreckingBallNodeDef>& wreckingDef) = 0;

	protected:
		iPhysicsFactory() {}
		iPhysicsFactory(const iPhysicsFactory& other) {}
		iPhysicsFactory& operator=(const iPhysicsFactory& other) { return *this; }
	};
}