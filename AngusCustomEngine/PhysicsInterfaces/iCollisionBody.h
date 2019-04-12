#pragma once
#include <game_math.h>
#include "eBodyType.h"

namespace nPhysics
{
	class iCollisionBody
	{
	public:
		virtual ~iCollisionBody() {}

		virtual void GetAABB(glm::vec3& minOut, glm::vec3& maxOut) = 0;

		inline eBodyType GetShapeType() const { return mBodyType; }

	protected:
		iCollisionBody(eBodyType bodyType) : mBodyType(bodyType) {}
		iCollisionBody(const iCollisionBody& other) {}
		iCollisionBody() {}
		iCollisionBody& operator=(const iCollisionBody& other) { return *this; }
	private:
		eBodyType mBodyType;
	};
}