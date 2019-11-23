#ifndef BOXCOLLISION_H
#define BOXCOLLISION_H
#include <Component/Component.hpp>

class btRigidBody;
class btCollisionShape;
class btTransform;
class BoxCollision: public Component
{

public:

	//btTransform startTransform;
	//int phy_order;    //直接從Dynamic World 的Collision vector中 找到相應的對象
	float _Mass = 1.0f;
	int _Mask = 2;
	int _Group = 2;
	btRigidBody* body;   
	
	bool _needdebug = false;

	BoxCollision(Actor* _a)
	{
		this->_actor = _a;
		this->enabled = true;
		this->CreateBox();
	}	
	BoxCollision(Actor* _a, float _mas)
	{
		this->_actor = _a;
		this->enabled = true;
		this->_Mass = _mas;
		this->CreateBox();
	}
	void SaveFile( pugi::xml_node* _node) override;
	void OpenFile( pugi::xml_node* _node) override;
	void Copy(Actor* _actor) override;
	void UpdateCollision();
	void ReSetCollisionFlag();
private:
	void CreateBox();
protected:
	btCollisionShape* colShape;

};






#endif // !BOXCOLLISION_H
