#ifndef BOXCOLLISION_H
#define BOXCOLLISION_H
#include <Component/Component.h>
#include <Component/Meshrender.h>
#include <vector>

class btRigidBody;

class BoxCollision: public Component
{

public:
	
	//std::vector<glm::vec3> Vertices;


	//glm::vec4 _Color;
	unsigned int VBO, VAO;
	//btTransform startTransform;
	//int phy_order;    //直接從Dynamic World 的Collision vector中 找到相應的對象
	float _Mass;
	int _Mask;
	int _Group;
	btRigidBody* body;   //因為需要跨function用到 所以另外拉出來宣告
	bool _needdebug = false;
	BoxCollision(Actor* _a)
	{
		
		_actor = _a;
		this->enabled = true;
		//_Color = { 0,1,0,1 };  //Green
		//phy_order = -1;
		_Mass = 1.0f;
		_Mask = _Group = 2;
		this->CreateBox();
	}
	
	void SaveFile(pugi::xml_node _node) override;
	void OpenFile(pugi::xml_node _node) override;

	void UpdateCollision();
	
	void UpdateState();

private:
	
	void CreateBox();
};






#endif // !BOXCOLLISION_H
