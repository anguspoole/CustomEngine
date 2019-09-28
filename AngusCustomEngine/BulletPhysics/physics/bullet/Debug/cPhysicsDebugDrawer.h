#pragma once

//#include <iDebugDrawer.h>
#include <iDebugRenderer.h>
#include "../bullet/cBulletDebugDrawer.h"

namespace nPhysics
{
	class cPhysicsDebugDrawer : public iDebugRenderer
	{
	public:

		cPhysicsDebugDrawer();
		virtual ~cPhysicsDebugDrawer() { };

		virtual void drawLine(const glm::vec3& from, const glm::vec3& to, const glm::vec3& colour);
		virtual void drawContactPoint(const glm::vec3& PointOnB,
			const glm::vec3& normalOnB,
			float distance,
			int lifeTime,
			const glm::vec3& color) { }
		virtual void reportErrorWarning(const char* warningString) { }
		virtual void draw3dText(const glm::vec3& location, const char* textString) { }
		virtual void setDebugMode(int mode) { }
		virtual int getDebugMode() const { return 1; }

		cBulletDebugDrawer* getBulletDebugDrawer() { return mDebugDrawer; }

	private:
		cBulletDebugDrawer* mDebugDrawer;
	};
}
