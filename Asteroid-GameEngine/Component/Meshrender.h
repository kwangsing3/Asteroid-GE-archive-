#ifndef MESHRENDER_H
#define MESHRENDER_H
#include <Component/Component.h>
#include <AGE_Model.h>


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
	glm::vec3 VertexColor= glm::vec3(1.0f);


	Meshrender(Actor* _a, Shape _shape)
	{
		this->_actor = _a;
		this->enabled = true;
		_model = new AGE_Model(_shape);
		CreateMouseCollision();
	}
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
	void Draw(Shader* _shader, bool _renderShadow);
private:

	void CreateMouseCollision();

protected:
	btCollisionShape* colShape;
	// Draw Pipeline Variable
	float near_plane = 1.0f;
	float far_plane = 25.0f;
	glm::mat4 shadowProj, lightView;
	glm::mat4 lightSpaceMatrix;
	glm::vec3 lightPos;
	int Light_Length = 3;
	glm::vec3 EulerAngles;
	btDefaultMotionState* myMotionState;
};

#endif //MESHRENDER_H


