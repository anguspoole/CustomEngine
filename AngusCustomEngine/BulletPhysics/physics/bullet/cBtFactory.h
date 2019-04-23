#pragma once
#include <iPhysicsFactory.h>
#include <physics_interfaces.h>
#include "cBulletPhysicsWorld.h"
#include "cBulletRigidBody.h"
#include "cBtConstraint.h"

namespace nPhysics
{
	class cBulletPhysicsFactory : public iPhysicsFactory
	{
	public:
		cBulletPhysicsFactory();
		~cBulletPhysicsFactory() {}

		//World
		virtual iPhysicsWorld* CreatePhysicsWorld();

		//Rigidbodies
		virtual iRigidBody* CreateRigidBody(const sRigidBodyDef& def, iShape* shape);

		//Shapes
		virtual iSphereShape* CreateSphereShape(float radius);
		virtual iPlaneShape* CreatePlaneShape(const glm::vec3& normal, float constant);
		virtual iBoxShape* CreateBoxShape(const glm::vec3& extents);
		virtual iCylinderShape* CreateCylinderShape(const glm::vec3& extents);
		virtual iConeShape* CreateConeShape(float radius, float height);
		virtual iCapsuleShape* CreateCapsuleShape(float radius, float height);
		virtual iConvexHullShape* CreateConvexHullShape(const sModelPoint* point, size_t numPoints);
		virtual iSoftBody* CreateSoftBody(const sSoftBodyDef& sbDef);

		//Contraints
		virtual iConstraint* CreatePointPointConstraint(iRigidBody* rb, const glm::vec3& pivot);
		virtual iConstraint* CreatePointPointConstraint(iRigidBody* rbA, iRigidBody* rbB, const glm::vec3& pivotA, const glm::vec3& pivotB);
		virtual iConstraint* CreateHingeConstraint(iRigidBody* rb, const glm::vec3& pivot, const glm::vec3& axis);
		virtual iConstraint* CreateHingeConstraint(iRigidBody* rbA, iRigidBody* rbB, const glm::vec3& pivotA, const glm::vec3& pivotB, const glm::vec3& axisA, const glm::vec3& axisB);
		virtual iConstraint* CreateConeTwistConstraint(iRigidBody* rb) ;
		virtual iConstraint* CreateConeTwistConstraint(iRigidBody* rbA, iRigidBody* rbB);
		virtual iConstraint* CreateSliderConstraint(iRigidBody* rb);
		virtual iConstraint* CreateSliderConstraint(iRigidBody* rbA, iRigidBody* rbB);
		virtual iConstraint* CreateFixedConstraint(iRigidBody* rbA, iRigidBody* rbB);

	};
}