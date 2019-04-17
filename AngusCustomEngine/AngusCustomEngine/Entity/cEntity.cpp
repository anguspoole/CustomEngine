#include "cEntity.h"
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

void cEntity::Update(double deltaTime)
{
	this->m_EntityPhysics->Update(deltaTime);
	UpdateHealthTimer(deltaTime);

}

void cEntity::UpdateHealthTimer(float dt)
{
	if (this->healthTimer > 0)
	{
			this->healthTimer -= dt;
	}
	else if (this->healthTimer <= 0 && this->status == eEntityStatus::TAKING_DAMAGE)
	{
			//kill or idle?
		if (this->friendlyName == "Enemy0")
		{
			//std::cout << this->status << std::endl;
		}
	}

	//kill this entity if they are low enough
	if (this->health < 1.0f && this->m_EntityPhysics->physObjType == cEntityPhysics::ePhysicsObjType::RIGID_BODY)
	{
		//Set to a flat 0
		this->health = 0.0f;

		//If this is an enemy, use the enemy animation
		if (this->m_EntityPhysics->rigidBody->GetEntityType() == eEntityType::ENEMY)
		{
			if (this->status != eEntityStatus::DEAD && this->m_EntityMesh->pAniState->vecAnimationQueue.size() < 1)
			{
				cAnimationState::sStateDetails newState;
				newState.name = "EnemyDeath";
				newState.status = eEntityStatus::DEAD;
				this->m_EntityMesh->pAniState->vecAnimationQueue.push_back(newState);
			}
		}
	}
}
