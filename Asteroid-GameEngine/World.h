#ifndef WORLD_H
#define WORLD_H

#include "btBulletDynamicsCommon.h"
#include <SceneManager.h>
class World
{
public:
	static btBroadphaseInterface* broadphase;
	static btDefaultCollisionConfiguration* collisionConfiguration;  // Set up the collision configuration and dispatcher
	static btCollisionDispatcher* dispatcher;
	static btSequentialImpulseConstraintSolver* solver;   	// The actual physics solver
	static btDiscreteDynamicsWorld* dynamicsWorld;    	// The world.
	static btAlignedObjectArray<btCollisionShape*> collisionShapes;
	World()
	{
		//Bullet Physics creation
		// --------------------
		// Build the broadphase
		this->broadphase = new btDbvtBroadphase();
		this->collisionConfiguration = new btDefaultCollisionConfiguration();
		this->dispatcher = new btCollisionDispatcher(collisionConfiguration);
		this->solver = new btSequentialImpulseConstraintSolver;
		this->dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
		this->dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));
	}
	void UpdateFrame()
	{
		//Pyhscis Pipeline
		this->dynamicsWorld->stepSimulation(1.f / 60.f, 10);   //  這句才是讓物理動起來的精隨

		for (int i = 0; i < SceneManager::Objects.size(); i++)
		{
			if (SceneManager::Objects[i]->boxcollision != NULL)
			{
				int _order = SceneManager::Objects[i]->boxcollision->phy_order;
				btCollisionObject* obj = this->dynamicsWorld->getCollisionObjectArray()[_order];
				btRigidBody* body = btRigidBody::upcast(obj);
				btTransform trans;
				if (body && body->getMotionState())
				{
					body->getMotionState()->getWorldTransform(trans);
				}
				else
				{
					trans = obj->getWorldTransform();
				}
				SceneManager::Objects[i]->transform->position = glm::vec3(float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
				//btScalar _rotX, _rotY, _rotZ;
				// 根據物理做旋轉的變換還沒做
				//SceneManager::Objects[i]->transform->rotation = glm::vec3(float(trans.getRotation().getAxis().getX()), float(trans.getRotation().getAxis().getY()), float(trans.getRotation().getAxis().getZ()));
			}
		}
		this->dynamicsWorld->debugDrawWorld();

		// Draw Pipeline
		for (int i = 0; i < SceneManager::vec_ObjectsToRender.size(); i++)
		{
			SceneManager::vec_ObjectsToRender[i]->Draw();
		}


	}

};



#endif // !WORLD_H

