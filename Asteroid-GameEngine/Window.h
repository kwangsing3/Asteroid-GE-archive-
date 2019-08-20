#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GraphicEngine/imgui.h>
#include <Units/Actor.h>
#include <fstream>
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
	static bool show_simple_overlay;
	static bool All_UIElement;
	static Game_Mode _mode;
	static glm::vec2 _mouseClickpos;

	WindowUI()
	{


	}

	static void SetMouseClickPos(double xpos, double ypos)
	{
		_mouseClickpos = glm::vec2(xpos, ypos);
	}


};



class Window
{
public:
	static GLFWwindow* MainGLFWwindow;
	static unsigned int _Width;
	static unsigned int _Height;
	static ImVec2 viewport_pos;
	static ImVec2 viewport_size;
	static Camera _editorCamera;
	static bool DeBug_Mode;
	static std::vector<int> vec_ID;
	static WindowUI *_Main_UI;
	static bool WindowShouldClose;


	Window(void* framebuffer_size_callback, void* mouse_callback, void* scroll_callback)
	{
		WindowShouldClose = false;
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
				this->_Width = _node.attribute("width").as_uint();
				this->_Height = _node.attribute("height").as_uint();
			}
			this->_Main_UI->All_UIElement = _doc.child("GlobalSettings").child("ProjectSetting").child("All_UIElement").attribute("All_UIElement").as_bool();
			this->_Main_UI->_mode = Game_Mode(_doc.child("GlobalSettings").child("ProjectSetting").child("Game_Mode").attribute("Game_Mode").as_int());
			this->DeBug_Mode = _doc.child("GlobalSettings").child("WindowSetting").child("Game_Mode").attribute("Game_Mode").as_bool();
			_XMLstream.close();
		}
		MainGLFWwindow = glfwCreateWindow(Window::_Width, Window::_Height, "LearnOpenGL", NULL, NULL);
		if (MainGLFWwindow == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			//return;
		}

		glfwMakeContextCurrent(MainGLFWwindow);
		glfwSetFramebufferSizeCallback(MainGLFWwindow, (GLFWframebuffersizefun)framebuffer_size_callback);
		glfwSetCursorPosCallback(MainGLFWwindow, (GLFWcursorposfun)mouse_callback);
		glfwSetScrollCallback(MainGLFWwindow, (GLFWscrollfun)scroll_callback);
		// tell GLFW to capture our mouse
		glfwSetInputMode(MainGLFWwindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// glad: load all OpenGL function pointers
		// ---------------------------------------
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			//return;
		}
		
		if (this->_Main_UI->_mode == Mode_3D)
		{
			this->_editorCamera.SwitchCamera3D(true);
		}
		else
		{
			this->_editorCamera.SwitchCamera3D(false);
		}
		this->_editorCamera.EnableFrameBuffer(true);

		/*調試用函數*/



	}

private: 


};














namespace MyImGui
{
	void ShowMyImGUIDemoWindow(bool *p_open, unsigned int *width, unsigned int *height, unsigned int textureColorbuffer);

}


#endif // !WINDOW_H