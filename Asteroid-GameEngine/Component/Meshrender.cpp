#include <Component/Meshrender.h>

#include <shader_m.h>
#include <Component/Transform.h>
#include <World.h>
#include <Units/Actor.h>



extern Camera _editorCamera;
extern World* _MainWorld;


void Meshrender::SaveFile(pugi::xml_node* _node)
{
	if (_node == NULL || this->_actor->meshrender == NULL) return;
	_node->append_attribute("meshrender") = 1;
	pugi::xml_node _n = _node->append_child("MeshRender");
	_n.append_attribute("VertexColorX") = this->_actor->meshrender->VertexColor.x;
	_n.append_attribute("VertexColorY") = this->_actor->meshrender->VertexColor.y;
	_n.append_attribute("VertexColorZ") = this->_actor->meshrender->VertexColor.z;
	//_n.append_attribute("Shape") = this->_actor->meshrender->_shape;
	//_n.append_attribute("_path") = this->_actor->meshrender->Model_path.c_str();
}
void Meshrender::OpenFile(pugi::xml_node* _node)
{
	if (_node == NULL || this->_actor->meshrender == NULL) return;
	pugi::xml_node _n = _node->child("MeshRender");
	this->VertexColor = glm::vec3(_n.attribute("VertexColorX").as_float(), _n.attribute("VertexColorY").as_float(), _n.attribute("VertexColorZ").as_float());
}
void Meshrender::Copy(Actor* _actor)
{
	if (_actor == NULL || _actor->meshrender == NULL) return;
	this->enabled = _actor->meshrender->enabled;
	//-----Component-----
	this->_needdebug = _actor->meshrender->_needdebug;
	this->_visable = _actor->meshrender->_visable;
	this->VertexColor = _actor->meshrender->VertexColor;
	/*this->_shape = _actor->meshrender->_shape;
	this->textures_loaded= _actor->meshrender->textures_loaded;
	this->meshes = _actor->meshrender->meshes;
	this->directory = _actor->meshrender->directory;
	this->_Mat4model = _actor->meshrender->_Mat4model;
	this->Model_path = _actor->meshrender->Model_path;*/

	
	
}

void Meshrender::Draw(Shader* _shader, bool _renderShadow)
{
	if (!this->_visable)  return;
	if (_shader==NULL) { std::cout << "Meshrender Shader Pass failed" << std::endl; return; }

	//Shader* _shader = _renderShadow ? _ShadowShader:_StandardShader;
	_shader->use();
	lightPos = SceneManager::vec_DirectionlLight.size() > 0 ? SceneManager::vec_DirectionlLight[0]->_actor->transform->position : glm::vec3(0, 5, 0);
	if (_renderShadow)
	{
		
			//紀錄一下  目前光影只會對第一個Directional Ligiht做反應，照理來說應該有更好的解法，雖然有興趣，不過因為先完善完整功能更重要，所以先放著   最佳展示角度Y要-0.3f~0.3f
		
		///lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
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
		lightSpaceMatrix = shadowProj * lightView;    //這行應該跟Directional light 有關
		// render scene from light's point of view
		for (unsigned int i = 0; i < shadowTransforms.size(); ++i)
			_shader->setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
	}
	else
	{
		_shader->setFloat("material.shininess", 32.0f);
		
		//Directional Light
		for (int i = 0; i < Light_Length; i++)
		{
			if (i + 1 > SceneManager::vec_DirectionlLight.size())
			{
				_shader->setVec3("dirLight[" + std::to_string(i) + "].direction", glm::vec3(0, 0, 0));
				_shader->setVec3("dirLight[" + std::to_string(i) + "].ambient", glm::vec3(0, 0, 0));
				_shader->setVec3("dirLight[" + std::to_string(i) + "].diffuse", glm::vec3(0, 0, 0));
				_shader->setVec3("dirLight[" + std::to_string(i) + "].specular", glm::vec3(0, 0, 0));
				continue;
			}
			_shader->setVec3("dirLight[" + std::to_string(i) + "].direction", SceneManager::vec_DirectionlLight[i]->_actor->transform->rotation);
			_shader->setVec3("dirLight[" + std::to_string(i) + "].ambient", SceneManager::vec_DirectionlLight[i]->Ambient);
			_shader->setVec3("dirLight[" + std::to_string(i) + "].diffuse", SceneManager::vec_DirectionlLight[i]->Diffuse);
			_shader->setVec3("dirLight[" + std::to_string(i) + "].specular", SceneManager::vec_DirectionlLight[i]->Specular);
		}
		//Point Light
		for (int i = 0; i < Light_Length; i++)
		{
			if (i + 1 > SceneManager::vec_PointLight.size())
			{
				_shader->setVec3("pointLights[" + std::to_string(i) + "].position", glm::vec3(0, 0, 0));
				_shader->setVec3("pointLights[" + std::to_string(i) + "].ambient", glm::vec3(0, 0, 0));
				_shader->setVec3("pointLights[" + std::to_string(i) + "].diffuse", glm::vec3(0, 0, 0));
				_shader->setVec3("pointLights[" + std::to_string(i) + "].specular", glm::vec3(0, 0, 0));
				_shader->setFloat("pointLights[" + std::to_string(i) + "].constant", 0);
				_shader->setFloat("pointLights[" + std::to_string(i) + "].linear", 0);
				_shader->setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0);
				continue;
			}
			_shader->setVec3("pointLights[" + std::to_string(i) + "].position", SceneManager::vec_PointLight[i]->_actor->transform->position);
			_shader->setVec3("pointLights[" + std::to_string(i) + "].ambient", SceneManager::vec_PointLight[i]->Ambient);
			_shader->setVec3("pointLights[" + std::to_string(i) + "].diffuse", SceneManager::vec_PointLight[i]->Diffuse);
			_shader->setVec3("pointLights[" + std::to_string(i) + "].specular", SceneManager::vec_PointLight[i]->Specular);
			_shader->setFloat("pointLights[" + std::to_string(i) + "].constant", SceneManager::vec_PointLight[i]->Constant);
			_shader->setFloat("pointLights[" + std::to_string(i) + "].linear", SceneManager::vec_PointLight[i]->linear);
			_shader->setFloat("pointLights[" + std::to_string(i) + "].quadratic", SceneManager::vec_PointLight[i]->quadratic);
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
		_shader->setVec3("Color", this->VertexColor.x, this->VertexColor.y, this->VertexColor.z);
	}

	//_shader.setVec3("viewPos", _editorCamera.transform.position);
	// 共通
	_shader->setMat4("projection", _editorCamera.Projection);
	_shader->setMat4("view", _editorCamera.GetViewMatrix());
	glm::mat4 _Mat4model(1.0f);
	_Mat4model = glm::translate(_Mat4model, glm::vec3(this->_actor->transform->position.x, this->_actor->transform->position.y, this->_actor->transform->position.z));

	EulerAngles = glm::vec3(glm::radians(this->_actor->transform->rotation.x), glm::radians(-this->_actor->transform->rotation.y), glm::radians(this->_actor->transform->rotation.z));
	glm::mat4 RotationMatrix = glm::toMat4(glm::quat(EulerAngles));
	_Mat4model = _Mat4model * RotationMatrix;
	_Mat4model = glm::scale(_Mat4model, glm::vec3(this->_actor->transform->scale.x, this->_actor->transform->scale.y, this->_actor->transform->scale.z));
	_shader->setMat4("model", _Mat4model);
	
	///Shadow
	_shader->setFloat("far_plane", far_plane);
	_shader->setVec3("viewPos", _editorCamera.transform.position);
	_shader->setVec3("lightPos", lightPos);
	// 共通

	/// Draw Pipeline
	for (unsigned int i = 0; i < this->_model->_meshes.size(); i++)
		this->_model->_meshes[i]->Draw(_shader);

}


//  --------------------------------- Mouse Collision --------------------------------- 
#include <World.h>
//btCollisionShape* colShape;
void Meshrender::CreateMouseCollision()
{
	_MainWorld->InitPhysics = true;
	btVector3 localInertia(0, 0, 0);
	//create a dynamic rigidbody
	if (colShape == NULL)
	{
		colShape = new btBoxShape(btVector3(this->_actor->transform->scale.x, this->_actor->transform->scale.y, this->_actor->transform->scale.z));
		_MainWorld->m_collisionShapes.push_back(colShape);
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
	 myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);

	body = new btRigidBody(rbInfo);
	body->setCenterOfMassTransform(startTransform);
	
	
	int Collision_flag = _needdebug ? 4 : btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT;
	body->setCollisionFlags(Collision_flag);

	int _group = 1;
	int _mask = 1;
	body->_ActorInBullet = this->_actor;
	_MainWorld->m_dynamicsWorld->addRigidBody(body, _group, _mask);
	
	//World::dynamicsWorld->updateSingleAabb(body);
}
void Meshrender::UpdateCollision()
{
	SceneManager::NeedInitedDraw = true;
	if (this->body == NULL) return;

	if (!_MainWorld->_PlayMode)
	{
		_MainWorld->deleteRigidBody(this->body);
		CreateMouseCollision();
	}
	else
	{
		btTransform initialTransform;
		initialTransform.setOrigin(btVector3(this->_actor->transform->position.x, this->_actor->transform->position.y, this->_actor->transform->position.z));
		btQuaternion quat;
		quat.setEulerZYX(glm::radians(this->_actor->transform->rotation.z), glm::radians(-this->_actor->transform->rotation.y), glm::radians(this->_actor->transform->rotation.x));
		initialTransform.setRotation(quat);
		this->body->setWorldTransform(initialTransform);
		this->myMotionState->setWorldTransform(initialTransform);
		_MainWorld->m_dynamicsWorld->updateSingleAabb(this->body);
	}
	
}
void Meshrender::SetVisable(bool _bool)
{
	this->_visable = _bool;
	SceneManager::UpdateRenderPipeline(this);
}
void Meshrender::ReSetCollisionFlag()
{
	int Collision_flag = _needdebug ? 4 : btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT;
	this->body->setCollisionFlags(Collision_flag);
}


