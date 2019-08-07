#ifndef MESHRENDER_H
#define MESHRENDER_H

#include <Component/Component.h>
#include <Component/Transform.h>




class Meshrender :public Component
{
public:
	Transform *transform ;
	Meshrender()
	{
	//	transform = new Transform();
	}
};


#endif //MESHRENDER_H


