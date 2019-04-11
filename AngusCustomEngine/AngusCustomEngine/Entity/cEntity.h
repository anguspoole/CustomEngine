#ifndef _C_ENTITY_HG_
#define _C_ENTITY_HG_

#include "EntityMesh/cEntityMesh.h"
#include "EntityPhysics/cEntityPhysics.h"

class cEntity
{
public:
	cEntity();
	~cEntity();

	cEntity(cEntityMesh* entityMesh);

	//The entity's mesh info - made public because it will be accessed frequently
	cEntityMesh* m_EntityMesh;
	
	//The entity's physics info - made public because it will be accessed frequently
	cEntityPhysics* m_EntityPhysics;

	std::string friendlyName;	// Human "Bob" "Ali" "Player"

	// Child objects inside this one
	std::vector< cEntity* > vec_pChildrenEntities;

	void Update(double deltaTime);

private:
};

#endif // !_C_ENTITY_HG_

