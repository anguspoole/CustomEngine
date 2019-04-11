#include "cEntity.h"



cEntity::cEntity()
{
}


cEntity::~cEntity()
{
	this->m_EntityMesh = 0;
	delete this->m_EntityMesh;
}

cEntity::cEntity(cEntityMesh * entityMesh)
{
	this->m_EntityMesh = entityMesh;
}

void cEntity::Update(double deltaTime)
{
	this->m_EntityPhysics->Update(deltaTime);
}
