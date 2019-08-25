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


enum Shape {Plane ,Cube ,Sphere ,Capsule ,Cylinder};
enum RenderMode { RMode2D, RMode3D };

class Meshrender :public Component
{
public:
	Transform *transform;
	glm::vec3 VertexColor;
	Shape _shape;
	RenderMode _mode;
	
	//---------------------------------
	unsigned int VBO, VAO;
	unsigned int Texture;
	virtual void Draw();
	int _index = 0;

	std::vector<glm::vec3> Vectices_Debug;        //相對座標       (基於Meshrender.cpp裡面的CubeVertices)
	std::vector<glm::vec3> Worldvectices_Debug;   //世界座標
	std::vector<glm::vec3> Spacevectices_Debug;   //螢幕座標
	


	Meshrender(int m)
	{
		this->_mode = RenderMode(m);
		this->transform = new Transform();
		this->transform->name = (char*)"Cube";
		this->enabled = true;
		this->CreateCube(_mode);
		this->VertexColor = glm::vec3(1, 1, 1);
		//transform->name = (char*)"Cube";
	}



private:
	void CreateCube(RenderMode _m);







	unsigned int LoadTexture(const char* path);
};


#endif //MESHRENDER_H


