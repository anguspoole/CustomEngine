#include "gOpenGL.h"

//Function for printing glfw errors
void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

//Function for getting key callback
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

}

void InitWindow(GLFWwindow* window, int width, int height, const char* title)
{

	//***************************************************************
	//	OPENGL WINDOW INITIALIZATION - START
	//***************************************************************

	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);

	//***************************************************************
	//	OPENGL WINDOW INITIALIZATION - END
	//***************************************************************

}