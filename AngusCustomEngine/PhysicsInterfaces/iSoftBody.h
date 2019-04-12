#pragma once
#include <game_math.h>

namespace nPhysics
{
	class iSoftBody
	{
	public:
		virtual ~iSoftBody() {}

		virtual void GetNodePosition(size_t index, glm::vec3& positionOut) = 0;
		virtual void GetNodeRadius(size_t index, float& nodeRadiusOut) = 0;

		virtual size_t NumNodes() = 0;

	protected:
		iSoftBody() {}
		iSoftBody(const iSoftBody& other) {}
		iSoftBody& operator=(const iSoftBody& other) { return *this; }
	};
}