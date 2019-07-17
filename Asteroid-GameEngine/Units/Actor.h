#ifndef ACTOR_H
#define ACTOR_H
#include <Component/Transform.h>

class Actor
{
public:
	
	Transform transform;
	DirectionalLight _Dirlight;
	bool enabled;
	void printTransform();
	//bool IsSelected;

	Actor()
	{
		transform.name = (char*)"Actor";
		enabled = true;
		
	}

	virtual void Draw();

};

#endif // !ACTOR_H





