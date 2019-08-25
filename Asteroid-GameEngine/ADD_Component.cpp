
#include <ADD_Component.h>
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

Actor* ADD_Component::Add_Cube(Actor* _actor)
{
	if (_actor)// 經由按鈕製造的項目會跑這行
	{
		if (_actor->meshrender == NULL)
		{
			_actor->meshrender = new Meshrender(1);

			SceneManager::vec_ObjectsToRender.push_back(_actor->meshrender);
		}
		_actor->meshrender->transform = _actor->transform;
		
		return _actor;
	}

	Actor* _newactor = ADD_Component::Add_Actor();
	_newactor->meshrender = new Meshrender(1);
	_newactor->transform->name = (char*)"Cube";

	SceneManager::vec_ObjectsToRender.push_back(_newactor->meshrender);

	return _newactor;
}
Actor* ADD_Component::Add_Cube2D(Actor* _actor)
{
	if (_actor)
	{
		_actor->meshrender = new Meshrender(0);

		SceneManager::vec_ObjectsToRender.push_back(_actor->meshrender);
		_actor->meshrender->transform = _actor->transform;
		return _actor;
	}
	Actor* _newactor = ADD_Component::Add_Actor();
	_newactor->meshrender = new Meshrender(0);
	_newactor->transform->name = (char*)"Cube";

	SceneManager::vec_ObjectsToRender.push_back(_newactor->meshrender);
	return _newactor;
}
Actor* ADD_Component::Add_DirectionalLight(Actor* _actor)
{
	if (_actor)
	{
		_actor->_Dirlight = new DirectionalLight();
		_actor->_Dirlight->transform = _actor->transform;
		SceneManager::vec_DirectionlLight.push_back(_actor->_Dirlight);
		return _actor;
	}
	Actor* _newactor = ADD_Component::Add_Actor();
	_newactor->_Dirlight = new DirectionalLight();
	_newactor->transform->name = (char*)"DirectionalLight";
	
	SceneManager::vec_DirectionlLight.push_back(_newactor->_Dirlight);
	return _newactor;
}
Actor* ADD_Component::Add_PointLight(Actor* _actor)
{
	if (_actor)
	{
		_actor->_PointLight = new PointLight();
		_actor->_PointLight->transform = _actor->transform;
		SceneManager::vec_PointLight.push_back(_actor->_PointLight);
		return _actor;
	}
	Actor* _newactor = ADD_Component::Add_Actor();
	_newactor->_PointLight = new PointLight();
	_newactor->transform->name = (char*)"PointLight";

	SceneManager::vec_PointLight.push_back(_newactor->_PointLight);
	return _newactor;
}
Actor* ADD_Component::Add_BoxCollision(Actor* _actor)
{
	if (_actor)
	{
		_actor->boxcollision = new BoxCollision();
		_actor->boxcollision->transform = _actor->transform;
		_actor->boxcollision->ResetDynamic();
		
		return _actor;
	}

	Actor* _newactor = ADD_Component::Add_Actor();
	_newactor->boxcollision = new BoxCollision();
	_newactor->transform->name = (char*)"Cube (With Collision)";
	_newactor->boxcollision->ResetDynamic();
	return _newactor;
}

Actor* ADD_Component::Add_BoxCollision2D(Actor* _actor)
{

	return NULL;
}






