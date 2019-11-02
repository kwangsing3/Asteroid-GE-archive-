#include <AGE_Mesh.h>


#include <Units/Camera.h>


#include <shader_m.h>
#include <SceneManager.h>

unsigned int diffuseNr = 1;
unsigned int specularNr = 1;
unsigned int normalNr = 1;
unsigned int heightNr = 1;
std::string number;
std::string name;
void Mesh::Draw(Shader* shader)
{
	//shader->use();
	// bind appropriate textures
	 diffuseNr = 1;
	 specularNr = 1;
	 normalNr = 1;
	 heightNr = 1;
	//***************************************
	// Bind Textures
	//***************************************
	for (unsigned int i = 0; i < this->_Textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE2 + i); // active proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		
		name = this->_Textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++); // transfer unsigned int to stream
		else if (name == "texture_normal")
			number = std::to_string(normalNr++); // transfer unsigned int to stream
		else if (name == "texture_height")
			number = std::to_string(heightNr++); // transfer unsigned int to stream
												 // now set the sampler to the correct texture unit
		glUniform1i(glGetUniformLocation(shader->ID, (name + number).c_str()), i);
		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, this->_Textures[i].id);
	}
	//***************************************
	// Shader Uniforms
	//***************************************


	shader->setBool("Has_Bone", Has_Bone);
	// draw mesh
	glBindVertexArray(VAO);
	//glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, 1);
	if (!indices.empty())
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	else
	{
		glDrawArrays(GL_LINES, 0, vertices.size());
	}
	glBindVertexArray(0);
	//glActiveTexture(GL_TEXTURE0);

	// always good practice to set everything back to defaults once configured.
	
}


