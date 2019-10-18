#include "cEntity.h"
#include "../Global/globalParticle.h"
#include <iostream>


cEntity::cEntity()
{
	this->m_EntityMesh = new cEntityMesh();
	this->m_EntityPhysics = new cEntityPhysics();
}


cEntity::~cEntity()
{
	this->m_EntityMesh = 0;
	delete this->m_EntityMesh;
}

void cEntity::UpdateHitStatus(bool status)
{
	if (this->m_EntityPhysics->physObjType == cEntityPhysics::ePhysicsObjType::RIGID_BODY)
	{
		//this->m_EntityPhysics->rigidBody->SetHitStatus(status);
		//eEntityType eT = this->m_EntityPhysics->rigidBody->GetEntityType();
		//if (eT == eEntityType::PLAYER || eT == eEntityType::ENEMY)
		//{
		//	if (this->healthTimer < 0.0f)
		//	{
		//		this->healthTimer = 0.0f;
		//		this->m_EntityPhysics->rigidBody->SetHitStatus(status);
		//	}
		//}
		//else if (eT == eEntityType::PLAYERWEAPON || eT == eEntityType::ENEMYWEAPON)
		//{
		//	this->m_EntityPhysics->rigidBody->SetHitStatus(status);
		//}
	}
}

void cEntity::Update(double deltaTime)
{
	this->m_EntityPhysics->Update(deltaTime);
	UpdateHealthTimer(deltaTime);
	//UpdateHitStatus(false); //reset the hit status to false after every update
}

void cEntity::UpdateHealthTimer(float dt)
{
	if (this->healthTimer > 0)
	{
			this->healthTimer -= dt;
	}
	else if (this->healthTimer <= 0 && this->status == eEntityStatus::TAKING_DAMAGE)
	{
		//this->status = eEntityStatus::IDLE;
		//this->m_EntityPhysics->rigidBody->SetHitStatus(false);
		//kill or idle?
		//if (this->friendlyName == "Enemy0")
		//{
		//	//std::cout << this->status << std::endl;
		//}
	}

	//kill this entity if they are low enough
	if (this->health < 1.0f && this->m_EntityPhysics->physObjType == cEntityPhysics::ePhysicsObjType::RIGID_BODY)
	{
		//Set to a flat 0
		this->health = 0.0f;

		//If this is an enemy, use the enemy animation
		if (this->m_EntityPhysics->rigidBody->GetEntityType() == eEntityType::ENEMY)
		{
			if (this->status != eEntityStatus::DEAD)
			{
				cAnimationState::sStateDetails newState;
				newState.name = "EnemyDeath";
				newState.status = eEntityStatus::DEAD;
				this->m_EntityMesh->pAniState->vecAnimationQueue.clear();
				this->m_EntityMesh->pAniState->vecAnimationQueue.push_back(newState);
				glm::vec3 swordPos;
				this->m_EntityPhysics->rigidBody->GetPosition(swordPos);
				g_map_pParticleEmitters["Fire01"]->UpdateParamsPosition(swordPos);
				g_map_pParticleEmitters["Fire01"]->Reset(true);
			}
		}
		else if (this->m_EntityPhysics->rigidBody->GetEntityType() == eEntityType::PLAYER)
		{
			if (this->status != eEntityStatus::DEAD)
			{
				cAnimationState::sStateDetails newState;
				newState.name = "Death";
				newState.status = eEntityStatus::DEAD;
				this->m_EntityMesh->pAniState->vecAnimationQueue.clear();
				this->m_EntityMesh->pAniState->vecAnimationQueue.push_back(newState);
				glm::vec3 swordPos;
				this->m_EntityPhysics->rigidBody->GetPosition(swordPos);
				g_map_pParticleEmitters["Smoke01"]->UpdateParamsPosition(swordPos);
				g_map_pParticleEmitters["Smoke01"]->Reset(true);
			}
		}
	}
}

void cEntity::Equip(int boneID, cEntity * eqEntity)
{
	this->boneObjectMap.insert(std::pair<int, cEntity*>(boneID, eqEntity));
}
