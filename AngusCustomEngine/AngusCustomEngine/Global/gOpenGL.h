#ifndef _G_OPENGL_HG_
#define _G_OPENGL_HG_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void error_callback(int error, const char* description);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void InitWindow(GLFWwindow*, int width, int height, const char* title);

#endif
