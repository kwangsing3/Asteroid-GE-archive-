#include "Cube.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif // !STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Window.h>
#include <SceneManager.h>


#include "GraphicEngine/imgui.h"
#include "GraphicEngine/imgui_impl_glfw.h"
#include "GraphicEngine/imgui_impl_opengl3.h"
unsigned int VBO,VAO;
//unsigned int _Texture;

float Cubevertices[] = {
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
int _index = 1;

void Cube::CreateCube()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Cubevertices), Cubevertices, GL_STATIC_DRAW);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	_Texture = LoadTexture("Texture\\container2.png");
	
	SceneManager::vec_ShaderProgram[_index].use();
	SceneManager::vec_ShaderProgram[_index].setInt("texture1", 0);
}
void Cube::Draw()
{

	SceneManager::vec_ShaderProgram[_index].use();
	SceneManager::vec_ShaderProgram[_index].setVec3("viewPos", Window::_editorCamera.transform.position);
	SceneManager::vec_ShaderProgram[_index].setFloat("material.shininess", 32.0f);
	
	for (int i = 0; i <8; i++)
	{
		if (i+1 > SceneManager::vec_DirectionlLight.size())
		{
			SceneManager::vec_ShaderProgram[_index].setVec3("dirLight[" + std::to_string(i) + "].direction", glm::vec3(0,0,0));
			SceneManager::vec_ShaderProgram[_index].setVec3("dirLight[" + std::to_string(i) + "].ambient", glm::vec3(0, 0, 0));
			SceneManager::vec_ShaderProgram[_index].setVec3("dirLight[" + std::to_string(i) + "].diffuse", glm::vec3(0, 0, 0));
			SceneManager::vec_ShaderProgram[_index].setVec3("dirLight[" + std::to_string(i) + "].specular", glm::vec3(0, 0, 0));
			continue;
		}
		SceneManager::vec_ShaderProgram[_index].setVec3("dirLight[" + std::to_string(i) + "].direction", SceneManager::vec_DirectionlLight[i]->transform->rotation);
		SceneManager::vec_ShaderProgram[_index].setVec3("dirLight[" + std::to_string(i) + "].ambient", SceneManager::vec_DirectionlLight[i]->Ambient);
		SceneManager::vec_ShaderProgram[_index].setVec3("dirLight[" + std::to_string(i) + "].diffuse", SceneManager::vec_DirectionlLight[i]->Diffuse);
		SceneManager::vec_ShaderProgram[_index].setVec3("dirLight[" + std::to_string(i) + "].specular", SceneManager::vec_DirectionlLight[i]->Specular);
	}
	for (int i = 0; i <8; i++)
	{
		if (i + 1 > SceneManager::vec_PointLight.size())
		{
			SceneManager::vec_ShaderProgram[_index].setVec3("pointLights[" + std::to_string(i) + "].position", glm::vec3(0, 0, 0));
			SceneManager::vec_ShaderProgram[_index].setVec3("pointLights[" + std::to_string(i) + "].ambient", glm::vec3(0, 0, 0));
			SceneManager::vec_ShaderProgram[_index].setVec3("pointLights[" + std::to_string(i) + "].diffuse", glm::vec3(0, 0, 0));
			SceneManager::vec_ShaderProgram[_index].setVec3("pointLights[" + std::to_string(i) + "].specular", glm::vec3(0, 0, 0));
			SceneManager::vec_ShaderProgram[_index].setFloat("pointLights[" + std::to_string(i) + "].constant", 0);
			SceneManager::vec_ShaderProgram[_index].setFloat("pointLights[" + std::to_string(i) + "].linear", 0);
			SceneManager::vec_ShaderProgram[_index].setFloat("pointLights[" + std::to_string(i) + "].quadratic",0);
			continue;
		}
		SceneManager::vec_ShaderProgram[_index].setVec3("pointLights[" + std::to_string(i) + "].position", SceneManager::vec_PointLight[i]->transform->position);
		SceneManager::vec_ShaderProgram[_index].setVec3("pointLights[" + std::to_string(i) + "].ambient", SceneManager::vec_PointLight[i]->Ambient);
		SceneManager::vec_ShaderProgram[_index].setVec3("pointLights[" + std::to_string(i) + "].diffuse", SceneManager::vec_PointLight[i]->Diffuse);
		SceneManager::vec_ShaderProgram[_index].setVec3("pointLights[" + std::to_string(i) + "].specular", SceneManager::vec_PointLight[i]->Specular);
		SceneManager::vec_ShaderProgram[_index].setFloat("pointLights[" + std::to_string(i) + "].constant", SceneManager::vec_PointLight[i]->Constant);
		SceneManager::vec_ShaderProgram[_index].setFloat("pointLights[" + std::to_string(i) + "].linear", SceneManager::vec_PointLight[i]->linear);
		SceneManager::vec_ShaderProgram[_index].setFloat("pointLights[" + std::to_string(i) + "].quadratic", SceneManager::vec_PointLight[i]->quadratic);
	}
	
	// spotLight
	/*SceneManager::vec_ShaderProgram[_index].setVec3("spotLight.position", Window::_editorCamera.transform.position);
	SceneManager::vec_ShaderProgram[_index].setVec3("spotLight.direction", Window::_editorCamera.Front);
	SceneManager::vec_ShaderProgram[_index].setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
	SceneManager::vec_ShaderProgram[_index].setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
	SceneManager::vec_ShaderProgram[_index].setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
	SceneManager::vec_ShaderProgram[_index].setFloat("spotLight.constant", 1.0f);
	SceneManager::vec_ShaderProgram[_index].setFloat("spotLight.linear", 0.09);
	SceneManager::vec_ShaderProgram[_index].setFloat("spotLight.quadratic", 0.032);
	SceneManager::vec_ShaderProgram[_index].setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	SceneManager::vec_ShaderProgram[_index].setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));*/
	glBindVertexArray(VAO);

	// pass projection matrix to shader (note that in this case it could change every frame)  // 現在在Camera.h上有Projection 要記得改 
	glm::mat4 projection = glm::perspective(glm::radians(Window::_editorCamera.Zoom), (float)Window::WINDOW_WIDTH / (float)Window::WINDOW_HEIGHT, 0.1f, 100.0f);
	SceneManager::vec_ShaderProgram[_index].setMat4("projection", projection);

	// camera/view transformation
	glm::mat4 view = Window::_editorCamera.GetViewMatrix();
	SceneManager::vec_ShaderProgram[_index].setMat4("view", view);
	
	glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first	
	model = glm::translate(model, glm::vec3(this->transform->position.x, this->transform->position.y, this->transform->position.z));
	float angle[3] = { this->transform->rotation.x ,this->transform->rotation.y ,this->transform->rotation.z };
	model = glm::rotate(model, glm::radians(angle[0]), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(angle[1]), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(angle[2]), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(this->transform->scale.x, this->transform->scale.y, this->transform->scale.z));
	SceneManager::vec_ShaderProgram[_index].setMat4("model", model);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _Texture);



	glDrawArrays(GL_TRIANGLES, 0, 36);
	//printf("draw from cube");
	glBindTexture(GL_TEXTURE_2D, 0);

	

}





unsigned int Cube::LoadTexture(const char* path)
{
	unsigned int  _texture;
	glGenTextures(1, &_texture);
	glBindTexture(GL_TEXTURE_2D, _texture);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);

//	glBindTexture(GL_TEXTURE_2D, 0);
	// load image, create texture and generate mipmaps
	unsigned char * data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture: " << (char)_texture << std::endl;
	}
	stbi_image_free(data);

	return _texture;
}