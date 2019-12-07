//
//  Transform.cpp
//  Asteroid-GameEngine
//
//  Created by KS on 2019/11/20.
//

#include <Component/Transform.hpp>

#include <Actor.hpp>
#include "btBulletCollisionCommon.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <Window.hpp>
void Transform::SaveFile(pugi::xml_node* _node)
{
	if (_node == NULL || this->_actor->transform == NULL) return;
	pugi::xml_node _childPos = _node->append_child("Position");
	_childPos.append_attribute("x") = this->_actor->transform->position.x;
	_childPos.append_attribute("y") = this->_actor->transform->position.y;
	_childPos.append_attribute("z") = this->_actor->transform->position.z;
	pugi::xml_node _childRot = _node->append_child("Rotation");
	_childRot.append_attribute("x") = this->_actor->transform->rotation.x;
	_childRot.append_attribute("y") = this->_actor->transform->rotation.y;
	_childRot.append_attribute("z") = this->_actor->transform->rotation.z;
	pugi::xml_node _childSca = _node->append_child("Scale");
	_childSca.append_attribute("x") = this->_actor->transform->scale.x;
	_childSca.append_attribute("y") = this->_actor->transform->scale.y;
	_childSca.append_attribute("z") = this->_actor->transform->scale.z;
}

void Transform::OpenFile(pugi::xml_node* _node)
{
	if (_node == NULL || this->_actor->transform == NULL) return;
	this->position.x = _node->child("Position").attribute("x").as_float();
	this->position.y = _node->child("Position").attribute("y").as_float();
	this->position.z = _node->child("Position").attribute("z").as_float();
	this->rotation.x = _node->child("Rotation").attribute("x").as_float();
	this->rotation.y = _node->child("Rotation").attribute("y").as_float();
	this->rotation.z = _node->child("Rotation").attribute("z").as_float();
	this->scale.x = _node->child("Scale").attribute("x").as_float();
	this->scale.y = _node->child("Scale").attribute("y").as_float();
	this->scale.z = _node->child("Scale").attribute("z").as_float();
}

void Transform::Copy(Component_Interface* _information)
{
	if (_information == NULL) return;



}

void Transform::Inspector()
{
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen) | false)
	{
		float _test[3] = { glm::radians(this->rotation.x),glm::radians(this->rotation.y),glm::radians(this->rotation.z) };
		if (ImGui::DragFloat3("Position", (float*)&this->position, 0.01f)) { this->Translate(this->position); Window::_MainWorld->_SceneManager->NeedInitedDraw = true; }
		if (ImGui::DragFloat3("Rotation", (float*)&this->rotation, 0.1f)) { this->Rotate(this->rotation); Window::_MainWorld->_SceneManager->NeedInitedDraw = true; }
		if (ImGui::DragFloat3("Scale", (float*)&this->scale, 0.01f)) { this->Scale(this->scale); Window::_MainWorld->_SceneManager->NeedInitedDraw = true; }
	}
}

void Transform::Translate(glm::vec3 _pos)
{
	this->position = _pos;

	for (auto &com:this->_actor->_Components)
	{
		com->MoveEvent();
	}

	//if (this->_actor->meshrender != NULL) this->_actor->meshrender->UpdateCollision();
	//if (this->_actor->boxcollision != NULL) this->_actor->boxcollision->UpdateCollision();
}

void Transform::Translate(glm::mat4 _pos)
{
	//glm::mat4 transformation; // your transformation matrix.
	//glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(_pos, this->scale, rotation, this->position, skew, perspective);
}

void Transform::Rotate(glm::vec3 _rot)
{
	this->rotation = _rot;
	//if (this->_actor->meshrender != NULL) this->_actor->meshrender->UpdateCollision();
	//if (this->_actor->boxcollision != NULL) this->_actor->boxcollision->UpdateCollision();
}

/*void Transform::Rotate(glm::Quaternion _qu);
{
	EulerAngles _EA = Math::ToEulerAngles(_qu);
	glm::vec3 _rot(glm::degrees(_EA.roll), glm::degrees(-_EA.pitch) , glm::degrees(_EA.yaw));

	this->Rotate(_rot);
	if (this->_actor->meshrender != NULL) this->_actor->meshrender->UpdateCollision();
	//if (this->_actor->boxcollision != NULL) this->_actor->boxcollision->UpdateCollision();
}*/

void Transform::Scale(glm::vec3 _scal)
{
	this->scale = _scal;
	//if (this->_actor->meshrender != NULL) this->_actor->meshrender->UpdateCollision();
	//if (this->_actor->boxcollision != NULL) 
	//	this->_actor->boxcollision->UpdateCollision();
}
btScalar _x, _y, _z;
void Transform::MoveByPhysics(btTransform* _trans)
{
	_trans->getRotation().getEulerZYX(_z, _y, _x);
	this->_actor->transform->Rotate(glm::vec3(glm::degrees(_x), -glm::degrees(_y), glm::degrees(_z)));
	this->_actor->transform->Translate(glm::vec3(_trans->getOrigin().getX(), _trans->getOrigin().getY(), _trans->getOrigin().getZ()));
	//_trans->getRotation().getEulerZYX(this->rotation.z,this->rotation.y, this->rotation.x);
}
