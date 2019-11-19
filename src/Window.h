﻿#ifndef WINDOW_H
#define WINDOW_H
#include <fstream>
#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <AGE_Assert.h>
#include <pugixml.hpp>

//#include <World.h>



//#include <GraphicEngine/imgui.h>


extern unsigned int Window_Width;
extern unsigned int Window_Height;
//SceneManager* _sceneManager;
//class World;
//   ---------------------------------UI---------------------------------
class WindowUI                                                                             
{
public:

	//static SelectObject* cur_SelectObject;

	static void CopyEvent();
	static void PasteEvent();

	static bool show_simple_overlay;
	static bool All_UIElement;

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

	static void ShowMyImGUIDemoWindow(bool *p_open, unsigned int *width, unsigned int *height);

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

		pugi::xml_document _doc;
		std::ifstream _XMLstream("Texture/GlobalSettings.xml");
		pugi::xml_parse_result result = _doc.load(_XMLstream);
		std::cout << "Load result: " << result.description() << std::endl;
		if (_doc != NULL)
		{
			pugi::xml_node _node = _doc.child("GlobalSettings").child("WindowSetting").child("viewport");
			this->_Main_UI->All_UIElement = _doc.child("GlobalSettings").child("ProjectSetting").child("All_UIElement").attribute("All_UIElement").as_bool();
			this->DeBug_Mode = _doc.child("GlobalSettings").child("WindowSetting").child("Game_Mode").attribute("Game_Mode").as_bool();
			this->progect_I_am_focus = _doc.child("GlobalSettings").child("ProjectSetting").child("Project").attribute("Project_Focus").as_int();
		}
		_XMLstream.close();
		

		GLFWmonitor* primary = glfwGetPrimaryMonitor();
		GLFWmonitor *monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode *mode = glfwGetVideoMode(monitor);
		Window_Width = mode->width;
		Window_Height = mode->height;
		MainGLFWwindow = glfwCreateWindow(Window_Width, Window_Height, "Asteroid-GameEngine", isFullscreen ? monitor : NULL, NULL);
		AGE_ASSERT(MainGLFWwindow != NULL);  //Failed to create window

		glfwMaximizeWindow(MainGLFWwindow);
		//glfwHideWindow(MainGLFWwindow);
		//glfwSetWindowMonitor(MainGLFWwindow, monitor, 0, 0, _Width, _Height, mode->refreshRate);
		glfwMakeContextCurrent(MainGLFWwindow);
		glfwSetFramebufferSizeCallback(MainGLFWwindow, (GLFWframebuffersizefun)framebuffer_size_callback);
		glfwSetCursorPosCallback(MainGLFWwindow, (GLFWcursorposfun)mouse_callback);
		glfwSetScrollCallback(MainGLFWwindow, (GLFWscrollfun)scroll_callback);
		glfwSetMouseButtonCallback(MainGLFWwindow, (GLFWmousebuttonfun)mouse_Click_callback);
		glfwSetInputMode(MainGLFWwindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		// glad: load all OpenGL function pointers
		// ---------------------------------------
		AGE_ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)); //Failed to inited glad

	
		//InitWorld();
		/*調試用函數*/

		

	}

private:
	//void InitWorld();
	

};




#endif // !WINDOW_H