#include "cBtConstraint.h"

namespace nPhysics
{
	// Constructor with one rigid body
	cBtPointPointConstraint::cBtPointPointConstraint(cBulletRigidBody * rb, const btVector3 & pivot)
		: iConstraint(eConstraintType::POINT_TO_POINT)
	{
		this->constraint = new btPoint2PointConstraint(*rb->GetBulletBody(), pivot);
	}

	// Constructor with two rigid bodies
	cBtPointPointConstraint::cBtPointPointConstraint(cBulletRigidBody * rbA, cBulletRigidBody * rbB, const btVector3 & pivotA, const btVector3 & pivotB)
		: iConstraint(eConstraintType::POINT_TO_POINT)
	{
		this->constraint = new btPoint2PointConstraint(*rbA->GetBulletBody(), *rbB->GetBulletBody(), pivotA, pivotB);
	}

	// Deconstructor
	cBtPointPointConstraint::~cBtPointPointConstraint()
	{
		delete this->constraint;
	}

	eConstraintType cBtPointPointConstraint::GetConstraintType()
	{
		return eConstraintType::POINT_TO_POINT;
	}

	btPoint2PointConstraint* cBtPointPointConstraint::GetConstraint()
	{
		return this->constraint;
	}

	// Constructor with one rigid body
	cBtHingeConstraint::cBtHingeConstraint(cBulletRigidBody * rb, const btVector3 & pivot, const btVector3 & axis)
		: iConstraint(eConstraintType::HINGE)
	{
		this->constraint = new btHingeConstraint(*rb->GetBulletBody(), pivot, axis);
	}

	cBtHingeConstraint::cBtHingeConstraint(cBulletRigidBody * rbA, cBulletRigidBody * rbB, const btVector3 & pivotA, const btVector3 & pivotB, const btVector3 & axisA, const btVector3 & axisB)
		: iConstraint(eConstraintType::HINGE)
	{
		this->constraint = new btHingeConstraint(*rbA->GetBulletBody(), *rbB->GetBulletBody(), pivotA, pivotB, axisA, axisB);
	}

	// Deconstructor
	cBtHingeConstraint::~cBtHingeConstraint()
	{
		delete this->constraint;
	}

	eConstraintType cBtHingeConstraint::GetConstraintType()
	{
		return eConstraintType::HINGE;
	}

	btHingeConstraint* cBtHingeConstraint::GetConstraint()
	{
		return this->constraint;
	}

	cBtConeTwistConstraint::cBtConeTwistConstraint(cBulletRigidBody * rb)
		: iConstraint(eConstraintType::CONETWIST)
	{
		this->constraint = new btConeTwistConstraint(*rb->GetBulletBody(), btTransform::getIdentity());
	}

	cBtConeTwistConstraint::cBtConeTwistConstraint(cBulletRigidBody * rbA, cBulletRigidBody * rbB)
		: iConstraint(eConstraintType::CONETWIST)
	{
		this->constraint = new btConeTwistConstraint(*rbA->GetBulletBody(), *rbB->GetBulletBody(), btTransform::getIdentity(), btTransform::getIdentity());
	}

	cBtConeTwistConstraint::~cBtConeTwistConstraint()
	{
		delete this->constraint;
	}

	eConstraintType cBtConeTwistConstraint::GetConstraintType()
	{
		return eConstraintType::CONETWIST;
	}

	btConeTwistConstraint * cBtConeTwistConstraint::GetConstraint()
	{
		return this->constraint;
	}

	// Constructor with one rigid body
	cBtSliderConstraint::cBtSliderConstraint(cBulletRigidBody * rb)
		: iConstraint(eConstraintType::SLIDER)
	{
		this->constraint = new btSliderConstraint(*rb->GetBulletBody(), btTransform::getIdentity(), true);
	}

	cBtSliderConstraint::cBtSliderConstraint(cBulletRigidBody * rbA, cBulletRigidBody * rbB)
		: iConstraint(eConstraintType::SLIDER)
	{
		this->constraint = new btSliderConstraint(*rbA->GetBulletBody(), *rbB->GetBulletBody(), btTransform::getIdentity(), btTransform::getIdentity(), true);
	}

	// Deconstructor
	cBtSliderConstraint::~cBtSliderConstraint()
	{
		delete this->constraint;
	}

	eConstraintType cBtSliderConstraint::GetConstraintType()
	{
		return eConstraintType::SLIDER;
	}

	btSliderConstraint* cBtSliderConstraint::GetConstraint()
	{
		return this->constraint;
	}
}
