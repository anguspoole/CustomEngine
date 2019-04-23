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
			glm::vec3 startPos = globList[i]->m_EntityPhysics->rigidBody->GetColPos();
			startPos.y += 0.01f;
			LoadPaintCube(paintList, startPos);
			globList[i]->m_EntityPhysics->rigidBody->SetHitStatus(false);
			globList[i]->m_EntityPhysics->rigidBody->SetEntityType(eEntityType::NONE);
			globList[i]->m_EntityMesh->bIsVisible = false;
			timeRemaining += 10.0f;
		}
	}
}

void CheckForGlobHits()
{
	for (int i = 0; i < globList.size(); i++)
	{
		if (globList[i]->m_EntityPhysics->rigidBody->GetHitStatus())
		{
			//std::cout << "Glob hit!" << std::endl;
			glm::vec3 startPos = globList[i]->m_EntityPhysics->rigidBody->GetColPos();
			startPos.y += 0.01f;
			LoadPaintCube(paintList, startPos);
			globList[i]->m_EntityPhysics->rigidBody->SetHitStatus(false);
			globList[i]->m_EntityPhysics->rigidBody->SetEntityType(eEntityType::NONE);
			globList[i]->m_EntityMesh->bIsVisible = false;
			timeRemaining += 10.0f;
		}
	}
}

void CheckForHits(cEntity* enemy, cEntity* player, cEntity* weapon)
{
	//gPhysicsWorld->Update

	if (weapon->m_EntityPhysics->rigidBody->GetHitStatus())
	{
		if (enemy->m_EntityPhysics->rigidBody->GetHitStatus())
		{
			if (enemy->healthTimer < 0.001f &&
				(enemy->status != eEntityStatus::TAKING_DAMAGE)
				&& (player->status == eEntityStatus::ATTACKING))
			{
				enemy->healthTimer = 2.0f;

				cAnimationState::sStateDetails newState;
				newState.name = "EnemyHit";
				newState.status = eEntityStatus::TAKING_DAMAGE;

				enemy->health -= 10.0f;

				enemy->m_EntityMesh->pAniState->vecAnimationQueue.push_back(newState);
				resetHackTime(enemy);
			}
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