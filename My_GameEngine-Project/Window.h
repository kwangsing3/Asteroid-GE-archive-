#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
<<<<<<< HEAD
#include <Camera.h>
=======
>>>>>>> ed1bdf2babff7727a9f89eb22a35a8e9126ad568
#include <vector>
#include <Actor.h>
class Window
{
	

public:
	//Window setting
	static GLFWwindow* _Mainwindow;
	static unsigned int WINDOW_WIDTH;
	static unsigned int WINDOW_HEIGHT;
	static unsigned int GetWidth();
	static unsigned int GetHeight();
	static GLFWwindow* CreateWindow(void* framebuffer_size_callback, void* mouse_callback, void* scroll_callback);
<<<<<<< HEAD
	static Camera _editorCamera;

	static std::vector<Actor*> vec_ObjectsToRender;
=======
	//Render Popline
	static std::vector<Actor> vec_RenderObjects;

	
>>>>>>> ed1bdf2babff7727a9f89eb22a35a8e9126ad568
};


#endif // !1