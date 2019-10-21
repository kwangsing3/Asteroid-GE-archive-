﻿#ifndef WINDOW_H
#define WINDOW_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Xml/include/pugixml.hpp>
#include <fstream>
#include <iostream>
#include <vector>
//#include <World.h>



//#include <GraphicEngine/imgui.h>


class Actor;
//class SceneManager;
struct SelectObject
{
	Actor* _actor;
	bool Is_selected = false;
	bool Is_renaming = false;
	SelectObject(Actor* _ac)
	{
		_actor = _ac;
	}
};
enum  Game_Mode { Mode_2D, Mode_3D };
extern unsigned int _Width;
extern unsigned int _Height;
//SceneManager* _sceneManager;
//class World;
//   ---------------------------------UI---------------------------------
class WindowUI                                                                             
{
public:

	//static SelectObject* cur_SelectObject;
	static std::vector<SelectObject*> cur_SelectObject_List;
	static std::vector<SelectObject*> copy_SelectObject_List;
	static void Deletecur_actor(SelectObject* cur_actor);
	static void Renamecur_actor(SelectObject* cur_actor);
	static void SelectThisActor(Actor* _actor);
	static void SelectThisObject(SelectObject *actor);
	static void ListInspectorCur(SelectObject* _sel);
	static void CopyEvent();
	static void PasteEvent();

	static bool show_simple_overlay;
	static bool All_UIElement;
	static Game_Mode _mode;
	static float _FPS;
	static float UI_Left_X;
	static float UI_Left_Y;
	//static float UI_Right_X;
	static float UI_Right_X;
	WindowUI()
	{
		UI_Left_X = 0;
		UI_Right_X = 0;
		
	}

	static void ShowMyImGUIDemoWindow(bool *p_open, unsigned int *width, unsigned int *height, unsigned int textureColorbuffer);

};
class Window
{
public:
	static GLFWwindow* MainGLFWwindow;
	bool isFullscreen;
	static int progect_I_am_focus;
	static bool DeBug_Mode;
	static std::vector<int> vec_ID;
	static WindowUI *_Main_UI;
	static bool WindowShouldClose;

	//static World* _Mainworld;

	Window(void* framebuffer_size_callback, void* mouse_callback, void* scroll_callback, void* mouse_Click_callback)
	{
		WindowShouldClose = false;
		isFullscreen = false;

		_Main_UI = new WindowUI();
		//Xml_SettingImport();
		{
			pugi::xml_document _doc;
			std::ifstream _XMLstream("GlobalSettings.xml");
			pugi::xml_parse_result result = _doc.load(_XMLstream);
			std::cout << "Load result: " << result.description() << std::endl;
			pugi::xml_node _node = _doc.child("GlobalSettings").child("WindowSetting").child("viewport");
			if (_node)
			{
				_Width = _node.attribute("width").as_uint();
				_Height = _node.attribute("height").as_uint();
			}
			this->_Main_UI->All_UIElement = _doc.child("GlobalSettings").child("ProjectSetting").child("All_UIElement").attribute("All_UIElement").as_bool();
			this->_Main_UI->_mode = Game_Mode(_doc.child("GlobalSettings").child("ProjectSetting").child("Game_Mode").attribute("Game_Mode").as_int());
			this->DeBug_Mode = _doc.child("GlobalSettings").child("WindowSetting").child("Game_Mode").attribute("Game_Mode").as_bool();
			this->progect_I_am_focus = _doc.child("GlobalSettings").child("ProjectSetting").child("Project").attribute("Project_Focus").as_int();

			_XMLstream.close();
		}	
		GLFWmonitor* primary = glfwGetPrimaryMonitor();
		GLFWmonitor *monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode *mode = glfwGetVideoMode(monitor);
		_Width = mode->width;
		_Height = mode->height;
		MainGLFWwindow = glfwCreateWindow(_Width, _Height, "Asteroid-GameEngine", isFullscreen ? monitor : NULL, NULL);
		if (MainGLFWwindow == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			//return;
		}
		glfwMaximizeWindow(MainGLFWwindow);
		//glfwHideWindow(MainGLFWwindow);
		//glfwSetWindowMonitor(MainGLFWwindow, monitor, 0, 0, _Width, _Height, mode->refreshRate);
		glfwMakeContextCurrent(MainGLFWwindow);
		glfwSetFramebufferSizeCallback(MainGLFWwindow, (GLFWframebuffersizefun)framebuffer_size_callback);
		glfwSetCursorPosCallback(MainGLFWwindow, (GLFWcursorposfun)mouse_callback);
		glfwSetScrollCallback(MainGLFWwindow, (GLFWscrollfun)scroll_callback);
		glfwSetMouseButtonCallback(MainGLFWwindow, (GLFWmousebuttonfun)mouse_Click_callback);
		// tell GLFW to capture our mouse
		glfwSetInputMode(MainGLFWwindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// glad: load all OpenGL function pointers
		// ---------------------------------------
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			//return;
		}
	

	
		//InitWorld();
		/*調試用函數*/

		

	}

private:
	//void InitWorld();
	

};




#endif // !WINDOW_H