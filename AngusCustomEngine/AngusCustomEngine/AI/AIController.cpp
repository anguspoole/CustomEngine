#include "AIController.h"
#include "../Global/global.h"


AIController::AIController()
{
}


AIController::~AIController()
{
}

void AIController::UpdateBehaviours(std::vector<cEntity*> aiEntities, cEntity* player)
{
	//cEntity * playerEntity = findObjectByFriendlyName("Player");
	//cEntity * katana = findObjectByFriendlyName("Katana");
	cEntity * katana = player->vec_pChildrenEntities[0];

	for (int i = 0; i < aiEntities.size(); i++)
	{
		if (CheckForHits(aiEntities[i], player, katana))
		{
			continue;
		}
		if (aiEntities[i]->status == eEntityStatus::IDLE || aiEntities[i]->status == eEntityStatus::RUNNING)
		{
			glm::mat4 aiTransform;
			glm::mat4 playerTransform;
			aiEntities[i]->m_EntityPhysics->rigidBody->GetTransform(aiTransform);
			player->m_EntityPhysics->rigidBody->GetTransform(playerTransform);
			glm::vec3 playerPos = glm::vec3(playerTransform[3].x, playerTransform[3].y, playerTransform[3].z);
			glm::vec3 aiPos = glm::vec3(aiTransform[3].x, aiTransform[3].y, aiTransform[3].z);
			if (glm::distance(playerPos, aiPos) < 30.0f)
			{
				glm::mat4 newLook = glm::inverse(glm::lookAt(playerPos, aiPos, glm::vec3(0.0f, 1.0f, 0.0f)));
				//glm::quat newOrient = glm::toQuat(newLook);
				aiEntities[i]->m_EntityPhysics->rigidBody->SetOrientation(newLook);
				if (aiEntities[i]->m_EntityMesh->pAniState->vecAnimationQueue.size() == 0)
				{
					cAnimationState::sStateDetails newState;
					newState.name = "EnemyRun";
					newState.status = eEntityStatus::RUNNING;
					aiEntities[i]->m_EntityMesh->pAniState->vecAnimationQueue.push_back(newState);

					//Reset animation time only if we were in idle
					if (aiEntities[i]->status == eEntityStatus::IDLE)
					{
						resetHackTime(aiEntities[i]);
					}
				}
				if (aiEntities[i]->status == eEntityStatus::RUNNING) //we're running, so set speed
				{
					//Need to get the velocity so we're still affected by gravity
					glm::vec3 vel;
					aiEntities[i]->m_EntityPhysics->rigidBody->GetVelocity(vel);
					float runSpeed = 10.0f;

					glm::vec3 run = glm::vec3(0.0f, vel.y, runSpeed);
					//aiEntities[i]->m_EntityPhysics->rigidBody->SetVelocity(run);
				}
				if (glm::distance(playerPos, aiPos) < 15.0f)
				{
					glm::mat4 newLook = glm::inverse(glm::lookAt(playerPos, aiPos, glm::vec3(0.0f, 1.0f, 0.0f)));
					//glm::quat newOrient = glm::toQuat(newLook);
					aiEntities[i]->m_EntityPhysics->rigidBody->SetOrientation(newLook);

					cAnimationState::sStateDetails newState;
					newState.name = "EnemyAttack";
					newState.status = eEntityStatus::ATTACKING;
					aiEntities[i]->m_EntityMesh->pAniState->vecAnimationQueue.clear();
					aiEntities[i]->m_EntityMesh->pAniState->vecAnimationQueue.push_back(newState);
				}
			}
		}
	}
}
