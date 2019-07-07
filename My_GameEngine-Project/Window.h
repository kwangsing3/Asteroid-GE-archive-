#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window
{
	

public:
	static GLFWwindow* _Mainwindow;
	static unsigned int WINDOW_WIDTH;
	static unsigned int WINDOW_HEIGHT;

	static unsigned int GetWidth();
	static unsigned int GetHeight();

	static GLFWwindow* CreateWindow(void* framebuffer_size_callback, void* mouse_callback, void* scroll_callback);
	

	//void Xml_SettingImport();
};


#endif // !1