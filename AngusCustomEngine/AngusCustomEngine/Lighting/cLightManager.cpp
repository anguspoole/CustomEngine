#include "cLightManager.h"



void sLight::SetLightType( std::string typeAsString )
{
	// TODO:

	return;
}

void sLight::SetLightType( sLight::eLightType lightType )
{
	switch ( lightType )
	{
	case sLight::POINT_LIGHT:
		this->param1.x = 0.0f;		// Point
		break;
	case sLight::SPOT_LIGHT:
		this->param1.x = 1.0f;		// Spot
		break;
	case sLight::DIRECTIONAL_LIGHT:
		this->param1.x = 2.0f;		// Directional
		break;
	default:
		// Make point if we don't know
		// (shouldn't happen)
		this->param1.x = 0.0f;		// Point
		break;
	};

	return;
}

void sLight::SetSpotConeAngles( float innerAngleDegrees, float outerAngleDegrees )
{
	//if ( outerAngleDegrees > innerAngleDegrees )
	//{	// Switch them
	//	float temp = innerAngleDegrees;
	//	innerAngleDegrees = outerAngleDegrees;
	//	outerAngleDegrees = temp;
	//}

	//	glm::vec4 param1;		// x = lightType, y = inner angle, z = outer angle, w = TBD
	this->param1.y = innerAngleDegrees;
	this->param1.z = outerAngleDegrees;

	return;
}

void sLight::SetRelativeDirection( glm::vec3 relDirection )
{
	// To set the vec4 that's being passed
	this->direction = glm::vec4( relDirection, 1.0f );
	return;
}

void sLight::SetRelativeDirectionByEulerAngles( glm::vec3 directionAngle )
{
	// Take the angles and make a quaternion out of them
	// Use a mat4x4 x vec4 tranformation (just like the shader or in physics)
	// Use the final xyz location to send to SetRelativeDirectionByLookAt()

	// Ta-Da!

	return;
}

void sLight::SetRelativeDirectionByLookAt( glm::vec3 pointInWorldXYZ )
{
	// The vector from what I'm looking at to where I am, then normalize

	glm::vec3 lookVector = pointInWorldXYZ - glm::vec3(this->position);

	lookVector = glm::normalize( lookVector );

	this->SetRelativeDirection( lookVector );

	return;
}



