#include "Cube.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif // !STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Window.h>
#include <Shader_List.h>


#include "GraphicEngine/imgui.h"
#include "GraphicEngine/imgui_impl_glfw.h"
#include "GraphicEngine/imgui_impl_opengl3.h"
unsigned int VBO,VAO;
//unsigned int _Texture;

float Cubevertices[] = {
	// Back face
  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
   0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
   0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
   0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
  // Front face
  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
   0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
   0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
   0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
  -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
  // Left face
  -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
  -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
  -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
  // Right face
   0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
   0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
   0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
   0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
   0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
   0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
  // Bottom face
  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
   0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
   0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
   0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
  // Top face
  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
   0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
   0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
   0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
  -0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left        
};


void Cube::CreateCube()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Cubevertices), Cubevertices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	_Texture = LoadTexture("Texture\\container2.png");
	
	Shader_List::vec_ShaderProgram[0].use();
	Shader_List::vec_ShaderProgram[0].setInt("texture1", 0);
}

void Cube::Draw()
{
	glBindVertexArray(VAO);
	Shader_List::vec_ShaderProgram[0].use();
	// pass projection matrix to shader (note that in this case it could change every frame)
	glm::mat4 projection = glm::perspective(glm::radians(Window::_editorCamera.Zoom), (float)Window::WINDOW_WIDTH / (float)Window::WINDOW_HEIGHT, 0.1f, 100.0f);
	Shader_List::vec_ShaderProgram[0].setMat4("projection", projection);

	// camera/view transformation
	glm::mat4 view = Window::_editorCamera.GetViewMatrix();
	Shader_List::vec_ShaderProgram[0].setMat4("view", view);

	glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first	
	model = glm::translate(model, glm::vec3(0, 0, 0));
	Shader_List::vec_ShaderProgram[0].setMat4("model", model);

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