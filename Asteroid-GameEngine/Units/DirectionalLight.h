#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H
#include <Units/Actor.h>




class DirectionalLight:public Actor
{
public:
	float ambient;
	DirectionalLight()
	{
		transform.name = (char*) "DirectionalLight";
		ambient = 1.0f;
	}
	
};




#endif // !DIRECTIONALLIGHT_H



