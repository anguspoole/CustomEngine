#pragma once
#include <game_math.h>
#include "LinearMath/btVector3.h"
#include "LinearMath/btTransform.h"

namespace nPhysics
{
	namespace nConvert
	{
		inline glm::vec3 ToSimple(const btVector3& vecIn)
		{
			return glm::vec3(vecIn.x(), vecIn.y(), vecIn.z());
		}
		inline void ToSimple(const btVector3& vecIn, glm::vec3& vecOut)
		{
			vecOut.x = vecIn.x();
			vecOut.y = vecIn.x();
			vecOut.z = vecIn.x();
		}
		inline btVector3 ToBullet(const glm::vec3& vec)
		{
			return btVector3(vec.x, vec.y, vec.z);
		}
		inline void ToBullet(const glm::vec3& vecIn, btVector3& vecOut)
		{
			vecOut.setValue(vecIn.x, vecIn.y, vecIn.z);
		}
		inline glm::mat4 ToSimple(const btTransform& transformIn)
		{
			glm::mat4 mat;
			transformIn.getOpenGLMatrix(&mat[0][0]);
			return mat;
		}
		inline void ToSimple(const btTransform& transform, glm::mat4& matOut)
		{
			transform.getOpenGLMatrix(&matOut[0][0]);
		}
	}
}
