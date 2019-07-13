#ifndef POINTLIGHT_H
#define POINTLIGHT_H
#include <Units/Actor.h>




class PointLight:public Actor
{
public:
	PointLight()
	{
		transform.name = "PointLight";
	}
	
};




#endif // !DIRECTIONALLIGHT_H



