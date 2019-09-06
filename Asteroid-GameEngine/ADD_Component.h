#ifndef ADD_FUNCTION_H
#define ADD_FUNCTION_H
#include  <Units/Actor.h>

class Actor;
class ADD_Component
{


public:
	static Actor* Add_Actor();
	static Meshrender* Add_Meshrender(Actor* _actor, Shape _sha);
	static Meshrender* Add_Pivot(Actor* _actor = nullptr);
	static DirectionalLight* Add_DirectionalLight(Actor* _actor = nullptr);
	static PointLight* Add_PointLight(Actor* _actor = nullptr);


	static BoxCollision* Add_BoxCollision(Actor* _actor = nullptr);







};


#endif // !EVENTSMANAGER_H


