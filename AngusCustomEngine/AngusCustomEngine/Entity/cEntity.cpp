#include "cEntity.h"



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
}
