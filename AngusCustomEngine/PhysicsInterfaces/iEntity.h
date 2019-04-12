#pragma once
#include "iRigidBody.h"
#include "iSoftBody.h"
#include "iShape.h"

namespace nPhysics
{
	class iEntity
	{
	public:
		virtual ~iEntity() {}

		virtual void makeBody(iRigidBody* body) = 0; //method to create a body for the entity
		virtual void makeSoftBody(iSoftBody* body) = 0; //method to create a body for the entity
		virtual void makeShape(iShape* shape) = 0; //method to create a shape for the entity
		virtual iRigidBody* getBody() = 0; //method to get the entity's body
		virtual iSoftBody* getSoftBody() = 0; //method to get the entity's body
		virtual iShape* getShape() = 0; //method to get the entity's shape
	};
}