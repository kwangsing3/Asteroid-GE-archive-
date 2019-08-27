#include <Window.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "GraphicEngine/imgui.h"

#include <ADD_Component.h>
#include <SceneManager.h>
#include <Raycast.h>

 GLFWwindow* Window::MainGLFWwindow = NULL;

//Default  settings

 unsigned int Window::_Width = 800;
 unsigned int Window::_Height = 600;
 ImVec2  Window::viewport_pos;
 ImVec2  Window::viewport_size;
 Camera Window::_editorCamera(glm::vec3(0.0f,0.0f,3.0f));
 bool Window::WindowShouldClose = false;
 bool Window::DeBug_Mode=false;
 


 //關於專案設定
 bool WindowUI::All_UIElement=true;
 Game_Mode WindowUI::_mode= Mode_3D;
 std::vector<int> Window::vec_ID;
 WindowUI *Window::_Main_UI;


// ---------------------------UI---------------------------
static void MainMenuBar();
static void Menu_File();
//static void ShowExampleAppLayout(bool* p_open);
static float _maineditorX = 854, _maineditorY = 590;
static float _LogoutX = 854;
static float _SceneX = 213, _SceneY = 360;
static void ShowSimpleOverlay(bool* p_open);
bool WindowUI::show_simple_overlay = true;
static SelectObject * _headSelectObject = new SelectObject();
static SelectObject *_cSelectObject = _headSelectObject;

void Clear_ListBool()
{
	_cSelectObject = _headSelectObject;
	_cSelectObject->Is_selected = false;
	_cSelectObject->Is_renaming = false;
	while (_cSelectObject->next != NULL)
	{
		if (_cSelectObject->next == NULL)
			return;
		_cSelectObject->Is_selected = false;
		_cSelectObject->Is_renaming = false;
		_cSelectObject = _cSelectObject->next;
	}
	return;
}

SelectObject *WindowUI::cur_SelectObject;



void WindowUI::Deletecur_actor(SelectObject* cur_selectobject)
{
	if (cur_selectobject == NULL) return;

	for (auto it = SceneManager::Objects.begin(); it != SceneManager::Objects.end(); it++)
	{
		if (*it == cur_selectobject->_actor)
		{
			SceneManager::Objects.erase(it);
			break;
		}
	}
	for (auto it = SceneManager::vec_ObjectsToRender.begin(); it != SceneManager::vec_ObjectsToRender.end(); it++)
	{
		if (*it == cur_selectobject->_actor->meshrender)
		{
			SceneManager::vec_ObjectsToRender.erase(it);
			break;
		}
	}
	if (cur_selectobject->_actor->_PointLight != NULL)
	{
		for (auto it = SceneManager::vec_PointLight.begin(); it != SceneManager::vec_PointLight.end(); it++)
		{
			if (*it == cur_selectobject->_actor->_PointLight)
			{
				SceneManager::vec_PointLight.erase(it);
				break;
			}
		}
	}
	if (cur_selectobject->_actor->_Dirlight != NULL)
	{
		for (auto it = SceneManager::vec_DirectionlLight.begin(); it != SceneManager::vec_DirectionlLight.end(); it++)
		{
			if (*it == cur_selectobject->_actor->_Dirlight)
			{
				SceneManager::vec_DirectionlLight.erase(it);
				break;
			}
		}
	}
	cur_selectobject = NULL;
}
void WindowUI::Renamecur_actor(SelectObject * cur_actor)
{
	if (cur_actor != NULL)
		cur_actor->Is_renaming = true;
}
void WindowUI::SelectThisActor(Actor * _actor)
{
	if (_actor != NULL)
	{
		_cSelectObject = _headSelectObject;
		while (_cSelectObject->next != NULL)
		{
			if (_cSelectObject->_actor == _actor)
			{
				WindowUI::SelectThisObject(_cSelectObject);
				break;
			}

			if (_cSelectObject->next != NULL)_cSelectObject = _cSelectObject->next;
		}
	}
	else
	{
		WindowUI::ListInspectorCur(NULL);
	}
}
void WindowUI::SelectThisObject(SelectObject * selectobject)
{
	Clear_ListBool();
	selectobject->Is_selected = !selectobject->Is_selected;
	cur_SelectObject = selectobject;
	static unsigned int AxisVAO, AxisVBO;
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
void WindowUI::ListInspectorCur(SelectObject* _sel)
{
	if (_sel != NULL)
	{
		if (_sel->_actor->transform != NULL && _sel->_actor->transform->enabled)
		{

			if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen) | false)
			{
				if (ImGui::DragFloat3("Position", (float*)&_sel->_actor->transform->position, 0.01f)) { if (_sel->_actor->boxcollision != NULL)_sel->_actor->boxcollision->ResetDynamic(); _sel->_actor->meshrender->ResetDynamic(); }
				if (ImGui::DragFloat3("Rotation", (float*)&_sel->_actor->transform->rotation, 1.0f) ){if (_sel->_actor->boxcollision != NULL)_sel->_actor->boxcollision->ResetDynamic(); _sel->_actor->meshrender->ResetDynamic();}
				if (ImGui::DragFloat3("Scale", (float*)&_sel->_actor->transform->scale, 0.01f)){if (_sel->_actor->boxcollision != NULL)_sel->_actor->boxcollision->ResetDynamic(); _sel->_actor->meshrender->ResetDynamic();}
				
			}
		}
		if (_sel->_actor->_Dirlight != NULL && _sel->_actor->_Dirlight->enabled)
		{
			if (ImGui::CollapsingHeader("DirectionalLight", ImGuiTreeNodeFlags_DefaultOpen) | false)
			{
				ImGui::ColorEdit3("Ambient", (float*)&_sel->_actor->_Dirlight->Ambient);
				ImGui::ColorEdit3("Diffuse", (float*)&_sel->_actor->_Dirlight->Diffuse);
				ImGui::ColorEdit3("Specular", (float*)&_sel->_actor->_Dirlight->Specular);
			}
		}
		if (_sel->_actor->_PointLight != NULL && _sel->_actor->_PointLight->enabled)
		{
			if (ImGui::CollapsingHeader("PointLight", ImGuiTreeNodeFlags_DefaultOpen) | false)
			{
				ImGui::ColorEdit3("Ambient", (float*)&_sel->_actor->_PointLight->Ambient);
				ImGui::ColorEdit3("Diffuse", (float*)&_sel->_actor->_PointLight->Diffuse);
				ImGui::ColorEdit3("Specular", (float*)&_sel->_actor->_PointLight->Specular);
				ImGui::DragFloat("Constant", &_sel->_actor->_PointLight->Constant, 0.01f);
				ImGui::DragFloat("linear", &_sel->_actor->_PointLight->linear, 0.01f);
				ImGui::DragFloat("quadratic", &_sel->_actor->_PointLight->quadratic, 0.01f);
			}
		}
		if (_sel->_actor->meshrender != NULL && _sel->_actor->meshrender->enabled)
		{
			if (ImGui::CollapsingHeader("MeshRender", ImGuiTreeNodeFlags_DefaultOpen) | false)
			{
				ImGui::Text("MeshRender");
				const char* items[] = { "Cube","Sphere","目前還沒有功能" };
				static int item_current = 0;
				ImGui::Combo("", &item_current, items, IM_ARRAYSIZE(items));

				 ImVec4 color = ImVec4(
					 _sel->_actor->meshrender->VertexColor.x *255.0f,
					 _sel->_actor->meshrender->VertexColor.y *255.0f,
					 _sel->_actor->meshrender->VertexColor.z *255.0f,
					255.0f / 255.0f);
				if (ImGui::ColorPicker4("##picker", (float*)&color, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview))
				{
					_sel->_actor->meshrender->VertexColor.x = color.x;
					_sel->_actor->meshrender->VertexColor.y = color.y;
					_sel->_actor->meshrender->VertexColor.z = color.z;
				}



			}
		}
		if (_sel->_actor->boxcollision != NULL && _sel->_actor->boxcollision->enabled)
		{	
			if (ImGui::CollapsingHeader("BoxCollision", ImGuiTreeNodeFlags_DefaultOpen) | false)
			{
				 float f0 = _sel->_actor->boxcollision->_Mass;
				if (ImGui::InputFloat("Mass", &f0, 0.1f, 1.0f, "%.3f"))
				{
					_sel->_actor->boxcollision->_Mass = f0;
					_sel->_actor->boxcollision->ResetDynamic();
				}
			}
		}
	}


}



void WindowUI::ShowMyImGUIDemoWindow(bool *p_open, unsigned int *width, unsigned int *height, unsigned int textureColorbuffer)
{

	//Main Background--------------------------------------------------------------------------------------
	if (WindowUI::All_UIElement)
	{
		//Main Editor------------------------------------------------------------------------------------------
		ImGui::SetNextWindowPos(ImVec2(0, *height / 35), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(_maineditorX, _maineditorY), ImGuiCond_Always);
		{
			if (!ImGui::Begin("Main Editor", p_open, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoBringToFrontOnFocus))
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
					Window::viewport_pos = ImVec2(50, 100);
					Window::viewport_size = ImVec2(640 ,360);

					ImGui::GetWindowDrawList()->AddImage(
						(void *)textureColorbuffer, 
						ImVec2(50,100),ImVec2(50+640, 100+360),
						ImVec2(0, 1), ImVec2(1, 0));
						// 基於這個視窗的大小
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
		ImGui::SetNextWindowSize(ImVec2(_maineditorX, *height - _maineditorY), ImGuiCond_Always);
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
				ImGui::Checkbox("No bring to front", &no_bring_to_front);*/
			}
			_maineditorX = ImGui::GetWindowWidth();
			_maineditorY = *height - ImGui::GetWindowHeight();
			ImGui::End();
		}

		//Scene------------------------------------------------------------------------------------------------
		ImGui::SetNextWindowPos(ImVec2(_maineditorX, *height / 35), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(_SceneX, _SceneY), ImGuiCond_Always);
		{
			if (!ImGui::Begin("Scene", p_open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus))
			{
				// Early out if the window is collapsed, as an optimization.
				ImGui::End();
				return;
			}
			//------------------------------------------------------------------------------------------------
			if (SceneManager::Objects.size() > 0)
			{
				//buf1 =(char*) "";
				for (int n = 0; n < SceneManager::Objects.size(); n++)
				{
					char * buf1 = new char[64]();
					_cSelectObject->_actor = SceneManager::Objects[n];
					if (_cSelectObject->Is_renaming)
					{
						if (ImGui::InputText("", buf1, 64))
						{
							SceneManager::Objects[n]->transform->name = buf1;
						}
					}
					else
					{
						std::string _newname = SceneManager::Objects[n]->transform->name + std::to_string(n);
						const char* newchar = _newname.c_str();
						if (ImGui::Selectable(newchar, _cSelectObject->Is_selected))
						{
							if (!ImGui::GetIO().KeyCtrl)    // Clear selection when CTRL is not held
								WindowUI::SelectThisObject(_cSelectObject);
						}
					}
					if (_cSelectObject->next == NULL)
						_cSelectObject->next = new SelectObject();
					_cSelectObject = _cSelectObject->next;
				}
				_cSelectObject = _headSelectObject;
			}
		

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

			if (!ImGui::Begin("Asset", p_open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus))
			{
				// Early out if the window is collapsed, as an optimization.
				ImGui::End();
				return;
			}
			//------------------------------------------------------------------------------------------------
			ImGui::Text("fsdfsdad我是中文dfd");
			ImGui::Text("Kanjis: \xe6\x97\xa5\xe6\x9c\xac\xe8\xaa\x9e (nihongo)");

			if (ImGui::Button("新增一個空物件"))
			{
				Actor* _ac = ADD_Component::Add_Actor();
				_ac->transform->name = (char*) "New Actor";
			}
			if (ImGui::Button("新增一個方塊物件"))
			{
				Actor* _ac= ADD_Component::Add_Cube(ADD_Component::Add_Actor());
				_ac->transform->name =(char*) "New Cube";
			}

			if (ImGui::Button("新增一個方向光源物件"))
			{
				Actor* _ac = ADD_Component::Add_DirectionalLight(ADD_Component::Add_Actor());
				_ac->transform->name = (char*) "New DirectionalLight";
			}
			if (ImGui::Button("新增一個點光源物件"))
			{
				Actor* _ac = ADD_Component::Add_PointLight(ADD_Component::Add_Actor());
				_ac->transform->name = (char*) "New PointLight";
			}
			_SceneX = ImGui::GetWindowWidth();
			_SceneY = *height - ImGui::GetWindowHeight();
			ImGui::End();

		}
		//Inspector-----------------------------------------------------------------------------------------
		ImGui::SetNextWindowPos(ImVec2(_maineditorX + _SceneX, *height / 35), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(*width - (_maineditorX + _SceneX), *height), ImGuiCond_Always);
		{

			if (!ImGui::Begin("Inspector", p_open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus))
			{
				// Early out if the window is collapsed, as an optimization.
				ImGui::End();
				return;
			}
			WindowUI::ListInspectorCur(WindowUI::cur_SelectObject);
			//------------------------------------------------------------------------------------------------
			ImGui::End();

		}
	}

	//Main Background--------------------------------------------------------------------------------------
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(*width, *height), ImGuiCond_Always);
	{
		if (!ImGui::Begin("Main Background", p_open, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoBringToFrontOnFocus))
		{
			// Early out if the window is collapsed, as an optimization.
			ImGui::End();
			return;
		}
		if (!WindowUI::All_UIElement)
		{
			Window::viewport_pos = ImGui::GetWindowPos();
			Window::viewport_size = ImGui::GetWindowSize();
			ImGui::GetWindowDrawList()->AddImage(
				(void *)textureColorbuffer,
				ImVec2(ImGui::GetCursorScreenPos()),
				ImVec2(ImGui::GetCursorScreenPos().x + Window::viewport_size.x,
					ImGui::GetCursorScreenPos().y + Window::viewport_size.y), ImVec2(0, 1), ImVec2(1, 0));
		}
		MainMenuBar();
		ImGui::End();
	}
	//調試用
	if (Window::DeBug_Mode)
	{
	
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
			if (ImGui::MenuItem("Add MeshRender")) { if (WindowUI::cur_SelectObject->_actor != NULL) ADD_Component::Add_Cube(WindowUI::cur_SelectObject->_actor); }

			ImGui::Separator();
			if (ImGui::MenuItem("Add DirectionalLight")) { if (WindowUI::cur_SelectObject->_actor != NULL) ADD_Component::Add_DirectionalLight(WindowUI::cur_SelectObject->_actor); }
			if (ImGui::MenuItem("Add PointLight")) { if (WindowUI::cur_SelectObject->_actor != NULL) ADD_Component::Add_PointLight(WindowUI::cur_SelectObject->_actor); }
			if (ImGui::MenuItem("Add TestComponent")) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Add BoxCollision")) { if (WindowUI::cur_SelectObject->_actor != NULL) ADD_Component::Add_BoxCollision(WindowUI::cur_SelectObject->_actor); }
			if (ImGui::MenuItem("Add BoxCollision2D")) {}
			if (ImGui::MenuItem("Add SphereCollision")) {}
			if (ImGui::MenuItem("Add SphereCollision2D")) {}
			
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("UI Element"))
		{
			if (ImGui::MenuItem("All Element", "", &WindowUI::All_UIElement))
			{
				
			}
			if (ImGui::MenuItem("Switch_GameMode"))
			{
				if (WindowUI::_mode == Mode_3D)
				{
					WindowUI::_mode = Mode_2D;
					Window::_editorCamera.SwitchCamera3D(false);
				}	
				else
				{
					WindowUI::_mode = Mode_3D;
					Window::_editorCamera.SwitchCamera3D(true);
				}		
			}
			if (ImGui::MenuItem("Simple Overlay","",&WindowUI::show_simple_overlay))
			{
				

			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}
static void Menu_File()
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
			ImGui::SetNextWindowPos(ImVec2(window_pos.x, window_pos.y + 10), ImGuiCond_Always, window_pos_pivot);
		}
		ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
		if (ImGui::Begin("Example: Simple overlay", p_open, (corner != -1 ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
		{
			ImGui::Text("Simple overlay\n" "in the corner of the screen.\n" "(right-click to change position)");
			ImGui::Separator();
			if (ImGui::IsMousePosValid())
			{
				ImGui::Text("Click Position: (%.1f,%.1f)", Raycast::X_pos, Raycast::Y_pos);
				ImGui::Text("World Position: (%.1f,%.1f,%.1f)", Raycast::GetWorldPosition(0.0f).x, Raycast::GetWorldPosition(0.0f).y, Raycast::GetWorldPosition(0.0f).z);
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

			ImGui::Text(WindowUI::_mode ? "Game Mode: 3D" : "Game Mode: 2D");
			ImGui::Text(Window::DeBug_Mode ? "Debug Mode: Active" : "Debug Mode: inValid");
		}
		ImGui::End();

		



	}
}