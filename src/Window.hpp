#ifndef WINDOW_H
#define WINDOW_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <pugixml.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <GraphicEngine/imgui.h>
#include <World.hpp>
#include <Raycast.hpp>
#include <AGE_FileBrowser.hpp>

extern unsigned int Window_Width;
extern unsigned int Window_Height;
class Actor;
struct SelectObject
{
	Actor* _actor;
	bool Is_selected = false;
	bool Is_renaming = false;
	SelectObject(Actor* _ac){_actor = _ac;}
};



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

	AGE_FileBrowser* _Filebrowser;
	WindowUI() { _Filebrowser = new AGE_FileBrowser(); }
	static void Updated_WindowEvent(bool* p_open, unsigned int* width, unsigned int* height);

};
class World;
class Window
{
protected:
	void initialized();
public:
	static GLFWwindow* MainGLFWwindow;
	static int progect_I_am_focus;
	static std::vector<int> vec_ID;

	static WindowUI* _Main_UI;
	static World* _MainWorld;
	static float lastX;
	static float lastY;
	static bool WindowShouldClose;
	static bool isClickingPivot;
	Window() { initialized(); }


	static void processInput(GLFWwindow* window)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			WindowShouldClose = true;

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
				_MainWorld->_editorCamera->ProcessKeyboard(FORWARD, _Main_UI->deltaTime);
			else
				if (_MainWorld->_piv != NULL)_MainWorld->_piv->SwitchDragMode(0);
		}

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			_MainWorld->_editorCamera->ProcessKeyboard(BACKWARD, _Main_UI->deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			_MainWorld->_editorCamera->ProcessKeyboard(LEFT, _Main_UI->deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			_MainWorld->_editorCamera->ProcessKeyboard(RIGHT, _Main_UI->deltaTime);
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		{
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
				_MainWorld->_editorCamera->ProcessKeyboard(UP, _Main_UI->deltaTime);
			else
				if (_MainWorld->_piv != NULL)_MainWorld->_piv->SwitchDragMode(1);
		}
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			_MainWorld->_editorCamera->ProcessKeyboard(DOWN, _Main_UI->deltaTime);
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
				WindowUI::CopyEvent();
			if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
				WindowUI::PasteEvent();
		}
		
	}
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}
	static void mouse_move_callback(GLFWwindow* window, double xpos, double ypos)
	{
		double xoffset = xpos - lastX;
		double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
		// Pivot Drag Function
		if (isClickingPivot && _MainWorld->_piv != NULL)
		{
			if (_MainWorld->_piv->Drag[0])
			{
				_MainWorld->_SceneManager->NeedInitedDraw = true;
				if (_MainWorld->_piv->_DragMode[0])
					_MainWorld->_piv->Translate(glm::vec3(_MainWorld->_piv->_actor->transform->position.x + xoffset / 200, _MainWorld->_piv->_actor->transform->position.y, _MainWorld->_piv->_actor->transform->position.z));
				else if (_MainWorld->_piv->_DragMode[1])
					_MainWorld->_piv->Rotate(glm::vec3(_MainWorld->_piv->_actor->transform->rotation.x + xoffset / 20, _MainWorld->_piv->_actor->transform->rotation.y, _MainWorld->_piv->_actor->transform->rotation.z));
				else if (_MainWorld->_piv->_DragMode[2])
					_MainWorld->_piv->Scale(glm::vec3(_MainWorld->_piv->_actor->transform->scale.x + xoffset / 200, _MainWorld->_piv->_actor->transform->scale.y, _MainWorld->_piv->_actor->transform->scale.z));
			}
			else if (_MainWorld->_piv->Drag[1])
			{
				_MainWorld->_SceneManager->NeedInitedDraw = true;
				if (_MainWorld->_piv->_DragMode[0])
					_MainWorld->_piv->Translate(glm::vec3(_MainWorld->_piv->_actor->transform->position.x, _MainWorld->_piv->_actor->transform->position.y + yoffset / 200, _MainWorld->_piv->_actor->transform->position.z));
				else if (_MainWorld->_piv->_DragMode[1])
					_MainWorld->_piv->Rotate(glm::vec3(_MainWorld->_piv->_actor->transform->rotation.x, _MainWorld->_piv->_actor->transform->rotation.y + yoffset / 20, _MainWorld->_piv->_actor->transform->rotation.z));
				else if (_MainWorld->_piv->_DragMode[2])
					_MainWorld->_piv->Scale(glm::vec3(_MainWorld->_piv->_actor->transform->scale.x, _MainWorld->_piv->_actor->transform->scale.y + yoffset / 200, _MainWorld->_piv->_actor->transform->scale.z));
			}

			else if (_MainWorld->_piv->Drag[2])
			{
				_MainWorld->_SceneManager->NeedInitedDraw = true;
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
			_MainWorld->_editorCamera->ProcessMouseMovement(xoffset, yoffset);
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
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
					if (RayCallback.m_collisionObject->_ActorInBullet != NULL && RayCallback.m_collisionObject->_ActorInBullet->transform->name == (char*)"Pivot")  //½T©w¬O§_À»¤¤Pivot
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
						WindowUI::SelectThisActor(RayCallback.m_collisionObject->_ActorInBullet);
						
					
				}
				else if (!ImGui::IsAnyWindowFocused())
				{
					WindowUI::SelectThisActor(NULL);
				}
			}
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
			isClickingPivot = false;
	}
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		if (!ImGui::IsAnyWindowFocused())
			_MainWorld->_editorCamera->ProcessMouseScroll(yoffset);
	}
};



#endif // !WINDOW_H
