#ifndef BOXCOLLISION_H
#define BOXCOLLISION_H
#include <Component/Component.h>
#include <Component/Meshrender.h>
#include <vector>
#include "btBulletDynamicsCommon.h"


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
	btTransform startTransform;
	int phy_order;

	BoxCollision()
	{
		//transform = new Transform();
		this->enabled = true;
		_Color = { 0,1,0,1 };  //Green
		phy_order = -1;
		this->CreateBox();
		



	}

	void Draw() override;

private:
	void CreateBox();
};






#endif // !BOXCOLLISION_H
