#include <World.h>


bool World::_PlayMode;
_Pivot* World::_piv;
//unsigned int World::depthMapFBO;

void _Pivot::CreateMouseCollision()
{

	if (this->_visable)
	{
		if (colshape.size() < 3)
		{
			colshape.clear();
			btCollisionShape* _shap;
			_shap = new btCapsuleShapeX(0.08f, 0.55f);
			colshape.push_back(_shap);
			Window::_Mainworld->m_collisionShapes.push_back(_shap);
			_shap = new btCapsuleShape(0.08f, 0.55f);
			colshape.push_back(_shap);
			Window::_Mainworld->m_collisionShapes.push_back(_shap);
			_shap = new btCapsuleShapeZ(0.08f, 0.55f);
			colshape.push_back(_shap);
			Window::_Mainworld->m_collisionShapes.push_back(_shap);
		}

		btTransform startTransform[3]; startTransform[0].setIdentity(); startTransform[1].setIdentity(); startTransform[2].setIdentity();
		btQuaternion quat;
		//quat.setEuler(-glm::radians(this->_actor->transform->rotation.y), glm::radians(this->_actor->transform->rotation.x), glm::radians(this->_actor->transform->rotation.z));//or quat.setEulerZYX depending on the ordering you want
		quat.setEulerZYX(glm::radians(this->_actor->transform->rotation.z), glm::radians(-this->_actor->transform->rotation.y), glm::radians(this->_actor->transform->rotation.x));
		startTransform[0].setOrigin(btVector3(this->_actor->transform->position.x + .5f, this->_actor->transform->position.y, this->_actor->transform->position.z));
		startTransform[1].setOrigin(btVector3(this->_actor->transform->position.x, this->_actor->transform->position.y + .5f, this->_actor->transform->position.z));
		startTransform[2].setOrigin(btVector3(this->_actor->transform->position.x, this->_actor->transform->position.y, this->_actor->transform->position.z + .5f));
		for (int i = 0; i < 3; i++)
		{
			btVector3 localInertia(0, 0, 0);
		
			btScalar mass(0);
			startTransform[i].setRotation(quat);
			bool isDynamic = (mass != 0.f);
			if (isDynamic)
				colshape[i]->calculateLocalInertia(mass, localInertia);
			btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform[i]);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colshape[i], localInertia);
			this->body[i] = new btRigidBody(rbInfo);
			
			//body[0]->setCenterOfMassTransform(startTransform[0]);
			body[i]->_actor = this->_actor;
			Collision_flag = _needdebug ? 1 : btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT;
			body[i]->setCollisionFlags(Collision_flag);
			
			Window::_Mainworld->m_dynamicsWorld->addRigidBody(body[i], _group, _mask);
		}
	}
}


void _Pivot::UpdateCollision()
{
	DeleteCollision();
	CreateMouseCollision();
}
void _Pivot::DeleteCollision()
{
	if (this->body[0] != NULL)Window::_Mainworld->m_dynamicsWorld->removeRigidBody(body[0]);
	if (this->body[1] != NULL)Window::_Mainworld->m_dynamicsWorld->removeRigidBody(body[1]);
	if (this->body[2] != NULL)Window::_Mainworld->m_dynamicsWorld->removeRigidBody(body[2]);
}
void _Pivot::AddCollision()
{
	CreateMouseCollision();
}