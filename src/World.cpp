#include <World.hpp>

#include <Window.hpp>     //�̭��� #include <Camera.h>




//unsigned int World::depthMapFBO;
extern World* _MainWorld;
void _Pivot::CreateMouseCollision()
{
	_needdebug = true;
	if (this->_visable)
	{
		colshape.clear();
		btCollisionShape* _shap;
		_shap = new btCapsuleShapeX(0.2f, 0.55f * (1 + this->_actor->transform->scale.x));
		colshape.push_back(_shap);
		_MainWorld->m_collisionShapes.push_back(_shap);
		_shap = new btCapsuleShape(0.2f, 0.55f * (1 + this->_actor->transform->scale.x));
		colshape.push_back(_shap);
		_MainWorld->m_collisionShapes.push_back(_shap);
		_shap = new btCapsuleShapeZ(0.2f, 0.55f * (1 + this->_actor->transform->scale.x));
		colshape.push_back(_shap);
		_MainWorld->m_collisionShapes.push_back(_shap);

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
	DeleteCollision();
	CreateMouseCollision();
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
unsigned int loadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
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
	_deb->setDebugMode(GLDebugDrawer::DBG_FastWireframe);


	this->m_dynamicsWorld->setDebugDrawer(_deb);
	//_piv = new _Pivot(new Actor());
}
void World::init_PhysicsProgress()
{
	if (!InitPhysics) return;
	InitPhysics = false;
	depose_init_PhysicsProgress();
	for (int i = 0; i < this->m_dynamicsWorld->getNumCollisionObjects(); i++)
	{
		if (this->m_dynamicsWorld->getCollisionObjectArray()[i]->_ActorInBullet->meshrender == NULL)
			continue;
		if (this->m_dynamicsWorld->getCollisionObjectArray()[i] == this->m_dynamicsWorld->getCollisionObjectArray()[i]->_ActorInBullet->meshrender->body)
			continue;

		_PhysicsProgress.push_back(new _PhysicsStruct(i, this->m_dynamicsWorld->getCollisionObjectArray()[i]->_ActorInBullet, false));
	}
}
void World::depose_init_PhysicsProgress()
{
	if (_PhysicsProgress.empty()) return;
	for (int i = 0; i < _PhysicsProgress.size(); i++)
	{
		this->m_dynamicsWorld->getCollisionObjectArray()[_PhysicsProgress[i]->_index]->_ActorInBullet = _PhysicsProgress[i]->_actor;
	}
	_PhysicsProgress.clear();
	InitPhysics = true;
}
void World::CreateDepthMap()
{
	// Point Light Depth Cubemap Texture
	glGenFramebuffers(1, &depthMapFBO_PoLight);
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
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO_PoLight);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glGenFramebuffers(1, &depthMapFBO_DirLight);
	// Directional Light Depth Texture
	glGenTextures(1, &depthTexture_DirLight);
	glBindTexture(GL_TEXTURE_2D, depthTexture_DirLight);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO_DirLight);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture_DirLight, 0);
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
		/*  �ݭn�S�O�B�z�@�U�ABulletEngine�����D����|��ک�Pointer��s  �ҥH�u�n�b��s���e�����ڰ����аO  �b��Ƨ�s��̷�Actor���ǲ��ʡA�M��A�Ƽg�^�h    ps.�A�Ƽg�^�h�O���F�U������A���o�@��
			�s�@�@��Funcition �M���bø�s�einitPhysics, ��struct�x�sindex�H�δ��e���actor, �M��u�ݭn�̷Ӧ�struct�i����޵M����ܪ����m�C
			����A��������ɽƼg�^�h  */

		for (int i = 0; i < _PhysicsProgress.size(); i++)
		{
			if (_PhysicsProgress[i]->Static) continue;
			_PhysicsProgress[i]->_actor->transform->MoveByPhysics(&this->m_dynamicsWorld->getCollisionObjectArray()[_PhysicsProgress[i]->_index]->getWorldTransform());
		}
	}
	else
	{
		if (!_PhysicsProgress.empty()) depose_init_PhysicsProgress();
	}
	_SceneManager.vec_SpecializedDraw();  //�u�ݭn�e�@�����D���q�����S��ø�s   (�y�жb�BAxis ... etc) 
	this->m_dynamicsWorld->debugDrawWorld();
	glCullFace(GL_FRONT);
	///Shadw
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	if (_RenderShadow && !_SceneManager.vec_DirectionlLight.empty())
	{
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO_DirLight);
		glClear(GL_DEPTH_BUFFER_BIT);
		_SceneManager.DrawScene(RenderShadowType::DirectionalLight);
	}

	if (false && !_SceneManager.vec_PointLight.empty())
	{
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO_PoLight);
		glClear(GL_DEPTH_BUFFER_BIT);
		_SceneManager.DrawScene(RenderShadowType::PointLight);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glCullFace(GL_BACK);
	// Draw Pipeline
	glViewport(0, 0, Window_Width, Window_Height);



	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		
/*	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);    */
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthTexture_DirLight);
	//ImGui::Image((void*)depthTexture_DirLight,ImVec2(300,300));
	  /*  */
	if (!_SceneManager.vec_ObjectsToRender.empty() || !_SceneManager.vec_ObjectsToRender_Instancing.empty())
		_SceneManager.DrawScene(RenderShadowType::Normal);  //False �N��S���b��V���v




}



