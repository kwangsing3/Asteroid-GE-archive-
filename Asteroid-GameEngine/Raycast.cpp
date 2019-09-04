#include "Raycast.h"

#include <Window.h>
#include <GraphicEngine/imgui.h>

glm::vec3 Raycast::MousePosInWorld = glm::vec3{ 0,0,0 };
float Raycast::X_pos = 0;
float Raycast::Y_pos = 0;



glm::vec3 Raycast::GetWorldPosition(float alpha)
{
	//ImGuiIO& io = ImGui::GetIO();
	return glm::unProject(glm::vec3(Raycast::X_pos, Window::_Height - 1.0f - Raycast::Y_pos, alpha), Window::_editorCamera.GetViewMatrix(), Window::_editorCamera.Projection, glm::vec4(0.0, 0.0, Window::_Width, Window::_Height));
	// 看來GLFW自帶的函數G輸入的位置Y數值會稍微下面一點
	//應該是對的                                    ? Z:1;                                           ↑只是要知道這個的值應該是多少
}



glm::vec3 Raycast::GetScreenPosition(glm::vec3 _pos)
{
	return glm::project(_pos, Window::_editorCamera.GetViewMatrix(), Window::_editorCamera.Projection, glm::vec4(0, 0, Window::_Width, Window::_Height));
}


