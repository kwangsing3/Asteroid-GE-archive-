#include <Collision/BoxCollision.h>
#include <SceneManager.h>
#include <Window.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

int _index = 0;
float Vertices[] = {
	// positions          // normals           // texture coords
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};


void BoxCollision::Draw()
{
	SceneManager::vec_ShaderProgram[_index].use();
	SceneManager::vec_ShaderProgram[_index].setVec3("viewPos", Window::_editorCamera.transform.position);
	SceneManager::vec_ShaderProgram[_index].setFloat("material.shininess", 32.0f);



	//Draw Popline
	glBindVertexArray(VAO);
	glm::mat4 projection = Window::_editorCamera.Projection;
	SceneManager::vec_ShaderProgram[_index].setMat4("projection", projection);
	// camera/view transformation
	glm::mat4 view = Window::_editorCamera.GetViewMatrix();
	SceneManager::vec_ShaderProgram[_index].setMat4("view", view);

	glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first	
	model = glm::translate(model, glm::vec3(this->transform->position.x, this->transform->position.y, this->transform->position.z));
	float angle[3] = { this->transform->rotation.x ,this->transform->rotation.y ,this->transform->rotation.z };
	model = glm::rotate(model, glm::radians(angle[0]), glm::vec3(1, 0, 0));
	model = glm::rotate(model, -glm::radians(angle[1]), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(angle[2]), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(this->transform->scale.x, this->transform->scale.y, this->transform->scale.z));
	SceneManager::vec_ShaderProgram[_index].setMat4("model", model);

	SceneManager::vec_ShaderProgram[_index].setVec3("Color", this->_Color.x, this->_Color.y, this->_Color.z);
	

	glDrawArrays(GL_TRIANGLES, 0,36);
	glBindTexture(GL_TEXTURE_2D, 0);

	//Debug Popline
	if (Window::DeBug_Mode)
	{
		for (int i = 0; i < Vectices_Debug.size(); i++)
		{
			//從相對座標轉成世界座標
			Vectices_Debug[i] = model * glm::vec4(Vectices_Debug[i], 1);
				
		}

		//UpdateSpaceVertices
		for (int i = 0; i < Vectices_Debug.size(); i++)
		{
			glm::vec4 _sp = glm::vec4(Vectices_Debug[i], 0)*model*view;// *projection;
			Vectices_Debug[i] = glm::vec3(_sp.x, _sp.y, 0) / _sp.w;
		}
		ImGuiIO& io = ImGui::GetIO();

		//model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f));

		for (int i = 0; i < Spacevectices_Debug.size(); i++)
		{
			static bool _ts = true;
			_ts = true;
			glm::vec3 windowSpacePos = glm::project(Vectices_Debug[i], Window::_editorCamera.GetViewMatrix(), projection, glm::vec4(0, 0, Window::_Width, Window::_Height));
			ImVec2 _nextwindowpos = ImVec2(windowSpacePos.x*Window::viewport_size.x / Window::_Width, (Window::_Height - windowSpacePos.y)*Window::viewport_size.y / Window::_Height);

			if ((_nextwindowpos.x > Window::viewport_pos.x + Window::viewport_size.x) || (_nextwindowpos.x < Window::viewport_pos.x) || (_nextwindowpos.y > Window::viewport_pos.y + Window::viewport_size.y) || (_nextwindowpos.y < Window::viewport_pos.y))
				continue;
			ImGui::SetNextWindowPos(_nextwindowpos, ImGuiCond_Always);


			char s[sizeof(&this->ID) + sizeof(&this->transform->name) + sizeof(&i)];
			//char n[sizeof(i)];
			//sprintf(n,"%d",i);
			sprintf(s, "%d%s%d", this->ID, this->transform->name, i);
			//strcat(s, this->transform->name);
			//strcat(s, n);
			ImGui::SetNextWindowBgAlpha(0.2f);
			if (ImGui::Begin(s, &_ts, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
			{
				//ImGui::Text("%d Position: (%.1f,%.1f)",s[0], (2- Spacevectices_Debug[i].x-1)/2*io.DisplaySize.x, (2 - Spacevectices_Debug[i].y) / 2 * io.DisplaySize.y);
				ImGui::Text("SPosition: (%.1f,%.1f)", Vectices_Debug[i].x, Vectices_Debug[i].y);
				ImGui::End();
			}
		}
	}






}

void BoxCollision::CreateBox()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices),Vertices, GL_STATIC_DRAW);
	
	

	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	SceneManager::vec_ShaderProgram[_index].use();
	SceneManager::vec_ShaderProgram[_index].setInt("texture1", 0);
	//Debug Mode


	

	for (int i = 0; i < sizeof(Vertices) / sizeof(Vertices[0]); i = i + 8)
	{
		Vectices_Debug.push_back(glm::vec3(Vertices[i], Vertices[i + 1], Vertices[i + 2]));
	}
	Worldvectices_Debug=Spacevectices_Debug=Vectices_Debug;
}
