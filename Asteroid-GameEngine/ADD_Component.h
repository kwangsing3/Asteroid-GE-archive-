#ifndef ADD_FUNCTION_H
#define ADD_FUNCTION_H
#include  <Units/Actor.h>


class ADD_Component
{


public:
	static Actor* Add_Actor();
	static Meshrender* Add_Meshrender(Actor* _actor = nullptr);
	static Meshrender* Add_Line(Actor * _actor, glm::vec3 from, glm::vec3 to);
	static DirectionalLight* Add_DirectionalLight(Actor* _actor = nullptr);
	static PointLight* Add_PointLight(Actor* _actor = nullptr);


	static BoxCollision* Add_BoxCollision(Actor* _actor = nullptr);







};


#endif // !EVENTSMANAGER_H


