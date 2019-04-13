#include "bullet_shapes.h"
#include "nConvert.h"

namespace nPhysics
{
	//************************************************
	//SPHERE SHAPE
	//************************************************
	//iBulletShapeInterface::iBulletShapeInterface()
	//	: iShape(SHAPE_TYPE_SPHERE)
	//{

	//}

	cBulletSphereShape::cBulletSphereShape(float radius)
		: iBulletShapeInterface()
		, iShape(SHAPE_TYPE_SPHERE)
	{
		this->mBulletShape = new btSphereShape(btScalar(radius));
	}

	cBulletSphereShape::~cBulletSphereShape()
	{
		delete mBulletShape;
	}

	//Placeholder function, unused but could be used in future
	float cBulletSphereShape::GetRadius()
	{
		return 0.0f;
	}

	//Placeholder function, unused but could be used in future
	void cBulletSphereShape::SetRadius(float value)
	{
		mBulletShape->getShapeType();
	}

	//***********************************************
	//PLANE SHAPE
	//***********************************************
	//iBulletShapeInterface::iBulletShapeInterface()
	//	: iShape(SHAPE_TYPE_PLANE)
	//{

	//}

	cBulletPlaneShape::cBulletPlaneShape(btVector3 planeNormal, float constant)
		: iBulletShapeInterface()
		, iShape(SHAPE_TYPE_PLANE)
	{
		this->mBulletShape = new btStaticPlaneShape(planeNormal, constant);
	}

	cBulletPlaneShape::~cBulletPlaneShape()
	{
		delete mBulletShape;
	}

	float cBulletPlaneShape::GetPlaneConstant()
	{
		return ((btStaticPlaneShape*)mBulletShape)->getPlaneConstant();
	}

	glm::vec3 cBulletPlaneShape::GetPlaneNormal()
	{
		btVector3 norm = ((btStaticPlaneShape*)mBulletShape)->getPlaneNormal();
		glm::vec3 normOut = nConvert::ToSimple(norm);
		return normOut;
	}

	//***********************************************
	//BOX SHAPE
	//***********************************************

	cBulletBoxShape::cBulletBoxShape(btVector3 extents)
		: iBulletShapeInterface()
		, iShape(SHAPE_TYPE_BOX)
	{
		this->mBulletShape = new btBoxShape(extents);
	}

	cBulletBoxShape::~cBulletBoxShape()
	{
		delete mBulletShape;
	}

	glm::vec3 cBulletBoxShape::GetBoxExtents()
	{
		btVector3 extents = ((btBoxShape*)mBulletShape)->getHalfExtentsWithoutMargin();
		glm::vec3 extentsOut = nConvert::ToSimple(extents);
		return extentsOut;
	}

	//***********************************************
	// CYLINDER SHAPE
	//***********************************************

	cBulletCylinderShape::cBulletCylinderShape(btVector3 extents)
		: iBulletShapeInterface()
		, iShape(SHAPE_TYPE_CYLINDER)
	{
		this->mBulletShape = new btCylinderShape(extents);
	}

	cBulletCylinderShape::~cBulletCylinderShape()
	{
		delete mBulletShape;
	}

	glm::vec3 cBulletCylinderShape::GetCylinderExtents()
	{
		btVector3 extents = ((btCylinderShape*)mBulletShape)->getHalfExtentsWithoutMargin();
		glm::vec3 extentsOut = nConvert::ToSimple(extents);
		return extentsOut;
	}

	//***********************************************
	// CONE SHAPE
	//***********************************************

	cBulletConeShape::cBulletConeShape(float radius, float height)
		: iBulletShapeInterface()
		, iShape(SHAPE_TYPE_CONE)
	{
		this->mBulletShape = new btConeShape(btScalar(radius), btScalar(height));
	}

	cBulletConeShape::~cBulletConeShape()
	{
		delete this->mBulletShape;
	}

	//Placeholder function, unused but could be used in future
	float cBulletConeShape::GetRadius()
	{
		return 0.0f;
	}

	//Placeholder function, unused but could be used in future
	float cBulletConeShape::GetHeight()
	{
		return 0.0f;
	}

}