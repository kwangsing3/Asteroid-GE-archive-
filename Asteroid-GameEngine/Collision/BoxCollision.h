#ifndef BOXCOLLISION_H
#define BOXCOLLISION_H
#include <Component/Component.h>
#include <Component/Meshrender.h>
#include <vector>



class BoxCollision: public Component
{

public:
	Transform *transform;
	//std::vector<glm::vec3> Vertices;

	std::vector<glm::vec3> Vectices_Debug;        //相對座標       (基於BoxCollision.cpp裡面的Vertices)
	std::vector<glm::vec3> Worldvectices_Debug;   //世界座標
	std::vector<glm::vec3> Spacevectices_Debug;   //螢幕座標
	glm::vec4 _Color;
	unsigned int VBO, VAO;


	BoxCollision()
	{
		transform = new Transform();
		this->enabled = true;
		_Color = { 0,1,0,1 };  //Green
		
		this->CreateBox();




	}

	void Draw() override;

private:
	void CreateBox();
};






#endif // !BOXCOLLISION_H
