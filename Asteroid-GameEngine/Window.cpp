#include <Window.h>

#include "GraphicEngine/imgui.h"

#include <Units/Actor.h>
#include <ADD_Component.h>
#include <World.h>
#include <SceneManager.h>
#include <Units/Camera.h>
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
Game_Mode WindowUI::_mode = Mode_3D;
std::vector<int> Window::vec_ID;
WindowUI *Window::_Main_UI;

Camera _editorCamera(glm::vec3(0.0f, 0.0f, 3.0f));

//static unsigned int _Width = 800;
//static unsigned int _Height = 600;

/*void Window::InitWorld()
{
	this->_Mainworld = new World();
	this->_Mainworld->CreateDepthMap();
}*/
// ---------------------------UI---------------------------
static void MainMenuBar();
static void Menu_File();
//static void ShowExampleAppLayout(bool* p_open);
static float _maineditorX = _Width / 3, _maineditorY = _Height / 3;
static float _LogoutX = _maineditorX * 2;
static float _SceneX = 213, _SceneY = 360;
static void ShowSimpleOverlay(bool* p_open);
bool WindowUI::show_simple_overlay = true;
static SelectObject * _headSelectObject = new SelectObject();
static SelectObject *_cSelectObject = _headSelectObject;
float WindowUI::UI_Left_X;
float WindowUI::UI_Left_Y;
float WindowUI::UI_Right_X;
float WindowUI::_FPS=0;
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
	if (cur_selectobject == NULL) return;    //先暫時移除

	/*for (auto it = SceneManager::Objects.begin(); it != SceneManager::Objects.end(); it++)
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
	}*/
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
		cur_SelectObject = NULL;
		WindowUI::ListInspectorCur(NULL);
		if (_MainWorld->_piv != NULL) _MainWorld->_piv->AttachObject(NULL);
	}
}
void WindowUI::SelectThisObject(SelectObject * selectobject)
{
	Clear_ListBool();
	selectobject->Is_selected = !selectobject->Is_selected;
	cur_SelectObject = selectobject;

	if (_MainWorld->_piv != NULL)
	{
		_MainWorld->_piv->AttachObject(NULL);
		_MainWorld->_piv->AttachObject(selectobject->_actor);
	}
}
void WindowUI::ListInspectorCur(SelectObject* _sel)
{
	if (_sel != NULL)
	{
		if (_sel->_actor->transform != NULL && _sel->_actor->transform->enabled)
		{
			if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen) | false)
			{

				float _test[3] = { glm::radians(_sel->_actor->transform->rotation.x),glm::radians(_sel->_actor->transform->rotation.y),glm::radians(_sel->_actor->transform->rotation.z) };
				if (ImGui::DragFloat3("Position", (float*)&_sel->_actor->transform->position, 0.01f)) { _sel->_actor->transform->Translate(_sel->_actor->transform->position); }
				if (ImGui::DragFloat3("Rotation", (float*)&_sel->_actor->transform->rotation, 0.1f)) { _sel->_actor->transform->Rotate(_sel->_actor->transform->rotation); }

				if (ImGui::DragFloat3("Scale", (float*)&_sel->_actor->transform->scale, 0.01f)) { _sel->_actor->transform->Scale(_sel->_actor->transform->scale); }

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

				ImGui::ColorEdit4("Diffuse", (float*)&_sel->_actor->meshrender->VertexColor);

				if (ImGui::Checkbox("Debug", &_sel->_actor->meshrender->_needdebug))
				{
					_sel->_actor->meshrender->UpdateCollision();
				}
				static int _curco = 0;
				if (ImGui::Button("Reload Shader"))  SceneManager::NeedReload = true;
				
				ImGui::Checkbox("Visable", &_sel->_actor->meshrender->_visable);
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
					_sel->_actor->boxcollision->UpdateCollision();
				}
				if (ImGui::Checkbox("Debug Line", &_sel->_actor->boxcollision->_needdebug))
				{
					_sel->_actor->boxcollision->UpdateCollision();
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

			UI_Left_X = ImGui::GetWindowWidth()+ ImGui::GetWindowPos().x;
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

			if (ImGui::Button("新增一個空物件"))
			{
				Actor* _ac = ADD_Component::Add_Actor();
				_ac->transform->name = (char*) "New Actor";
			}
			if (ImGui::Button("Create a cube"))
			{
				Actor* _ac = ADD_Component::Add_Actor();
				ADD_Component::Add_Meshrender(_ac,Cube);
				_ac->transform->name = (char*) "New Cube";
			}
			if (ImGui::Button("Create a sphere"))
			{
				Actor* _ac = ADD_Component::Add_Actor();
				ADD_Component::Add_Meshrender(_ac, Sphere);
				_ac->transform->name = (char*) "New Sphere";
			}
			if (ImGui::Button("Create a planet"))
			{
				Actor* _ac = ADD_Component::Add_Actor();
				ADD_Component::Add_Meshrender(_ac, "ExampleModel/planet.obj");
				_ac->transform->name = (char*) "New Plantet";
			}

			if (ImGui::Button("Create a Directional light"))
			{
				Actor* _ac = ADD_Component::Add_Actor();
				ADD_Component::Add_DirectionalLight(_ac);
				_ac->transform->name = (char*) "New DirectionalLight";
			}
			if (ImGui::Button("Create a PointLight"))
			{
				Actor* _ac = ADD_Component::Add_Actor();
				ADD_Component::Add_PointLight(_ac);
				_ac->transform->name = (char*) "New PointLight";
			}
			if (ImGui::Button("Create 100 Asteroids"))
			{
				unsigned int amount = 100;
				
				srand(glfwGetTime()); // initialize random seed	
				float radius = 50.0;
				float offset = 2.5f;
				for (unsigned int i = 0; i < amount; i++)
				{
					Actor* _ac = ADD_Component::Add_Actor();
					ADD_Component::Add_Meshrender(_ac, "ExampleModel/rock.obj");
					_ac->transform->name = (char*) "New Asteroid";
					

					//glm::mat4 model = glm::mat4(1.0f);
					// 1. translation: displace along circle with 'radius' in range [-offset, offset]
					float angle = (float)i / (float)amount * 360.0f;
					float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
					float x = sin(angle) * radius + displacement;
					displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
					float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
					displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
					float z = cos(angle) * radius + displacement;
					//model = glm::translate(model, );
					_ac->transform->Translate(glm::vec3(x, y, z));
					// 2. scale: Scale between 0.05 and 0.25f
					float scale = (rand() % 20) / 100.0f + 0.05;
					_ac->transform->Scale(glm::vec3(scale));
					// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
					float rotAngle = (rand() % 360);
					//model = glm::rotate(model, rotAngle, );
					_ac->transform->Rotate(glm::vec3(0.4f*rotAngle, 0.6f*rotAngle, 0.8f*rotAngle));
					// 4. now add to list of matrices
					

					
				}

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
			WindowUI::ListInspectorCur(WindowUI::cur_SelectObject);

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
			ImGui::GetWindowDrawList()->AddImage(
				(void *)(intptr_t)textureColorbuffer,
				ImVec2(ImGui::GetCursorScreenPos()),
				ImVec2(ImGui::GetCursorScreenPos().x + _Width,
					ImGui::GetCursorScreenPos().y + _Height), ImVec2(0, 1), ImVec2(1, 0));
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
			if (ImGui::MenuItem("Add MeshRender")) { if (WindowUI::cur_SelectObject->_actor != NULL) ADD_Component::Add_Meshrender(WindowUI::cur_SelectObject->_actor,Cube); }

			ImGui::Separator();
			if (ImGui::MenuItem("Add DirectionalLight")) { if (WindowUI::cur_SelectObject->_actor != NULL) ADD_Component::Add_DirectionalLight(WindowUI::cur_SelectObject->_actor); }
			if (ImGui::MenuItem("Add PointLight")) { if (WindowUI::cur_SelectObject->_actor != NULL) ADD_Component::Add_PointLight(WindowUI::cur_SelectObject->_actor); }
			if (ImGui::MenuItem("Add TestComponent")) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Add BoxCollision")) 
			{ 
				if (WindowUI::cur_SelectObject != NULL&&WindowUI::cur_SelectObject->_actor != NULL)
					ADD_Component::Add_BoxCollision(WindowUI::cur_SelectObject->_actor); 
					
			}
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
				
			}
			if (ImGui::MenuItem("Simple Overlay", "", &WindowUI::show_simple_overlay)) {}
			if (ImGui::MenuItem("PlayMode", "", &World::_PlayMode))
			{
				for (int i = 0; i < SceneManager::Objects.size(); i++)
				{
					if (SceneManager::Objects[i]->boxcollision != NULL) SceneManager::Objects[i]->boxcollision->UpdateCollision();
				}
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

			ImGui::Text(WindowUI::_mode ? "Game Mode: 3D" : "Game Mode: 2D");
			ImGui::Text(Window::DeBug_Mode ? "Debug Mode: Active" : "Debug Mode: inValid");
		}
		ImGui::End();





	}
}