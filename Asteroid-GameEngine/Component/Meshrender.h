#ifndef MESHRENDER_H
#define MESHRENDER_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Component/Component.h>
#include <Component/Transform.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//#include <Window.h>
//#include <SceneManager.h>

#include "btBulletDynamicsCommon.h"
class Shader;

enum Shape {Plane ,Cube ,Sphere ,Capsule ,Cylinder, Line, Pivot};
enum RenderMode { RMode2D, RMode3D };

class Meshrender :public Component
{
public:
	
	glm::vec3 VertexColor;
	Shape _shape;
	RenderMode _mode;
	
	//---------------------------------
	unsigned int VBO, VAO;
	unsigned int Texture;
	void Draw(Shader _shader);
	bool _needdebug = false;
	

	std::vector<glm::vec3> Vectices_Debug;        //相對座標       (基於Meshrender.cpp裡面的CubeVertices)
	std::vector<glm::vec3> Worldvectices_Debug;   //世界座標
	std::vector<glm::vec3> Spacevectices_Debug;   //螢幕座標
	btRigidBody* body;
	Meshrender(Actor* _a)
	{
		_actor = _a;
		this->_mode = RenderMode(1);
		this->enabled = true;
		this->CreateCube(_mode);
		this->VertexColor = glm::vec3(1, 1, 1);
		
		//transform->name = (char*)"Cube";
	    //  滑鼠判定的碰撞體
		CreateMouseCollision();
	}

	Meshrender(Actor* _a, glm::vec3 from, glm::vec3  to)
	{
		_actor = _a;
		this->_mode = RenderMode(1);
		this->enabled = true;
		this->CreateLine(from, to, glm::vec3(1,0,0));
		this->VertexColor = glm::vec3(1, 1, 1);

		//transform->name = (char*)"Cube";
		//  滑鼠判定的碰撞體
		//CreateMouseCollision();
	}
	Meshrender(Actor* _a, Shape _shape)
	{
		_actor = _a;
		this->_mode = RenderMode(1);
		this->enabled = true;
		this->CreateShape(_shape);
		this->VertexColor = glm::vec3(1, 1, 1);
		CreatePivotVollision();
		//transform->name = (char*)"Cube";
		//  滑鼠判定的碰撞體
		//CreateMouseCollision();
	}
	
	void SaveFile(pugi::xml_node _node) override;
	void OpenFile(pugi::xml_node _node) override;
	void UpdateCollision();

private:
	
	void CreateCube(RenderMode _m);
	void CreateLine(glm::vec3 from, glm::vec3  to, glm::vec3 color);
	unsigned int LoadTexture(const char* path);
	void CreateMouseCollision();
	void CreateShape(Shape _shape);
	void CreatePivotVollision();
};

#endif //MESHRENDER_H


//  現在正在為物體製作滑鼠點選的碰撞體