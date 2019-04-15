#include "../Global/gOpenGL.h"

#include "../Global/global.h"

#include <iostream>
#include <algorithm>
#include <glm/gtx/rotate_vector.hpp>

bool g_MouseIsInsideWindow = false;

void cursor_enter_callback(GLFWwindow* window, int entered)
{
	if (entered)
	{
		::g_MouseIsInsideWindow = true;
		std::cout << "Mouse moved indide window" << std::endl;
	}
	else
	{
		::g_MouseIsInsideWindow = false;
		std::cout << "Mouse moved outside window" << std::endl;
	}
	return;
}//cursor_enter_callback(...

// Mouse (cursor) callback
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{

	return;
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

	return;
}

//Function for getting key callback
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	cEntity* pSM = findObjectByFriendlyName("Player");

	if (animationComplete)
	{
		//If not moving, default to Idle animation
		if ((pSM->m_EntityPhysics->velocity.x + pSM->m_EntityPhysics->velocity.y + pSM->m_EntityPhysics->velocity.z) < 0.01f && pSM->m_EntityMesh->currentAnimation != "Unarmed-Walk")
		{
			pSM->m_EntityMesh->currentAnimation = "Idle";
		}
	}

	//Close window if Escape is pressed
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

}

bool IsShiftDown(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT)) { return true; }
	// both are up
	return false;
}

bool IsCtrlDown(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL)) { return true; }
	// both are up
	return false;
}

bool IsAltDown(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_RIGHT_ALT)) { return true; }
	// both are up
	return false;
}

bool AreAllModifiersUp(GLFWwindow* window)
{
	if (IsShiftDown(window)) { return false; }
	if (IsCtrlDown(window)) { return false; }
	if (IsAltDown(window)) { return false; }
	// Yup, they are all UP
	return true;
}

void ProcessAsyncKeys(GLFWwindow* window)
{
	// If no keys are down, move the camera
	if (AreAllModifiersUp(window))
	{
		float speed = 2.0f;

		//cEntity* userSphere = findObjectByFriendlyName("sphere0");
		cEntity* userSphere = findObjectByFriendlyName("Player");


		
		glm::mat4 targetTransform;
		player->m_EntityPhysics->rigidBody->GetTransform(targetTransform);
		glm::vec3 oldTargetPos = glm::vec3(targetTransform[3].x, targetTransform[3].y, targetTransform[3].z);
		glm::vec3 targetPos = oldTargetPos;
		targetPos.y = player_Camera->eye.y;
		glm::vec3 targetDir = glm::normalize(targetPos - player_Camera->eye);
		glm::quat q = glm::inverse(glm::lookAt(targetPos - player_Camera->eye, targetDir, player_Camera->getUpVector()));
		player_Camera->setMeshOrientationQ(q);
		player_Camera->m_UpdateAtFromOrientation();

		glm::mat4 matVelRotation = glm::mat4(::player_Camera->getQOrientation());

		//Need to get the velocity so we're still affected by gravity
		glm::vec3 vel;
		player->m_EntityPhysics->rigidBody->GetVelocity(vel);

		if (glfwGetKey(window, GLFW_KEY_W))
		{
			glm::vec3 force = matVelRotation * glm::vec4(0.0f, 0.0f, 2.0f, 1.0f);
			force.y = vel.y;
			player->m_EntityPhysics->rigidBody->SetVelocity(force);
		}
		if (glfwGetKey(window, GLFW_KEY_S))
		{
			glm::vec3 force = matVelRotation * glm::vec4(0.0f, 0.0f, -2.0f, 1.0f);
			force.y = vel.y;
			player->m_EntityPhysics->rigidBody->SetVelocity(force);
		}
		if (glfwGetKey(window, GLFW_KEY_A))
		{
			glm::vec3 force = matVelRotation * glm::vec4(-2.0f, 0.0f, 0.0f, 1.0f);
			force.y = vel.y;
			player->m_EntityPhysics->rigidBody->SetVelocity(force);
		}
		if (glfwGetKey(window, GLFW_KEY_D))
		{
			glm::vec3 force = matVelRotation * glm::vec4(2.0f, 0.0f, 0.0f, 1.0f);
			force.y = vel.y;
			player->m_EntityPhysics->rigidBody->SetVelocity(force);
		}

		//if (glfwGetKey(window, GLFW_KEY_I)) { pTheShip->adjMeshOrientationEulerAngles(glm::vec3(95.0f, 0.0f, 0.0f), true); }
		if (glfwGetKey(window, GLFW_KEY_I)) {
			//userSphere->adjMeshOrientationEulerAngles(glm::vec3(glm::radians(10.0f), glm::radians(0.0f), glm::radians(0.0f)), true);
			::player_Camera->adjMeshOrientationEulerAngles(glm::vec3(glm::radians(10.0f), glm::radians(0.0f), glm::radians(0.0f)), true);
		}
		if (glfwGetKey(window, GLFW_KEY_K)) {
			//userSphere->adjMeshOrientationEulerAngles(glm::vec3(glm::radians(-10.0f), glm::radians(0.0f), glm::radians(0.0f)), true);
			::player_Camera->adjMeshOrientationEulerAngles(glm::vec3(glm::radians(-10.0f), glm::radians(0.0f), glm::radians(0.0f)), true);
		}
		if (glfwGetKey(window, GLFW_KEY_J)) {
			//userSphere->adjMeshOrientationEulerAngles(glm::vec3(glm::radians(0.0f), glm::radians(10.0f), glm::radians(0.0f)), true);
			::player_Camera->adjMeshOrientationEulerAngles(glm::vec3(glm::radians(0.0f), glm::radians(10.0f), glm::radians(0.0f)), true);
		}
		if (glfwGetKey(window, GLFW_KEY_L)) {
			//userSphere->adjMeshOrientationEulerAngles(glm::vec3(glm::radians(0.0f), glm::radians(-10.0f), glm::radians(0.0f)), true);
			::player_Camera->adjMeshOrientationEulerAngles(glm::vec3(glm::radians(0.0f), glm::radians(-10.0f), glm::radians(0.0f)), true);
		}
		if (glfwGetKey(window, GLFW_KEY_U)) {
			//userSphere->adjMeshOrientationEulerAngles(glm::vec3(glm::radians(0.0f), glm::radians(0.0f), glm::radians(-10.0f)), true);
			::player_Camera->adjMeshOrientationEulerAngles(glm::vec3(glm::radians(0.0f), glm::radians(0.0f), glm::radians(-10.0f)), true);
		}
		if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)	// "down"
		{
			::player_Camera->MoveForward_Z(speed);
			//			::player_Camera->MoveUpDown_Y( -cameraSpeed );
		}
		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)	// "down"
		{
			::player_Camera->MoveForward_Z(-speed);
			//			::player_Camera->MoveUpDown_Y( -cameraSpeed );
		}


		targetPos = oldTargetPos;
		targetDir = glm::normalize(targetPos - player_Camera->eye);
		q = glm::inverse(glm::lookAt(targetPos - player_Camera->eye, targetDir, player_Camera->getUpVector()));
		player_Camera->setMeshOrientationQ(q);
		player_Camera->m_UpdateAtFromOrientation();
	}
}