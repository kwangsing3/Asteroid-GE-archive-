#ifndef ADD_FUNCTION_H
#define ADD_FUNCTION_H

#include <iostream>
#include <AGE_Model.h>
class Actor;
class Meshrender;
class DirectionalLight;
class PointLight;
class BoxCollision;



class ADD_Component
{

public:
	static Actor* Add_Actor();
	static Meshrender* Add_Meshrender(Actor* _actor, Shape _sha);
	static Meshrender* Add_Meshrender(Actor* _actor, std::string _path);
	static DirectionalLight* Add_DirectionalLight(Actor* _actor = nullptr);
	static PointLight* Add_PointLight(Actor* _actor = nullptr);
	static BoxCollision* Add_BoxCollision(Actor* _actor = nullptr, float mass = 1);
	static Actor* Copy_Actor(Actor* _actor);
};


#endif // !EVENTSMANAGER_H


