﻿#include <Collision/BoxCollision.h>
#include <SceneManager.h>
#include <Window.h>

#include <glm/gtc/matrix_transform.hpp>
#include <World.h>
#include "btBulletDynamicsCommon.h"
extern World* _MainWorld;
void BoxCollision::SaveFile(pugi::xml_node* _node)
{
	if (_node == NULL || this->_actor->boxcollision == NULL) return;
	_node->append_attribute("_BoxCollision") = 1;
	_node->append_child("BoxCollision").append_attribute("Mass") = this->_actor->boxcollision->_Mass;
}
void BoxCollision::OpenFile(pugi::xml_node* _node)
{
	if (_node == NULL || this->_actor->boxcollision == NULL) return;
	float _f = _node->child("BoxCollision").attribute("Mass").as_float();
	this->_Mass = _f;
}

void BoxCollision::CreateBox()
{
	btVector3 localInertia(0, 0, 0);

	if (colShape == NULL)
	{
		colShape = new btBoxShape(btVector3(this->_actor->transform->scale.x, this->_actor->transform->scale.y, this->_actor->transform->scale.z));
		_MainWorld->m_collisionShapes.push_back(colShape);
	}
	/// Create Dynamic Objects
	btTransform startTransform;
	startTransform.setIdentity();
	btQuaternion quat;
	//quat.setEuler(-glm::radians(this->_actor->transform->rotation.y), glm::radians(this->_actor->transform->rotation.x), glm::radians(this->_actor->transform->rotation.z));//or quat.setEulerZYX depending on the ordering you want
	quat.setEulerZYX(glm::radians(this->_actor->transform->rotation.z), glm::radians(-this->_actor->transform->rotation.y), glm::radians(this->_actor->transform->rotation.x));
	startTransform.setRotation(quat);
	
	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (_Mass != 0.f);
	if (isDynamic)
		colShape->calculateLocalInertia(_Mass, localInertia);
	startTransform.setOrigin(btVector3(this->_actor->transform->position.x, this->_actor->transform->position.y, this->_actor->transform->position.z));

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(_Mass, myMotionState, colShape, localInertia);
	
	body = new btRigidBody(rbInfo);
	body->setCenterOfMassTransform(startTransform);
	int Collision_flag = 0;


	body->_ActorInBullet = this->_actor; 

	_MainWorld->m_dynamicsWorld->addRigidBody(body, this->_Group, this->_Mask);
	
	
	
	
}



void BoxCollision::UpdateCollision()
{
	if (this->body == NULL) return;

	body->translate(btVector3(this->_actor->transform->position.x, this->_actor->transform->position.y, this->_actor->transform->position.z));
	_MainWorld->deleteRigidBody(this->body);
	CreateBox();
}

