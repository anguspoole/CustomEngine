#include "../Global/global.h"

void makeSphere(cEntity * obj, nPhysics::sRigidBodyDef bodyDef0)
{
	nPhysics::iSphereShape* sphereShape0 = gPhysicsFactory->CreateSphereShape(bodyDef0.Mass);
	nPhysics::iRigidBody* rigidBody0 = gPhysicsFactory->CreateRigidBody(bodyDef0, sphereShape0);
	gPhysicsWorld->AddBody(rigidBody0);
	obj->m_EntityPhysics->rigidBody = rigidBody0;
	rigidBody0->SetName(obj->friendlyName);
}

void makeSphere(cEntity * obj, nPhysics::sRigidBodyDef bodyDef0, int group, int mask)
{
	nPhysics::iSphereShape* sphereShape0 = gPhysicsFactory->CreateSphereShape(bodyDef0.Mass);
	nPhysics::iRigidBody* rigidBody0 = gPhysicsFactory->CreateRigidBody(bodyDef0, sphereShape0);
	gPhysicsWorld->AddBody(rigidBody0, group, mask);
	obj->m_EntityPhysics->rigidBody = rigidBody0;
	rigidBody0->SetName(obj->friendlyName);
}

void makePlane(cEntity * obj, glm::vec3 n, nPhysics::sRigidBodyDef bodyDef0, float d)
{
	nPhysics::iPlaneShape* planeShape0 = gPhysicsFactory->CreatePlaneShape(n, d);
	nPhysics::iRigidBody* rigidBody0 = gPhysicsFactory->CreateRigidBody(bodyDef0, planeShape0);
	gPhysicsWorld->AddBody(rigidBody0);
	obj->m_EntityPhysics->rigidBody = rigidBody0;
	rigidBody0->SetName(obj->friendlyName);
}

void makePlane(cEntity * obj, glm::vec3 n, nPhysics::sRigidBodyDef bodyDef0, float d, int group, int mask)
{
	nPhysics::iPlaneShape* planeShape0 = gPhysicsFactory->CreatePlaneShape(n, d);
	nPhysics::iRigidBody* rigidBody0 = gPhysicsFactory->CreateRigidBody(bodyDef0, planeShape0);
	gPhysicsWorld->AddBody(rigidBody0, group, mask);
	obj->m_EntityPhysics->rigidBody = rigidBody0;
	rigidBody0->SetName(obj->friendlyName);
}

void makeBox(cEntity * obj, nPhysics::sRigidBodyDef bodyDef0, glm::vec3 extents)
{
	nPhysics::iBoxShape* boxShape0 = gPhysicsFactory->CreateBoxShape(extents);
	nPhysics::iRigidBody* rigidBody0 = gPhysicsFactory->CreateRigidBody(bodyDef0, boxShape0);
	gPhysicsWorld->AddBody(rigidBody0);
	obj->m_EntityPhysics->rigidBody = rigidBody0;
}

void makeBox(cEntity * obj, nPhysics::sRigidBodyDef bodyDef0, glm::vec3 extents, int group, int mask)
{
	nPhysics::iBoxShape* boxShape0 = gPhysicsFactory->CreateBoxShape(extents);
	nPhysics::iRigidBody* rigidBody0 = gPhysicsFactory->CreateRigidBody(bodyDef0, boxShape0);
	gPhysicsWorld->AddBody(rigidBody0, group, mask);
	obj->m_EntityPhysics->rigidBody = rigidBody0;
}

void makeCylinder(cEntity * obj, nPhysics::sRigidBodyDef bodyDef0)
{
	glm::vec3 extents = glm::vec3(bodyDef0.Mass);
	extents.x = 1;
	nPhysics::iCylinderShape* cylinderShape0 = gPhysicsFactory->CreateCylinderShape(extents);
	nPhysics::iRigidBody* rigidBody0 = gPhysicsFactory->CreateRigidBody(bodyDef0, cylinderShape0);
	gPhysicsWorld->AddBody(rigidBody0);
	obj->m_EntityPhysics->rigidBody = rigidBody0;
}

void makeCylinder(cEntity * obj, nPhysics::sRigidBodyDef bodyDef0, glm::vec3 extents)
{
	nPhysics::iCylinderShape* cylinderShape0 = gPhysicsFactory->CreateCylinderShape(extents);
	nPhysics::iRigidBody* rigidBody0 = gPhysicsFactory->CreateRigidBody(bodyDef0, cylinderShape0);
	gPhysicsWorld->AddBody(rigidBody0);
	obj->m_EntityPhysics->rigidBody = rigidBody0;
}

void makeCylinder(cEntity * obj, nPhysics::sRigidBodyDef bodyDef0, glm::vec3 extents, int group, int mask)
{
	nPhysics::iCylinderShape* cylinderShape0 = gPhysicsFactory->CreateCylinderShape(extents);
	nPhysics::iRigidBody* rigidBody0 = gPhysicsFactory->CreateRigidBody(bodyDef0, cylinderShape0);
	gPhysicsWorld->AddBody(rigidBody0, group, mask);
	obj->m_EntityPhysics->rigidBody = rigidBody0;
}

void makeCone(cEntity * obj, nPhysics::sRigidBodyDef bodyDef0)
{
	nPhysics::iConeShape* coneShape0 = gPhysicsFactory->CreateConeShape(bodyDef0.Mass, bodyDef0.Mass);
	nPhysics::iRigidBody* rigidBody0 = gPhysicsFactory->CreateRigidBody(bodyDef0, coneShape0);
	gPhysicsWorld->AddBody(rigidBody0);
	obj->m_EntityPhysics->rigidBody = rigidBody0;
}

void makeCone(cEntity * obj, nPhysics::sRigidBodyDef bodyDef0, int group, int mask)
{
	nPhysics::iConeShape* coneShape0 = gPhysicsFactory->CreateConeShape(bodyDef0.Mass, bodyDef0.Mass);
	nPhysics::iRigidBody* rigidBody0 = gPhysicsFactory->CreateRigidBody(bodyDef0, coneShape0);
	gPhysicsWorld->AddBody(rigidBody0, group, mask);
	obj->m_EntityPhysics->rigidBody = rigidBody0;
}

void makeCapsule(cEntity * obj, nPhysics::sRigidBodyDef bodyDef0, float radius, float height)
{
	nPhysics::iCapsuleShape* capsuleShape0 = gPhysicsFactory->CreateCapsuleShape(radius, height);
	nPhysics::iRigidBody* rigidBody0 = gPhysicsFactory->CreateRigidBody(bodyDef0, capsuleShape0);
	gPhysicsWorld->AddBody(rigidBody0);
	obj->m_EntityPhysics->rigidBody = rigidBody0;
}

void makeCapsule(cEntity * obj, nPhysics::sRigidBodyDef bodyDef0, float radius, float height, int group, int mask)
{
	nPhysics::iCapsuleShape* capsuleShape0 = gPhysicsFactory->CreateCapsuleShape(radius, height);
	nPhysics::iRigidBody* rigidBody0 = gPhysicsFactory->CreateRigidBody(bodyDef0, capsuleShape0);
	gPhysicsWorld->AddBody(rigidBody0, group, mask);
	obj->m_EntityPhysics->rigidBody = rigidBody0;
}

void makeConvexHull(cEntity * obj, nPhysics::sRigidBodyDef bodyDef0, nPhysics::sModelPoint* point, size_t numPoints)
{
	nPhysics::iConvexHullShape* convexShape0 = gPhysicsFactory->CreateConvexHullShape(point, numPoints);
	nPhysics::iRigidBody* rigidBody0 = gPhysicsFactory->CreateRigidBody(bodyDef0, convexShape0);
	gPhysicsWorld->AddBody(rigidBody0);
	obj->m_EntityPhysics->rigidBody = rigidBody0;
}

void makePointPointConstraint(cEntity * obj, glm::vec3 pivot)
{
	nPhysics::iRigidBody* rigidBody0 = obj->m_EntityPhysics->rigidBody;
	nPhysics::iRigidBody* rigidBody1 = obj->m_EntityPhysics->rigidBody;
	nPhysics::iConstraint* constraint = gPhysicsFactory->CreatePointPointConstraint(rigidBody0, pivot);
	gPhysicsWorld->AddConstraint(constraint);
}

void makePointPointConstraint(cEntity * objA, cEntity * objB, glm::vec3 pivotA, glm::vec3 pivotB)
{
	nPhysics::iRigidBody* rigidBody0 = objA->m_EntityPhysics->rigidBody;
	nPhysics::iRigidBody* rigidBody1 = objB->m_EntityPhysics->rigidBody;
	//nPhysics::iConstraint* constraint = gPhysicsFactory->CreatePointPointConstraint(rigidBody0, pivotA);
	nPhysics::iConstraint* constraint = gPhysicsFactory->CreatePointPointConstraint(rigidBody0, rigidBody1, pivotA, pivotB);
	gPhysicsWorld->AddConstraint(constraint);
}

void makeHingeConstraint(cEntity * obj, glm::vec3 pivot, glm::vec3 axis)
{
	nPhysics::iRigidBody* rigidBody0 = obj->m_EntityPhysics->rigidBody;
	nPhysics::iConstraint* constraint = gPhysicsFactory->CreateHingeConstraint(rigidBody0, pivot, axis);
	gPhysicsWorld->AddConstraint(constraint);
}

void makeHingeConstraint(cEntity * objA, cEntity* objB, glm::vec3 pivotA, glm::vec3 pivotB, glm::vec3 axisA, glm::vec3 axisB)
{
	nPhysics::iRigidBody* rigidBody0 = objA->m_EntityPhysics->rigidBody;
	nPhysics::iRigidBody* rigidBody1 = objB->m_EntityPhysics->rigidBody;
	nPhysics::iConstraint* constraint = gPhysicsFactory->CreateHingeConstraint(rigidBody0, rigidBody1, pivotA, pivotB, axisA, axisB);
	gPhysicsWorld->AddConstraint(constraint);
}

void makeConeTwistConstraint(cEntity * obj)
{
	nPhysics::iRigidBody* rigidBody0 = obj->m_EntityPhysics->rigidBody;
	nPhysics::iConstraint* constraint = gPhysicsFactory->CreateConeTwistConstraint(rigidBody0);
	gPhysicsWorld->AddConstraint(constraint);
}

void makeConeTwistConstraint(cEntity * objA, cEntity* objB)
{
	nPhysics::iRigidBody* rigidBody0 = objA->m_EntityPhysics->rigidBody;
	nPhysics::iRigidBody* rigidBody1 = objB->m_EntityPhysics->rigidBody;
	nPhysics::iConstraint* constraint = gPhysicsFactory->CreateConeTwistConstraint(rigidBody0, rigidBody1);
	gPhysicsWorld->AddConstraint(constraint);
}

void makeSliderConstraint(cEntity * obj)
{
	nPhysics::iRigidBody* rigidBody0 = obj->m_EntityPhysics->rigidBody;
	nPhysics::iConstraint* constraint = gPhysicsFactory->CreateConeTwistConstraint(rigidBody0);
	gPhysicsWorld->AddConstraint(constraint);
}

void makeSliderConstraint(cEntity * objA, cEntity* objB)
{
	nPhysics::iRigidBody* rigidBody0 = objA->m_EntityPhysics->rigidBody;
	nPhysics::iRigidBody* rigidBody1 = objB->m_EntityPhysics->rigidBody;
	nPhysics::iConstraint* constraint = gPhysicsFactory->CreateConeTwistConstraint(rigidBody0, rigidBody1);
	gPhysicsWorld->AddConstraint(constraint);
}

void makeFixedConstraint(cEntity * objA, cEntity* objB)
{
	nPhysics::iRigidBody* rigidBody0 = objA->m_EntityPhysics->rigidBody;
	nPhysics::iRigidBody* rigidBody1 = objB->m_EntityPhysics->rigidBody;
	nPhysics::iConstraint* constraint = gPhysicsFactory->CreateFixedConstraint(rigidBody0, rigidBody1);
	gPhysicsWorld->AddConstraint(constraint);
}

void makeSoftBody(cEntity * obj, nPhysics::sSoftBodyDef bodyDef0)
{
	nPhysics::iSoftBody* softBody0 = gPhysicsFactory->CreateSoftBody(bodyDef0);
	gPhysicsWorld->AddBody(softBody0);
	obj->m_EntityPhysics->softBody = softBody0;
}