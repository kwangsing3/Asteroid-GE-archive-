#include "Raycast.h"

#include <Window.h>
#include <GraphicEngine/imgui.h>

glm::vec3 Raycast::MousePosInWorld = glm::vec3{0,0,0};
float Raycast::X_pos = 0;
float Raycast::Y_pos = 0;



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
	glm::vec4 lRayStart_NDC(
		(Raycast::X_pos / Window::_Width - 0.5f) * 2.0f, // [0,1024] -> [-1,1]
		(-Raycast::Y_pos / Window::_Height - 0.5f) * 2.0f, // [0, 768] -> [-1,1]
		-1.0, // The near plane maps to Z=-1 in Normalized Device Coordinates
		1.0f
	);
	glm::vec4 lRayEnd_NDC(
		(Raycast::X_pos / Window::_Width - 0.5f) * 2.0f,
		(Raycast::Y_pos / Window::_Height - 0.5f) * 2.0f,
		0.0,
		1.0f
	);

	glm::mat4 M = glm::inverse(Window::_editorCamera.Projection * Window::_editorCamera.GetViewMatrix());
	glm::vec4 lRayStart_world = M * lRayStart_NDC; lRayStart_world/=lRayStart_world.w;
	glm::vec4 lRayEnd_world   = M * lRayEnd_NDC  ; lRayEnd_world  /=lRayEnd_world.w;

	glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
	//if(lRayDir_world.length!=0)
		//lRayDir_world = glm::normalize(lRayDir_world);

	return lRayStart_world;
}

