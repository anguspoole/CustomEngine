#pragma once
#include <glm/glm.hpp>

namespace nPhysics
{
	class iDebugDrawer
	{
	public:
		virtual ~iDebugDrawer() {}
		virtual void drawLine(const glm::vec3& from, const glm::vec3& to, const glm::vec3& colour) = 0;
		virtual void drawContactPoint(const glm::vec3& PointOnB,
			const glm::vec3& normalOnB,
			float distance,
			int lifeTime,
			const glm::vec3& colour) = 0;
		virtual void reportErrorWarning(const char* warningString) = 0;
		virtual void draw3dText(const glm::vec3& location, const char* textString) = 0;
		virtual void setDebugMode(int mode) = 0;
		virtual int getDebugMode() const = 0;
	};
}