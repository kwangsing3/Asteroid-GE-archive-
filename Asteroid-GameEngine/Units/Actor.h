#ifndef ACTOR_H
#define ACTOR_H
#include <Component/Transform.h>

#include <Component/DirectionalLight.h>
#include <Component/PointLight.h>

class Actor
{
public:
	
	Transform* transform;
	DirectionalLight* _Dirlight;
	PointLight* _PointLight;
	bool enabled;


	//bool IsSelected;

	Actor()
	{
		transform = new Transform();
		transform->name = (char*)"Actor";
		enabled = true;
		

	}

	virtual void Draw();

};

#endif // !ACTOR_H





