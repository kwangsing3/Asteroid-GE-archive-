#ifndef WINDOW_H
#define WINDOW_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <pugixml.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <Raycast.hpp>
#include <Camera.hpp>
#include <World.hpp>
#include <GraphicEngine/imgui.h>

extern World* _MainWorld;
extern Camera _editorCamera;

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

extern unsigned int Window_Width;
extern unsigned int Window_Height;
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
	static void SelectThisObject(SelectObject* actor);
	static void ListInspectorCur(SelectObject* _sel);
	static void CopyEvent();
	static void PasteEvent();

	static bool show_simple_overlay;
	static bool All_UIElement;

	static float _FPS;
	static float UI_Left_X;
	static float UI_Left_Y;
	//static float UI_Right_X;
	static float UI_Right_X;

	static float deltaTime;
	static float lastFrame;
	static float previousTime;
	static int frameCount;
	static bool isClickingPivot;
	static float lastX;
	static float lastY;
	static bool WindowShouldClose;


	WindowUI()
	{
		UI_Left_X = 0;
		UI_Right_X = 0;

	}

	static void Updated_WindowEvent(bool* p_open, unsigned int* width, unsigned int* height);
	static void processInput(GLFWwindow* window)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			WindowShouldClose = true;

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
				_editorCamera.ProcessKeyboard(FORWARD, deltaTime);
			else
				if (_MainWorld->_piv != NULL)_MainWorld->_piv->SwitchDragMode(0);
		}

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			_editorCamera.ProcessKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			_editorCamera.ProcessKeyboard(LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			_editorCamera.ProcessKeyboard(RIGHT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		{
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
				_editorCamera.ProcessKeyboard(UP, deltaTime);
			else
				if (_MainWorld->_piv != NULL)_MainWorld->_piv->SwitchDragMode(1);
		}
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			_editorCamera.ProcessKeyboard(DOWN, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_DELETE) == GLFW_PRESS)
			std::cout << "DELETE KEY PRESS" << std::endl;//WindowUI::Deletecur_actor(WindowUI::cur_SelectObject);
		if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
			std::cout << "RENAME(F2) KEY PRESS" << std::endl;//WindowUI::Renamecur_actor(WindowUI::cur_SelectObject);
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		{
			if (_MainWorld->_piv != NULL)_MainWorld->_piv->SwitchDragMode(2);
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		{
			if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
				CopyEvent();
			if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
				PasteEvent();
		}

	}

	// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
	// ---------------------------------------------------------------------------------------------------------


	// glfw: whenever the window size changed (by OS or user resize) this callback function executes
	// ---------------------------------------------------------------------------------------------
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		// make sure the viewport matches the new window dimensions; note that width and
		// height will be significantly larger than specified on retina displays.
		glViewport(0, 0, width, height);
	}

	// glfw: whenever the mouse moves, this callback is called
	// -------------------------------------------------------
	static void mouse_move_callback(GLFWwindow* window, double xpos, double ypos)
	{
		double xoffset = xpos - lastX;
		double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
		// Pivot Drag Function
		if (isClickingPivot && _MainWorld->_piv != NULL)
		{
			if (_MainWorld->_piv->Drag[0])
			{
				_MainWorld->_SceneManager.NeedInitedDraw = true;
				if (_MainWorld->_piv->_DragMode[0])
					_MainWorld->_piv->Translate(glm::vec3(_MainWorld->_piv->_actor->transform->position.x + xoffset / 200, _MainWorld->_piv->_actor->transform->position.y, _MainWorld->_piv->_actor->transform->position.z));
				else if (_MainWorld->_piv->_DragMode[1])
					_MainWorld->_piv->Rotate(glm::vec3(_MainWorld->_piv->_actor->transform->rotation.x + xoffset / 20, _MainWorld->_piv->_actor->transform->rotation.y, _MainWorld->_piv->_actor->transform->rotation.z));
				else if (_MainWorld->_piv->_DragMode[2])
					_MainWorld->_piv->Scale(glm::vec3(_MainWorld->_piv->_actor->transform->scale.x + xoffset / 200, _MainWorld->_piv->_actor->transform->scale.y, _MainWorld->_piv->_actor->transform->scale.z));
			}
			else if (_MainWorld->_piv->Drag[1])
			{
				_MainWorld->_SceneManager.NeedInitedDraw = true;
				if (_MainWorld->_piv->_DragMode[0])
					_MainWorld->_piv->Translate(glm::vec3(_MainWorld->_piv->_actor->transform->position.x, _MainWorld->_piv->_actor->transform->position.y + yoffset / 200, _MainWorld->_piv->_actor->transform->position.z));
				else if (_MainWorld->_piv->_DragMode[1])
					_MainWorld->_piv->Rotate(glm::vec3(_MainWorld->_piv->_actor->transform->rotation.x, _MainWorld->_piv->_actor->transform->rotation.y + yoffset / 20, _MainWorld->_piv->_actor->transform->rotation.z));
				else if (_MainWorld->_piv->_DragMode[2])
					_MainWorld->_piv->Scale(glm::vec3(_MainWorld->_piv->_actor->transform->scale.x, _MainWorld->_piv->_actor->transform->scale.y + yoffset / 200, _MainWorld->_piv->_actor->transform->scale.z));
			}

			else if (_MainWorld->_piv->Drag[2])
			{
				_MainWorld->_SceneManager.NeedInitedDraw = true;
				if (_MainWorld->_piv->_DragMode[0])
					_MainWorld->_piv->Translate(glm::vec3(_MainWorld->_piv->_actor->transform->position.x, _MainWorld->_piv->_actor->transform->position.y, _MainWorld->_piv->_actor->transform->position.z + xoffset / 200));
				else if (_MainWorld->_piv->_DragMode[1])
					_MainWorld->_piv->Rotate(glm::vec3(_MainWorld->_piv->_actor->transform->rotation.x, _MainWorld->_piv->_actor->transform->rotation.y, _MainWorld->_piv->_actor->transform->rotation.z + xoffset / 20));
				else if (_MainWorld->_piv->_DragMode[2])
					_MainWorld->_piv->Scale(glm::vec3(_MainWorld->_piv->_actor->transform->scale.x, _MainWorld->_piv->_actor->transform->scale.y, _MainWorld->_piv->_actor->transform->scale.z + xoffset / 200));
			}


		}
		lastX = xpos;
		lastY = ypos;

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			_editorCamera.ProcessMouseMovement(xoffset, yoffset);
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}

		//記得拿掉

	}
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			if (lastX > WindowUI::UI_Left_X&& lastX<WindowUI::UI_Right_X && lastY>WindowUI::UI_Left_Y)
			{
				Raycast::SetMousePosition(lastX, lastY + Window_Height / 45);
				btCollisionWorld::ClosestRayResultCallback RayCallback(
					btVector3(Raycast::GetWorldPosition(0).x, Raycast::GetWorldPosition(0).y, Raycast::GetWorldPosition(0).z), btVector3(Raycast::GetWorldPosition(1).x, Raycast::GetWorldPosition(1).y, Raycast::GetWorldPosition(1).z)
				);
				RayCallback.m_collisionFilterMask = 1;
				RayCallback.m_collisionFilterGroup = 1;

				_MainWorld->m_dynamicsWorld->rayTest(
					btVector3(Raycast::GetWorldPosition(0).x, Raycast::GetWorldPosition(0).y, Raycast::GetWorldPosition(0).z), btVector3(Raycast::GetWorldPosition(1).x, Raycast::GetWorldPosition(1).y, Raycast::GetWorldPosition(1).z),
					RayCallback
				);

				if (RayCallback.hasHit())
				{
					//_cur_World->dynamicsWorld.
					if (RayCallback.m_collisionObject->_ActorInBullet != NULL && RayCallback.m_collisionObject->_ActorInBullet->transform->name == (char*)"Pivot")  //確定是否擊中Pivot
					{
						_MainWorld->_piv->_actor = RayCallback.m_collisionObject->_ActorInBullet;
						_MainWorld->_piv->Drag[0] = false; _MainWorld->_piv->Drag[1] = false; _MainWorld->_piv->Drag[2] = false;
						if (RayCallback.m_collisionObject == _MainWorld->_piv->body[0])
							_MainWorld->_piv->Drag[0] = true;
						else if (RayCallback.m_collisionObject == _MainWorld->_piv->body[1])
							_MainWorld->_piv->Drag[1] = true;
						else if (RayCallback.m_collisionObject == _MainWorld->_piv->body[2])
							_MainWorld->_piv->Drag[2] = true;
						isClickingPivot = true;
					}
					else
					{
						WindowUI::SelectThisActor(RayCallback.m_collisionObject->_ActorInBullet);
					}
				}
				else if (!ImGui::IsAnyWindowFocused())
				{
					WindowUI::SelectThisActor(NULL);
				}
			}
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
		{
			isClickingPivot = false;
			//_piv->_actor = NULL;
		}
	}
	// glfw: whenever the mouse scroll wheel scrolls, this callback is called
	// ----------------------------------------------------------------------
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		if (!ImGui::IsAnyWindowFocused())
			_editorCamera.ProcessMouseScroll(yoffset);
	}


};

class Window
{
public:
	static GLFWwindow* MainGLFWwindow;
	bool isFullscreen;
	static int progect_I_am_focus;
	static bool DeBug_Mode;
	static std::vector<int> vec_ID;
	static WindowUI* _Main_UI;


	//static World* _Mainworld;

	Window()
	{
		
		_Main_UI = new WindowUI();
		//Xml_SettingImport();
		
		pugi::xml_document _doc;
		std::ifstream _XMLstream("GlobalSettings.xml");
		pugi::xml_parse_result result = _doc.load(_XMLstream);
		std::cout << "Load result: " << result.description() << std::endl;
		if (result != NULL)
		{
			this->_Main_UI->All_UIElement = _doc.child("GlobalSettings").child("ProjectSetting").child("All_UIElement").attribute("All_UIElement").as_bool();
			this->DeBug_Mode = _doc.child("GlobalSettings").child("WindowSetting").child("Game_Mode").attribute("Game_Mode").as_bool();
			this->progect_I_am_focus = _doc.child("GlobalSettings").child("ProjectSetting").child("Project").attribute("Project_Focus").as_int();
		}

		_XMLstream.close();
		
		GLFWmonitor* primary = glfwGetPrimaryMonitor();
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		Window_Width = mode->width;
		Window_Height = mode->height;
		MainGLFWwindow = glfwCreateWindow(Window_Width, Window_Height, "Asteroid-GameEngine", NULL, NULL);
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
		glfwSetFramebufferSizeCallback(MainGLFWwindow, (GLFWframebuffersizefun)WindowUI::framebuffer_size_callback);
		glfwSetCursorPosCallback(MainGLFWwindow, (GLFWcursorposfun)WindowUI::mouse_move_callback);
		glfwSetScrollCallback(MainGLFWwindow, (GLFWscrollfun)WindowUI::scroll_callback);
		glfwSetMouseButtonCallback(MainGLFWwindow, (GLFWmousebuttonfun)WindowUI::mouse_button_callback);
		// tell GLFW to capture our mouse
		glfwSetInputMode(MainGLFWwindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// glad: load all OpenGL function pointers
		// ---------------------------------------
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			//return;
		}
	}




};



#endif // !WINDOW_H
