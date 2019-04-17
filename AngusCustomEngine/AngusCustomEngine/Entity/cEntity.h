#ifndef _C_ENTITY_HG_
#define _C_ENTITY_HG_

#include "EntityMesh/cEntityMesh.h"
#include "EntityPhysics/cEntityPhysics.h"
#include "eEntityStatus.h"

class cEntity
{
public:
	cEntity();
	~cEntity();

	//The entity's mesh info - made public because it will be accessed frequently
	cEntityMesh* m_EntityMesh;
	
	//The entity's physics info - made public because it will be accessed frequently
	cEntityPhysics* m_EntityPhysics;

	std::string friendlyName;	// Human "Bob" "Ali" "Player"

	eEntityStatus status = eEntityStatus::IDLE;

	// Child objects inside this one
	std::vector< cEntity* > vec_pChildrenEntities;

	float animTime = 0.0f;

	float health = 0.0f;
	float healthTimer = 0.0f;

	void Update(double deltaTime);
	void UpdateHealthTimer(float dt);

private:

};

#endif // !_C_ENTITY_HG_

