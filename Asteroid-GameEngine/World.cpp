#include <World.h>

#include <Window.h>     //裡面有 #include <Camera.h>


//unsigned int World::depthMapFBO;
extern World* _MainWorld;
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
			_MainWorld->m_collisionShapes.push_back(_shap);
			_shap = new btCapsuleShape(0.08f, 0.55f);
			colshape.push_back(_shap);
			_MainWorld->m_collisionShapes.push_back(_shap);
			_shap = new btCapsuleShapeZ(0.08f, 0.55f);
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
			body[i]->_actor = this->_actor;
			Collision_flag = _needdebug ? 1 : btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT;
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
