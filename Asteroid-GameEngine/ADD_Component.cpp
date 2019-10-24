
#include<ADD_Component.h>
#include <Units/Actor.h>
#include <Component/Meshrender.h>
#include <Component/DirectionalLight.h>
#include <Component/PointLight.h>
#include <Component/BoxCollision.h>

#include <SceneManager.h>



Actor * ADD_Component::Add_Actor()
{
	Actor* _Actor = new Actor();
	SceneManager::Objects.push_back(_Actor);
	return _Actor;
}

Meshrender* ADD_Component::Add_Meshrender(Actor* _actor,Shape _sha)
{
	Actor* _ac = _actor == NULL ? ADD_Component::Add_Actor() : _actor;
	Meshrender* _mesh = new Meshrender(_actor, _sha);
	_ac->meshrender = _mesh;
	if(_sha== Shape::_Model)
		SceneManager::AddToRenderPipeline(_mesh);
	else
		SceneManager::AddToRenderPipeline_Instancing(_mesh);
	
	return _mesh;
}

Meshrender * ADD_Component::Add_Meshrender(Actor * _actor, std::string _path)
{
	Actor* _ac = _actor == NULL ? ADD_Component::Add_Actor() : _actor;
	Meshrender* _mesh = new Meshrender(_actor, _path);
	_ac->meshrender = _mesh;
	if (_mesh->_model->_shape == Shape::_Model)
		SceneManager::AddToRenderPipeline(_mesh);
	else
		SceneManager::AddToRenderPipeline_Instancing(_mesh);
	
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
BoxCollision* ADD_Component::Add_BoxCollision(Actor* _actor, float mass)
{
	Actor* _ac = _actor == NULL ? ADD_Component::Add_Actor() : _actor;
	if (_ac->meshrender == NULL) ADD_Component::Add_Meshrender(_ac, Shape::Cube);
	BoxCollision* _box = new BoxCollision(_actor, mass);
	//_ac->transform->name = (char*)"Cube";
	_ac->boxcollision = _box;
	
	return _box;
}

Actor* ADD_Component::Copy_Actor(Actor* _actor)
{
	if (_actor == NULL) return NULL;
	Actor* _ac =  ADD_Component::Add_Actor();
	_ac->transform->Copy(_actor);    

	if (_actor->meshrender != NULL)
		ADD_Component::Add_Meshrender(_ac, _actor->meshrender->_model->_shape)->Copy(_actor);
	if (_actor->_Dirlight != NULL)
		ADD_Component::Add_DirectionalLight(_ac)->Copy(_actor);
	if (_actor->_PointLight != NULL)
		ADD_Component::Add_PointLight(_ac)->Copy(_actor);
	if (_actor->boxcollision != NULL)
		ADD_Component::Add_BoxCollision(_ac, _actor->boxcollision->_Mass)->Copy(_actor);


	return _ac;
}



