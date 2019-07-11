#include "ADD_Function.h"
#include <Window.h>

#include <Cube.h>



void ADD_Function::Add_Cube()
{
	
	Window::vec_ObjectsToRender.push_back(new Cube());

	printf("Add_Cube");
}



void ADD_Function::Add_DirectionalLight()
{
	printf("Add_DirectionalLight");
}









