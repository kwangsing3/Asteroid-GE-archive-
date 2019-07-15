#ifndef ACTOR_H
#define ACTOR_H
#include <Component/Transform.h>

class Actor
{
public:
	
	Transform transform;
	bool enabled;
	void printTransform();
	//bool IsSelected;

	Actor()
	{
		transform.name = (char*)"Actor";
		enabled = true;
	//	IsSelected = false;
	}

	virtual void Draw();

};

#endif // !ACTOR_H





