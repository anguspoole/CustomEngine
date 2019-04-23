#include "cBtFactory.h"
#include "bullet_shapes.h"
#include "nConvert.h"

namespace nPhysics
{
	cBulletPhysicsFactory::cBulletPhysicsFactory()
	{

	}

	iPhysicsWorld * cBulletPhysicsFactory::CreatePhysicsWorld()
	{
		iPhysicsWorld * world = new cBulletPhysicsWorld();
		return world;
	}

	iRigidBody* cBulletPhysicsFactory::CreateRigidBody(const sRigidBodyDef& def, iShape* shape)
	{
		iRigidBody* body = new cBulletRigidBody(def, shape);
		return body;
	}

	iSphereShape* cBulletPhysicsFactory::CreateSphereShape(float radius)
	{
		cBulletSphereShape* shape = new cBulletSphereShape(radius);
		return shape;
	}

	iPlaneShape* cBulletPhysicsFactory::CreatePlaneShape(const glm::vec3& normal, float constant)
	{
		btVector3 norm = nConvert::ToBullet(normal);

		//Use 0.0f for the constant for now
		cBulletPlaneShape* shape = new cBulletPlaneShape(norm, constant);
		return shape;
	}
	iBoxShape * cBulletPhysicsFactory::CreateBoxShape(const glm::vec3 & extents)
	{
		btVector3 btExtents = nConvert::ToBullet(extents);
		cBulletBoxShape* shape = new cBulletBoxShape(btExtents);
		return shape;
	}
	iCylinderShape * cBulletPhysicsFactory::CreateCylinderShape(const glm::vec3 & extents)
	{
		btVector3 btExtents = nConvert::ToBullet(extents);
		cBulletCylinderShape* shape = new cBulletCylinderShape(btExtents);
		return shape;
	}
	iConeShape * cBulletPhysicsFactory::CreateConeShape(float radius, float height)
	{
		cBulletConeShape* shape = new cBulletConeShape(radius, height);
		return shape;
	}

	iCapsuleShape * cBulletPhysicsFactory::CreateCapsuleShape(float radius, float height)
	{
		cBulletCapsuleShape* shape = new cBulletCapsuleShape(radius, height);
		return shape;
	}

	iConvexHullShape * cBulletPhysicsFactory::CreateConvexHullShape(const sModelPoint * point, size_t numPoints)
	{
		cBulletConvexHullShape* shape = new cBulletConvexHullShape(point, numPoints);
		return shape;
	}
	
	iSoftBody * cBulletPhysicsFactory::CreateSoftBody(const sSoftBodyDef & sbDef)
	{
		return nullptr;
	}

	iConstraint * cBulletPhysicsFactory::CreatePointPointConstraint(iRigidBody * rb, const glm::vec3 & pivot)
	{
		cBtPointPointConstraint* constraint = new cBtPointPointConstraint(dynamic_cast<cBulletRigidBody*>(rb), nConvert::ToBullet(pivot));
		return constraint;
	}

	iConstraint * cBulletPhysicsFactory::CreatePointPointConstraint(iRigidBody * rbA, iRigidBody * rbB, const glm::vec3 & pivotA, const glm::vec3 & pivotB)
	{
		cBtPointPointConstraint* constraint = new cBtPointPointConstraint(dynamic_cast<cBulletRigidBody*>(rbA), dynamic_cast<cBulletRigidBody*>(rbB), nConvert::ToBullet(pivotA), nConvert::ToBullet(pivotB));
		return constraint;
	}

	iConstraint * cBulletPhysicsFactory::CreateHingeConstraint(iRigidBody * rb, const glm::vec3 & pivot, const glm::vec3 & axis)
	{
		cBtHingeConstraint* constraint = new cBtHingeConstraint(dynamic_cast<cBulletRigidBody*>(rb), nConvert::ToBullet(pivot), nConvert::ToBullet(axis));
		return constraint;
	}

	iConstraint* cBulletPhysicsFactory::CreateHingeConstraint(iRigidBody* rbA, iRigidBody* rbB, const glm::vec3& pivotA, const glm::vec3& pivotB, const glm::vec3& axisA, const glm::vec3& axisB)
	{
		cBtHingeConstraint* constraint = new cBtHingeConstraint(dynamic_cast<cBulletRigidBody*>(rbA), dynamic_cast<cBulletRigidBody*>(rbB), nConvert::ToBullet(pivotA), nConvert::ToBullet(pivotB), nConvert::ToBullet(axisA), nConvert::ToBullet(axisA));
		return constraint;
	}

	iConstraint* cBulletPhysicsFactory::CreateConeTwistConstraint(iRigidBody* rb)
	{
		cBtConeTwistConstraint* constraint = new cBtConeTwistConstraint(dynamic_cast<cBulletRigidBody*>(rb));
		return constraint;
	}

	iConstraint* cBulletPhysicsFactory::CreateConeTwistConstraint(iRigidBody* rbA, iRigidBody* rbB)
	{
		cBtConeTwistConstraint* constraint = new cBtConeTwistConstraint(dynamic_cast<cBulletRigidBody*>(rbA), dynamic_cast<cBulletRigidBody*>(rbB));
		return constraint;
	}
	
	iConstraint* cBulletPhysicsFactory::CreateSliderConstraint(iRigidBody* rb)
	{
		cBtSliderConstraint* constraint = new cBtSliderConstraint(dynamic_cast<cBulletRigidBody*>(rb));
		return constraint;
	}
	
	iConstraint* cBulletPhysicsFactory::CreateSliderConstraint(iRigidBody* rbA, iRigidBody* rbB)
	{
		cBtSliderConstraint* constraint = new cBtSliderConstraint(dynamic_cast<cBulletRigidBody*>(rbA), dynamic_cast<cBulletRigidBody*>(rbB));
		return constraint;
	}

	iConstraint* cBulletPhysicsFactory::CreateFixedConstraint(iRigidBody* rbA, iRigidBody* rbB)
	{
		cBtFixedConstraint* constraint = new cBtFixedConstraint(dynamic_cast<cBulletRigidBody*>(rbA), dynamic_cast<cBulletRigidBody*>(rbB), dynamic_cast<cBulletRigidBody*>(rbA)->GetBtTransform(), dynamic_cast<cBulletRigidBody*>(rbA)->GetBtTransform());
		return constraint;
	}
}