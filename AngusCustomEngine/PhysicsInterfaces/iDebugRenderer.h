#pragma once

#include <game_math.h>

namespace nPhysics
{
	class iDebugRenderer
	{
	public: 
		virtual ~iDebugRenderer() {}

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

		//virtual void DrawSphere(const glm::vec3& position, float radius) = 0;
		//virtual void DrawPlane(const glm::vec3& normal, float constant) = 0;
		//virtual void DrawLine(const glm::vec3& pointA, const glm::vec3& pointB) = 0;
		//virtual void DrawCone(const glm::vec3& point, const glm::vec3& direction) = 0;
	};
}