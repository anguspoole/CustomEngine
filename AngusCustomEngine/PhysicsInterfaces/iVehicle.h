#pragma once
#include <game_math.h>

namespace nPhysics
{
	//iCollisionBody should have getType and getAABB

	//the idea is we can add various bodies (cube, sphere, vehicle, etc) all in one method
	//based on the type, instead of a different method for every type
	//ie waste of time for add rigid, add soft, add vehicle, etc. when it can be done in 1 universal method
	//that checks the type

	class iVehicle : public iCollisionBody
	{
	public:
		virtual ~iVehicle() {}
		virtual void SetSteering(float steeringValue) = 0;
		virtual void SetBrakes(float brakesValue) = 0;
		virtual void MakeEngineGo(float howMuchToGo) = 0;
		virtual void GetChassisTransform(glm::mat4& transformOut) = 0;
		virtual void GetWheelTransform(glm::mat4& transformOut) = 0;
	};
}