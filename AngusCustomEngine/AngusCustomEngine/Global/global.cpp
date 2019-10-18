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
					float timer = enemyList[i]->m_EntityMesh->pSimpleSkinnedMesh->GetDuration("EnemyHit");
					enemyList[i]->healthTimer = timer;

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

	//for (int i = 0; i < globList.size(); i++)
	//{
	//	if (globList[i]->m_EntityPhysics->rigidBody->GetHitStatus())
	//	{
	//		//std::cout << "Glob hit!" << std::endl;
	//		glm::vec3 startPos = globList[i]->m_EntityPhysics->rigidBody->GetColPos();
	//		startPos.y += 0.01f;
	//		LoadPaintCube(paintList, startPos);
	//		//globList[i]->m_EntityPhysics->rigidBody->SetHitStatus(false);
	//		globList[i]->m_EntityPhysics->rigidBody->SetEntityType(eEntityType::NONE);
	//		globList[i]->m_EntityMesh->bIsVisible = false;
	//		gPhysicsWorld->RemoveBody(globList[i]->m_EntityPhysics->rigidBody);
	//		timeRemaining += 10.0f;
	//	}
	//}
}

void CheckForGlobHits()
{
	for (int i = 0; i < globList.size(); i++)
	{
		if (globList[i]->m_EntityPhysics->rigidBody->GetHitStatus())
		{
			globList[i]->m_EntityPhysics->rigidBody->SetHitStatus(false);
			//std::cout << "Glob hit!" << std::endl;
			glm::vec3 startPos = globList[i]->m_EntityPhysics->rigidBody->GetColPos();
			startPos.y += 0.01f;
			LoadPaintCube(paintList, startPos);
			//globList[i]->m_EntityPhysics->rigidBody->SetHitStatus(false);
			globList[i]->m_EntityPhysics->rigidBody->SetEntityType(eEntityType::NONE);
			globList[i]->m_EntityMesh->bIsVisible = false;
			gPhysicsWorld->RemoveBody(globList[i]->m_EntityPhysics->rigidBody);
			timeRemaining += 10.0f;
		}
	}
}

bool CheckForHits(cEntity* enemy, cEntity* player, cEntity* weapon)
{
	//gPhysicsWorld->Update

	if (weapon->m_EntityPhysics->rigidBody->GetHitStatus())
	{
		if (enemy->m_EntityPhysics->rigidBody->GetHitStatus())
		{
			if (enemy->healthTimer < 0.001f)
			{
				if (enemy->status != eEntityStatus::TAKING_DAMAGE && enemy->status != eEntityStatus::DEAD)
				{
					if (player->status == eEntityStatus::ATTACKING)
					{

						float timer = enemy->m_EntityMesh->pSimpleSkinnedMesh->GetDuration("EnemyHit");
						enemy->healthTimer = timer;

						cAnimationState::sStateDetails newState;
						newState.name = "EnemyHit";
						newState.status = eEntityStatus::TAKING_DAMAGE;

						enemy->health -= 10.0f;

						enemy->m_EntityMesh->pAniState->vecAnimationQueue.clear();
						enemy->m_EntityMesh->pAniState->vecAnimationQueue.push_back(newState);
						resetHackTime(enemy);
						return true;
					}
					else
					{
						//enemy->m_EntityPhysics->rigidBody->SetHitStatus(false);
					}
				}
			}
		}
	}
	return false;
}

bool CheckForPlayerHit(cEntity* player, cEntity* weapon, cEntity* enemy)
{
	//gPhysicsWorld->Update

	if (weapon->m_EntityPhysics->rigidBody->GetHitStatus()) //if weapon hit
	{
		if (player->m_EntityPhysics->rigidBody->GetHitStatus()) //if player has been hit
		{
			if (player->healthTimer < 0.001f) //if player can take damage again
			{
				if (player->status != eEntityStatus::TAKING_DAMAGE && player->status != eEntityStatus::DEAD)
				{
					if (enemy->status == eEntityStatus::ATTACKING)
					{

						float timer = player->m_EntityMesh->pSimpleSkinnedMesh->GetDuration("Hit");
						player->healthTimer = timer;

						cAnimationState::sStateDetails newState;
						newState.name = "Hit";
						newState.status = eEntityStatus::TAKING_DAMAGE;

						player->health -= 10.0f;

						player->m_EntityMesh->pAniState->vecAnimationQueue.clear();
						player->m_EntityMesh->pAniState->vecAnimationQueue.push_back(newState);
						resetHackTime(player);
						return true;
					}
					else
					{
						//player->m_EntityPhysics->rigidBody->SetHitStatus(false);
					}
				}
			}
		}
	}
	return false;
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