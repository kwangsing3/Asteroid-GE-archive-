#ifndef WORLD_H
#define WORLD_H
#include <AGE_Assert.hpp>
#include "btBulletDynamicsCommon.h"
#include <GraphicEngine/GLDebugDrawer.hpp>
#include <CommonInterfaces/CommonRigidBodyBase.h>
#include <Camera.hpp>

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>


#include <ADD_Component.hpp>
#include <Actor.hpp>

#include <Component/Meshrender.hpp>
#include <Component/Transform.hpp>
// 為了方便釐清， 先做新的Class 來當作Pivot

class Camera;

struct World : public CommonRigidBodyBase   //是一種Scene
{
	unsigned int depthMapFBO_PoLight;
	unsigned int depthCubemap = 0;
	unsigned int depthMapFBO_DirLight = 0;
	unsigned int depthTexture_DirLight = 0;
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	virtual void initPhysics();
	
public:
	World() : CommonRigidBodyBase()
	{
		_PlayMode = false;
		initPhysics();
		CreateDepthMap();// ------- Shadow -------
	}
	bool InitPhysics = true;
	bool _PlayMode;
	bool _RenderShadow = true;
	class _Pivot : public AGE_Model
	{

	public:
		btRigidBody* body[3] = { NULL,NULL ,NULL };
		Actor* _actor = NULL;
		bool _visable = false;
		std::vector<Actor*> _lowwerActor;
		bool _attaching = false;
		bool Drag[3] = { false,false,false };
		bool _DragMode[3] = { false,false,false };
		bool _needdebug = true;
		std::vector<btCollisionShape*> colshape;
		int _group = 1, _mask = 1;
		World* world_this = NULL;
		unsigned int VBO, VAO;
		_Pivot(Actor* _a, World* _world) :world_this(_world)
		{
			SwitchDragMode(0);
			_actor = _a;
			_actor->transform->name = "Pivot";
			this->CreatePivot();
			CreateMouseCollision();
		}
		void CreatePivot()
		{
			_shape = Shape::DEBUG;
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

			// vec_ShaderProgram[0]->use();
			//SceneManager::vec_ShaderProgram[0]->setInt("material.diffuse", 0);
			//Worldvectices_Debug = Spacevectices_Debug = Vectices_Debug;


			//SceneManager::AddToRenderPipeline(this);
		}
		void Draw(Shader* _shader, bool _renderShadow)// override
		{
			
		}
		void Translate(glm::vec3 _pos)
		{
			Transform* _trans;
		


			if (_lowwerActor.size() > 1)
			{
				for (int i = 0; i < _lowwerActor.size(); i++)
				{
					_trans->Translate(this->_lowwerActor[i]->transform->position + (_pos - this->_actor->transform->position));
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
			//_thisWorld->_SceneManager.NeedInitedDraw = true;
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
			//_thisWorld->_SceneManager.NeedInitedDraw = true;
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
			//_thisWorld->_SceneManager.NeedInitedDraw = true;
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
				float _dis = glm::distance(this->_actor->transform->position, world_this->_editorCamera->transform.position);
				this->_actor->transform->Scale(_dis > 1 ? glm::vec3(1.0f) * (1 + _dis / 15) : glm::vec3(1.0f));

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
				float _dis = glm::distance(this->_actor->transform->position, world_this->_editorCamera->transform.position);
				this->_actor->transform->Scale(_dis > 1 ? glm::vec3(1.0f) * (1 + _dis / 15) : glm::vec3(1.0f));

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
		void UpdateCollision();
		void DeleteCollision();
		void AddCollision();


	};
	_Pivot* _piv = NULL;
	Camera* _editorCamera;
	SceneManager* _SceneManager;
	struct _PhysicsStruct
	{
		int _index = -1;
		Actor* _actor;
		bool Static = true;
		_PhysicsStruct(int _i, Actor* _a, bool _boo) :_index(_i), _actor(_a), Static(_boo) {}
	};
	void UpdateFrame();
	void init_PhysicsProgress();
	void depose_init_PhysicsProgress();
	void CreateDepthMap();
protected:
	std::vector<_PhysicsStruct*> _PhysicsProgress;
};


#endif // !WORLD_H