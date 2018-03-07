#include "Keyboard.h"
#include "cGameObject.h"
#include <iostream>
#include <vector>
#include <glm\glm.hpp>

extern glm::vec3 g_cameraXYZ;
extern glm::vec3 g_cameraTarget_XYZ;
extern cGameObject* g_curGameObject;
extern std::vector<cGameObject*> g_vecGameSpeeders;
extern std::vector<cGameObject*> g_vecGameObjects;

extern int g_SpeederIndex;
extern int g_GameObjectIndex;

void Keyboard::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{

	}

	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
	{
		//		::g_GameObjects[1]->position.y += 0.01f;
		g_cameraTarget_XYZ = g_curGameObject->position;
	}



	const float CAMERASPEED = 2.0f;
	switch (key)
	{
	case GLFW_KEY_A:		// Left
		g_cameraXYZ.x -= CAMERASPEED;
		break;
	case GLFW_KEY_D:		// Right
		g_cameraXYZ.x += CAMERASPEED;
		break;
	case GLFW_KEY_W:		// Forward (along z)
		g_cameraXYZ.z += CAMERASPEED;
		break;
	case GLFW_KEY_S:		// Backwards (along z)
		g_cameraXYZ.z -= CAMERASPEED;
		break;
	case GLFW_KEY_Q:		// "Down" (along y axis)
		g_cameraXYZ.y -= CAMERASPEED;
		break;
	case GLFW_KEY_E:		// "Up" (along y axis)
		g_cameraXYZ.y += CAMERASPEED;
		break;

	case GLFW_KEY_UP:
		g_vecGameObjects[0]->vel.y += 0.5f;
		break;
	case GLFW_KEY_DOWN:
		g_vecGameObjects[0]->vel.y -= 0.5f;
		break;
	case GLFW_KEY_LEFT:
		g_vecGameObjects[0]->vel.x -= 0.5f;
		break;
	case GLFW_KEY_RIGHT:
		g_vecGameObjects[0]->vel.x += 0.5f;
		break;
	case GLFW_KEY_RIGHT_SHIFT:
		g_vecGameObjects[0]->vel.z -= 0.5f;
		break;
	case GLFW_KEY_KP_1:
		g_vecGameObjects[0]->vel.z += 0.5f;
		break;

	case GLFW_KEY_I:
		g_curGameObject->position.z += 1.5f;
		g_cameraTarget_XYZ = g_curGameObject->position;
		break;
	case GLFW_KEY_K:
		g_curGameObject->position.z -= 1.5f;
		g_cameraTarget_XYZ = g_curGameObject->position;

		break;
	case GLFW_KEY_L:
		g_curGameObject->position.x += 1.5f;
		g_cameraTarget_XYZ = g_curGameObject->position;
		break;
	case GLFW_KEY_J:
		g_curGameObject->position.x -= 1.5f;
		g_cameraTarget_XYZ = g_curGameObject->position;
		break;
	}

	if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
	{
		//each time tab is pressed it changes what object is being focused
		if (g_GameObjectIndex == g_vecGameObjects.size() - 1)
		{
			g_GameObjectIndex = 0;
		}
		else
			g_GameObjectIndex++;

		//set the current object from the	 vector 
		g_curGameObject = g_vecGameObjects[g_GameObjectIndex];
		//set the camera to look at the object
		g_cameraTarget_XYZ = g_curGameObject->position;
	}

	if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS)
	{
		std::cout << "ObjectNum: " << g_GameObjectIndex << " x: " << g_curGameObject->position.x << " y: " << g_curGameObject->position.y << " z: " << g_curGameObject->position.z << std::endl;
	}

	return;
}