#include <World.h>

#include <Window.h>     //裡面有 #include <Camera.h>





//unsigned int World::depthMapFBO;
extern World* _MainWorld;
void _Pivot::CreateMouseCollision()   // 重複執行太多次了
{
	_needdebug = true;
	if (this->_visable)
	{
		if (colshape.size() < 3)
		{
			colshape.clear();
			btCollisionShape* _shap;
			_shap = new btCapsuleShapeX(0.08f, 0.55f*(1+ this->_actor->transform->scale.x));
			colshape.push_back(_shap);
			_MainWorld->m_collisionShapes.push_back(_shap);
			_shap = new btCapsuleShape(0.08f, 0.55f*(1 + this->_actor->transform->scale.x));
			colshape.push_back(_shap);
			_MainWorld->m_collisionShapes.push_back(_shap);
			_shap = new btCapsuleShapeZ(0.08f, 0.55f*(1 + this->_actor->transform->scale.x));
			colshape.push_back(_shap);
			_MainWorld->m_collisionShapes.push_back(_shap);
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
			body[i]->_ActorInBullet = this->_actor;
			Collision_flag = _needdebug ? 4 : btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT;
			body[i]->setCollisionFlags(Collision_flag);
			
			_MainWorld->m_dynamicsWorld->addRigidBody(body[i], _group, _mask);
		}
	}
}
void _Pivot::UpdateCollision()
{
	//DeleteCollision();
	//CreateMouseCollision();
}
void _Pivot::DeleteCollision()
{
	if (this->body[0] != NULL)_MainWorld->m_dynamicsWorld->removeRigidBody(body[0]);
	if (this->body[1] != NULL)_MainWorld->m_dynamicsWorld->removeRigidBody(body[1]);
	if (this->body[2] != NULL)_MainWorld->m_dynamicsWorld->removeRigidBody(body[2]);
}
void _Pivot::AddCollision()
{
	CreateMouseCollision();
}

#include <stb_image.h>
unsigned int loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}
void World::initPhysics()
{
	createEmptyDynamicsWorld();

	GLDebugDrawer* _deb = new GLDebugDrawer();
	this->m_dynamicsWorld->setDebugDrawer(_deb);
	//_piv = new _Pivot(new Actor());
}
void World::init_PhysicsProgress()
{
	if (!InitPhysics) return;
	InitPhysics = false;
	depose_init_PhysicsProgress();
	for (int i = 0;i < this->m_dynamicsWorld->getNumCollisionObjects(); i++)
	{
		_PhysicsProgress.push_back(new _PhysicsStrut(i, this->m_dynamicsWorld->getCollisionObjectArray()[i]->_ActorInBullet, 
			this->m_dynamicsWorld->getCollisionObjectArray()[i]== this->m_dynamicsWorld->getCollisionObjectArray()[i]->_ActorInBullet->meshrender->body?true:false
			));
	}
}
void World::depose_init_PhysicsProgress()      
{
	if (_PhysicsProgress.size() < 1) return;
	for (int i = 0; i < _PhysicsProgress.size(); i++)
	{
		
		this->m_dynamicsWorld->getCollisionObjectArray()[_PhysicsProgress[i]->_index]->_ActorInBullet = _PhysicsProgress[i]->_actor;	
	}
	_PhysicsProgress.clear();
	InitPhysics = true;
}
void World::CreateDepthMap()
{
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
	// attach depth texture as FBO's depth buffer                                            // Shadow buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void World::UpdateFrame()
{
	//Pyhscis Pipeline
	if (this->_PlayMode)
	{
		if (InitPhysics) init_PhysicsProgress();

		this->m_dynamicsWorld->stepSimulation(1.f / 60.0f, 1);
		/*  需要特別處理一下，BulletEngine不知道為何會把我放的Pointer刷新  所以只好在刷新之前提取我做的標記  在秒數更新後依照Actor順序移動，然後再複寫回去    ps.再複寫回去是為了下次能夠再取得一次
		    製作一個Funcition 專門在繪製前initPhysics, 用struct儲存index以及提前抽取actor, 然後只需要依照此struct進行索引然後改變物體位置。
		    之後再停止模擬時複寫回去  */
		
		for (int i = 0; i < _PhysicsProgress.size(); i++)
		{
			if (_PhysicsProgress[i]->Static) continue;
			_PhysicsProgress[i]->_actor->transform->MoveByPhysics(
				&this->m_dynamicsWorld->getCollisionObjectArray()[_PhysicsProgress[i]->_index]->getWorldTransform()
			);
		}
	}
	else
	{
		depose_init_PhysicsProgress();
	}
	/*glCullFace(GL_FRONT);
	///Shadw
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	SceneManager::DrawScene(true);   //True 代表在渲染陰影

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glCullFace(GL_BACK);
	// Draw Pipeline
	glViewport(0, 0, _Width, _Height);
	//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		
	

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);    //這個綁陰影的動作很醜，還能夠優化*/
	this->m_dynamicsWorld->debugDrawWorld();
	SceneManager::DrawScene(false, depthCubemap);  //False 代表沒有在渲染陰影

	//this->dynamicsWorld->debugDrawWorld();
}

