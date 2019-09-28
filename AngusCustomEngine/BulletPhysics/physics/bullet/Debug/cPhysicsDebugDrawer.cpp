#include "cPhysicsDebugDrawer.h"
#include "../nConvert.h"

namespace nPhysics
{
	nPhysics::cPhysicsDebugDrawer::cPhysicsDebugDrawer()
	{
		mDebugDrawer = new cBulletDebugDrawer();
	}

	void nPhysics::cPhysicsDebugDrawer::drawLine(const glm::vec3& from,
		const glm::vec3& to,
		const glm::vec3& colour)
	{
		mDebugDrawer->drawLine(nConvert::ToBullet(from), nConvert::ToBullet(to), nConvert::ToBullet(colour));
	}
}
