#ifndef MESHRENDER_H
#define MESHRENDER_H
#include <Component/Component.h>
#include <AGE_Model.h>
#include <glm/gtc/random.hpp>

class btRigidBody;
class btCollisionShape;
class Transform;
class btTransform;
class btDefaultMotionState;
class Shader;
class Actor;
class Meshrender :public Component
{
public:
	bool _needdebug = false;
	bool _visable = true;
	//---------------------------------
	btRigidBody* body;	
	AGE_Model* _model;
	glm::vec3 VertexColor= glm::vec3(0.8f);

	Meshrender(Actor* _a)
	{
		this->_actor = _a;
		this->enabled = true;

	}
	/*Meshrender(Actor* _a, Shape _shape)
	{
		this->_actor = _a;
		this->enabled = true;
		_model = new AGE_Model(_shape);
		CreateMouseCollision();
	}*/
	Meshrender(Actor* _a, std::string _path)
	{
		this->_actor = _a;
		this->enabled = true;

		_model = new AGE_Model(_path);
		CreateMouseCollision();

	}
	void SaveFile( pugi::xml_node* _node) override;
	void OpenFile( pugi::xml_node* _node) override;
	void Copy(Actor* _actor) override;




	virtual void UpdateCollision();
	void SetVisable(bool _bool);
	void ReSetCollisionFlag();
	void Draw(Shader* _shader);
	void CreateMouseCollision();
private:

	

protected:
	btCollisionShape* colShape;
	// Draw Pipeline Variable
	float near_plane = 1.0f;
	float far_plane = 25.0f;
	glm::mat4 _ModelMatrix4;
	glm::vec3 EulerAngles;
	btDefaultMotionState* myMotionState;
};

#endif //MESHRENDER_H


