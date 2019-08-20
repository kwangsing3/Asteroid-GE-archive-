#ifndef WORLD_H
#define WORLD_H

#include "btBulletDynamicsCommon.h"
class World
{
public:

	static btBroadphaseInterface* broadphase;

	// Set up the collision configuration and dispatcher
	static btDefaultCollisionConfiguration* collisionConfiguration;
	static btCollisionDispatcher* dispatcher;

	// The actual physics solver
	static btSequentialImpulseConstraintSolver* solver;

	// The world.
	static btDiscreteDynamicsWorld* dynamicsWorld;

	static btAlignedObjectArray<btCollisionShape*> collisionShapes;
	World()
	{
		//Bullet Physics creation
		// --------------------
		// Build the broadphase
		this->broadphase = new btDbvtBroadphase();

		// Set up the collision configuration and dispatcher
		this->collisionConfiguration = new btDefaultCollisionConfiguration();
		this->dispatcher = new btCollisionDispatcher(collisionConfiguration);

		// The actual physics solver
		this->solver = new btSequentialImpulseConstraintSolver;

		// The world.
		this->dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
		this->dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));
	}
	void UpdatePhysics()
	{

	}
};



#endif // !WORLD_H

