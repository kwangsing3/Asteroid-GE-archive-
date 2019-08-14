﻿#include "Raycast.h"

#include <Window.h>
#include <GraphicEngine/imgui.h>

glm::vec3 Raycast::hitposition = {-1,-1,-1};



glm::vec3 Raycast::GetWorldPosition(float alpha)
{
	ImGuiIO& io = ImGui::GetIO();
	return glm::unProject(glm::vec3(io.MousePos.x, Window::_Height - 1.0f - io.MousePos.y, alpha), Window::_editorCamera.GetViewMatrix(), Window::_editorCamera.Projection, glm::vec4(0.0, 0.0, Window::_Width, Window::_Height));

	//應該是對的                                    ? Z:1;                                   ↑只是要知道這個的值應該是多少
}

glm::vec3 Raycast::GetScreenPosition(glm::vec3 _pos)
{
	return glm::project(_pos, Window::_editorCamera.GetViewMatrix(), Window::_editorCamera.Projection, glm::vec4(0, 0, Window::_Width, Window::_Height));
}

glm::vec3 Raycast::GetRaycastVector()
{
	return GetWorldPosition(0.0f)- GetWorldPosition(1.0f);
}

