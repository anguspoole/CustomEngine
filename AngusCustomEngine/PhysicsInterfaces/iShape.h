#pragma once

#include "eShapeType.h"

namespace nPhysics
{
	class iShape
	{
	public:
		virtual ~iShape() {}

		inline eShapeType GetShapeType() const { return mShapeType; }
		//virtual void GetAABB(const glm::mat4& transformMat, glm::vec3& minBoundsOut, glm::vec3& maxBoundsOut) = 0;

		//Another way of dealing with different types of shapes instead of iSphereShape and such:
		//virtual bool GetSphereRadius(float& radiusOut) { return false; }
		//virtual bool GetPlaneConstant(float& planeConstantOut) { return false; }
		//virtual bool GetPlaneNormal(glm::vec3& planeNormalOut) { return false; }
	protected:
		iShape(eShapeType shapeType) : mShapeType(shapeType) {}
		iShape(const iShape& other) {}
		iShape() {}
		iShape& operator=(const iShape& other) { return *this; }
	private:
		eShapeType mShapeType;
	};
}