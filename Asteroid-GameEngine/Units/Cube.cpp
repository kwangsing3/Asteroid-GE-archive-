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
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

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
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
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
glm::vec3 pointLightPositions[] = {
	   glm::vec3(0.0f,  0.0f, 3.0f)
};
void Cube::Draw()
{
	//SceneManager::vec_ShaderProgram[_index].use();
	//SceneManager::vec_ShaderProgram[_index].setInt("NR_POINT_LIGHTS", SceneManager::vec_DirectionlLight.size());
	// be sure to activate shader when setting uniforms/drawing objects
	
	SceneManager::vec_ShaderProgram[_index].use();
	SceneManager::vec_ShaderProgram[_index].setVec3("viewPos", Window::_editorCamera.transform.position);
	SceneManager::vec_ShaderProgram[_index].setFloat("material.shininess", 32.0f);
/*
	// directional light
	SceneManager::vec_ShaderProgram[_index].setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	SceneManager::vec_ShaderProgram[_index].setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
	SceneManager::vec_ShaderProgram[_index].setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	SceneManager::vec_ShaderProgram[_index].setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

	
	for (int i = 0; i < SceneManager::vec_DirectionlLight.size(); i++)
	{
		
	}*/
	
	for (int i = 0; i < SceneManager::vec_DirectionlLight.size(); i++)
	{
		SceneManager::vec_ShaderProgram[_index].setVec3("dirLight[" + std::to_string(i) + "].direction", -0.2f, -1.0f, -0.3f);
		SceneManager::vec_ShaderProgram[_index].setVec3("dirLight[" + std::to_string(i) + "].ambient", 0.05f, 0.05f, 0.05f);
		SceneManager::vec_ShaderProgram[_index].setVec3("dirLight[" + std::to_string(i) + "].diffuse", 0.4f, 0.4f, 0.4f);
		SceneManager::vec_ShaderProgram[_index].setVec3("dirLight[" + std::to_string(i) + "].specular", 0.5f, 0.5f, 0.5f);
	}
	
	for (int i = 0; i < SceneManager::vec_PointLight.size(); i++)
	{
		SceneManager::vec_ShaderProgram[_index].setVec3("pointLights[" + std::to_string(i) + "].position", SceneManager::vec_PointLight[i]->transform.position);
		SceneManager::vec_ShaderProgram[_index].setVec3("pointLights[" + std::to_string(i) + "].ambient", 0.05f, 0.05f, 0.05f);
		SceneManager::vec_ShaderProgram[_index].setVec3("pointLights[" + std::to_string(i) + "].diffuse", 0.8f, 0.8f, 0.8f);
		SceneManager::vec_ShaderProgram[_index].setVec3("pointLights[" + std::to_string(i) + "].specular", 1.0f, 1.0f, 1.0f);
		SceneManager::vec_ShaderProgram[_index].setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0f);
		SceneManager::vec_ShaderProgram[_index].setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09);
		SceneManager::vec_ShaderProgram[_index].setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.032);
	}
	// point light 1
	/*
	// point light 2
	SceneManager::vec_ShaderProgram[_index].setVec3("pointLights[1].position", pointLightPositions[0]);
	SceneManager::vec_ShaderProgram[_index].setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
	SceneManager::vec_ShaderProgram[_index].setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
	SceneManager::vec_ShaderProgram[_index].setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
	SceneManager::vec_ShaderProgram[_index].setFloat("pointLights[1].constant", 1.0f);
	SceneManager::vec_ShaderProgram[_index].setFloat("pointLights[1].linear", 0.09);
	SceneManager::vec_ShaderProgram[_index].setFloat("pointLights[1].quadratic", 0.032);
	// point light 3
	SceneManager::vec_ShaderProgram[_index].setVec3("pointLights[2].position", pointLightPositions[0]);
	SceneManager::vec_ShaderProgram[_index].setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
	SceneManager::vec_ShaderProgram[_index].setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
	SceneManager::vec_ShaderProgram[_index].setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
	SceneManager::vec_ShaderProgram[_index].setFloat("pointLights[2].constant", 1.0f);
	SceneManager::vec_ShaderProgram[_index].setFloat("pointLights[2].linear", 0.09);
	SceneManager::vec_ShaderProgram[_index].setFloat("pointLights[2].quadratic", 0.032);
	// point light 4
	SceneManager::vec_ShaderProgram[_index].setVec3("pointLights[3].position", pointLightPositions[0]);
	SceneManager::vec_ShaderProgram[_index].setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
	SceneManager::vec_ShaderProgram[_index].setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
	SceneManager::vec_ShaderProgram[_index].setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
	SceneManager::vec_ShaderProgram[_index].setFloat("pointLights[3].constant", 1.0f);
	SceneManager::vec_ShaderProgram[_index].setFloat("pointLights[3].linear", 0.09);
	SceneManager::vec_ShaderProgram[_index].setFloat("pointLights[3].quadratic", 0.032);*/
	// spotLight
	SceneManager::vec_ShaderProgram[_index].setVec3("spotLight.position", Window::_editorCamera.transform.position);
	SceneManager::vec_ShaderProgram[_index].setVec3("spotLight.direction", Window::_editorCamera.Front);
	SceneManager::vec_ShaderProgram[_index].setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
	SceneManager::vec_ShaderProgram[_index].setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
	SceneManager::vec_ShaderProgram[_index].setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
	SceneManager::vec_ShaderProgram[_index].setFloat("spotLight.constant", 1.0f);
	SceneManager::vec_ShaderProgram[_index].setFloat("spotLight.linear", 0.09);
	SceneManager::vec_ShaderProgram[_index].setFloat("spotLight.quadratic", 0.032);
	SceneManager::vec_ShaderProgram[_index].setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	SceneManager::vec_ShaderProgram[_index].setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

	
	

	glBindVertexArray(VAO);
	
	// pass projection matrix to shader (note that in this case it could change every frame)
	glm::mat4 projection = glm::perspective(glm::radians(Window::_editorCamera.Zoom), (float)Window::WINDOW_WIDTH / (float)Window::WINDOW_HEIGHT, 0.1f, 100.0f);
	SceneManager::vec_ShaderProgram[_index].setMat4("projection", projection);

	// camera/view transformation
	glm::mat4 view = Window::_editorCamera.GetViewMatrix();
	SceneManager::vec_ShaderProgram[_index].setMat4("view", view);

	glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first	
	model = glm::translate(model, glm::vec3(0, 0, 0));
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