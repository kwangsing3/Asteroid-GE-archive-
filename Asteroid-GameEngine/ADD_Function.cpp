#include "ADD_Function.h"
#include <SceneManager.h>

#include <Units/Cube.h>
#include <Units/DirectionalLight.h>
#include <Units/PointLight.h>

void ADD_Function::Add_Cube()
{
	Cube* _NewCube= new Cube();
	SceneManager::vec_ObjectsToRender.push_back(_NewCube);
	SceneManager::Objects.push_back(_NewCube);
	
}



void ADD_Function::Add_DirectionalLight()
{
	DirectionalLight* _NewDirectionlLight = new DirectionalLight();
	SceneManager::vec_DirectionlLight.push_back(_NewDirectionlLight);
	SceneManager::Objects.push_back(_NewDirectionlLight);
}

void ADD_Function::Add_PointLight()
{
	PointLight* _NewPointLight = new PointLight();
	SceneManager::vec_PointLight.push_back(_NewPointLight);
	SceneManager::Objects.push_back(_NewPointLight);
}









