#pragma once
#include <shapes.h>
#include "btBulletDynamicsCommon.h"

namespace nPhysics
{
	class iBulletShapeInterface : public virtual iShape
	{
	public:
		//iBulletShapeInterface() : iShape(SHAPE_TYPE_SPHERE) {};
		//iBulletShapeInterface() : iShape(SHAPE_TYPE_PLANE) {};

		virtual ~iBulletShapeInterface() {};
		inline btCollisionShape* GetBulletShape()
		{
			return mBulletShape;
		}
	protected:
		//iBulletShapeInterface();
		btCollisionShape* mBulletShape;
	};

	class cBulletSphereShape : public iBulletShapeInterface, public iSphereShape
	{
	public:
		cBulletSphereShape(float radius);
		virtual ~cBulletSphereShape();
		virtual float GetRadius();
		virtual void SetRadius(float value);
	};

	class cBulletPlaneShape : public iBulletShapeInterface, public iPlaneShape
	{
	public:
		cBulletPlaneShape(btVector3 planeNormal, float constant);
		virtual ~cBulletPlaneShape();
		virtual float GetPlaneConstant();
		virtual glm::vec3 GetPlaneNormal();
	};

	class cBulletBoxShape : public iBulletShapeInterface, public iBoxShape
	{
	public:
		cBulletBoxShape(btVector3 extents);
		virtual ~cBulletBoxShape();
		virtual glm::vec3 GetBoxExtents();
		//btCollisionShape* mBtShape;
	};

	class cBulletCylinderShape : public iBulletShapeInterface, public iCylinderShape
	{
	public:
		cBulletCylinderShape(btVector3 extents);
		virtual ~cBulletCylinderShape();
		virtual glm::vec3 GetCylinderExtents();
	};

	class cBulletConeShape : public iBulletShapeInterface, public iConeShape
	{
	public:
		cBulletConeShape(float radius, float height);
		virtual ~cBulletConeShape();
		virtual float GetRadius();
		virtual float GetHeight();
	};
}