#pragma once
#include <game_math.h>
#include <iRigidBody.h>
#include <cBulletRigidBody.h>
#include <iConstraint.h>

namespace nPhysics
{
	class cBtPointPointConstraint : public iConstraint
	{
	public:
		//Constructors
		cBtPointPointConstraint(cBulletRigidBody* rb, const btVector3& pivot);
		cBtPointPointConstraint(cBulletRigidBody* rbA, cBulletRigidBody* rbB, const btVector3& pivotA, const btVector3& pivotB);

		~cBtPointPointConstraint();

		virtual eConstraintType GetConstraintType();
		
		btPoint2PointConstraint* GetConstraint();

	private:
		btPoint2PointConstraint* constraint;
	};

	class cBtHingeConstraint : public iConstraint
	{
	public:
		//Constructor
		cBtHingeConstraint(cBulletRigidBody* rb, const btVector3& pivot, const btVector3& axis);
		cBtHingeConstraint(cBulletRigidBody* rbA, cBulletRigidBody* rbB, const btVector3& pivotA, const btVector3& pivotB, const btVector3& axisA, const btVector3& axisB);


		~cBtHingeConstraint();

		virtual eConstraintType GetConstraintType();

		btHingeConstraint* GetConstraint();

	private:
		btHingeConstraint* constraint;
	};

	class cBtConeTwistConstraint : public iConstraint
	{
	public:
		//Constructor
		cBtConeTwistConstraint(cBulletRigidBody* rb);
		cBtConeTwistConstraint(cBulletRigidBody* rbA, cBulletRigidBody* rbB);


		~cBtConeTwistConstraint();

		virtual eConstraintType GetConstraintType();

		btConeTwistConstraint* GetConstraint();

	private:
		btConeTwistConstraint* constraint;
	};

	class cBtSliderConstraint : public iConstraint
	{
	public:
		//Constructor
		cBtSliderConstraint(cBulletRigidBody* rb);
		cBtSliderConstraint(cBulletRigidBody* rbA, cBulletRigidBody* rbB);


		~cBtSliderConstraint();

		virtual eConstraintType GetConstraintType();

		btSliderConstraint* GetConstraint();

	private:
		btSliderConstraint* constraint;
	};

	class cBtFixedConstraint : public iConstraint
	{
	public:
		//Constructors
		cBtFixedConstraint(cBulletRigidBody* rbA, cBulletRigidBody* rbB, const btTransform& tA, const btTransform& tB);

		~cBtFixedConstraint();

		virtual eConstraintType GetConstraintType();

		btFixedConstraint* GetConstraint();

	private:
		btFixedConstraint* constraint;
	};

}