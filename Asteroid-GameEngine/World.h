﻿#ifndef WORLD_H
#define WORLD_H
#pragma once
#include "btBulletDynamicsCommon.h"
#include <GraphicEngine/GLDebugDrawer.h>
#include <Physics Engine/CommonInterfaces/CommonRigidBodyBase.h>
#include <Units/Camera.h>


#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <SceneManager.h>

//#include <Window.h>
//#include <Component/Meshrender.h>
// 為了方便釐清， 先做新的Class 來當作Pivot

extern Camera _editorCamera;

class _Pivot : public Meshrender
{
	bool _needdebug = true;
public:
	btRigidBody* body[3];
	Actor* _lowwerActor;
	bool _attaching = false;
	bool Drag[3] = { false,false,false };
	bool _DragMode[3] = { false,false,false };
	std::vector<btCollisionShape*> colshape;
	int _group = 1;
	int _mask = 1;
	unsigned int VBO, VAO;

	_Pivot(Actor* _a)
	{
		_visable = false;
		SwitchDragMode(0);
		_actor = _a;
		_actor->transform->name = (char*) "Pivot";
		_actor->meshrender = new Meshrender(_a, NONE);
		this->_mode = RenderMode(1);
		this->enabled = true;
		this->CreatePivot();
		this->VertexColor = glm::vec3(1, 1, 1);
		//CreateMouseCollision();
		//transform->name = (char*)"Cube";
		//  滑鼠判定的碰撞體
		//initshapes



		CreateMouseCollision();
	}
	void CreatePivot()
	{
		_shape = Shape::NONE;
		float PIVOTVERTICES[] = {
		0.0f,0.0f,0.0f,   1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,   1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,   1.0f,0.0f,0.0f,
		0.8f,0.1f,0.0f,   1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,   1.0f,0.0f,0.0f,
		0.8f,-0.1f,0.0f,  1.0f,0.0f,0.0f,// X-Axis
		0.0f,0.0f,0.0f,   0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,   0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,   0.0f,1.0f,0.0f,
		0.1f,0.8f,0.0f,   0.0f,1.0f,0.0f,
		0.0f,0.1f,0.0f,   0.0f,1.0f,0.0f,
		-0.1f,0.8f,0.0f,  0.0f,1.0f,0.0f,// Y-Axis
		0.0f,0.0f,0.0f,   0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,   0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,   0.0f,0.0f,1.0f,
		0.1f,0.0f,0.8f,   0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,   0.0f,0.0f,1.0f,
		-0.1f,0.0f,0.8f,  0.0f,0.0f,1.0f,// Z-Axis
		};
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindVertexArray(VAO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glBufferData(GL_ARRAY_BUFFER, sizeof(PIVOTVERTICES), PIVOTVERTICES, GL_STATIC_DRAW);
		glBindVertexArray(0);

		SceneManager::vec_ShaderProgram[0].use();
		SceneManager::vec_ShaderProgram[0].setInt("material.diffuse", 0);
		//Worldvectices_Debug = Spacevectices_Debug = Vectices_Debug;
		SceneManager::vec_ObjectsToRender.push_back(this);
	}
	void Draw(Shader _shader) override
	{
		if (this->_visable)
		{
			{
				_shader.use();
				_shader.setVec3("viewPos", _editorCamera.transform.position);
				_shader.setFloat("material.shininess", 32.0f);   // 先暫時關掉燈光   確認跟燈光沒關係

				glm::mat4 projection = _editorCamera.Projection;
				glm::mat4 view = _editorCamera.GetViewMatrix();
				_shader.setMat4("projection", projection);
				_shader.setMat4("view", view);
				glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first	
				model = glm::translate(model, glm::vec3(this->_actor->transform->position.x, this->_actor->transform->position.y, this->_actor->transform->position.z));
				glm::quat MyQuaternion;
				glm::vec3 EulerAngles(glm::radians(this->_actor->transform->rotation.x), glm::radians(-this->_actor->transform->rotation.y), glm::radians(this->_actor->transform->rotation.z));
				MyQuaternion = glm::quat(EulerAngles);
				glm::mat4 RotationMatrix = glm::toMat4(MyQuaternion);
				model = model * RotationMatrix;
				model = glm::scale(model, glm::vec3(this->_actor->transform->scale.x, this->_actor->transform->scale.y, this->_actor->transform->scale.z));
				_shader.setMat4("model", model);
				_shader.setVec3("Color", this->VertexColor.x, this->VertexColor.y, this->VertexColor.z);
				_shader.setBool("shadows", true); // enable/disable shadows by pressing 'SPACE'
				_shader.setBool("reverse_normals", false); // enable/disable shadows by pressing 'SPACE'
			}
			glBindVertexArray(VAO);
			glDrawArrays(GL_LINES, 0, 36);
		}
	}
	void Translate(glm::vec3 _pos)
	{
		this->_actor->transform->position = _pos;
		this->UpdateCollision();
		if (_attaching)
			this->_lowwerActor->transform->Translate(this->_actor->transform->position);
	}
	void Rotate(glm::vec3 _rot)
	{
		this->_actor->transform->rotation = _rot;
		this->UpdateCollision();
		if (_attaching)
			this->_lowwerActor->transform->Rotate(this->_actor->transform->rotation);
	}
	void Scale(glm::vec3 _sca)
	{
		this->_actor->transform->scale = _sca;
		this->UpdateCollision();
		if (_attaching)
			this->_lowwerActor->transform->Scale(this->_actor->transform->scale);
	}
	void AttachObject(Actor* _ac)
	{
		if (_ac != NULL)
		{
			this->_visable = true;
			AddCollision();

			this->_lowwerActor = _ac;
			this->_attaching = true;
			this->Translate(_ac->transform->position);
			this->Rotate(_ac->transform->rotation);
			//this->Scale(_ac->transform->position);
		}
		else
		{
			this->_visable = false;
			this->_lowwerActor = NULL;
			this->_attaching = false;
			DeleteCollision();
		}
	}
	void SwitchDragMode(int i)
	{
		_DragMode[0] = false; _DragMode[1] = false; _DragMode[2] = false;
		switch (i)
		{
		case 0:
			_DragMode[0] = true;
			break;
		case 1:
			_DragMode[1] = true;
			break;
		case 2:
			_DragMode[2] = true;
			break;
		default:
			break;
		}
	}
private:
	int Collision_flag = 0;
	void CreateMouseCollision();
	void UpdateCollision() override;
	void DeleteCollision();
	void AddCollision();


};

struct World : public CommonRigidBodyBase
{
	unsigned int depthMapFBO;
	unsigned int depthCubemap;
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	World(): CommonRigidBodyBase()
	{
		//Bullet Physics creation// --------------------// Build the broadphase
		_PlayMode = false;
		   // 我的ShaderProgram在建構函數中創建   目前需要在world的類別前宣告
		initPhysics();
		// -----------------------
		//const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
		//unsigned int depthMapFBO;
		_piv = new _Pivot(new Actor());
		// ----------------------- 
	
	}
public:
	 bool _PlayMode;
	
	 _Pivot* _piv;
	virtual ~World()
	{
	
	}
	void UpdateFrame()
	{
		//Pyhscis Pipeline
		if (this->_PlayMode)
		{
			/*this->m_dynamicsWorld->stepSimulation(1.f / 60.0f, 1);   //  這句才是讓物理動起來的精隨
			for (int i = 0; i < this->m_dynamicsWorld->getNumCollisionObjects(); i++)
			{
				btCollisionObject* obj = this->m_dynamicsWorld->getCollisionObjectArray()[i];
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
			}*/
		}
		//glCullFace(GL_FRONT);
		///Shadw
		/*glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
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
		//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		*/
		this->m_dynamicsWorld->debugDrawWorld();
		for (int i = 0; i < SceneManager::vec_ObjectsToRender.size(); i++)
		{
			//glActiveTexture(GL_TEXTURE1);
			//glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
			SceneManager::vec_ObjectsToRender[i]->Draw(SceneManager::vec_ShaderProgram[SceneManager::vec_ObjectsToRender[i]->_shape==NONE?0:4]); // 這是暫時的  記得要改 非常難看  而且非常難懂
		}
		//this->dynamicsWorld->debugDrawWorld();
	}

	virtual void initPhysics()
	{
		createEmptyDynamicsWorld();

		GLDebugDrawer* _deb = new GLDebugDrawer();
		this->m_dynamicsWorld->setDebugDrawer(_deb);
	}
	void CreateDepthMap()
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

		SceneManager::vec_ShaderProgram[1].use();                                //要想辦法改掉   應該要直接串給MeshRender的主要Shader上 
		SceneManager::vec_ShaderProgram[1].setInt("diffuseTexture", 0);
		SceneManager::vec_ShaderProgram[1].setInt("depthMap", 1);
	}

};



#endif // !WORLD_H

