#include "Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include "Xml/pugixml.hpp"


 GLFWwindow* Window::_Mainwindow = NULL;

void Xml_SettingImport();
//Default  settings
 unsigned int Window::WINDOW_WIDTH = 800;
 unsigned int Window::WINDOW_HEIGHT = 600;
 Camera Window::_editorCamera(glm::vec3(0.0f, 0.0f, 3.0f));
 bool Window::WindowShouldClose = false;


GLFWwindow* Window::CreateWindow(void* framebuffer_size_callback, void* mouse_callback, void* scroll_callback)
{
	WindowShouldClose = false;	
	Xml_SettingImport();
	_Mainwindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);

	if (_Mainwindow == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		//return;
	}
	glfwMakeContextCurrent(_Mainwindow);
	glfwSetFramebufferSizeCallback(_Mainwindow, (GLFWframebuffersizefun)framebuffer_size_callback);
	glfwSetCursorPosCallback(_Mainwindow, (GLFWcursorposfun)mouse_callback);
	glfwSetScrollCallback(_Mainwindow, (GLFWscrollfun)scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(_Mainwindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		//return;
	}


	_editorCamera.EnableFrameBuffer(true);
	return _Mainwindow;
}



void Xml_SettingImport()
{
	pugi::xml_document _doc;
	std::ifstream _XMLstream("GlobalSettings.xml");

	pugi::xml_parse_result result = _doc.load(_XMLstream);
	std::cout << "Load result: " << result.description() << std::endl;

	pugi::xml_node _node = _doc.child("GlobalSettings").child("WindowSetting").child("viewport");
	if (_node)
	{
	  Window::WINDOW_WIDTH = _node.attribute("width").as_uint();
      Window::WINDOW_HEIGHT = _node.attribute("height").as_uint();
	}
	_XMLstream.close();

	return;
}


