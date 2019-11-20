#include <Window.hpp>

#include <imgui.h>

#include <Actor.hpp>
#include <ADD_Component.hpp>

#include <World.hpp>
#include <SceneManager.hpp>
#include <Camera.hpp>

#include <AGE_FileBrowser.hpp>

//#include <AGE_Model.h>


GLFWwindow* Window::MainGLFWwindow = NULL;
//Default  settings

// ImVec2  Window::viewport_pos;
// ImVec2  Window::viewport_size;

bool Window::WindowShouldClose = false;
bool Window::DeBug_Mode = false;

//World* Window::_Mainworld;
extern World* _MainWorld;
//關於專案設定
bool WindowUI::All_UIElement = true;

std::vector<int> Window::vec_ID;
WindowUI* Window::_Main_UI;

Camera _editorCamera(glm::vec3(0.0f, 0.0f, 3.0f));



// ---------------------------UI---------------------------
static void MainMenuBar();
static void Menu_File();

static float _maineditorX = Window_Width / 3, _maineditorY = Window_Height / 3;
static float _LogoutX = _maineditorX * 2;
static float _SceneX = 213, _SceneY = 360;
static void ShowSimpleOverlay(bool* p_open);

bool WindowUI::show_simple_overlay = true;


float WindowUI::UI_Left_X;
float WindowUI::UI_Left_Y;
float WindowUI::UI_Right_X;
float WindowUI::_FPS=0;
int Window::progect_I_am_focus;


//SelectObject *WindowUI::cur_SelectObject;



void WindowUI::ShowMyImGUIDemoWindow(bool* p_open, unsigned int* width, unsigned int* height)
{

	//Main Background--------------------------------------------------------------------------------------
	if (WindowUI::All_UIElement)
	{
		//Scene------------------------------------------------------------------------------------------------
		ImGui::SetNextWindowPos(ImVec2(0, *height / 35), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(_SceneX, _SceneY), ImGuiCond_FirstUseEver);
		{
			if (!ImGui::Begin("Scene", p_open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus))
			{
				// Early out if the window is collapsed, as an optimization.
				ImGui::End();
				return;
			}
			//------------------------------------------------------------------------------------------------這裡根據物件太多可能會導致性能瓶頸  應該可以優化
			/*if (_MainWorld->_SceneManager.Objects.size() > 0)
			{
				if (_MainWorld->_SceneManager.Objects.size() != SceneObject_List.size())  //刷新
				{
					SceneObject_List.clear();
					for (int n = 0; n < _MainWorld->_SceneManager.Objects.size(); n++)
					{
						SceneObject_List.push_back(new SelectObject(_MainWorld->_SceneManager.Objects[n]));
					}
				}

				//buf1 =(char*) "";
				for (int n = 0; n < SceneObject_List.size(); n++)
				{

					std::string _newname = _MainWorld->_SceneManager.Objects[n]->transform->name + std::to_string(n);
					if (ImGui::Selectable(_newname.c_str(), SceneObject_List[n]->Is_selected))
					{
						WindowUI::SelectThisObject(SceneObject_List[n]);
					}
				}

			}*/


			

			UI_Left_X = ImGui::GetWindowWidth() + ImGui::GetWindowPos().x;
			UI_Left_Y = ImGui::GetWindowPos().y;
			_SceneX = ImGui::GetWindowWidth();
			_SceneY = ImGui::GetWindowHeight();
			//delete [] array;
			ImGui::End();

		}
		//Asset------------------------------------------------------------------------------------------------
		ImGui::SetNextWindowPos(ImVec2(0, *height / 35 + _SceneY), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(_SceneX, *height - _SceneY), ImGuiCond_FirstUseEver);
		{

			if (!ImGui::Begin("Asset", p_open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus))
			{
				// Early out if the window is collapsed, as an optimization.
				ImGui::End();
				return;
			}
			//------------------------------------------------------------------------------------------------
			ImGui::Text("fsdfsdad我是中文dfd");
			ImGui::Text("Kanjis: \xe6\x97\xa5\xe6\x9c\xac\xe8\xaa\x9e (nihongo)");

			if (ImGui::Button("Create a empty object"))
			{
			
			}
			if (ImGui::Button("Create a cube"))
			{
				
			}
			if (ImGui::Button("Create a sphere"))
			{
				
			}
			if (ImGui::Button("Create a planet"))
			{

			}

			if (ImGui::Button("Create a Directional light"))
			{
				
			}
			if (ImGui::Button("Create a PointLight"))
			{
				
			}
			if (ImGui::Button("Create 100 Asteroids"))
			{


			}
			if (ImGui::Button("Create a doll"))
			{
			
			}
			if (ImGui::Button("Create a Lighting"))
			{
				
			}
			if (ImGui::Button("Create a 2B"))
			{
				
			}
			if (ImGui::Button("Create a phoenix"))
			{
				
			}

			_SceneX = ImGui::GetWindowWidth();
			_SceneY = *height - ImGui::GetWindowHeight();
			ImGui::End();

		}
		//Main Editor------------------------------------------------------------------------------------------
	/*	ImGui::SetNextWindowPos(ImVec2(0+ _SceneX, *height / 35), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(_maineditorX, _maineditorY), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowBgAlpha(0.0f);
		{
			if (!ImGui::Begin("Main Editor", p_open, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoBringToFrontOnFocus|ImGuiWindowFlags_NoResize))
			{
				// Early out if the window is collapsed, as an optimization.
				ImGui::End();
				return;
			}
			Window::viewport_pos = ImVec2(0,0);
			Window::viewport_size = ImVec2(0,0);
			ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_Reorderable;
		//  這是甚麼神奇的情況?    馬掉的畫滑鼠座標就不能用了?'
			ImGui::Separator();
			ImGui::End();

		}*/
		//Inspector-----------------------------------------------------------------------------------------
		ImGui::SetNextWindowPos(ImVec2(_SceneX + _maineditorX, *height / 35), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(*width - (_maineditorX + _SceneX), *height), ImGuiCond_FirstUseEver);
		{

			if (!ImGui::Begin("Inspector", p_open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus))
			{
				// Early out if the window is collapsed, as an optimization.
				ImGui::End();
				return;
			}
			

			UI_Right_X = ImGui::GetWindowPos().x;
			//------------------------------------------------------------------------------------------------
			ImGui::End();

		}
		//Logout layer-----------------------------------------------------------------------------------------
		/*
		ImGui::SetNextWindowPos(ImVec2(_SceneX + _maineditorX, *height/2), ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImVec2(_maineditorX, *height - _maineditorY), ImGuiCond_Once);
		{

			if (!ImGui::Begin("options", p_open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus))
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
				ImGui::Checkbox("No bring to front", &no_bring_to_front);
			}
			_maineditorX = ImGui::GetWindowWidth();
			_maineditorY = *height - ImGui::GetWindowHeight();
			ImGui::End();
		}*/
	}

	//Main Background--------------------------------------------------------------------------------------
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(10, 10), ImGuiCond_Once);
	{
		if (!ImGui::Begin("Main Background", p_open, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoBackground))
		{
			// Early out if the window is collapsed, as an optimization.
			ImGui::End();
			return;
		}
		if (!WindowUI::All_UIElement)
		{
			//Window::viewport_pos = ImGui::GetWindowPos();
			//Window::viewport_size = ImGui::GetWindowSize();
			/*ImGui::GetWindowDrawList()->AddImage(
				(void*)(intptr_t)textureColorbuffer,
				ImVec2(ImGui::GetCursorScreenPos()),
				ImVec2(ImGui::GetCursorScreenPos().x + _Width,
					ImGui::GetCursorScreenPos().y + _Height), ImVec2(0, 1), ImVec2(1, 0));*/
		}
		MainMenuBar();
		ImGui::End();
	}
	//調試用

	{
		if (!ImGui::Begin("FileBrowser", p_open))	// Early out if the window is collapsed, as an optimization.
		{
			ImGui::End();
			return;
		}
		
		AGE_FileBrowser::ImGUIListTheBrowser();

		ImGui::End();
	}







	ShowSimpleOverlay(&WindowUI::show_simple_overlay);

	ImGui::ShowDemoWindow(p_open);
}




static void MainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			Menu_File();
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
		if (ImGui::BeginMenu("Objects"))
		{
			if (ImGui::MenuItem("Add Cube")) {}
			if (ImGui::MenuItem("Add Sphere")) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Add capsule")) {}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Add Component"))
		{
			if (ImGui::MenuItem("Add MeshRender")) {  }

			ImGui::Separator();
			if (ImGui::MenuItem("Add DirectionalLight")) {}
			if (ImGui::MenuItem("Add PointLight")) { }
			if (ImGui::MenuItem("Add TestComponent")) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Add BoxCollision")) 
			{ 
				
				
			}
			if (ImGui::MenuItem("Add BoxCollision2D")) {}
			if (ImGui::MenuItem("Add SphereCollision")) {}
			if (ImGui::MenuItem("Add SphereCollision2D")) {}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("DEBUG_setting"))
		{
			ImGui::MenuItem("All Element", "", &WindowUI::All_UIElement);
			//ImGui::MenuItem("Render shadow", "", &_MainWorld->_RenderShadow);
			ImGui::MenuItem("Simple Overlay", "", &WindowUI::show_simple_overlay);
			//ImGui::MenuItem("PlayMode", "", &_MainWorld->_PlayMode);
			
			const char* items[] = { "Basic","Color","Light","Shadow" };
			static int item_current = 0;
			if (ImGui::Combo("", &item_current, items, IM_ARRAYSIZE(items)))
			{
				
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Example"))
		{
			for (int i = 0 ; i < 10; i++)
			{
				if (ImGui::MenuItem(std::to_string(i).c_str())) { }
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}
static void Menu_File()
{
	ImGui::MenuItem("(dummy menu)", NULL, false, false);
	if (ImGui::MenuItem("New")) { }
	if (ImGui::MenuItem("Open", "Ctrl+O")) {  }

	if (ImGui::MenuItem("Save", "Ctrl+S")) {  }
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
static void ShowSimpleOverlay(bool* p_open)
{
	if (*p_open)
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x / 640 * 100, io.DisplaySize.y / 1080 * 200), ImGuiCond_Always);
		const float DISTANCE = 10.0f;
		static int corner = 0;

		if (corner != -1)
		{
			ImVec2 window_pos = ImVec2((corner & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io.DisplaySize.y - DISTANCE : DISTANCE);
			ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
			ImGui::SetNextWindowPos(ImVec2(WindowUI::UI_Left_X, window_pos.y + 10), ImGuiCond_Always, window_pos_pivot);
		}
		ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
		if (ImGui::Begin("Example: Simple overlay", p_open, (corner != -1 ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
		{
			ImGui::Text("Simple overlay\n" "in the corner of the screen.\n" "(right-click to change position)");
			ImGui::Separator();
			
			ImGui::Text("FPS: %.1f", WindowUI::_FPS);
			
			
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

			
			ImGui::Text(Window::DeBug_Mode ? "Debug Mode: Active" : "Debug Mode: inValid");
			//ImGui::Text(_MainWorld->_SceneManager._FilePAth.c_str());
			//ImGui::Text(std::to_string(glm::length(_editorCamera.transform.position)).c_str());
		}
		ImGui::End();





	}
}