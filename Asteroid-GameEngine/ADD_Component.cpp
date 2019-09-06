
#include<ADD_Component.h>
#include <Component/Meshrender.h>

#include <Component/DirectionalLight.h>
#include <Component/PointLight.h>
#include <Collision/BoxCollision.h>
#include <SceneManager.h>


Actor * ADD_Component::Add_Actor()
{
	Actor* _Actor = new Actor();
	SceneManager::Objects.push_back(_Actor);
	return _Actor;
}

Meshrender* ADD_Component::Add_Meshrender(Actor* _actor)
{
	Actor* _ac = _actor == NULL ? ADD_Component::Add_Actor() : _actor;
	Meshrender* _mesh = new Meshrender(_actor);
	_actor->meshrender = _mesh;
	SceneManager::vec_ObjectsToRender.push_back(_mesh);
	//_ac->transform->name = (char*)"Cube";
	return _mesh;
}

Meshrender * ADD_Component::Add_Pivot(Actor* _actor)
{
	Actor* _ac = _actor == NULL ? ADD_Component::Add_Actor() : _actor;
	Meshrender* _mesh = new Meshrender(_actor,Pivot);
	_actor->meshrender = _mesh;
	_mesh->_shape = Pivot;
	SceneManager::vec_ObjectsToRender.push_back(_mesh);
	//_ac->transform->name = (char*)"Cube";
	return _mesh;
}

DirectionalLight* ADD_Component::Add_DirectionalLight(Actor* _actor)
{
	Actor* _ac = _actor == NULL ? ADD_Component::Add_Actor() : _actor;
	DirectionalLight* _DirLight = new DirectionalLight(_actor);
//	_ac->transform->name = (char*)"Directional Light";
	SceneManager::vec_DirectionlLight.push_back(_DirLight);
	_ac->_Dirlight = _DirLight;
	//_ac->transform->rotation = { -0.2f, -1.0f, -0.3f };
	return _DirLight;
}
PointLight* ADD_Component::Add_PointLight(Actor* _actor)
{
	Actor* _ac = _actor == NULL ? ADD_Component::Add_Actor() : _actor;
	PointLight* _PoLight = new PointLight(_actor);
	//_ac->transform->name = (char*)"Point Light";
	SceneManager::vec_PointLight.push_back(_PoLight);
	_ac->_PointLight = _PoLight;
	return _PoLight;
}
BoxCollision* ADD_Component::Add_BoxCollision(Actor* _actor)
{
	Actor* _ac = _actor == NULL ? ADD_Component::Add_Actor() : _actor;
	BoxCollision* _box = new BoxCollision(_actor);
	//_ac->transform->name = (char*)"Cube";
	_ac->boxcollision = _box;
	
	return _box;
}







