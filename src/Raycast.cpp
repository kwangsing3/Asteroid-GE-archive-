#include <Raycast.hpp>
#include <Camera.hpp>
#include <Window.hpp>


glm::vec3 Raycast::MousePosInWorld = glm::vec3{ 0,0,0 };
float Raycast::X_pos = 0;
float Raycast::Y_pos = 0;

extern unsigned int Window_Width;
extern unsigned int Window_Height;


glm::vec3 Raycast::GetWorldPosition(float alpha)
{
	//ImGuiIO& io = ImGui::GetIO();

	return glm::unProject(glm::vec3(Raycast::X_pos, Window_Height - 1.0f - Raycast::Y_pos, alpha), Window::_MainWorld->_editorCamera->GetViewMatrix(), Window::_MainWorld->_editorCamera->Projection, glm::vec4(0.0, 0.0, Window_Width, Window_Height));
	// 看來GLFW自帶的函數G輸入的位置Y數值會稍微下面一點
	//應該是對的                                    ? Z:1;                                           ↑只是要知道這個的值應該是多少
}



glm::vec3 Raycast::GetScreenPosition(glm::vec3 _pos)
{
	return glm::project(_pos, Window::_MainWorld->_editorCamera->GetViewMatrix(), Window::_MainWorld->_editorCamera->Projection, glm::vec4(0, 0, Window_Width, Window_Height));
}


