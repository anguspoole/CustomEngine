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
}
