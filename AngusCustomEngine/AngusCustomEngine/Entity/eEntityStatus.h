#ifndef _E_ENTITYSTATUS_HG_
#define _E_ENTITYSTATUS_HG_

//The "state" of an entity, for animation and control purposes.

enum eEntityStatus
{
	IDLE = 0,
	RUNNING = 1,
	ATTACKING = 2,
	TAKING_DAMAGE = 3,
	DODGING = 4,
	DEAD = 5,
};

#endif // !_E_ENTITYSTATUS_HG_
