#include "Raycast.h"

#include <Window.h>
#include <GraphicEngine/imgui.h>

glm::vec3 Raycast::hitposition = {-1,-1,-1};



glm::vec3 Raycast::GetWorldPosition()
{
	ImGuiIO& io = ImGui::GetIO();
	

	return glm::unProject(glm::vec3(io.MousePos.x, Window::WINDOW_HEIGHT - 1.0f - io.MousePos.y,0.0f), Window::_editorCamera.GetViewMatrix(), Window::_editorCamera.Projection, glm::vec4(0.0, 0.0, Window::WINDOW_WIDTH, Window::WINDOW_HEIGHT));

	//應該是對的                                    ? Z:1;                                        ↑只是要知道這個的值應該是多少

}



