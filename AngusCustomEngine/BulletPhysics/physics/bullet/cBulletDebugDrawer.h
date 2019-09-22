#pragma once

/*
	The cBulletDebugDrawer class is for implementing debug drawing of bullet physics
	This will be used to verify that way objects appear and interact the same way physically and graphically.
*/

#include <btBulletDynamicsCommon.h> //includes the btIDebugDrawer
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace nPhysics
{
	class cBulletDebugDrawer : public btIDebugDraw
	{
	public:
		cBulletDebugDrawer(); //constructor
		~cBulletDebugDrawer(); //deconstructor

		virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);

		virtual void drawContactPoint(const btVector3 &, const btVector3 &, btScalar, int, const btVector3 &) { }
		virtual void reportErrorWarning(const char *) { }
		virtual void draw3DText(const btVector3 &, const char *) { }
		virtual void setDebugMode(int mode) { debugMode = mode; }
		virtual int getDebugMode(void) const { return debugMode; }

	private:
		int debugMode = 1;
		unsigned int debugVAO;
		unsigned int debugVBO;
	};
}