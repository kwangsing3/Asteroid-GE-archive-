#include <Collision/BoxCollision.h>
#include <SceneManager.h>
#include <Window.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <World.h>




void BoxCollision::CreateBox()
{

	btVector3 localInertia(0,0,0);
	//create a dynamic rigidbody
	btCollisionShape* colShape = new btBoxShape(btVector3(transform->scale.x, transform->scale.y, transform->scale.z)/2);
	btScalar mass(this->_Mass);
	//colShape->setLocalScaling(btVector3(transform->scale.x, transform->scale.y, transform->scale.z));
	
   //btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
	
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
	this->phy_order = World::dynamicsWorld->getNumCollisionObjects() - 1;
	
}


void BoxCollision::ResetDynamic()
{
	btTransform _T;
	_T.setOrigin(btVector3(transform->position.x, transform->position.y, transform->position.z));
	btQuaternion quat;
	quat.setEuler(-glm::radians(transform->rotation.y), glm::radians(transform->rotation.x), glm::radians(transform->rotation.z));//or quat.setEulerZYX depending on the ordering you want
	_T.setRotation(quat);
	body->setWorldTransform(_T);
	

}
//  已經可以讓物品掉落了    可是要想辦法跟 startTransform接起來    而且關於物理的程式碼非常亂