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
#include <Math/Math.h>
#include "GraphicEngine/imgui.h"
#include "GraphicEngine/imgui_impl_glfw.h"
#include "GraphicEngine/imgui_impl_opengl3.h"
#include "btBulletDynamicsCommon.h"

std::vector<ModelStruct> Meshrender::ModelList;



void Meshrender::Draw(Shader _shader)
{
	if (!this->_visable)  return;
	_shader.use();
	_shader.setMat4("projection", Window::_editorCamera.Projection);
	_shader.setMat4("view", Window::_editorCamera.GetViewMatrix());
	_Mat4model = glm::mat4(1.0f);
	_Mat4model = glm::translate(_Mat4model, glm::vec3(this->_actor->transform->position.x, this->_actor->transform->position.y, this->_actor->transform->position.z));
	glm::quat MyQuaternion;
	glm::vec3 EulerAngles(glm::radians(this->_actor->transform->rotation.x), glm::radians(-this->_actor->transform->rotation.y), glm::radians(this->_actor->transform->rotation.z));
	MyQuaternion = glm::quat(EulerAngles);
	glm::mat4 RotationMatrix = glm::toMat4(MyQuaternion);
	_Mat4model = _Mat4model * RotationMatrix;
	_Mat4model = glm::scale(_Mat4model, glm::vec3(this->_actor->transform->scale.x, this->_actor->transform->scale.y, this->_actor->transform->scale.z));
	_shader.setMat4("model", _Mat4model);

	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(_shader);

	/*
	{
		///Shader Setting
		{
			_shader.setVec3("viewPos", Window::_editorCamera.transform.position);
			_shader.setFloat("material.shininess", 32.0f);   // 先暫時關掉燈光   確認跟燈光沒關係
			
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
			// 只有XY是相對旋轉，其他Z是世界旋轉    //有複雜的相對關係
			// 不會受到位置影響
			model = glm::scale(model, glm::vec3(this->_actor->transform->scale.x, this->_actor->transform->scale.y, this->_actor->transform->scale.z));
			_shader.setMat4("model", model);
			_shader.setVec3("Color", this->VertexColor.x, this->VertexColor.y, this->VertexColor.z);
			///shadow
			glm::mat4 shadowProj, lightView;
			glm::mat4 lightSpaceMatrix;
			glm::vec3 lightPos = SceneManager::vec_DirectionlLight.size() > 0 ? SceneManager::vec_DirectionlLight[0]->_actor->transform->rotation : glm::vec3(0, 0, 0);
			/*
				紀錄一下  目前光影只會對第一個Directional Ligiht做反應，照理來說應該有更好的解法，雖然有興趣，不過因為先完善完整功能更重要，所以先放著   最佳展示角度Y要-0.3f~0.3f
			*/
			//lightPos.x = sin(glfwGetTime()) * 3.0f;
			//lightPos.z = cos(glfwGetTime()) * 2.0f;
			//lightPos.y = 5.0 + cos(glfwGetTime()) * 1.0f;
			//lightPos.z = sin(glfwGetTime() * 0.5) * 3.0;
			///lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		/*	float near_plane = 1.0f;
			float far_plane = 25.0f;
			shadowProj = glm::perspective(glm::radians(90.0f), (float)1024 / (float)1024, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
			std::vector<glm::mat4> shadowTransforms;
			if (SceneManager::vec_DirectionlLight.size() > 0)  //目前只有Directional Light有效果
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
		//glBindTexture(GL_TEXTURE_2D, this->Texture);

		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindTexture(GL_TEXTURE_2D, 0);
		
	}*/


}





//  --------------------------------- Mouse Collision --------------------------------- 
#include <World.h>
//btCollisionShape* colShape;
void Meshrender::CreateMouseCollision()
{
	btVector3 localInertia(0, 0, 0);
	//create a dynamic rigidbody
	if (colShape == NULL)
	{
		colShape = new btBoxShape(btVector3(this->_actor->transform->scale.x, this->_actor->transform->scale.y, this->_actor->transform->scale.z) / 2);
		Window::_Mainworld->m_collisionShapes.push_back(colShape);
	}

	btScalar mass(0);
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
	int Collision_flag=0;
	Collision_flag = _needdebug ? 1 : btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT;
	body->setCollisionFlags(Collision_flag);

	int _group = 1;
	int _mask = 1;

	Window::_Mainworld->m_dynamicsWorld->addRigidBody(body, _group, _mask);
	body->_actor = this->_actor;
	//World::dynamicsWorld->updateSingleAabb(body);
}
void Meshrender::UpdateCollision()
{
	if (this->body == NULL) return;
	Window::_Mainworld->deleteRigidBody(this->body);
	CreateMouseCollision();
}
void Meshrender::SaveFile(pugi::xml_node _node)
{
	if (_node == NULL || this->_actor->meshrender == NULL) return;
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
