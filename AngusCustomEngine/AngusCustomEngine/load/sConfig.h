#pragma once
#include <string>
#include <vector>
#include <sRigidBodyDef.h>
#include <sSoftBodyDef.h>
#include <eShapeType.h>

namespace nLoad
{
	struct sConfig
	{
		std::string PhysicsLibrary;
		std::vector<std::string> RigidObjTypes;
		std::vector<std::string> SoftObjTypes;
		std::vector<std::string> RigidShapeTypes;
		std::vector<std::string> SoftShapeTypes;
		std::vector<nPhysics::sRigidBodyDef> RigidBodyDefs;
		std::vector<nPhysics::sSoftBodyDef> SoftBodyDefs;
	};
}