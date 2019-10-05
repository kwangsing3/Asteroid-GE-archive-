#ifndef WORLD_H
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
#include <ADD_Component.h>
//#include <Window.h>
//#include <Component/Meshrender.h>
// 為了方便釐清， 先做新的Class 來當作Pivot

extern Camera _editorCamera;

class _Pivot : public Meshrender
{
	
public:
	btRigidBody* body[3];
	std::vector<Actor*> _lowwerActor;
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

		SceneManager::vec_ShaderProgram[0]->use();
		SceneManager::vec_ShaderProgram[0]->setInt("material.diffuse", 0);
		//Worldvectices_Debug = Spacevectices_Debug = Vectices_Debug;
		

		SceneManager::AddToRenderPipeline(this);
	}
	void Draw(Shader* _shader,bool _renderShadow)// override
	{
		if (_renderShadow) return;
		//Shader* _shader = _StandardShader? _StandardShader : new Shader("Shader/SimpleVertexShader.vs", "Shader/SimpleFragmentShader.fs");
		//return;
		if (this->_visable)
		{
			{
				_shader->use();
				//_shader.setVec3("viewPos", _editorCamera.transform.position);
				//_shader.setFloat("material.shininess", 32.0f);   // 先暫時關掉燈光   確認跟燈光沒關係

				glm::mat4 projection = _editorCamera.Projection;
				glm::mat4 view = _editorCamera.GetViewMatrix();
				_shader->setMat4("projection", projection);
				_shader->setMat4("view", view);

				glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first	
				model = glm::translate(model, glm::vec3(this->_actor->transform->position.x, this->_actor->transform->position.y, this->_actor->transform->position.z));
				glm::quat MyQuaternion;
				glm::vec3 EulerAngles(glm::radians(this->_actor->transform->rotation.x), glm::radians(-this->_actor->transform->rotation.y), glm::radians(this->_actor->transform->rotation.z));
				MyQuaternion = glm::quat(EulerAngles);
				glm::mat4 RotationMatrix = glm::toMat4(MyQuaternion);
				model = model * RotationMatrix;
				model = glm::scale(model, glm::vec3(this->_actor->transform->scale.x, this->_actor->transform->scale.y, this->_actor->transform->scale.z));
				_shader->setMat4("model", model);
				_shader->setVec3("Color", this->VertexColor.x, this->VertexColor.y, this->VertexColor.z);
				//_shader.setBool("shadows", true); // enable/disable shadows by pressing 'SPACE'
				//_shader.setBool("reverse_normals", false); // enable/disable shadows by pressing 'SPACE'
			}
			glBindVertexArray(VAO);
			glDrawArrays(GL_LINES, 0, 36);
		}
	}
	void Translate(glm::vec3 _pos)
	{
		if (_lowwerActor.size() > 1)
		{
			for (int i = 0; i < _lowwerActor.size(); i++)
			{
				this->_lowwerActor[i]->transform->Translate(this->_lowwerActor[i]->transform->position+(_pos - this->_actor->transform->position));
			}
		}
		else
		{
			this->_actor->transform->position = _pos;
			this->UpdateCollision();
			if (_attaching)
			{
				for (int i = 0; i < _lowwerActor.size(); i++)
				{
					this->_lowwerActor[i]->transform->Translate(this->_actor->transform->position);
				}
			}
		}
			
	}
	void Rotate(glm::vec3 _rot)
	{
		if (_lowwerActor.size() > 1)
		{
			for (int i = 0; i < _lowwerActor.size(); i++)
			{
				this->_lowwerActor[i]->transform->Rotate(this->_lowwerActor[i]->transform->rotation + (_rot - this->_actor->transform->rotation));
			}
		}
		else
		{
			this->_actor->transform->rotation = _rot;
			this->UpdateCollision();
			if (_attaching)
			{
				for (int i = 0; i < _lowwerActor.size(); i++)
				{
					this->_lowwerActor[i]->transform->Rotate(this->_actor->transform->rotation);
				}
			}
		}	
	}
	void Scale(glm::vec3 _sca)
	{
		if (_lowwerActor.size() > 1)
		{
			for (int i = 0; i < _lowwerActor.size(); i++)
			{
				this->_lowwerActor[i]->transform->Scale(this->_lowwerActor[i]->transform->scale + (_sca - this->_actor->transform->scale));
			}
		}
		else
		{
			this->_actor->transform->scale = _sca;
			this->UpdateCollision();
			if (_attaching)
			{
				for (int i = 0; i < _lowwerActor.size(); i++)
				{
					this->_lowwerActor[i]->transform->Scale(this->_actor->transform->scale);
				}
			}
		}
	}
	void AttachObject(Actor* _ac)
	{
		
		if (_ac != NULL)
		{
			this->_visable = true;
			 
			_lowwerActor.clear();
			_lowwerActor.push_back(_ac);
			this->_attaching = true;
			this->_actor->transform->Translate(_ac->transform->position);
			this->_actor->transform->Rotate(_ac->transform->rotation);
			float _dis = glm::distance(this->_actor->transform->position, _editorCamera.transform.position);
			this->_actor->transform->Scale(_dis > 1? glm::vec3(1.0f)*(1+ _dis/15):glm::vec3(1.0f));

			AddCollision();
		}
		else
		{
			this->_visable = false;
			_lowwerActor.clear();
			this->_attaching = false;
			DeleteCollision();
		}
	}

	void AttachObject_Multiple(Actor* _ac)
	{
		
		if (_ac != NULL)
		{
			DeleteCollision();
			this->_visable = true;
			_lowwerActor.push_back(_ac);
			this->_attaching = true;

			glm::vec3 _npos(0.0f);
			for (int i = 0; i < _lowwerActor.size(); i++)
				_npos += _lowwerActor[i]->transform->position;
				
			_npos /= _lowwerActor.size();

			this->_actor->transform->Translate(_npos);

			this->_actor->transform->Rotate(glm::vec3(0.0f));
			float _dis = glm::distance(this->_actor->transform->position, _editorCamera.transform.position);
			this->_actor->transform->Scale(_dis > 1 ? glm::vec3(1.0f)*(1 + _dis / 15) : glm::vec3(1.0f));

			AddCollision();
		}
		else
			return;
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
extern unsigned int _Width, _Height;
struct _PhysicsStrut 
{
	int _index = -1;
	Actor* _actor;
	bool Static = true;
	_PhysicsStrut(int _i, Actor* _a, bool _boo)
	{
		_index = _i;
		_actor = _a;
		Static = _boo;
	}
};


struct World : public CommonRigidBodyBase
{
	unsigned int depthMapFBO;
	unsigned int depthCubemap;
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	
	World(): CommonRigidBodyBase()
	{
		_PlayMode = false;
		initPhysics();
		
		
		// ----------------------- Shadow -----------------------
		CreateDepthMap();
	}
public:
	 bool InitPhysics = true;
	 bool _PlayMode;
	 _Pivot* _piv  = new _Pivot(new Actor());
	virtual ~World()
	{
	
	}
	void UpdateFrame();

	virtual void initPhysics();
	void init_PhysicsProgress();
	void depose_init_PhysicsProgress();
	void CreateDepthMap();
protected:
	std::vector<_PhysicsStrut*> _PhysicsProgress;
};



#endif // !WORLD_H

