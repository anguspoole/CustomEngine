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
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		if (player->status == eEntityStatus::IDLE || player->status == eEntityStatus::RUNNING)
		{
			cAnimationState::sStateDetails newState;
			newState.name = "Stab-Attack";
			newState.status = player->status = eEntityStatus::ATTACKING;

			//if (player->m_EntityMesh->pAniState->vecAnimationQueue.size() == 0)
			//{
			player->m_EntityMesh->pAniState->vecAnimationQueue.clear();
			player->m_EntityMesh->pAniState->vecAnimationQueue.push_back(newState);
			//}
			resetHackTime(player);
		}
		//glm::vec3 swordPos;
		//player->vec_pChildrenEntities[0]->m_EntityPhysics->rigidBody->GetPosition(swordPos);
		//g_map_pParticleEmitters["Smoke01"]->UpdateParamsPosition(swordPos);
		//g_map_pParticleEmitters["Smoke01"]->Reset(true);
	}
	return;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	// A regular mouse wheel returns the y value
	::player_Camera->setMouseWheelDelta(yoffset);

	//	std::cout << "Mouse wheel: " << yoffset << std::endl;

	return;
}

void ProcessAsyncMouse(GLFWwindow* window)
{
	double x, y;
	glfwGetCursorPos(window, &x, &y);

	::player_Camera->setMouseXY(x, y);

	const float MOUSE_SENSITIVITY = 0.1f;

	//	std::cout << ::player_Camera->getMouseX() << ", " << ::player_Camera->getMouseY() << std::endl;

	glm::mat4 transform;
	player->m_EntityPhysics->rigidBody->GetTransform(transform);
	//player->vec_pChildrenEntities[0]->m_EntityPhysics->rigidBody->GetTransform(transform);
	glm::vec3 targetPos = glm::vec3(transform[3].x, transform[3].y, transform[3].z);

	//Direction from Camera to Target
	glm::vec3 dir = player_Camera->eye - targetPos;
	//Distance from Camera to Target
	float dist = glm::distance(player_Camera->eye, targetPos);
	//Normalized Direction from Camera to Target
	glm::vec3 dirN = glm::normalize(dir);

	//Move Camera to target position
	glm::vec3 oldEye = player_Camera->eye;
	player_Camera->eye = targetPos;

	// Mouse left (primary?) button pressed? 
	// AND the mouse is inside the window...
	if (g_MouseIsInsideWindow)
	//{
		// Mouse button is down so turn the camera
		::player_Camera->Yaw_LeftRight(-::player_Camera->getDeltaMouseX() * MOUSE_SENSITIVITY);
		glm::vec3 dirNR = glm::rotate(dirN, -::player_Camera->getDeltaMouseX() * MOUSE_SENSITIVITY * 0.1f, ::player_Camera->getUpVector());
		::player_Camera->Pitch_UpDown(::player_Camera->getDeltaMouseY() * MOUSE_SENSITIVITY);
	//}

	if (dist > player_Camera->maxDist)
	{
		dist = player_Camera->maxDist;
		player_Camera->tooFar = true;
	}
	else if (dist <= (player_Camera->maxDist - 5.0f))
	{
		player_Camera->tooFar = false;
	}

	if (player_Camera->tooFar)
	{
		player_Camera->eye -= (dirN * (float)(10.0f * deltaTime));
	}
	if (player_Camera->tooClose)
	{

		player_Camera->eye += (dirN * (float)(10.0f * deltaTime));
	}

	if (dist < player_Camera->minDist)
	{
		dist = player_Camera->minDist;
		player_Camera->tooClose = true;
	}
	else if (dist >= (player_Camera->minDist + 5.0f))
	{
		player_Camera->tooClose = false;
	}

	//Translate Camera back, and re-point at target
	player_Camera->eye = targetPos + (dirNR * dist);
	player_Camera->eye.y = oldEye.y;
	player_Camera->targetPos = glm::normalize(targetPos - player_Camera->eye);


	//// Adjust the mouse speed
	//if (::g_MouseIsInsideWindow)
	//{
	//	const float MOUSE_WHEEL_SENSITIVITY = 0.1f;

	//	// Adjust the movement speed based on the wheel position
	//	float mouseAdjust = (::player_Camera->getMouseWheel() * MOUSE_WHEEL_SENSITIVITY);
	//	if (mouseAdjust > 0.01f || mouseAdjust < -0.01f)
	//	{
	//		player_Camera->eye -= (dirN * mouseAdjust * (float)deltaTime);
	//		dist = glm::distance(player_Camera->eye, targetPos);
	//		if (dist > player_Camera->maxDist)
	//		{
	//			dist = player_Camera->maxDist - 1.0f;
	//			//player_Camera->eye -= (dirN);
	//			player_Camera->eye += (dirN);
	//			player_Camera->resetMouseWheel();
	//		}
	//		else if (dist < player_Camera->minDist)
	//		{
	//			dist = player_Camera->minDist + 1.0f;
	//			//player_Camera->eye -= (dirN);
	//			player_Camera->eye += (dirN);
	//			player_Camera->resetMouseWheel();
	//		}
	//	}
	//}

	return;
}//ProcessAsyncMouse(...

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

		//cEntity* userSphere = findObjectByFriendlyName("sphere0");
		cEntity* userSphere = findObjectByFriendlyName("Player");

		if (player->status == eEntityStatus::IDLE || player->status == eEntityStatus::RUNNING)
		{
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

			cEntity* katana = player->vec_pChildrenEntities[0];

			//eEntityStatus oldStatus = player->status;
			//player->status = eEntityStatus::IDLE;
			cAnimationState::sStateDetails newState;
			newState.name = "Idle";
			newState.status = eEntityStatus::IDLE;

			float movespeed = 10.0f;

			if (glfwGetKey(window, GLFW_KEY_W))
			{
				//glm::vec3 force = matVelRotation * glm::vec4(0.0f, 0.0f, 2.0f, 1.0f);
				glm::vec3 force = glm::vec4(0.0f, 0.0f, movespeed, 1.0f);
				force.y = vel.y;
				player->m_EntityPhysics->rigidBody->SetOrientation(matVelRotation);
				player->m_EntityPhysics->rigidBody->SetVelocity(force);
				//katana->m_EntityPhysics->rigidBody->SetOrientation(matVelRotation);
				newState.name = "Run";
				newState.status = eEntityStatus::RUNNING;
			}
			if (glfwGetKey(window, GLFW_KEY_S))
			{
				//glm::vec3 force = matVelRotation * glm::vec4(0.0f, 0.0f, -2.0f, 1.0f);
				glm::vec3 force = glm::vec4(0.0f, 0.0f, movespeed, 1.0f);
				force.y = vel.y;
				glm::mat4 matVelRotation2 = glm::rotate(matVelRotation, 3.0f, glm::vec3(0.0f, 1.0f, 0.0f));
				player->m_EntityPhysics->rigidBody->SetOrientation(matVelRotation2);
				player->m_EntityPhysics->rigidBody->SetVelocity(force);
				newState.name = "Run";
				newState.status = eEntityStatus::RUNNING;
			}
			if (glfwGetKey(window, GLFW_KEY_A))
			{
				glm::vec3 force = glm::vec4(0.0f, 0.0f, movespeed, 1.0f);
				force.y = vel.y;
				glm::mat4 matVelRotation2 = glm::rotate(matVelRotation, 1.5f, glm::vec3(0.0f, 1.0f, 0.0f));
				player->m_EntityPhysics->rigidBody->SetOrientation(matVelRotation2);
				player->m_EntityPhysics->rigidBody->SetVelocity(force);
				newState.name = "Run";
				newState.status = eEntityStatus::RUNNING;
			}
			if (glfwGetKey(window, GLFW_KEY_D))
			{
				glm::vec3 force = glm::vec4(0.0f, 0.0f, movespeed, 1.0f);
				force.y = vel.y;
				glm::mat4 matVelRotation2 = glm::rotate(matVelRotation, -1.5f, glm::vec3(0.0f, 1.0f, 0.0f));
				player->m_EntityPhysics->rigidBody->SetOrientation(matVelRotation2);
				player->m_EntityPhysics->rigidBody->SetVelocity(force);
				newState.name = "Run";
				newState.status = eEntityStatus::RUNNING;
			}

			if (newState.status != eEntityStatus::IDLE)
			{
				if (player->m_EntityMesh->pAniState->vecAnimationQueue.size() == 0)
				{
					player->m_EntityMesh->pAniState->vecAnimationQueue.push_back(newState);
				}
				if (newState.status != player->status)
				{
					resetHackTime(player);
				}
			}

			if (newState.status == eEntityStatus::IDLE && player->status == eEntityStatus::RUNNING)
			{
				//If not moving, default to Idle animation
				if ((player->m_EntityPhysics->velocity.x + player->m_EntityPhysics->velocity.y + player->m_EntityPhysics->velocity.z) < 0.01f)
				{
					//cAnimationState::sStateDetails newState;
					//newState.name = "Idle";
					//newState.status = eEntityStatus::IDLE;
					resetHackTime(player);
				}
			}

			targetPos = oldTargetPos;
			targetDir = glm::normalize(targetPos - player_Camera->eye);
			q = glm::inverse(glm::lookAt(targetPos - player_Camera->eye, targetDir, player_Camera->getUpVector()));
			player_Camera->setMeshOrientationQ(q);
			player_Camera->m_UpdateAtFromOrientation();
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
	}
}