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
	for (int i = 0; i < aiEntities.size(); i++)
	{
		if (aiEntities[i]->status == eEntityStatus::IDLE)
		{
			glm::mat4 aiTransform;
			glm::mat4 playerTransform;
			aiEntities[i]->m_EntityPhysics->rigidBody->GetTransform(aiTransform);
			player->m_EntityPhysics->rigidBody->GetTransform(playerTransform);
			if (glm::distance(playerTransform[3], aiTransform[3]) < 30.0f)
			{
				glm::mat4 newLook = glm::inverse(glm::lookAt(glm::vec3(playerTransform[3].x, playerTransform[3].y, playerTransform[3].z), glm::vec3(aiTransform[3].x, aiTransform[3].y, aiTransform[3].z), glm::vec3(0.0f, 1.0f, 0.0f)));
				//glm::quat newOrient = glm::toQuat(newLook);
				aiEntities[i]->m_EntityPhysics->rigidBody->SetOrientation(newLook);
				if (aiEntities[i]->m_EntityMesh->pAniState->vecAnimationQueue.size() == 0)
				{
					cAnimationState::sStateDetails newState;
					newState.name = "EnemyRun";
					newState.status = eEntityStatus::RUNNING;
					aiEntities[i]->m_EntityMesh->pAniState->vecAnimationQueue.push_back(newState);
					resetHackTime(aiEntities[i]);
				}
			}
		}
		else if (aiEntities[i]->status == eEntityStatus::RUNNING)
		{
			glm::mat4 aiTransform;
			glm::mat4 playerTransform;
			aiEntities[i]->m_EntityPhysics->rigidBody->GetTransform(aiTransform);
			player->m_EntityPhysics->rigidBody->GetTransform(playerTransform);
			if (glm::distance(playerTransform[3], aiTransform[3]) < 30.0f)
			{
				glm::mat4 newLook = glm::inverse(glm::lookAt(glm::vec3(playerTransform[3].x, playerTransform[3].y, playerTransform[3].z), glm::vec3(aiTransform[3].x, aiTransform[3].y, aiTransform[3].z), glm::vec3(0.0f, 1.0f, 0.0f)));
				//glm::quat newOrient = glm::toQuat(newLook);
				aiEntities[i]->m_EntityPhysics->rigidBody->SetOrientation(newLook);

				float runSpeed = 5.0f;

				glm::vec3 run = glm::vec3(0.0f, 0.0f, runSpeed);
				aiEntities[i]->m_EntityPhysics->rigidBody->SetVelocity(run);
			}
		}
	}

}
