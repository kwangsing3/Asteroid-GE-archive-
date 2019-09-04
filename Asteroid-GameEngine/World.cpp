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
//unsigned int World::depthMapFBO;