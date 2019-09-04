#ifndef WORLD_H
#define WORLD_H

#include "btBulletDynamicsCommon.h"
#include <SceneManager.h>
#include <glm/gtc/constants.hpp>
#include <Math/Math.h>
#include <Window.h>
class World
{
public:
	static btBroadphaseInterface* broadphase;
	static btDefaultCollisionConfiguration* collisionConfiguration;  // Set up the collision configuration and dispatcher
	static btCollisionDispatcher* dispatcher;
	static btSequentialImpulseConstraintSolver* solver;   	// The actual physics solver
	static btDiscreteDynamicsWorld* dynamicsWorld;    	// The world.
	static bool _PlayMode;
	unsigned int depthMapFBO;
	unsigned int depthCubemap;
	static btAlignedObjectArray<btCollisionShape*> collisionShapes;
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
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
		_PlayMode = false;
		// Shadow buffer

		 // configure depth map FBO
	// -----------------------
		//const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
		//unsigned int depthMapFBO;
		glGenFramebuffers(1, &depthMapFBO);
		// create depth texture

		glGenTextures(1, &depthCubemap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
		for (unsigned int i = 0; i < 6; ++i)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		// attach depth texture as FBO's depth buffer
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		SceneManager::vec_ShaderProgram[1].use();
		SceneManager::vec_ShaderProgram[1].setInt("diffuseTexture", 0);
		SceneManager::vec_ShaderProgram[1].setInt("depthMap", 1);
	}
	void UpdateFrame()
	{
		//Pyhscis Pipeline
		if (this->_PlayMode)
		{
			this->dynamicsWorld->stepSimulation(1.f / 60.0f, 1);   //  這句才是讓物理動起來的精隨
			for (int i = 0; i < this->dynamicsWorld->getNumCollisionObjects(); i++)
			{
				btCollisionObject* obj = this->dynamicsWorld->getCollisionObjectArray()[i];
				for (int j = 0; j < SceneManager::Objects.size(); j++)
				{
					if (SceneManager::Objects[j]->boxcollision != NULL && SceneManager::Objects[j]->boxcollision->body == obj)
					{
						//int _order = SceneManager::Objects[i]->boxcollision->phy_order;
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
						SceneManager::Objects[j]->transform->position = glm::vec3(float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
						Quaternion _btq;
						_btq.x = trans.getRotation().getX();
						_btq.y = trans.getRotation().getY();
						_btq.z = trans.getRotation().getZ();
						_btq.w = trans.getRotation().getW();

						//EulerAngles _ea = Math::ToEulerAngles(trans.getRotation());
						SceneManager::Objects[j]->transform->Rotate(_btq);
						//SceneManager::Objects[j]->transform->Rotate(Math::QuaternionToGlmVec3(_btq));
						break;
					}
				}
			}
		}
		this->dynamicsWorld->debugDrawWorld();
		//glCullFace(GL_FRONT);
		///Shadw
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		for (int i = 0; i < SceneManager::vec_ObjectsToRender.size(); i++)
		{
			SceneManager::vec_ObjectsToRender[i]->Draw(SceneManager::vec_ShaderProgram[2]);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glCullFace(GL_BACK);
		// Draw Pipeline
		glViewport(0, 0, Window::_Width, Window::_Height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (int i = 0; i < SceneManager::vec_ObjectsToRender.size(); i++)
		{
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
			SceneManager::vec_ObjectsToRender[i]->Draw(SceneManager::vec_ShaderProgram[1]);
		}
		
		//this->dynamicsWorld->debugDrawWorld();
	}



};

//  應該要整理個function來位移   以後要是要移動的話就靠那個fun移動

#endif // !WORLD_H

