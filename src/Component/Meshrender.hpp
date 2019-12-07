//
//  Meshrender.hpp
//  Asteroid-GameEngine
//
//  Created by KS on 2019/11/20.
//

#ifndef MESHRENDER_H
#define MESHRENDER_H
#include <Component/Component.hpp>
#include <AGE_Model.hpp>
#include <glm/gtc/random.hpp>


class btRigidBody;
class btCollisionShape;
class Transform;
class btTransform;
class btDefaultMotionState;
class Shader;
class Actor;
class Meshrender :public Component_Interface
{
public:
	bool _needdebug = false;
	bool _visable = true;
	//---------------------------------
	btRigidBody* body = NULL;
	AGE_Model* _model = NULL;
	glm::vec3 VertexColor = glm::vec3(0.8f);

	Meshrender(Actor* _a)
	{
		this->_actor = _a;
		this->enabled = true;
	}
	Meshrender(Actor* _a, std::string _path)
	{
		this->_actor = _a;
		this->enabled = true;

		_model = new AGE_Model(_path);
		CreateMouseCollision();

	}
	void SaveFile(pugi::xml_node* _node) override;
	void OpenFile(pugi::xml_node* _node) override;
	void Copy(Component_Interface* _information) override;
	void Inspector() override;
	void MoveEvent() { UpdateCollision(); }
	virtual void UpdateCollision();
	void SetVisable(bool _bool);
	void ReSetCollisionFlag();
	void Draw(Shader* _shader);
	void CreateMouseCollision();

private:



protected:
	btCollisionShape* colShape = NULL;
	// Draw Pipeline Variable
	float near_plane = 1.0f;
	float far_plane = 25.0f;
	glm::mat4 _ModelMatrix4 = glm::mat4(0);;
	glm::vec3 EulerAngles = glm::vec3(0,0,0);
	btDefaultMotionState* myMotionState = NULL;
};

#endif //MESHRENDER_H

