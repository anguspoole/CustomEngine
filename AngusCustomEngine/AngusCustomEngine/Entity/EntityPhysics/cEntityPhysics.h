#ifndef _C_ENTITY_PHYSICS_HG_
#define _C_ENTITY_PHYSICS_HG_

//This class holds physics info for cEntity objects

//In particular:
/*
Position, Velocity, Acceleration, Orientation, Mass
*/

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#define GLM_ENABLE_EXPERIMENTAL		// To get glm quaternion stuff to compile
#include <glm/gtx/quaternion.hpp>	// Note strange folder

// Physics Includes
#include <iEntity.h>

struct sTriangle
{
	glm::vec3 v[3];
	glm::vec3 n;
};

class cEntityPhysics
{
public:
	cEntityPhysics();
	~cEntityPhysics();

	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 accel;

	glm::vec3 nonUniformScale;
	float mass;
	float inverseMass;

	bool bIsUpdatedByPhysics;	// Phsyics "owns" the position	

	enum ePhysicsObjType
	{
		NONE = 0,
		RIGID_BODY = 1,
		SOFT_BODY = 2,
		SOFT_BODY_NODE = 3
	};

	ePhysicsObjType physObjType = ePhysicsObjType::NONE;

	nPhysics::iRigidBody* rigidBody;
	nPhysics::iSoftBody* softBody;

	glm::quat getQOrientation(void) { return this->m_meshQOrientation; };
	void setQOrientation(glm::quat newOrientation)
	{
		this->m_meshQOrientation = newOrientation;
	}
	void setMeshOrientationEulerAngles(glm::vec3 newAnglesEuler, bool bIsDegrees = false);
	void setMeshOrientationEulerAngles(float x, float y, float z, bool bIsDegrees = false);
	void adjMeshOrientationEulerAngles(glm::vec3 adjAngleEuler, bool bIsDegrees = false);
	void adjMeshOrientationEulerAngles(float x, float y, float z, bool bIsDegrees = false);
	void adjMeshOrientationQ(glm::quat adjOrientQ);

	void setUniformScale(float scale);

	void Update(double deltaTime);

private:
	glm::quat m_meshQOrientation;		// Like a mat3x3 rotation matrix
};

#endif // !_C_ENTITY_PHYSICS_HG_

