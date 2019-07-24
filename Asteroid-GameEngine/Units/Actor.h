#ifndef ACTOR_H
#define ACTOR_H
#include <Component/Transform.h>

#include <Component/DirectionalLight.h>
#include <Component/PointLight.h>
#include <Units/Cube.h>
class Actor
{
public:
	
	Transform* transform;
	DirectionalLight* _Dirlight;
	PointLight* _PointLight;
	Cube* meshrender;
	bool enabled;
	void translate(glm::vec3 position);

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





