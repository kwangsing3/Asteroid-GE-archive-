#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H
#include <Units/Actor.h>




class DirectionalLight:public Actor
{
public:
	DirectionalLight()
	{
		transform.name = "DirectionalLight";
	}
	
};




#endif // !DIRECTIONALLIGHT_H



