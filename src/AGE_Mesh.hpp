﻿#ifndef MESH_H
#define MESH_H

#include <glad/glad.h> // holds all OpenGL type declarations
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>


struct Vertex
{
	// position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	// texCoords
	glm::vec2 TexCoords;
	// tangent
	glm::vec3 Tangent;
	// bitangent
	glm::vec3 Bitangent;
};
struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};
class Shader;

class Mesh    //傳入已經分配好的數值來製作Mesh
{
private:

	unsigned int VBO=0, EBO=0;
	void setupMesh()                     // 依照普通Mesh來分配節點      如果是簡單的Debug是透過 Normal分配的位置來決定顏色    所以簡單的Debug物件只有使用 aPos、Normal
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		// load data into vertex buffers
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
		if (!indices.empty())
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		}
		


		// vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
		// vertex tangent
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
		// vertex bitangent
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
		glBindVertexArray(0);
	}
public:

	//***************************************
	// Model data
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> _Textures;
	bool Has_Bone = false;
	unsigned int VAO = 0;
	std::string _ModelPath;

	//***************************************
	// Shader Uniform
	glm::mat4 _Mat4model = glm::mat4(1.0f);
	//***************************************
	Mesh() {};
	Mesh(std::vector<Vertex> vert, std::vector<unsigned int> indi, std::vector<Texture> _Text)
	{
		this->vertices = vert;
		this->indices = indi;
		this->_Textures = _Text;
		setupMesh();
	}
	Mesh(std::vector<Vertex> vert)
	{
		this->vertices = vert;

		setupMesh();
	}
	virtual void Draw(Shader* shader);
	

};


	




#endif // !MESH_H
