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

		cEntity* userSphere = findObjectByFriendlyName("sphere0");

		glm::vec3 velocity;
		glm::mat4 transform;
		userSphere->m_EntityPhysics->rigidBody->GetVelocity(velocity);
		userSphere->m_EntityPhysics->rigidBody->GetTransform(transform);
		glm::vec3 pos = glm::vec3(transform[3].x, transform[3].y, transform[3].z);
		glm::vec3 newVel = g_Camera->getQOrientation() * velocity;

		glm::vec3 newAt = ::g_Camera->getAtInWorldSpace();
		newAt.y = ::g_Camera->eye.y;

		glm::vec3 dir = glm::normalize(newAt - g_Camera->eye);
		glm::vec3 veln = glm::normalize(velocity);
		glm::vec3 dir2 = glm::normalize(veln);

		glm::quat newRotation;
		glm::vec3 crossProduct = glm::cross(dir2, dir);
		newRotation.x = crossProduct.x;
		newRotation.y = crossProduct.y;
		newRotation.z = crossProduct.z;
		newRotation.w = glm::sqrt((dir2.length() * dir2.length())
			* (dir.length() * dir.length())) + glm::dot(dir2, dir);
		newRotation = glm::normalize(newRotation);

		if (glfwGetKey(window, GLFW_KEY_W))
		{
			velocity.z += speed;
			//velocity = newRotation * velocity;
			userSphere->m_EntityPhysics->rigidBody->ApplyForce(glm::vec3(0.0f, 0.0f, 10.0f));
			//userSphere->rigidBody->SetVelocity(velocity);
		}
		if (glfwGetKey(window, GLFW_KEY_S))
		{
			velocity.z -= speed;
			//velocity = newRotation * velocity;
			userSphere->m_EntityPhysics->rigidBody->ApplyForce(glm::vec3(0.0f, 0.0f, -10.0f));
			//userSphere->rigidBody->SetVelocity(velocity);
		}
		if (glfwGetKey(window, GLFW_KEY_A))
		{
			velocity.x += speed;
			//velocity = newRotation * velocity;
			userSphere->m_EntityPhysics->rigidBody->ApplyForce(glm::vec3(10.0f, 0.0f, 0.0f));
			//userSphere->rigidBody->SetVelocity(velocity);
		}
		if (glfwGetKey(window, GLFW_KEY_D))
		{
			velocity.x -= speed;
			//velocity = newRotation * velocity;
			userSphere->m_EntityPhysics->rigidBody->ApplyForce(glm::vec3(-10.0f, 0.0f, 0.0f));
			//userSphere->rigidBody->SetVelocity(velocity);
		}
		if (glfwGetKey(window, GLFW_KEY_Q))
		{
			velocity.y -= speed;
			//velocity = newRotation * velocity;
			//userSphere->rigidBody->SetVelocity(velocity);
			userSphere->m_EntityPhysics->rigidBody->ApplyForce(glm::vec3(0.0f, -2.0f, 0.0f));
		}
		if (glfwGetKey(window, GLFW_KEY_E))
		{
			velocity.y += speed;
			//velocity = newRotation * velocity;
			//userSphere->rigidBody->SetVelocity(velocity);
			userSphere->m_EntityPhysics->rigidBody->ApplyForce(glm::vec3(0.0f, 2.0f, 0.0f));
		}

		//if (glfwGetKey(window, GLFW_KEY_I)) { pTheShip->adjMeshOrientationEulerAngles(glm::vec3(95.0f, 0.0f, 0.0f), true); }
		if (glfwGetKey(window, GLFW_KEY_I)) {
			//userSphere->adjMeshOrientationEulerAngles(glm::vec3(glm::radians(10.0f), glm::radians(0.0f), glm::radians(0.0f)), true);
			::g_Camera->adjMeshOrientationEulerAngles(glm::vec3(glm::radians(10.0f), glm::radians(0.0f), glm::radians(0.0f)), true);
		}
		if (glfwGetKey(window, GLFW_KEY_K)) {
			//userSphere->adjMeshOrientationEulerAngles(glm::vec3(glm::radians(-10.0f), glm::radians(0.0f), glm::radians(0.0f)), true);
			::g_Camera->adjMeshOrientationEulerAngles(glm::vec3(glm::radians(-10.0f), glm::radians(0.0f), glm::radians(0.0f)), true);
		}
		if (glfwGetKey(window, GLFW_KEY_J)) {
			//userSphere->adjMeshOrientationEulerAngles(glm::vec3(glm::radians(0.0f), glm::radians(10.0f), glm::radians(0.0f)), true);
			::g_Camera->adjMeshOrientationEulerAngles(glm::vec3(glm::radians(0.0f), glm::radians(10.0f), glm::radians(0.0f)), true);
		}
		if (glfwGetKey(window, GLFW_KEY_L)) {
			//userSphere->adjMeshOrientationEulerAngles(glm::vec3(glm::radians(0.0f), glm::radians(-10.0f), glm::radians(0.0f)), true);
			::g_Camera->adjMeshOrientationEulerAngles(glm::vec3(glm::radians(0.0f), glm::radians(-10.0f), glm::radians(0.0f)), true);
		}
		if (glfwGetKey(window, GLFW_KEY_U)) {
			//userSphere->adjMeshOrientationEulerAngles(glm::vec3(glm::radians(0.0f), glm::radians(0.0f), glm::radians(-10.0f)), true);
			::g_Camera->adjMeshOrientationEulerAngles(glm::vec3(glm::radians(0.0f), glm::radians(0.0f), glm::radians(-10.0f)), true);
		}
		if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)	// "down"
		{
			::g_Camera->MoveForward_Z(speed);
			//			::g_Camera->MoveUpDown_Y( -cameraSpeed );
		}
		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)	// "down"
		{
			::g_Camera->MoveForward_Z(-speed);
			//			::g_Camera->MoveUpDown_Y( -cameraSpeed );
		}
	}
}