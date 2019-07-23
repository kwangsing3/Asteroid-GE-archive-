
#include "MainUI.h"
#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <iostream>
#include "GraphicEngine/imgui.h"
#include <ctype.h>          // toupper
#include <limits.h>         // INT_MIN, INT_MAX
#include <math.h>           // sqrtf, powf, cosf, sinf, floorf, ceilf
#include <stdio.h>          // vsnprintf, sscanf, printf
#include <stdlib.h>         // NULL, malloc, free, atoi
#include <ADD_Function.h>
#include <SceneManager.h>
#include <Raycast.h>
#include <Window.h>

#if defined(_MSC_VER) && _MSC_VER <= 1500 // MSVC 2008 or earlier
#include <stddef.h>         // intptr_t
#else
#include <stdint.h>         // intptr_t
#endif

#ifdef _MSC_VER
#pragma warning (disable: 4996) // 'This function or variable may be unsafe': strcpy, strdup, sprintf, vsnprintf, sscanf, fopen
#endif
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wold-style-cast"             // warning : use of old-style cast                              // yes, they are more terse.
#pragma clang diagnostic ignored "-Wdeprecated-declarations"    // warning : 'xx' is deprecated: The POSIX name for this item.. // for strdup used in demo code (so user can copy & paste the code)
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast"   // warning : cast to 'void *' from smaller integer type 'int'
#pragma clang diagnostic ignored "-Wformat-security"            // warning : warning: format string is not a string literal
#pragma clang diagnostic ignored "-Wexit-time-destructors"      // warning : declaration requires an exit-time destructor       // exit-time destruction order is undefined. if MemFree() leads to users code that has been disabled before exit it might cause problems. ImGui coding style welcomes static/globals.
#pragma clang diagnostic ignored "-Wunused-macros"              // warning : warning: macro is not used                         // we define snprintf/vsnprintf on Windows so they are available, but not always used.
#if __has_warning("-Wzero-as-null-pointer-constant")
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"  // warning : zero as null pointer constant                  // some standard header variations use #define NULL 0
#endif
#if __has_warning("-Wdouble-promotion")
#pragma clang diagnostic ignored "-Wdouble-promotion"           // warning: implicit conversion from 'float' to 'double' when passing argument to function  // using printf() is a misery with this as C++ va_arg ellipsis changes float to double.
#endif
#if __has_warning("-Wreserved-id-macro")
#pragma clang diagnostic ignored "-Wreserved-id-macro"          // warning : macro name is a reserved identifier                //
#endif
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wpragmas"                      // warning: unknown option after '#pragma GCC diagnostic' kind
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"          // warning: cast to pointer from integer of different size
#pragma GCC diagnostic ignored "-Wformat-security"              // warning : format string is not a string literal (potentially insecure)
#pragma GCC diagnostic ignored "-Wdouble-promotion"             // warning: implicit conversion from 'float' to 'double' when passing argument to function
#pragma GCC diagnostic ignored "-Wconversion"                   // warning: conversion to 'xxxx' from 'xxxx' may alter its value
#pragma GCC diagnostic ignored "-Wmisleading-indentation"       // [__GNUC__ >= 6] warning: this 'if' clause does not guard this statement      // GCC 6.0+ only. See #883 on GitHub.
#endif
// Play it nice with Windows users. Notepad in 2017 still doesn't display text data with Unix-style \n.
#ifdef _WIN32
#define IM_NEWLINE  "\r\n"
#define snprintf    _snprintf
#define vsnprintf   _vsnprintf
#else
#define IM_NEWLINE  "\n"
#endif

#define IM_MAX(_A,_B)       (((_A) >= (_B)) ? (_A) : (_B))

//-----------------------------------------------------------------------------
// [SECTION] Forward Declarations, Helpers
//-----------------------------------------------------------------------------

#if !defined(IMGUI_DISABLE_OBSOLETE_FUNCTIONS) && defined(IMGUI_DISABLE_TEST_WINDOWS) && !defined(IMGUI_DISABLE_DEMO_WINDOWS)   // Obsolete name since 1.53, TEST->DEMO
#define IMGUI_DISABLE_DEMO_WINDOWS
#endif

static void ShowExampleAppMainMenuBar();
static void ShowExampleMenuFile();
static void ShowExampleAppLayout(bool* p_open);
static float _maineditorX = 854, _maineditorY = 590;
static float _LogoutX = 854;
static float _SceneX = 213, _SceneY = 360;
static void ShowExampleAppSimpleOverlay(bool* p_open);
struct listbool
{
	bool selected = false;
	listbool* next=NULL;
};
static listbool _headlistbool;
static listbool *_currentlistbool = &_headlistbool;
void Clear_ListBool(listbool* _headlist)
{
	listbool* _cur = _headlist;
	_cur->selected = false;
	while (_cur->next!=NULL)
	{
		if (_cur->next == NULL)
			return;
		_cur->selected = false;
		_cur = _cur->next;
	}
	return;
}

Actor *InspectorManager::cur_actor;
InspectorManager InspectorManager::_InspectorManager;




void MyImGui::ShowMyImGUIDemoWindow(bool *p_open, unsigned int *width, unsigned int *height, unsigned int textureColorbuffer)
{
	static bool show_app_main_menu_bar = false;
	show_app_main_menu_bar = p_open;
	

	float lastX = *width / 2.0f;
	float lastY = *height / 2.0f;

	static bool no_titlebar = true;
	static bool no_scrollbar = true;
	static bool no_menu = false;
	static bool no_move = true;
	static bool no_resize = true;
	static bool no_collapse = true;
	static bool no_close = true;
	static bool no_nav = false;
	static bool no_background = false;
	static bool no_bring_to_front = true;

	ImGuiWindowFlags window_flags = 0;
	if (no_titlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
	if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
	if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
	if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
	if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
	if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
	if (no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
	if (no_background)      window_flags |= ImGuiWindowFlags_NoBackground;
	if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	if (no_close)           p_open = NULL; // Don't pass our bool* to Begin




	ImGuiWindowFlags window_flags2 = 0;
	window_flags2 |= ImGuiWindowFlags_NoTitleBar;
	window_flags2 |= ImGuiWindowFlags_NoMove;
	window_flags2 |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	//Main Editor------------------------------------------------------------------------------------------
	ImGui::SetNextWindowPos(ImVec2(0, *height/35), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(_maineditorX, _maineditorY), ImGuiCond_Always);
	{
		

		if (!ImGui::Begin("Main Editor", p_open, window_flags2))
		{
			// Early out if the window is collapsed, as an optimization.
			ImGui::End();
			return;
		}
		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_Reorderable;
		if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
		{
			if (ImGui::BeginTabItem("Editor"))
			{

				ImGui::GetWindowDrawList()->AddImage(
					(void *)textureColorbuffer, ImVec2(ImGui::GetCursorScreenPos()),
					ImVec2(ImGui::GetCursorScreenPos().x + ImGui::GetWindowWidth(), ImGui::GetCursorScreenPos().y + ImGui::GetWindowHeight() - 80), ImVec2(0, 1), ImVec2(1, 0));

				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Game"))
			{


				ImGui::Text("This is the Broccoli tab!\nblah blah blah blah blah");
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
		ImGui::Separator();

		_maineditorX = ImGui::GetWindowWidth();
		_maineditorY = ImGui::GetWindowHeight();
		ImGui::End();

	}
	//Logout layer-----------------------------------------------------------------------------------------
	ImGui::SetNextWindowPos(ImVec2(0, *height / 35 + _maineditorY), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(_maineditorX, *height- _maineditorY), ImGuiCond_Always);
	{

	if (!ImGui::Begin("options", p_open, window_flags2))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}
	//------------------------------------------------------------------------------------------------
	if (ImGui::CollapsingHeader("Window options"))
	{
		/*ImGui::Checkbox("No titlebar", &no_titlebar); ImGui::SameLine(150);
		ImGui::Checkbox("No scrollbar", &no_scrollbar); ImGui::SameLine(300);
		ImGui::Checkbox("No menu", &no_menu);
		ImGui::Checkbox("No move", &no_move); ImGui::SameLine(150);
		ImGui::Checkbox("No resize", &no_resize); ImGui::SameLine(300);
		ImGui::Checkbox("No collapse", &no_collapse);
		ImGui::Checkbox("No close", &no_close); ImGui::SameLine(150);
		ImGui::Checkbox("No nav", &no_nav); ImGui::SameLine(300);
		ImGui::Checkbox("No background", &no_background);
		ImGui::Checkbox("No bring to front", &no_bring_to_front);*/
	}
	_maineditorX = ImGui::GetWindowWidth();
	_maineditorY =*height-ImGui::GetWindowHeight();
	ImGui::End();
    }
	
	//Scene------------------------------------------------------------------------------------------------
	ImGui::SetNextWindowPos(ImVec2(_maineditorX, *height / 35), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(_SceneX, _SceneY), ImGuiCond_Always);
	{

		if (!ImGui::Begin("Scene", p_open, window_flags2))
		{
			// Early out if the window is collapsed, as an optimization.
			ImGui::End();
			return;
		}
		//------------------------------------------------------------------------------------------------
		if (SceneManager::Objects.size() > 0)
		{
			for (int n = 0; n < SceneManager::Objects.size(); n++)
			{				
				std::string _newname = SceneManager::Objects[n]->transform->name + std::to_string(n);
				const char* newchar = _newname.c_str();
				if (ImGui::Selectable(newchar, _currentlistbool->selected))
				{
					if (!ImGui::GetIO().KeyCtrl)    // Clear selection when CTRL is not held
						Clear_ListBool(&_headlistbool);
					
					_currentlistbool->selected = !_currentlistbool->selected;
					InspectorManager::_InspectorManager.ShowInspector(SceneManager::Objects[n]);
				}
				if (_currentlistbool->next == NULL)
					_currentlistbool->next = new listbool();
				_currentlistbool = _currentlistbool->next;
			}
			_currentlistbool = &_headlistbool;
		}
		else
			_headlistbool.next = NULL;   //應該要全部clear才行*******
	
		/*
		Your IDs will collide if you have the same name multiple times at same point of hierarchy.
		You probably need to use ImGui::PushID(SceneManager::Objects[n]); ... Selectable .. ImGui::PopID() to avoid the ID collision.

		Please read the FAQ!     避免ID衝突
		*/


		_SceneX = ImGui::GetWindowWidth();
		_SceneY = ImGui::GetWindowHeight();
		//delete [] array;
		ImGui::End();

	}
	//Asset------------------------------------------------------------------------------------------------
	ImGui::SetNextWindowPos(ImVec2(_maineditorX, *height / 35 + _SceneY), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(_SceneX, *height - _SceneY), ImGuiCond_Always);
	{

		if (!ImGui::Begin("Asset", p_open, window_flags2))
		{
			// Early out if the window is collapsed, as an optimization.
			ImGui::End();
			return;
		}
		//------------------------------------------------------------------------------------------------
		ImGui::Text("fsdfsdad我是中文dfd");
		ImGui::Text("Kanjis: \xe6\x97\xa5\xe6\x9c\xac\xe8\xaa\x9e (nihongo)");

		if (ImGui::Button("新增一個方塊"))
		{
			ADD_Function::Add_Cube();
		}

		if (ImGui::Button("新增一個平光源"))
		{
			ADD_Function::Add_DirectionalLight();
		}
		if (ImGui::Button("新增一個點光源"))
		{
			ADD_Function::Add_PointLight();
		}
		_SceneX = ImGui::GetWindowWidth();
		_SceneY = *height - ImGui::GetWindowHeight();
		ImGui::End();

	}
	//Inspector-----------------------------------------------------------------------------------------
	ImGui::SetNextWindowPos(ImVec2(_maineditorX+_SceneX, *height / 35), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(*width - (_maineditorX + _SceneX), *height), ImGuiCond_Always);
	{

		if (!ImGui::Begin("Inspector", p_open, window_flags2))
		{
			// Early out if the window is collapsed, as an optimization.
			ImGui::End();
			return;
		}
		InspectorManager::_InspectorManager.ListInspectorCur();
		//------------------------------------------------------------------------------------------------
		ImGui::End();

	}

	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(*width, *height), ImGuiCond_Always);



	ShowExampleAppSimpleOverlay(&show_app_main_menu_bar);

	//Main Background--------------------------------------------------------------------------------------
	{
		if (!ImGui::Begin("Main Background", p_open, window_flags))
		{
			// Early out if the window is collapsed, as an optimization.
			ImGui::End();
			return;
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Menu"))
			{
				ShowExampleMenuFile();
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Help"))
			{
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImGui::End();
	}
	//Main Background--------------------------------------------------------------------------------------


	ImGui::ShowDemoWindow(p_open);
}




static void ShowExampleAppMainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ShowExampleMenuFile();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}
static void ShowExampleMenuFile()
{
	ImGui::MenuItem("(dummy menu)", NULL, false, false);
	if (ImGui::MenuItem("New")) { SceneManager::NewScene(); }
	if (ImGui::MenuItem("Open", "Ctrl+O")) { SceneManager::OpenFile(); }
	
	if (ImGui::MenuItem("Save", "Ctrl+S")) { SceneManager::SaveFile(); }
	if (ImGui::MenuItem("Save As..")) {}
	ImGui::Separator();
	if (ImGui::BeginMenu("Options"))
	{
		static bool enabled = true;
		ImGui::MenuItem("Enabled", "", &enabled);
		ImGui::BeginChild("child", ImVec2(0, 60), true);
		for (int i = 0; i < 10; i++)
			ImGui::Text("Scrolling Text %d", i);
		ImGui::EndChild();
		static float f = 0.5f;
		static int n = 0;
		static bool b = true;
		ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
		ImGui::InputFloat("Input", &f, 0.1f);
		ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
		ImGui::Checkbox("Check", &b);
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Colors"))
	{
		float sz = ImGui::GetTextLineHeight();
		for (int i = 0; i < ImGuiCol_COUNT; i++)
		{
			const char* name = ImGui::GetStyleColorName((ImGuiCol)i);
			ImVec2 p = ImGui::GetCursorScreenPos();
			ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz), ImGui::GetColorU32((ImGuiCol)i));
			ImGui::Dummy(ImVec2(sz, sz));
			ImGui::SameLine();
			ImGui::MenuItem(name);
		}
		ImGui::EndMenu();
	}
	
	if (ImGui::MenuItem("Checked", NULL, true)) 
	{
		ImGui::Text("version v1.0.0");
	}
	if (ImGui::MenuItem("Quit", "Alt+F4")) { Window::WindowShouldClose = true; }
}


void InspectorManager::Deletecur_actor()
{
	if (InspectorManager::cur_actor == NULL) return;
	
	for (auto it = SceneManager::Objects.begin(); it != SceneManager::Objects.end(); it++)
	{
		if (*it == InspectorManager::cur_actor) 
		{
			SceneManager::Objects.erase(it);
		 break;
		}
	}
	for (auto it = SceneManager::vec_ObjectsToRender.begin(); it != SceneManager::vec_ObjectsToRender.end(); it++)
	{
		if (*it == InspectorManager::cur_actor)
		{
			SceneManager::vec_ObjectsToRender.erase(it);
			break;
		}
	}
	if (InspectorManager::cur_actor->_PointLight != NULL)
	{
		for (auto it = SceneManager::vec_PointLight.begin(); it != SceneManager::vec_PointLight.end(); it++)
		{
			if (*it == InspectorManager::cur_actor->_PointLight)
			{
				SceneManager::vec_PointLight.erase(it);
				break;
			}
		}
	}
	if (InspectorManager::cur_actor->_Dirlight != NULL)
	{
		for (auto it = SceneManager::vec_DirectionlLight.begin(); it != SceneManager::vec_DirectionlLight.end(); it++)
		{
			if (*it == InspectorManager::cur_actor->_Dirlight)
			{
				SceneManager::vec_DirectionlLight.erase(it);
				break;
			}
		}
	}

}
void InspectorManager::ShowInspector(Actor * actor)
{
	cur_actor = NULL;
	cur_actor = actor;
	static unsigned int AxisVAO,AxisVBO;
	// Create Axis
/*	float AxisVertices[] = {
		//red			 //X-axis                          //arrow
		1.0f,0.0f,0.0f,  -4.0f,0.0f,0.0f,  4.0f,0.0f,0.0f,  4.0f,0.0f,0.0f, 3.0f,1.0f,0.0f, 4.0f,0.0f,0.0f, 3.0f,-1.0f,0.0f,
		//green          //Y-axis
		0.0f,1.0f,0.0f,  0.0f,-4.0f,0.0f,  0.0f,4.0f,0.0f,  0.0f,4.0f,0.0f, 1.0f,3.0f,0.0f, 0.0f,4.0f,0.0f, -1.0f,3.0f,0.0f,
		//blue           //Z-axis
		0.0f,0.0f,1.0f,  0.0f,0.0f,-4.0f,  0.0f,0.0f,4.0f,  0.0f,0.0f,4.0f, 0.0f,1.0f,3.0f, 0.0f,0.0f,4.0f, 0.0f,-1.0f,3.0f,
	};*/

}
void InspectorManager::ListInspectorCur()
{
	
	
	if (cur_actor!=NULL)
	{
		if (cur_actor->transform != NULL && cur_actor->transform->enabled)
		{
			
			if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)|false) 
			{
				ImGui::DragFloat3("Position", (float*)&cur_actor->transform->position, 0.01f);
				ImGui::DragFloat3("Rotation", (float*)&cur_actor->transform->rotation, 1.0f);
				ImGui::DragFloat3("Scale", (float*)&cur_actor->transform->scale, 0.01f);	
			}
		}
		if (cur_actor->_Dirlight != NULL && cur_actor->_Dirlight->enabled)
		{
			if (ImGui::CollapsingHeader("DirectionalLight", ImGuiTreeNodeFlags_DefaultOpen)|false)
			{
				ImGui::ColorEdit3("Ambient", (float*)&cur_actor->_Dirlight->Ambient);
				ImGui::ColorEdit3("Diffuse", (float*)&cur_actor->_Dirlight->Diffuse);
				ImGui::ColorEdit3("Specular", (float*)&cur_actor->_Dirlight->Specular);	
			}
		}
		if (cur_actor->_PointLight != NULL&&cur_actor->_PointLight->enabled)
		{
			if (ImGui::CollapsingHeader("PointLight", ImGuiTreeNodeFlags_DefaultOpen) | false)
			{
				ImGui::ColorEdit3("Ambient", (float*)&cur_actor->_PointLight->Ambient);
				ImGui::ColorEdit3("Diffuse", (float*)&cur_actor->_PointLight->Diffuse);
				ImGui::ColorEdit3("Specular", (float*)&cur_actor->_PointLight->Specular);
				ImGui::DragFloat("Constant", &cur_actor->_PointLight->Constant,0.01f);
				ImGui::DragFloat("linear", &cur_actor->_PointLight->linear, 0.01f);
				ImGui::DragFloat("quadratic", &cur_actor->_PointLight->quadratic, 0.01f);
			}
		}		
	}
}

static void ShowExampleAppSimpleOverlay(bool* p_open)
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x/640*100, io.DisplaySize.y/1080*150), ImGuiCond_Always);
	const float DISTANCE = 10.0f;
	static int corner = 0;
	
	if (corner != -1)
	{
		ImVec2 window_pos = ImVec2((corner & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io.DisplaySize.y-DISTANCE : DISTANCE);
		ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	}
	ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
	if (ImGui::Begin("Example: Simple overlay", p_open, (corner != -1 ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
	{
		ImGui::Text("Simple overlay\n" "in the corner of the screen.\n" "(right-click to change position)");
		ImGui::Separator();
		if (ImGui::IsMousePosValid())
		{
			ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
			ImGui::Text("World Position: (%.1f,%.1f,%.1f)", Raycast::GetWorldPosition().x, Raycast::GetWorldPosition().y, Raycast::GetWorldPosition().z);
		}
			
		else
			ImGui::Text("Mouse Position: <invalid>");
		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::MenuItem("Custom", NULL, corner == -1)) corner = -1;
			if (ImGui::MenuItem("Top-left", NULL, corner == 0)) corner = 0;
			if (ImGui::MenuItem("Top-right", NULL, corner == 1)) corner = 1;
			if (ImGui::MenuItem("Bottom-left", NULL, corner == 2)) corner = 2;
			if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
			if (p_open && ImGui::MenuItem("Close")) *p_open = false;
			ImGui::EndPopup();
		}
	}
	ImGui::End();
}