#pragma once

#include <game_math.h>
#include <string>

namespace nPhysics
{
	struct sRigidBodyDef
	{
		sRigidBodyDef() 
			: Mass(0.0f)
			, Position(0.0f, 0.0f, 0.0f)
			, Velocity(0.0f, 0.0f, 0.0f)
			, Orientation(0.0f, 0.0f, 0.0f)
			, Extents(1.0f, 1.0f, 1.0f)
			, Name("")
		{

		}
		glm::vec3 Position;
		glm::vec3 Velocity;
		// No acceleration to start
		glm::vec3 Orientation; // Euler Angles
		glm::vec3 Extents; // Extents for scaling
		float Mass;
		std::string Name;
	};
}