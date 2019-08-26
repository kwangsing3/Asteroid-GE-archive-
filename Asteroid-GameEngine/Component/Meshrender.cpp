#include <Component/Meshrender.h>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif // !STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <Window.h>
#include <SceneManager.h>


#include "GraphicEngine/imgui.h"
#include "GraphicEngine/imgui_impl_glfw.h"
#include "GraphicEngine/imgui_impl_opengl3.h"


float CubeVertices[] = {
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
float PlaneVertices[] = {
	// positions          // normals           // texture coords
			-0.5f, -0.5f, -0.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
			 0.5f,  -0.5f, -0.0f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
			 0.5f,  0.5f, -0.0f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
			 0.5f,  0.5f, -0.0f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
			-0.5f,  0.5f, -0.0f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
			-0.5f, -0.5f, -0.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
};

void Meshrender::Draw()
{
	SceneManager::vec_ShaderProgram[_index].use();
	SceneManager::vec_ShaderProgram[_index].setVec3("viewPos", Window::_editorCamera.transform.position);
	SceneManager::vec_ShaderProgram[_index].setFloat("material.shininess", 32.0f);
	//Directional Light
	for (int i = 0; i < 8; i++)
	{
		if (i + 1 > SceneManager::vec_DirectionlLight.size())
		{
			SceneManager::vec_ShaderProgram[_index].setVec3("dirLight[" + std::to_string(i) + "].direction", glm::vec3(0, 0, 0));
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
	//Point Light
	for (int i = 0; i < 8; i++)
	{
		if (i + 1 > SceneManager::vec_PointLight.size())
		{
			SceneManager::vec_ShaderProgram[_index].setVec3("pointLights[" + std::to_string(i) + "].position", glm::vec3(0, 0, 0));
			SceneManager::vec_ShaderProgram[_index].setVec3("pointLights[" + std::to_string(i) + "].ambient", glm::vec3(0, 0, 0));
			SceneManager::vec_ShaderProgram[_index].setVec3("pointLights[" + std::to_string(i) + "].diffuse", glm::vec3(0, 0, 0));
			SceneManager::vec_ShaderProgram[_index].setVec3("pointLights[" + std::to_string(i) + "].specular", glm::vec3(0, 0, 0));
			SceneManager::vec_ShaderProgram[_index].setFloat("pointLights[" + std::to_string(i) + "].constant", 0);
			SceneManager::vec_ShaderProgram[_index].setFloat("pointLights[" + std::to_string(i) + "].linear", 0);
			SceneManager::vec_ShaderProgram[_index].setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0);
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
	// Local Rotate
	/*
	model = glm::rotate(model, glm::radians(angle[0]), glm::vec3(1, 0, 0));
	model = glm::rotate(model, -glm::radians(angle[1]), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(angle[2]), glm::vec3(0, 0, 1));
	*/
	
	//Global Rotate
	model = glm::rotate(model, -glm::radians(angle[1]), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(angle[0]), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(angle[2]), glm::vec3(0, 0, 1));
	


	model = glm::scale(model, glm::vec3(this->transform->scale.x, this->transform->scale.y, this->transform->scale.z));
	SceneManager::vec_ShaderProgram[_index].setMat4("model", model);

	SceneManager::vec_ShaderProgram[_index].setVec3("Color", this->VertexColor.x, this->VertexColor.y, this->VertexColor.z);
	if (_index == 1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->Texture);
	}
	glDrawArrays(GL_TRIANGLES, 0, this->_mode == Mode_3D?36:6);
	glBindTexture(GL_TEXTURE_2D, 0);

	//Debug Popline
	if (Window::DeBug_Mode &&false/*直接指定*/)
	{
		for (int i = 0; i < Vectices_Debug.size(); i++)
		{
			//縮放
			//Worldvectices_Debug[i] = Vectices_Debug[i] * this->transform->scale;

			//旋轉
			Worldvectices_Debug[i] = model * glm::vec4(Vectices_Debug[i], 1);

			//Worldvectices_Debug[i] = glm::vec3(Worldvectices_Debug[i].x + this->transform->position.x, Worldvectices_Debug[i].y + this->transform->position.y, Worldvectices_Debug[i].z + this->transform->position.z);
			//位移	
		}

		//UpdateSpaceVertices
		for (int i = 0; i < Vectices_Debug.size(); i++)
		{
			glm::vec4 _sp = glm::vec4(Worldvectices_Debug[i], 0)*model*view;// *projection;
			Spacevectices_Debug[i] = glm::vec3(_sp.x, _sp.y, 0) / _sp.w;
		}
		ImGuiIO& io = ImGui::GetIO();

		//model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f));

		for (int i = 0; i < Spacevectices_Debug.size(); i++)
		{
			static bool _ts = true;
			_ts = true;
			glm::vec3 windowSpacePos = glm::project(Worldvectices_Debug[i], Window::_editorCamera.GetViewMatrix(), projection, glm::vec4(0, 0, Window::_Width, Window::_Height));
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
				ImGui::Text("SPosition: (%.1f,%.1f)", Worldvectices_Debug[i].x, Worldvectices_Debug[i].y);
				ImGui::End();
			}
		}
	}
}

void Meshrender::CreateCube(RenderMode _m)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	if (_m == RMode3D)
	{
		glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVertices), CubeVertices, GL_STATIC_DRAW);
	}
	else
	{
		glBufferData(GL_ARRAY_BUFFER, sizeof(PlaneVertices), PlaneVertices, GL_STATIC_DRAW);
	}

	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	Texture = LoadTexture("Texture\\White.png");
	SceneManager::vec_ShaderProgram[_index].use();
	SceneManager::vec_ShaderProgram[_index].setInt("texture1", 0);
	//Debug Mode


	if (_m == RMode3D)
	{
		for (int i = 0; i < sizeof(CubeVertices) / sizeof(CubeVertices[0]); i = i + 8)
		{
			Vectices_Debug.push_back(glm::vec3(CubeVertices[i], CubeVertices[i + 1], CubeVertices[i + 2]));
		}
	}
	else
	{
		for (int i = 0; i < sizeof(PlaneVertices) / sizeof(PlaneVertices[0]); i = i + 8)
		{
			Vectices_Debug.push_back(glm::vec3(PlaneVertices[i], PlaneVertices[i + 1], PlaneVertices[i + 2]));
		}
	}

	Worldvectices_Debug = Spacevectices_Debug = Vectices_Debug;
}



unsigned int Meshrender::LoadTexture(const char* path)
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

//  --------------------------------- Mouse Collision --------------------------------- 

#include <World.h>
void Meshrender::CreateMouseCollision()
{
	btVector3 localInertia(0, 0, 0);
	//create a dynamic rigidbody
	btCollisionShape* colShape = new btBoxShape(btVector3(transform->scale.x, transform->scale.y, transform->scale.z) / 2);
	btScalar mass(0);
	World::collisionShapes.push_back(colShape);
	/// Create Dynamic Objects
	btTransform startTransform;
	startTransform.setIdentity();
	btQuaternion quat;
	quat.setEuler(-glm::radians(transform->rotation.y), glm::radians(transform->rotation.x), glm::radians(transform->rotation.z));//or quat.setEulerZYX depending on the ordering you want
	startTransform.setRotation(quat);
	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);
	if (isDynamic)
		colShape->calculateLocalInertia(mass, localInertia);
	startTransform.setOrigin(btVector3(transform->position.x, transform->position.y, transform->position.z));
	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
	
	body = new btRigidBody(rbInfo);
	body->setCenterOfMassTransform(startTransform);

	World::dynamicsWorld->addRigidBody(body);
	World::dynamicsWorld->updateSingleAabb(body);
}
void Meshrender::ResetDynamic()
{
	World::dynamicsWorld->removeRigidBody(this->body);
	Meshrender::CreateMouseCollision();
}
