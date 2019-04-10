#ifndef _G_OPENGL_HG_
#define _G_OPENGL_HG_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

extern int windowWidth;
extern int windowHeight;

void error_callback(int error, const char* description);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
GLFWwindow* InitWindow(int width, int height, const char* title);

#endif
