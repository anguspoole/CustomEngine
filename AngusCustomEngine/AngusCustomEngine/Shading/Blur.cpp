#include "../Global/global.h"
#include "../Global/gOpenGL.h"
#include "cShaderManager.h"

void makeBlur(GLuint programID, cFBO* fboA, cFBO* fboB, GLuint program)
{
	cShaderManager::cShaderProgram* blurSP = ::pTheShaderManager->pGetShaderProgramFromFriendlyName("BasicUberShader");

	bool horizontal = true;
	int amount = 50;
	cFBO* currentFBO = g_pFBOBlurA;
	cFBO* otherFBO = g_pFBOBlurB;
	cFBO* tempFBO = NULL;

	cEntity* fullQuad = findObjectByFriendlyName("2SidedQuadBloomA");
	cEntity* fullQuad2 = findObjectByFriendlyName("2SidedQuadBloomB");
	GLint renderPassNumber_UniLoc = glGetUniformLocation(program, "renderPassNumber");

	for (unsigned int i = 0; i < amount; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, currentFBO->ID);

		if (i == 0 || i == 1)
		{
			glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
			float ratio = windowWidth / (float)windowHeight;
			glViewport(0, 0, windowWidth, windowHeight);

			glEnable(GL_DEPTH);		// Enables the KEEPING of the depth information
			glEnable(GL_DEPTH_TEST);	// When drawing, checked the existing depth
			glEnable(GL_CULL_FACE);	// Discared "back facing" triangles

			// 2. Clear everything **ON THE MAIN FRAME BUFFER** 
			//     (NOT the offscreen buffer)
			// This clears the ACTUAL screen framebuffer
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		GLint horizontal_LocID = glGetUniformLocation(blurSP->ID, "horizontal");
		glUniform1f(horizontal_LocID, horizontal);
		glUniform1f(renderPassNumber_UniLoc, 3.0f);	// Tell shader it's the 2nd pass

		glm::mat4 matModel = glm::mat4(1.0f);
		if (i % 2 == 0)
		{
			fullQuad->m_EntityMesh->bIsVisible = true;
			if (i == 0)
				DrawObject(fullQuad, matModel, program, 1, g_pFBOMain);
			else
				DrawObject(fullQuad, matModel, program, 0, otherFBO);
			fullQuad->m_EntityMesh->bIsVisible = false;
		}
		else
		{
			fullQuad2->m_EntityMesh->bIsVisible = true;
			DrawObject(fullQuad2, matModel, program, 0, otherFBO);
			fullQuad2->m_EntityMesh->bIsVisible = false;
		}

		horizontal = !horizontal;
		tempFBO = currentFBO;
		currentFBO = otherFBO;
		otherFBO = tempFBO;
	}
	//g_pFBOFinal = otherFBO;
}