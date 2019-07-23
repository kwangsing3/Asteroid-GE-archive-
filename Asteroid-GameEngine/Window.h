#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Units/Camera.h>
#include <vector>

class Window
{
	

public:
	static GLFWwindow* _Mainwindow;
	static unsigned int WINDOW_WIDTH;
	static unsigned int WINDOW_HEIGHT;

	
	static bool WindowShouldClose;

	static GLFWwindow* CreateWindow(void* framebuffer_size_callback, void* mouse_callback, void* scroll_callback);
	static Camera _editorCamera;

	
};


#endif // !1