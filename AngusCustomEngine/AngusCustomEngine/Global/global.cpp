//This file contains global functions, kept here to avoid cluttering other files
#include "global.h"

void CheckForHits()
{
	//gPhysicsWorld->Update

	cEntity * player = findObjectByFriendlyName("Player");
	cEntity * katana = findObjectByFriendlyName("Katana");

	for (int i = 0; i < enemyList.size(); i++)
	{
		if (katana->m_EntityPhysics->rigidBody->GetHitStatus())
		{
			if (enemyList[i]->m_EntityPhysics->rigidBody->GetHitStatus())
			{
				if (enemyList[i]->healthTimer < 0.001f && 
					(enemyList[i]->status != eEntityStatus::TAKING_DAMAGE)
					&& player->status == eEntityStatus::ATTACKING)
				{
					enemyList[i]->healthTimer = 2.0f;

					cAnimationState::sStateDetails newState;
					newState.name = "EnemyHit";
					newState.status = eEntityStatus::TAKING_DAMAGE;

					enemyList[i]->m_EntityMesh->pAniState->vecAnimationQueue.push_back(newState);
					resetHackTime(enemyList[i]);

					std::cout << "Enemy Hurt!" << std::endl;
				}
			}
		}
	}
}