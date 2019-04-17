#pragma once

#include "../Entity/cEntity.h"
#include "../Entity/eEntityStatus.h"

class AIController
{
public:
	AIController();
	~AIController();

	void UpdateBehaviours(std::vector<cEntity*> aiEntities, cEntity* player);
};

