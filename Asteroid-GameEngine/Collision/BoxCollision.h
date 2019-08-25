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


	glm::vec4 _Color;
	unsigned int VBO, VAO;
	//btTransform startTransform;
	int phy_order;    //直接從Dynamic World 的Collision vector中 找到相應的對象
	float _Mass;



	BoxCollision()
	{
		transform = new Transform();
		this->enabled = true;
		_Color = { 0,1,0,1 };  //Green
		phy_order = -1;

		this->CreateBox();
		_Mass = 0.0f;



	}
	void ResetDynamic();



private:
	btRigidBody* body;
	void CreateBox();
};






#endif // !BOXCOLLISION_H
