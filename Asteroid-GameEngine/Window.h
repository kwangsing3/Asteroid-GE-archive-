#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Camera.h>
#include <vector>
#include <Actor.h>
class Window
{
	

public:
	static GLFWwindow* _Mainwindow;
	static unsigned int WINDOW_WIDTH;
	static unsigned int WINDOW_HEIGHT;

	static unsigned int GetWidth();
	static unsigned int GetHeight();

	static GLFWwindow* CreateWindow(void* framebuffer_size_callback, void* mouse_callback, void* scroll_callback);
	static Camera _editorCamera;

	static std::vector<Actor*> vec_ObjectsToRender;
};


#endif // !1