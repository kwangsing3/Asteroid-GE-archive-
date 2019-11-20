//
//  Meshrender.cpp
//  Asteroid-GameEngine
//
//  Created by KS on 2019/11/20.
//

#include <Meshrender.hpp>

#include <shader_m.h>
#include <Transform.hpp>
#include <World.hpp>
#include <Actor.hpp>



extern Camera _editorCamera;
extern World* _MainWorld;


void Meshrender::SaveFile(pugi::xml_node* _node)                                                                      // 重新設定一下 SaveFile and OpenFile
{
	if (_node == NULL || this->_actor->meshrender == NULL) return;
	_node->append_attribute("meshrender") = 1;
	pugi::xml_node _n = _node->append_child("MeshRender");
	_n.append_attribute("VertexColorX") = this->_actor->meshrender->VertexColor.x;
	_n.append_attribute("VertexColorY") = this->_actor->meshrender->VertexColor.y;
	_n.append_attribute("VertexColorZ") = this->_actor->meshrender->VertexColor.z;
	//_n.append_attribute("Shape") = this->_actor->meshrender->_shape;
	_n.append_attribute("_path") = this->_model->_ModelPath.c_str();
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
	this->_Mat4model = _actor->meshrender->_Mat4model;*/
	this->_model->_ModelPath = _actor->meshrender->_model->_ModelPath;



}
void Meshrender::Draw(Shader* _shader)
{
	if (!this->_visable)  return;
	if (_shader == NULL) { AGE_PRINTCONSLE("Meshrender Shader Pass failed"); AGE_ASSERT(false); }

	// 共通
	_shader->setMat4("view", _editorCamera.GetViewMatrix());
	_shader->setVec3("Color", this->VertexColor.x, this->VertexColor.y, this->VertexColor.z);
	this->_ModelMatrix4 = glm::mat4(1.0f);
	this->_ModelMatrix4 = glm::translate(this->_ModelMatrix4, glm::vec3(this->_actor->transform->position.x, this->_actor->transform->position.y, this->_actor->transform->position.z));
	EulerAngles = glm::vec3(glm::radians(this->_actor->transform->rotation.x), glm::radians(-this->_actor->transform->rotation.y), glm::radians(this->_actor->transform->rotation.z));
	this->_ModelMatrix4 = _ModelMatrix4 * glm::toMat4(glm::quat(EulerAngles));
	this->_ModelMatrix4 = glm::scale(this->_ModelMatrix4, glm::vec3(this->_actor->transform->scale.x, this->_actor->transform->scale.y, this->_actor->transform->scale.z));
	_shader->setMat4("model", _ModelMatrix4);
	// 共通

	/// Draw Pipeline
	for (unsigned int i = 0; i < this->_model->_meshes.size(); i++)
		this->_model->_meshes[i]->Draw(_shader);

}
void Meshrender::SetVisable(bool _bool)
{
	this->_visable = _bool;
}

//  --------------------------------- Mouse Collision --------------------------------- 
#include <World.hpp>
//btCollisionShape* colShape;
/*
void Meshrender::CreateMouseCollision()
{

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

	this->body = new btRigidBody(rbInfo);
	this->body->setCenterOfMassTransform(startTransform);


	int Collision_flag = _needdebug ? 4 : btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT;
	this->body->setCollisionFlags(Collision_flag);

	int _group = 1;
	int _mask = 1;
	this->body->_ActorInBullet = this->_actor;
	_MainWorld->m_dynamicsWorld->addRigidBody(body, _group, _mask);
	_MainWorld->InitPhysics = true;
	//World::dynamicsWorld->updateSingleAabb(body);
}
void Meshrender::UpdateCollision()
{

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

void Meshrender::ReSetCollisionFlag()
{
	int Collision_flag = _needdebug ? 4 : btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT;
	this->body->setCollisionFlags(Collision_flag);
}


*/
