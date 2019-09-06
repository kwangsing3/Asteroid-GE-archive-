#ifndef ACTOR_H
#define ACTOR_H
#include <Component/Transform.h>
#include <Component/DirectionalLight.h>
#include <Component/PointLight.h>
#include <Component/Meshrender.h>
#include <Collision/BoxCollision.h>


class Actor
{
public:
	
	Transform* transform;
	DirectionalLight* _Dirlight;
	PointLight* _PointLight;
	Meshrender* meshrender;
	BoxCollision* boxcollision;
	bool enabled;
	
	void removeActor();
	//bool IsSelected;

	Actor()
	{
		transform = new Transform();
		transform->_actor = this;
		transform->name = (char*)"Actor";
		enabled = true;
		


	}

	virtual void Draw();
private:

};

#endif // !ACTOR_H





