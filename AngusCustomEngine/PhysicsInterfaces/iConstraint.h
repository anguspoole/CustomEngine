#pragma once

#include <game_math.h>
#include "eConstraintType.h"

namespace nPhysics
{
	class iConstraint
	{
	public:
		virtual ~iConstraint() {}
		virtual eConstraintType GetConstraintType() = 0;

	protected:

		iConstraint() {}
		iConstraint(eConstraintType type) {}
		iConstraint(const iConstraint& other) {}
		iConstraint& operator=(const iConstraint& other) { return *this; }
	};
}