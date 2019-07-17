#include "ADD_Function.h"
#include <SceneManager.h>

#include <Units/Cube.h>
#include <Component/DirectionalLight.h>
#include <Component/PointLight.h>

void ADD_Function::Add_Cube()
{
	Cube* _NewCube= new Cube();
	SceneManager::vec_ObjectsToRender.push_back(_NewCube);
	SceneManager::Objects.push_back(_NewCube);
	
}



void ADD_Function::Add_DirectionalLight()
{
	Actor* _NewDirectionlLight = new Actor();
	_NewDirectionlLight->_Dirlight = new DirectionalLight();
	_NewDirectionlLight->transform->name = (char*)"DirectionalLight";
	_NewDirectionlLight->transform =_NewDirectionlLight->_Dirlight->transform ;
	SceneManager::vec_DirectionlLight.push_back(_NewDirectionlLight->_Dirlight);
	SceneManager::Objects.push_back(_NewDirectionlLight);
}

void ADD_Function::Add_PointLight()
{
	Actor* _NewPointLight = new Actor();
	_NewPointLight->_PointLight = new PointLight();
	_NewPointLight->transform->name = (char*)"PointLight";
	_NewPointLight->transform = _NewPointLight->_PointLight->transform;
	SceneManager::vec_PointLight.push_back(_NewPointLight->_PointLight);
	SceneManager::Objects.push_back(_NewPointLight);
}









