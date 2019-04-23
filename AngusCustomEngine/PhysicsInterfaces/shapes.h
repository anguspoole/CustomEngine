#pragma once

#include "iShape.h"
#include <glm/glm.hpp>

namespace nPhysics
{
	class iSphereShape : public virtual iShape
	{
	public:
		virtual ~iSphereShape() {}
		virtual float GetRadius() = 0;
		virtual void SetRadius(float value) = 0;

	protected:
		//iSphereShape() {}
		iSphereShape() : iShape(SHAPE_TYPE_SPHERE) {}
		iSphereShape(const iSphereShape& other) : iShape(other) {}
		iSphereShape& operator=(const iSphereShape& other) { return *this; }
	};

	class iPlaneShape : public virtual iShape
	{
	public:
		virtual ~iPlaneShape() {}

		virtual float GetPlaneConstant() = 0;
		virtual glm::vec3 GetPlaneNormal() = 0;

	protected:
		//iPlaneShape() {}
		iPlaneShape() : iShape(SHAPE_TYPE_PLANE) {}
		iPlaneShape(const iPlaneShape& other) : iShape(other) {}
		iPlaneShape& operator=(const iPlaneShape& other) { return *this; }
	};

	// BOX
	class iBoxShape : public virtual iShape
	{
	public: 
		virtual ~iBoxShape() {}
		virtual glm::vec3 GetBoxExtents() = 0;
	protected:
		iBoxShape() : iShape(SHAPE_TYPE_BOX) {}
		iBoxShape(const iBoxShape& other) : iShape(other) {}
		iBoxShape& operator=(const iBoxShape& other) { return *this; }
	};

	// CYLINDER
	class iCylinderShape : public virtual iShape
	{
	public:
		virtual ~iCylinderShape() {}
		virtual glm::vec3 GetCylinderExtents() = 0;
	protected:
		iCylinderShape() : iShape(SHAPE_TYPE_CYLINDER) {}
		iCylinderShape(const iCylinderShape& other) : iShape(other) {}
		iCylinderShape& operator=(const iCylinderShape& other) { return *this; }
	};

	// CONE
	class iConeShape : public virtual iShape
	{
	public:
		virtual ~iConeShape() {}
		virtual float GetRadius() = 0;
		virtual float GetHeight() = 0;
	protected:
		iConeShape() : iShape(SHAPE_TYPE_CONE) {}
		iConeShape(const iConeShape& other) : iShape(other) {}
		iConeShape& operator=(const iConeShape& other) { return *this; }
	};

	// CAPSULE
	class iCapsuleShape : public virtual iShape
	{
	public:
		virtual ~iCapsuleShape() {}
		virtual float GetRadius() = 0;
		virtual float GetHeight() = 0;
	protected:
		iCapsuleShape() : iShape(SHAPE_TYPE_CAPSULE) {}
		iCapsuleShape(const iCapsuleShape& other) : iShape(other) {}
		iCapsuleShape& operator=(const iCapsuleShape& other) { return *this; }
	};

	// CONVEX HULL
	class iConvexHullShape : public virtual iShape
	{
	public:
		virtual ~iConvexHullShape() {}
	protected:
		iConvexHullShape() : iShape(SHAPE_TYPE_CONVEXHULL) {}
		iConvexHullShape(const iConvexHullShape& other) : iShape(other) {}
		iConvexHullShape& operator=(const iConvexHullShape& other) { return *this; }
	};
}