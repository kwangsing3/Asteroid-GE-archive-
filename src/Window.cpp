#include <Window.hpp>
#include <AGE_Assert.hpp>
#include <GraphicEngine/imgui.h>

#include <Actor.hpp>
#include <ADD_Component.hpp>

#include <World.hpp>
#include <SceneManager.hpp>
#include <Camera.hpp>
#include <AGE_FileBrowser.hpp>

//#include <AGE_Model.h>

GLFWwindow* Window::MainGLFWwindow = NULL;
World* Window::_MainWorld;
WindowUI* Window::_Main_UI;

void Window::initialized()
{
	pugi::xml_document _doc;
	std::ifstream _XMLstream("Texture/GlobalSettings.xml");
	pugi::xml_parse_result result = _doc.load(_XMLstream);
	std::cout << "Load result: " << result.description() << std::endl;
	if (result != NULL)
	{
		this->_Main_UI->All_UIElement = _doc.child("GlobalSettings").child("ProjectSetting").child("All_UIElement").attribute("All_UIElement").as_bool();
		//this->DeBug_Mode = _doc.child("GlobalSettings").child("WindowSetting").child("Game_Mode").attribute("Game_Mode").as_bool();
		this->progect_I_am_focus = _doc.child("GlobalSettings").child("ProjectSetting").child("Project").attribute("Project_Focus").as_int();
	}
	_XMLstream.close();
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	Window_Width = mode->width;
	Window_Height = mode->height;
	MainGLFWwindow = glfwCreateWindow(Window_Width, Window_Height, "Asteroid-GameEngine", NULL, NULL);
	if (MainGLFWwindow == NULL)
	{
		AGE_PRINTCONSLE("Failed to create GLFW window");
		glfwTerminate();
		AGE_ASSERT(0);
	}
	glfwMaximizeWindow(MainGLFWwindow);
	//glfwHideWindow(MainGLFWwindow);
	//glfwSetWindowMonitor(MainGLFWwindow, monitor, 0, 0, _Width, _Height, mode->refreshRate);
	glfwMakeContextCurrent(MainGLFWwindow);
	glfwSetFramebufferSizeCallback(MainGLFWwindow, (GLFWframebuffersizefun)framebuffer_size_callback);
	glfwSetCursorPosCallback(MainGLFWwindow, (GLFWcursorposfun)mouse_move_callback);
	glfwSetScrollCallback(MainGLFWwindow, (GLFWscrollfun)scroll_callback);
	glfwSetMouseButtonCallback(MainGLFWwindow, (GLFWmousebuttonfun)mouse_button_callback);
	glfwSetKeyCallback(MainGLFWwindow, key_callback);
	// tell GLFW to capture our mouse
	glfwSetInputMode(MainGLFWwindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { AGE_PRINTCONSLE("Failed to initialize GLAD"); AGE_ASSERT(0); }

	_MainWorld = new World();
	_Main_UI = new WindowUI();
}

static void MainMenuBar();
static void Menu_File();
//static void ShowExampleAppLayout(bool* p_open);
static float _maineditorX = Window_Width / 3, _maineditorY = Window_Height / 3;
static float _LogoutX = _maineditorX * 2;
static float _SceneX = 213, _SceneY = 360;
static void ShowSimpleOverlay(bool* p_open);

bool WindowUI::show_simple_overlay = true;
static std::vector<SelectObject*> SceneObject_List;
bool Window::WindowShouldClose = false;

//關於專案設定
bool WindowUI::All_UIElement = true;
std::vector<int> Window::vec_ID;
float WindowUI::deltaTime = 0.0f;
float WindowUI::lastFrame = 0.0f;
float WindowUI::previousTime = glfwGetTime();
int WindowUI::frameCount = 0;
bool Window::isClickingPivot = false;
float  Window::lastX = 500.0f;
float  Window::lastY = 300.0f;

void WindowUI::Updated_WindowEvent(bool* p_open, unsigned int* width, unsigned int* height)
{
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	frameCount++;
	if (currentFrame - previousTime >= 1.0f)
	{
		_FPS = frameCount;
		frameCount = 0;
		previousTime = currentFrame;
	}
	//Main Background--------------------------------------------------------------------------------------
	if (All_UIElement)
	{
		//Scene------------------------------------------------------------------------------------------------
		ImGui::SetNextWindowPos(ImVec2(0, *height / 35), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(_SceneX, _SceneY), ImGuiCond_FirstUseEver);
		{
			if (!ImGui::Begin("Scene", p_open,ImGuiWindowFlags_NoBringToFrontOnFocus))
			{
				// Early out if the window is collapsed, as an optimization.
				ImGui::End();
				return;
			}
			//------------------------------------------------------------------------------------------------這裡根據物件太多可能會導致性能瓶頸  應該可以優化
			if (Window::_MainWorld->_SceneManager->Objects.size() > 0)
			{
				if (Window::_MainWorld->_SceneManager->Objects.size() != SceneObject_List.size())  //刷新
				{
					SceneObject_List.clear();
					for (int n = 0; n < Window::_MainWorld->_SceneManager->Objects.size(); n++)
					{
						SceneObject_List.push_back(new SelectObject(Window::_MainWorld->_SceneManager->Objects[n]));
					}
				}
				for (int n = 0; n < SceneObject_List.size(); n++)
				{
					std::string _newname = Window::_MainWorld->_SceneManager->Objects[n]->transform->name + std::to_string(n);
					if (ImGui::Selectable(_newname.c_str(), SceneObject_List[n]->Is_selected))
					{
						WindowUI::SelectThisObject(SceneObject_List[n]);
					}
				}
			}

			UI_Left_X = ImGui::GetWindowWidth() + ImGui::GetWindowPos().x;
			UI_Left_Y = ImGui::GetWindowPos().y;
			_SceneX = ImGui::GetWindowWidth();
			_SceneY = ImGui::GetWindowHeight();
			ImGui::End();
		}
		//Asset------------------------------------------------------------------------------------------------
		ImGui::SetNextWindowPos(ImVec2(0, *height / 35 + _SceneY), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(_SceneX, *height - _SceneY), ImGuiCond_FirstUseEver);
		{
			if (!ImGui::Begin("Asset", p_open, ImGuiWindowFlags_NoBringToFrontOnFocus))
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
				Actor* _ac = Window::_MainWorld->_SceneManager->_ADDManager->Add_Actor();
				_ac->transform->name = (char*)"New Actor";
			}
			if (ImGui::Button("Create a cube"))
			{
				Actor* _ac = Window::_MainWorld->_SceneManager->_ADDManager->Add_Actor();
				Window::_MainWorld->_SceneManager->_ADDManager->Add_Meshrender(_ac, Shape::Cube);
				_ac->transform->name = (char*)"New Cube";
			}
			if (ImGui::Button("Create a sphere"))
			{
				Actor* _ac = Window::_MainWorld->_SceneManager->_ADDManager->Add_Actor();
				Window::_MainWorld->_SceneManager->_ADDManager->Add_Meshrender(_ac, Shape::Sphere);
				_ac->transform->name = (char*)"New Sphere";
			}
			if (ImGui::Button("Create a planet"))
			{
				Actor* _ac = Window::_MainWorld->_SceneManager->_ADDManager->Add_Actor();
				Window::_MainWorld->_SceneManager->_ADDManager->Add_Meshrender(_ac, "ExampleModel/planet.obj");
				_ac->transform->name = (char*)"New Plantet";
			}
			if (ImGui::Button("Create a Directional light"))
			{
				Actor* _ac = Window::_MainWorld->_SceneManager->_ADDManager->Add_Actor();
				Window::_MainWorld->_SceneManager->_ADDManager->Add_DirectionalLight(_ac);
				_ac->transform->name = (char*)"New DirectionalLight";
			}
			if (ImGui::Button("Create a PointLight"))
			{
				Actor* _ac = Window::_MainWorld->_SceneManager->_ADDManager->Add_Actor();
				Window::_MainWorld->_SceneManager->_ADDManager->Add_PointLight(_ac);
				_ac->transform->name = (char*)"New PointLight";
			}
			if (ImGui::Button("Create 100 Asteroids"))
			{
				unsigned int amount = 100;
				srand(glfwGetTime()); // initialize random seed	
				float radius = 50.0;
				float offset = 2.5f;
				for (unsigned int i = 0; i < amount; i++)
				{
					Actor* _ac = Window::_MainWorld->_SceneManager->_ADDManager->Add_Actor();
					Window::_MainWorld->_SceneManager->_ADDManager->Add_Meshrender(_ac, "ExampleModel/rock.obj");
					_ac->transform->name = (char*)"New Asteroid";
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
					_ac->transform->Rotate(glm::vec3(0.4f * rotAngle, 0.6f * rotAngle, 0.8f * rotAngle));
					// 4. now add to list of matrices
				}

			}
			if (ImGui::Button("Create a doll"))
			{
				Actor* _ac = Window::_MainWorld->_SceneManager->_ADDManager->Add_Actor();
				Window::_MainWorld->_SceneManager->_ADDManager->Add_Meshrender(_ac, "ExampleModel/model.dae");
				_ac->transform->name = (char*)"New doll";
			}
			if (ImGui::Button("Create a Lighting"))
			{
				Actor* _ac = Window::_MainWorld->_SceneManager->_ADDManager->Add_Actor();
				Window::_MainWorld->_SceneManager->_ADDManager->Add_Meshrender(_ac, "ExampleModel/FFXIII-Lighting/lightning_obj.obj");
				_ac->transform->name = (char*)"New Lighting";
			}
			if (ImGui::Button("Create a 2B"))
			{
				Actor* _ac = Window::_MainWorld->_SceneManager->_ADDManager->Add_Actor();
				Window::_MainWorld->_SceneManager->_ADDManager->Add_Meshrender(_ac, "ExampleModel/nierautomata-2b/source/nier-3dprint.fbx");
				_ac->transform->name = (char*)"New 2B";
			}
			if (ImGui::Button("Create a phoenix"))
			{
				Actor* _ac = Window::_MainWorld->_SceneManager->_ADDManager->Add_Actor();
				Window::_MainWorld->_SceneManager->_ADDManager->Add_Meshrender(_ac, "ExampleModel/phoenix-bird/source/fly.fbx");
				_ac->transform->name = (char*)"New phoenix";
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

			if (!ImGui::Begin("Inspector", p_open, ImGuiWindowFlags_NoBringToFrontOnFocus))
			{
				// Early out if the window is collapsed, as an optimization.
				ImGui::End();
				return;
			}
			WindowUI::ListInspectorCur(cur_SelectObject_List.size() > 0 ? cur_SelectObject_List[0] : NULL);

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
	if (!ImGui::Begin("FileBrowser", p_open))	// Early out if the window is collapsed, as an optimization.
	{
		ImGui::End();
		return;
	}
	AGE_FileBrowser::ImGUIListTheBrowser();
	ImGui::End();




	MainMenuBar();






	ShowSimpleOverlay(&WindowUI::show_simple_overlay);

	ImGui::ShowDemoWindow(p_open);
}

// Keep running


// ---------------------------UI---------------------------


float WindowUI::UI_Left_X;
float WindowUI::UI_Left_Y;
float WindowUI::UI_Right_X;
float WindowUI::_FPS = 0;
int Window::progect_I_am_focus;
std::vector<SelectObject*> WindowUI::cur_SelectObject_List;
std::vector<SelectObject*> WindowUI::copy_SelectObject_List;
void Clear_ListBool()
{
	for (int i = 0; i < SceneObject_List.size(); i++)
	{
		SceneObject_List[i]->Is_renaming = SceneObject_List[i]->Is_selected = false;
	}
}
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
void WindowUI::Renamecur_actor(SelectObject* cur_actor)
{
	if (cur_actor != NULL)
		cur_actor->Is_renaming = true;
}
void WindowUI::SelectThisActor(Actor* _actor)
{
	//Window::_MainWorld->depose_init_PhysicsProgress();
	//Window::_MainWorld->InitPhysics = true;

	if (_actor != NULL)
	{
		for (int i = 0; i < SceneObject_List.size(); i++)
		{
			if (SceneObject_List[i]->_actor == _actor)
			{
				WindowUI::SelectThisObject(SceneObject_List[i]);
				break;
			}
		}
	}
	else
	{
		Clear_ListBool();
		cur_SelectObject_List.clear();
		WindowUI::ListInspectorCur(NULL);
		if (Window::_MainWorld->_piv != NULL) Window::_MainWorld->_piv->AttachObject(NULL);
	}
}
void WindowUI::SelectThisObject(SelectObject* selectobject)
{
	if (!ImGui::GetIO().KeyCtrl)    // Clear selection when CTRL is not held  沒有按壓才會進來
	{
		Clear_ListBool();
		cur_SelectObject_List.clear();

		if (Window::_MainWorld->_piv != NULL)
		{
			Window::_MainWorld->_piv->AttachObject(NULL);
			Window::_MainWorld->_piv->AttachObject(selectobject->_actor);
		}
	}
	else
	{
		if (Window::_MainWorld->_piv != NULL)
		{
			Window::_MainWorld->_piv->AttachObject_Multiple(selectobject->_actor);
		}
	}
	selectobject->Is_selected = !selectobject->Is_selected;
	cur_SelectObject_List.push_back(selectobject);
}
void WindowUI::CopyEvent()
{
	if (cur_SelectObject_List.empty())
		return;

	copy_SelectObject_List = cur_SelectObject_List;
}
void WindowUI::PasteEvent()
{
	//Paste
	if (copy_SelectObject_List.empty())
		return;
	Clear_ListBool();
	cur_SelectObject_List.clear();
	if (Window::_MainWorld->_piv != NULL) Window::_MainWorld->_piv->_lowwerActor.clear();
	for (int i = 0; i < copy_SelectObject_List.size(); i++)
	{
		SelectObject* _ns = new SelectObject(Window::_MainWorld->_SceneManager->_ADDManager->Copy_Actor(copy_SelectObject_List[i]->_actor));
		_ns->Is_selected = true;
		SceneObject_List.push_back(_ns);
		cur_SelectObject_List.push_back(_ns);

		if (Window::_MainWorld->_piv != NULL)
		{
			Window::_MainWorld->_piv->AttachObject_Multiple(_ns->_actor);
		}
	}
	copy_SelectObject_List.clear();


	//Select
}
void WindowUI::ListInspectorCur(SelectObject* _sel)
{
	if (_sel != NULL)
	{
		for (auto& com : _sel->_actor->_Components)
		{
			com->Inspector();
		}
	}
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
			if (ImGui::MenuItem("Add MeshRender")) { if (WindowUI::cur_SelectObject_List[0]->_actor != NULL) Window::_MainWorld->_SceneManager->_ADDManager->Add_Meshrender(WindowUI::cur_SelectObject_List[0]->_actor, Shape::Cube); }

			ImGui::Separator();
			if (ImGui::MenuItem("Add DirectionalLight")) { if (WindowUI::cur_SelectObject_List[0]->_actor != NULL)  Window::_MainWorld->_SceneManager->_ADDManager->Add_DirectionalLight(WindowUI::cur_SelectObject_List[0]->_actor); }
			if (ImGui::MenuItem("Add PointLight")) { if (WindowUI::cur_SelectObject_List[0]->_actor != NULL)  Window::_MainWorld->_SceneManager->_ADDManager->Add_PointLight(WindowUI::cur_SelectObject_List[0]->_actor); }
			if (ImGui::MenuItem("Add TestComponent")) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Add BoxCollision"))
			{
				if (WindowUI::cur_SelectObject_List[0] != NULL && WindowUI::cur_SelectObject_List[0]->_actor != NULL)
					Window::_MainWorld->_SceneManager->_ADDManager->Add_BoxCollision(WindowUI::cur_SelectObject_List[0]->_actor);
			}
			if (ImGui::MenuItem("Add BoxCollision2D")) {}
			if (ImGui::MenuItem("Add SphereCollision")) {}
			if (ImGui::MenuItem("Add SphereCollision2D")) {}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("DEBUG_setting"))
		{
			ImGui::MenuItem("All Element", "", &WindowUI::All_UIElement);
			ImGui::MenuItem("Render shadow", "", &Window::_MainWorld->_RenderShadow);
			ImGui::MenuItem("Simple Overlay", "", &WindowUI::show_simple_overlay);
			ImGui::MenuItem("PlayMode", "", &Window::_MainWorld->_PlayMode);

			const char* items[] = { "Basic","Color","Light","Shadow" };
			static int item_current = 0;
			if (ImGui::Combo("", &item_current, items, IM_ARRAYSIZE(items)))
			{
				Window::_MainWorld->_SceneManager->_DebugRenderType = static_cast<DebugRenderType>(item_current);
				Window::_MainWorld->_SceneManager->NeedInitedShader = true;
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Example"))
		{
			for (int i = 0; i < 10; i++)
			{
				if (ImGui::MenuItem(std::to_string(i).c_str())) { Window::_MainWorld->_SceneManager->OpenFile(i); }
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}
static void Menu_File()
{
	ImGui::MenuItem("(dummy menu)", NULL, false, false);
	if (ImGui::MenuItem("New")) { Window::_MainWorld->_SceneManager->NewScene(); }
	if (ImGui::MenuItem("Open", "Ctrl+O")) { Window::_MainWorld->_SceneManager->OpenFile(); }

	if (ImGui::MenuItem("Save", "Ctrl+S")) { Window::_MainWorld->_SceneManager->SaveFile(); }
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

			ImGui::Text(Window::_MainWorld->_SceneManager->_FilePAth.c_str());
			ImGui::Text(std::to_string(glm::length(Window::_MainWorld->_editorCamera->transform.position)).c_str());
		}
		ImGui::End();





	}
}


