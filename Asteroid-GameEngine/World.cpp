#include <World.h>

btBroadphaseInterface* World::broadphase;
// Set up the collision configuration and dispatcher
btDefaultCollisionConfiguration* World::collisionConfiguration;
btCollisionDispatcher* World::dispatcher;
// The actual physics solver
btSequentialImpulseConstraintSolver* World::solver;
// The world.
btDiscreteDynamicsWorld* World::dynamicsWorld;

btAlignedObjectArray<btCollisionShape*>  World::collisionShapes;
bool World::_PlayMode;
_Pivot* World::_piv;
//unsigned int World::depthMapFBO;

void _Pivot::CreateMouseCollision()
{
	if (this->_visable)
	{
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
			btCollisionShape* colShape0;
			//create a dynamic rigidbody
			switch (i)
			{
			case 0:
				colShape0 = new btCapsuleShapeX(0.08f, 0.55f);
				break;
			case 1:
				colShape0 = new btCapsuleShape(0.08f, 0.55f);
				break;
			case 2:
				colShape0 = new btCapsuleShapeZ(0.08f, 0.55f);
				break;
			default:
				break;
			}
			btScalar mass(0);
			startTransform[i].setRotation(quat);
			World::collisionShapes.push_back(colShape0);
			bool isDynamic = (mass != 0.f);
			if (isDynamic)
				colShape0->calculateLocalInertia(mass, localInertia);
			btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform[i]);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape0, localInertia);
			body[i] = new btRigidBody(rbInfo);
			//body[0]->setCenterOfMassTransform(startTransform[0]);
			body[i]->_actor = this->_actor;
			Collision_flag = _needdebug ? 1 : btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT;
			body[i]->setCollisionFlags(Collision_flag);
			int _group = 1;
			int _mask = 1;
			World::dynamicsWorld->addRigidBody(body[i], _group, _mask);
		}
	}
}

void _Pivot::UpdateCollision()
{
	
	if (body[0] != NULL)World::dynamicsWorld->removeCollisionObject(body[0]);
	if (body[1] != NULL)World::dynamicsWorld->removeCollisionObject(body[1]);
	if (body[2] != NULL)World::dynamicsWorld->removeCollisionObject(body[2]);

	if(this->_visable)CreateMouseCollision();
}
