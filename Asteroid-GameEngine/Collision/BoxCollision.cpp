#include <Collision/BoxCollision.h>
#include <SceneManager.h>
#include <Window.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <World.h>
#include "btBulletDynamicsCommon.h"
void BoxCollision::SaveFile(pugi::xml_node _node)
{
	if (_node == NULL || this->_actor->boxcollision == NULL) return;

	
	_node.append_attribute("_BoxCollision") = 1;
	_node.append_child("BoxCollision").append_attribute("Mass") = this->_actor->boxcollision->_Mass;
}
void BoxCollision::OpenFile(pugi::xml_node _node)
{
	if (_node == NULL || this->_actor->boxcollision == NULL) return;
	float _f = _node.child("BoxCollision").attribute("Mass").as_float();
	this->_Mass = _f;

}

int Collision_flags = 0;

void BoxCollision::CreateBox()
{
	btVector3 localInertia(btVector3(this->_actor->transform->position.x, this->_actor->transform->position.y, this->_actor->transform->position.z));
	//create a dynamic rigidbody
	btCollisionShape* colShape = new btBoxShape(btVector3(this->_actor->transform->scale.x, this->_actor->transform->scale.y, this->_actor->transform->scale.z)/2);
	btScalar mass(this->_Mass);
	//colShape->setLocalScaling(btVector3(transform->scale.x, transform->scale.y, transform->scale.z));
   //btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
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

	Collision_flags = body->getCollisionFlags();
	body->setCollisionFlags(_needdebug ? Collision_flags : btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);
	body->setCustomDebugColor(btVector3(1,1,1));

	World::dynamicsWorld->addRigidBody(body, this->_Group, this->_Mask);
	

	//World::dynamicsWorld->addRigidBody(body);
	//this->phy_order = World::dynamicsWorld->getNumCollisionObjects() - 1;
}



void BoxCollision::UpdateCollision()
{
	World::dynamicsWorld->removeCollisionObject(this->body);
	/*btTransform transform;
	transform.setIdentity();
	btVector3 _pos(this->_actor->transform->position.x, this->_actor->transform->position.y, this->_actor->transform->position.z);
	transform.setOrigin(_pos);
	btQuaternion _rot;
	_rot.setEulerZYX(glm::radians(this->_actor->transform->rotation.z), glm::radians(-this->_actor->transform->rotation.y), glm::radians(this->_actor->transform->rotation.x));
	transform.setRotation(_rot);
	this->body->setCollisionFlags(_needdebug ? Collision_flags : btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);
	this->body->setWorldTransform(transform);
	//this->body->getMotionState()->setWorldTransform(transform);

	this->body->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
	this->body->setAngularVelocity(btVector3(0.0f, 0.0f, 0.0f));
	this->body->clearForces();*/
	this->UpdateState();
}


void BoxCollision::UpdateState()
{
	//Remove the rigid body from the dynamics world
	//World::dynamicsWorld->removeRigidBody(this->body);
	World::dynamicsWorld->removeCollisionObject(this->body);
	CreateBox();
}
