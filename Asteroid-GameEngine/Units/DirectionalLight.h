#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H
#include <Units/Actor.h>




class DirectionalLight:public Actor
{
public:
	DirectionalLight()
	{
		transform.name = (char*) "DirectionalLight";
	}
	
};




#endif // !DIRECTIONALLIGHT_H



