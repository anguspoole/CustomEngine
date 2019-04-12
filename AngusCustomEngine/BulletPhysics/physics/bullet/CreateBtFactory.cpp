#include "CreateBtFactory.h"
#include "cBtFactory.h"

EXTERN_DLL_EXPORT nPhysics::iPhysicsFactory* CreateFactory()
{
	return new nPhysics::cBulletPhysicsFactory();
}