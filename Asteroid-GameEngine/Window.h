#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Units/Actor.h>
#include <iostream>
#include <Xml/pugixml.hpp>

#include <Units/Camera.h>
#include <vector>
struct SelectObject
{
	Actor* _actor;
	int _index = 0;
	bool Is_selected = false;
	bool Is_renaming = false;
	SelectObject* next = NULL;
};
enum  Game_Mode { Mode_2D, Mode_3D };




class Window
{
public:
	static GLFWwindow* _Mainwindow;
	static unsigned int WINDOW_WIDTH;
	static unsigned int WINDOW_HEIGHT;

	
	static bool WindowShouldClose;

	static GLFWwindow* CreateWindow(void* framebuffer_size_callback, void* mouse_callback, void* scroll_callback);
	static Camera _editorCamera;
	static bool DeBug_Mode;
	
	
};

//   ---------------------------------UI---------------------------------

class WindowUI
{
public:

	static SelectObject *cur_SelectObject;
	static WindowUI _InspectorManager;
	static void Deletecur_actor(SelectObject* cur_actor);
	static void Renamecur_actor(SelectObject* cur_actor);
	void ShowInspector(SelectObject *actor);
	void ListInspectorCur();

	static bool All_UIElement;
	static Game_Mode _mode;


	WindowUI()
	{


	}


	
};



namespace MyImGui
{
	void ShowMyImGUIDemoWindow(bool *p_open, unsigned int *width, unsigned int *height, unsigned int textureColorbuffer);

}


#endif // !WINDOW_H