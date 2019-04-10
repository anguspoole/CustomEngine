#include <iostream>

#include "../Global/gOpenGL.h"
#include "../Shading/cShaderManager.h"

GLFWwindow* window;

cShaderManager* pTheShaderManager = NULL;		// "Heap" variable

void main()
{
	//Make window
	InitWindow(window, 1024, 480, "Hello World");

	//Create shader manager
	pTheShaderManager = new cShaderManager();
	pTheShaderManager->setBasePath("assets/shaders/");

	//Create vertex shader
	cShaderManager::cShader vertexShader;
	vertexShader.fileName = "vertex01.glsl";
	vertexShader.shaderType = cShaderManager::cShader::VERTEX_SHADER;

	//Create fragment shader
	cShaderManager::cShader fragmentShader;
	fragmentShader.fileName = "fragment01.glsl";
	fragmentShader.shaderType = cShaderManager::cShader::FRAGMENT_SHADER;

	if (pTheShaderManager->createProgramFromFile("BasicUberShader",
		vertexShader,
		fragmentShader))
	{		// Shaders are OK
		std::cout << "Compiled shaders OK." << std::endl;
	}
	else
	{
		std::cout << "OH NO! Compile error" << std::endl;
		std::cout << pTheShaderManager->getLastError() << std::endl;
	}

	system("PAUSE");
}