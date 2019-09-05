#include <Component/Meshrender.h>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif // !STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <Window.h>
#include <shader_m.h>
#include <SceneManager.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "GraphicEngine/imgui.h"
#include "GraphicEngine/imgui_impl_glfw.h"
#include "GraphicEngine/imgui_impl_opengl3.h"


float CubeVertices[] = {
	// Back face
		  -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
		  0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
		  0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
		  0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,  // top-right
		  -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // bottom-left
		  -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,// top-left
		  // Front face
		  -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
		  0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
		  0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // top-right
		  0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
		  -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
		  -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom-left
		  // Left face
		  -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
		  -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-left
		  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-left
		  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
		  -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
		  -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
		  // Right face
		  0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
		  0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
		  0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right         
		  0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-right
		  0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // top-left
		  0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left     
		  // Bottom face
		  -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
		  0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-left
		  0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,// bottom-left
		  0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
		  -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-right
		  -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
		  // Top face
		  -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
		  0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
		  0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right     
		  0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
		  -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
		  -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f // bottom-left        
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

void Meshrender::Draw(Shader _shader)
{

	///Shader Setting
	{
		_shader.use();
		_shader.setVec3("viewPos", Window::_editorCamera.transform.position);
		_shader.setFloat("material.shininess", 32.0f);   // 先暫時關掉燈光   確認跟燈光沒關係
		
		//Directional Light
		for (int i = 0; i < 8; i++)
		{
			if (i + 1 > SceneManager::vec_DirectionlLight.size())
			{
				_shader.setVec3("dirLight[" + std::to_string(i) + "].direction", glm::vec3(0, 0, 0));
				_shader.setVec3("dirLight[" + std::to_string(i) + "].ambient", glm::vec3(0, 0, 0));
				_shader.setVec3("dirLight[" + std::to_string(i) + "].diffuse", glm::vec3(0, 0, 0));
				_shader.setVec3("dirLight[" + std::to_string(i) + "].specular", glm::vec3(0, 0, 0));
				continue;
			}
			_shader.setVec3("dirLight[" + std::to_string(i) + "].direction", SceneManager::vec_DirectionlLight[i]->_actor->transform->rotation);
			_shader.setVec3("dirLight[" + std::to_string(i) + "].ambient", SceneManager::vec_DirectionlLight[i]->Ambient);
			_shader.setVec3("dirLight[" + std::to_string(i) + "].diffuse", SceneManager::vec_DirectionlLight[i]->Diffuse);
			_shader.setVec3("dirLight[" + std::to_string(i) + "].specular", SceneManager::vec_DirectionlLight[i]->Specular);
		}
		//Point Light
		for (int i = 0; i < 8; i++)
		{
			if (i + 1 > SceneManager::vec_PointLight.size())
			{
				_shader.setVec3("pointLights[" + std::to_string(i) + "].position", glm::vec3(0, 0, 0));
				_shader.setVec3("pointLights[" + std::to_string(i) + "].ambient", glm::vec3(0, 0, 0));
				_shader.setVec3("pointLights[" + std::to_string(i) + "].diffuse", glm::vec3(0, 0, 0));
				_shader.setVec3("pointLights[" + std::to_string(i) + "].specular", glm::vec3(0, 0, 0));
				_shader.setFloat("pointLights[" + std::to_string(i) + "].constant", 0);
				_shader.setFloat("pointLights[" + std::to_string(i) + "].linear", 0);
				_shader.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0);
				continue;
			}
			_shader.setVec3("pointLights[" + std::to_string(i) + "].position", SceneManager::vec_PointLight[i]->_actor->transform->position);
			_shader.setVec3("pointLights[" + std::to_string(i) + "].ambient", SceneManager::vec_PointLight[i]->Ambient);
			_shader.setVec3("pointLights[" + std::to_string(i) + "].diffuse", SceneManager::vec_PointLight[i]->Diffuse);
			_shader.setVec3("pointLights[" + std::to_string(i) + "].specular", SceneManager::vec_PointLight[i]->Specular);
			_shader.setFloat("pointLights[" + std::to_string(i) + "].constant", SceneManager::vec_PointLight[i]->Constant);
			_shader.setFloat("pointLights[" + std::to_string(i) + "].linear", SceneManager::vec_PointLight[i]->linear);
			_shader.setFloat("pointLights[" + std::to_string(i) + "].quadratic", SceneManager::vec_PointLight[i]->quadratic);
			// spotLight
			/*_shader.setVec3("spotLight.position", Window::_editorCamera.transform.position);
			_shader.setVec3("spotLight.direction", Window::_editorCamera.Front);
			_shader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
			_shader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
			_shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
			_shader.setFloat("spotLight.constant", 1.0f);
			_shader.setFloat("spotLight.linear", 0.09);
			_shader.setFloat("spotLight.quadratic", 0.032);
			_shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
			_shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));*/
		}
		
		glm::mat4 projection = Window::_editorCamera.Projection;
		glm::mat4 view = Window::_editorCamera.GetViewMatrix();
		_shader.setMat4("projection", projection);
		_shader.setMat4("view", view);
		glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first	
		model = glm::translate(model, glm::vec3(this->_actor->transform->position.x, this->_actor->transform->position.y, this->_actor->transform->position.z));
		glm::quat MyQuaternion;
		glm::vec3 EulerAngles(glm::radians(this->_actor->transform->rotation.x), glm::radians(-this->_actor->transform->rotation.y), glm::radians(this->_actor->transform->rotation.z));
		MyQuaternion = glm::quat(EulerAngles);
		glm::mat4 RotationMatrix = glm::toMat4(MyQuaternion);
		model = model * RotationMatrix;
		model = glm::scale(model, glm::vec3(this->_actor->transform->scale.x, this->_actor->transform->scale.y, this->_actor->transform->scale.z));
		_shader.setMat4("model", model);
		_shader.setVec3("Color", this->VertexColor.x, this->VertexColor.y, this->VertexColor.z);

		///shadow
		glm::mat4 shadowProj, lightView;
		glm::mat4 lightSpaceMatrix;
		glm::vec3 lightPos = SceneManager::vec_DirectionlLight.size() > 0 ? SceneManager::vec_DirectionlLight[0]->_actor->transform->rotation : glm::vec3(0,0,0);

		/*
			紀錄一下  目前光影只會對第一個Directional Ligiht做反應，照理來說應該有更好的解法，雖然有興趣，不過因為先完善完整功能更重要，所以先放著   最佳展示角度Y要0.3f
		*/

		//lightPos.x = sin(glfwGetTime()) * 3.0f;
		//lightPos.z = cos(glfwGetTime()) * 2.0f;
		//lightPos.y = 5.0 + cos(glfwGetTime()) * 1.0f;
		//lightPos.z = sin(glfwGetTime() * 0.5) * 3.0;
		///lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		float near_plane = 1.0f;
		float far_plane = 25.0f;
		shadowProj = glm::perspective(glm::radians(90.0f), (float)1024 / (float)1024, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
		std::vector<glm::mat4> shadowTransforms;
		if (SceneManager::vec_DirectionlLight.size() > 0)
		{
			shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
			shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
			shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
			shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
			shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
			shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
		}
		
		lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = shadowProj * lightView;
		// render scene from light's point of view
		_shader.setVec3("viewPos", Window::_editorCamera.transform.position);
		for (unsigned int i = 0; i < shadowTransforms.size(); ++i)
			_shader.setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
		_shader.setFloat("far_plane", far_plane);
		_shader.setVec3("lightPos", lightPos);
		_shader.setBool("shadows", true); // enable/disable shadows by pressing 'SPACE'
		_shader.setBool("reverse_normals", false); // enable/disable shadows by pressing 'SPACE'
	}
	///Shader Setting
	//Draw Popline
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->Texture);
	glDrawArrays(this->_shape==Line? GL_LINES:GL_TRIANGLES, 0, this->_mode == Mode_3D?36:6);
	glBindTexture(GL_TEXTURE_2D, 0);

	//Debug Popline
	/*
	if (Window::DeBug_Mode &&false直接指定)
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
			ImVec2 _nextwindowpos = ImVec2(windowSpacePos.x*Window::_Width / Window::_Width, (Window::_Height - windowSpacePos.y)*Window::_Height / Window::_Height);

			if ((_nextwindowpos.x > 0 + Window::_Width) || (_nextwindowpos.x < 0) || (_nextwindowpos.y > 0 + Window::_Height) || (_nextwindowpos.y < 0))
				continue;
			ImGui::SetNextWindowPos(_nextwindowpos, ImGuiCond_Always);


			char s[sizeof(&this->ID) + sizeof(&this->_actor->transform->name) + sizeof(&i)];
			//char n[sizeof(i)];
			//sprintf(n,"%d",i);
			sprintf(s, "%d%s%d", this->ID, this->_actor->transform->name, i);
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
	}*/
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
	SceneManager::vec_ShaderProgram[1].use();
	SceneManager::vec_ShaderProgram[1].setInt("material.diffuse",0);
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
	btCollisionShape* colShape = new btBoxShape(btVector3(this->_actor->transform->scale.x, this->_actor->transform->scale.y, this->_actor->transform->scale.z) / 2);
	btScalar mass(0);
	World::collisionShapes.push_back(colShape);
	/// Create Dynamic Objects
	btTransform startTransform;
	startTransform.setIdentity();
	btQuaternion quat;
	//quat.setEuler(-glm::radians(this->_actor->transform->rotation.y), glm::radians(this->_actor->transform->rotation.x), glm::radians(this->_actor->transform->rotation.z));//or quat.setEulerZYX depending on the ordering you want
	quat.setEulerZYX(glm::radians(this->_actor->transform->rotation.z), glm::radians(-this->_actor->transform->rotation.y), glm::radians(this->_actor->transform->rotation.x));
	startTransform.setRotation(quat);

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);
	if (isDynamic)
		colShape->calculateLocalInertia(mass, localInertia);
	startTransform.setOrigin(btVector3(this->_actor->transform->position.x, this->_actor->transform->position.y, this->_actor->transform->position.z));
	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
	
	body = new btRigidBody(rbInfo);
	body->setCenterOfMassTransform(startTransform);

	int f = body->getCollisionFlags();
	body->setCollisionFlags(f | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);
	
	int _group = 1;
	int _mask = 1;

	World::dynamicsWorld->addRigidBody(body, _group,_mask);
	//World::dynamicsWorld->updateSingleAabb(body);



}
void Meshrender::SaveFile(pugi::xml_node _node)
{
	if (_node == NULL|| this->_actor->meshrender==NULL) return;
	_node.append_attribute("meshrender") = 1;
	pugi::xml_node _n = _node.append_child("MeshRender");
	_n.append_attribute("VertexColorX") = this->_actor->meshrender->VertexColor.x;
	_n.append_attribute("VertexColorY") = this->_actor->meshrender->VertexColor.y;
	_n.append_attribute("VertexColorZ") = this->_actor->meshrender->VertexColor.z;
}
void Meshrender::OpenFile(pugi::xml_node _node)
{
	if (_node == NULL || this->_actor->meshrender == NULL) return;
	pugi::xml_node _n = _node.child("MeshRender");
	this->VertexColor = glm::vec3(_n.attribute("VertexColorX").as_float(), _n.attribute("VertexColorY").as_float(), _n.attribute("VertexColorZ").as_float());
}
void Meshrender::UpdateCollision()
{
	World::dynamicsWorld->removeCollisionObject(this->body);
	CreateMouseCollision();
}

void Meshrender::CreateLine(glm::vec3 from, glm::vec3  to, glm::vec3 color)
{
	this->_shape = Line;
	// Vertex data
	GLfloat points[12];

	points[0] = from.x;
	points[1] = from.y;
	points[2] = from.z;
	points[3] = color.x;
	points[4] = color.y;
	points[5] = color.z;

	points[6] = to.x;
	points[7] = to.y;
	points[8] = to.z;
	points[9] = color.x;
	points[10] = color.y;
	points[11] = color.z;

	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindVertexArray(0);

	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);
}
