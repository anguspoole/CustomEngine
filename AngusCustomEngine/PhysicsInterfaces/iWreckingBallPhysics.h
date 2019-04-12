#pragma once
#include <game_math.h>

namespace nPhysics
{
	struct sWreckingBallNodeDef
	{
		glm::vec3 Position;
		float Mass;
		float Radius;
	};

	class iWreckingBallPhysics
	{
	public:
		virtual ~iWreckingBallPhysics() {}

		virtual void GetControlSpherePosition(glm::vec3& positionOut) = 0;
		virtual void SetControlSpherePosition(const glm::vec3& positionIn) = 0;
		virtual size_t GetNumberOfChainNodes() = 0;
		virtual void GetChainNodeTransform(size_t index, glm::mat4& positionOut) = 0;

	};
}