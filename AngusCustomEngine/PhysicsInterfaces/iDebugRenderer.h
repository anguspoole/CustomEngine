#pragma once

#include <game_math.h>

namespace nPhysics
{
	class iDebugRenderer
	{
	public: 
		virtual ~iDebugRenderer() {}

		virtual void DrawSphere(const glm::vec3& position, float radius) = 0;
		virtual void DrawPlane(const glm::vec3& normal, float constant) = 0;
		//virtual void DrawLine(const glm::vec3& pointA, const glm::vec3& pointB) = 0;
		//virtual void DrawCone(const glm::vec3& point, const glm::vec3& direction) = 0;
	};
}