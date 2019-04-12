#pragma once
#include <game_math.h>
#include <vector>

namespace nPhysics
{
	struct sSoftBodyNodeDef
	{
		sSoftBodyNodeDef(const glm::vec3& position, float mass)
			: Position(position)
			, Mass(mass)
		{

		}
		glm::vec3 Position;
		float Mass;
	};

	struct sSoftBodyDef
	{
		float SpringConstant;
		float numAB;
		float numAC;
		std::vector<sSoftBodyNodeDef> Nodes;
		std::vector<std::pair<size_t, size_t>> Springs;
	};
}