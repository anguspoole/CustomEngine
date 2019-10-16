#ifndef _C_ENTITY_HG_
#define _C_ENTITY_HG_

#include "EntityMesh/cEntityMesh.h"
#include "EntityPhysics/cEntityPhysics.h"
#include "eEntityStatus.h"
#include "../Meshes/AssimpFBX/cAnimationState.h"
#include <map>

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

	std::map<int, cEntity*> boneObjectMap;

	float animTime = 0.0f;

	float health = 30.0f;
	float healthTimer = 0.0f;

	bool spray = true;

	int mazeX = 0;
	int mazeZ = 0;

	void Update(double deltaTime);
	void UpdateHealthTimer(float dt);
	void UpdateHitStatus(bool status);

	void Equip(int boneID, cEntity* eqEntity);
	//void Unequip(int boneID);

private:

};

#endif // !_C_ENTITY_HG_

