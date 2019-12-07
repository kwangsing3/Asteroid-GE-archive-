#include <Component/BoxCollision.hpp>
#include <SceneManager.hpp>
#include <Window.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include "btBulletDynamicsCommon.h"

void BoxCollision::SaveFile(pugi::xml_node* _node)
{
	if (_node == NULL) return;
	_node->append_attribute("_BoxCollision") = 1;
	_node->append_child("BoxCollision").append_attribute("Mass") = this->_Mass;
}
void BoxCollision::OpenFile(pugi::xml_node* _node)
{
	if (_node == NULL) return;
	float _f = _node->child("BoxCollision").attribute("Mass").as_float();
	this->_Mass = _f;
	UpdateCollision();
}

void BoxCollision::Copy(Component_Interface* _information)
{
	if (_information == NULL) return;
	





}

void BoxCollision::Inspector()
{
	if (ImGui::CollapsingHeader("BoxCollision", ImGuiTreeNodeFlags_DefaultOpen) | false)
	{
		float f0 = this->_Mass;
		if (ImGui::InputFloat("Mass", &f0, 0.1f, 1.0f, "%.3f"))
		{
			this->_Mass = f0;
			this->UpdateCollision();
		}
		if (ImGui::Checkbox("Debug Line", &this->_needdebug))
			this->ReSetCollisionFlag();
	}

}

void BoxCollision::CreateBox()
{
	Window::_MainWorld->InitPhysics = true;


	btVector3 localInertia(0, 0, 0);

	if (colShape == NULL)
	{
		colShape = new btBoxShape(btVector3(this->_actor->transform->scale.x, this->_actor->transform->scale.y, this->_actor->transform->scale.z));
		Window::_MainWorld->m_collisionShapes.push_back(colShape);
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
	int Collision_flag = _needdebug ? 4 : btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT;
	this->body->setCollisionFlags(Collision_flag);
	body->_ActorInBullet = this->_actor; 
	Window::_MainWorld->m_dynamicsWorld->addRigidBody(body, this->_Group, this->_Mask);
}

void BoxCollision::UpdateCollision()
{
	if (this->body == NULL) return;
	if (!Window::_MainWorld->_PlayMode)
	{
		Window::_MainWorld->deleteRigidBody(this->body);
		CreateBox();
	}
}

void BoxCollision::ReSetCollisionFlag()
{
	int Collision_flag = _needdebug ? 4 : btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT;
	this->body->setCollisionFlags(Collision_flag);
}

