#ifndef _G_OPENGL_HG_
#define _G_OPENGL_HG_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

extern int windowWidth;
extern int windowHeight;
extern GLFWwindow* window;

void error_callback(int error, const char* description);
GLFWwindow* InitWindow(int width, int height, const char* title);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void ProcessAsyncKeys(GLFWwindow* window);

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void ProcessAsyncMouse(GLFWwindow* window);

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_enter_callback(GLFWwindow* window, int entered);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void window_size_callback(GLFWwindow* window, int width, int height);
extern bool g_MouseIsInsideWindow;	// 

#endif
