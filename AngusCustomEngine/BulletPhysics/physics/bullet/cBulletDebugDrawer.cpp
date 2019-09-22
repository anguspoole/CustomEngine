#include "cBulletDebugDrawer.h"

namespace nPhysics
{
	cBulletDebugDrawer::cBulletDebugDrawer()
	{
		glGenBuffers(1, &debugVBO);
		glGenVertexArrays(1, &debugVAO);
	}

	cBulletDebugDrawer::~cBulletDebugDrawer()
	{
		glDeleteBuffers(1, &debugVBO);
		glDeleteVertexArrays(1, &debugVAO);
	}

	void cBulletDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
	{
		//Line Info
		GLfloat line[12];

		line[0] = from.x();
		line[1] = from.y();
		line[2] = from.z();
		line[3] = color.x();
		line[4] = color.y();
		line[5] = color.z();
		line[6] = to.x();
		line[7] = to.y();
		line[8] = to.z();
		line[9] = color.x();
		line[10] = color.y();
		line[11] = color.z();

		glBindVertexArray(debugVAO);
		glBindBuffer(GL_ARRAY_BUFFER, debugVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(line), &line, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glDrawArrays(GL_LINES, 0, 2);
		glBindVertexArray(0);
	}
}
