#ifndef ACTOR_H
#define ACTOR_H
#include <Component/Transform.h>

class Actor
{
public:
	
	Transform transform;
	bool enabled;
	void printTransform();

	Actor()
	{
	}

	virtual void Draw();

};

#endif // !ACTOR_H





