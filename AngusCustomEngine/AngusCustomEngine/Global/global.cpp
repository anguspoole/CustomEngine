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

					enemyList[i]->health -= 10.0f;

					enemyList[i]->m_EntityMesh->pAniState->vecAnimationQueue.push_back(newState);
					resetHackTime(enemyList[i]);

					std::cout << "Enemy Hurt!" << std::endl;
				}
			}
		}
	}

	for (int i = 0; i < globList.size(); i++)
	{
		if (globList[i]->m_EntityPhysics->rigidBody->GetHitStatus())
		{
			//std::cout << "Glob hit!" << std::endl;
		}
	}
}

void SpawnGlob(cEntity* obj, std::vector<cEntity*>& vec_pObjectsToDraw, GLuint program)
{
	glm::mat4 transform;
	obj->m_EntityPhysics->rigidBody->GetTransform(transform);
	glm::vec3 spawnPos = glm::vec3(transform[3].x, transform[3].y, transform[3].z);

	int startingVelX = (std::rand() % 8 + 2) - 4;
	int startingVelY = std::rand() % 12 + 10;
	int startingVelZ = std::rand() % (8 + 2) - 2;

	glm::vec3 startingVel = glm::vec3(startingVelX, startingVelY, startingVelZ);

	LoadPaintGlob(vec_pObjectsToDraw, program, spawnPos, startingVel);
}