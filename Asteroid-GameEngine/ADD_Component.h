#ifndef ADD_FUNCTION_H
#define ADD_FUNCTION_H
#include  <Units/Actor.h>


class ADD_Component
{


public:
	static Actor* Add_Cube(Actor* _actor = nullptr);
	static Actor* Add_Cube2D(Actor* _actor = nullptr);
	static Actor* Add_DirectionalLight(Actor* _actor = nullptr);
	static Actor* Add_PointLight(Actor* _actor = nullptr);
	static Actor* Add_Actor();

	static Actor* Add_BoxCollision(Actor* _actor = nullptr);
	static Actor* Add_BoxCollision2D(Actor* _actor = nullptr);






};


#endif // !EVENTSMANAGER_H


