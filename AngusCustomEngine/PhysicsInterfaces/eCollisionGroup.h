#pragma once

enum eCollisionGroup
{
	COL_NONE = 0,
	COL_ENVIRONMENT = 1 << 0,
	COL_PLAYER = 1 << 1,
	COL_ENEMY = 1 << 2,
	COL_PLAYERWEAPON = 1 << 3,
	COL_ENEMYWEAPON = 1 << 4,
	COL_PAINTGLOB = 1 << 5
};