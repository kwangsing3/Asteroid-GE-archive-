#ifndef ADD_FUNCTION_H
#define ADD_FUNCTION_H

#include <iostream>
#include <AGE_Model.hpp>

class Actor;
class Meshrender;
class DirectionalLight;
class PointLight;
class BoxCollision;
class SceneManager;


class ADD_Component
{

public:
	SceneManager* _OwnedSceneManager;
	ADD_Component(SceneManager* _sceneManager) { _OwnedSceneManager = _sceneManager; }



	Actor* Add_Actor();
	Meshrender* Add_Meshrender(Actor* _actor, Shape _sha);
	Meshrender* Add_Meshrender(Actor* _actor, std::string _path);
	Meshrender* ADD_CustomMesh(float* vertexs, unsigned int _Length);

	DirectionalLight* Add_DirectionalLight(Actor* _actor = nullptr);
	PointLight* Add_PointLight(Actor* _actor = nullptr);
	BoxCollision* Add_BoxCollision(Actor* _actor = nullptr, float mass = 1);
	Actor* Copy_Actor(Actor* _actor);
};


#endif // !EVENTSMANAGER_H


