#include "ADD_Function.h"



#include <Component/DirectionalLight.h>
#include <Component/PointLight.h>

Cube* ADD_Function::Add_Cube()
{
	Cube* _NewCube= new Cube();
	SceneManager::vec_ObjectsToRender.push_back(_NewCube);
	SceneManager::Objects.push_back(_NewCube);
	return _NewCube;
}


//現在做的東西都是新增一個Object 還沒有做到新增Component
Actor* ADD_Function::Add_DirectionalLight()    
{
	Actor* _NewDirectionlLight = new Actor();
	_NewDirectionlLight->_Dirlight = new DirectionalLight();
	_NewDirectionlLight->transform->name = (char*)"DirectionalLight";
	_NewDirectionlLight->transform =_NewDirectionlLight->_Dirlight->transform ;
	SceneManager::vec_DirectionlLight.push_back(_NewDirectionlLight->_Dirlight);
	SceneManager::Objects.push_back(_NewDirectionlLight);
	return _NewDirectionlLight;
}

Actor* ADD_Function::Add_PointLight()
{
	Actor* _NewPointLight = new Actor();
	_NewPointLight->_PointLight = new PointLight();
	_NewPointLight->transform->name = (char*)"PointLight";
	_NewPointLight->transform = _NewPointLight->_PointLight->transform;
	SceneManager::vec_PointLight.push_back(_NewPointLight->_PointLight);
	SceneManager::Objects.push_back(_NewPointLight);
	return _NewPointLight;
}

Actor * ADD_Function::Add_Actor()
{
	Actor* _Actor = new Actor();
	SceneManager::Objects.push_back(_Actor);
	return _Actor;
}








