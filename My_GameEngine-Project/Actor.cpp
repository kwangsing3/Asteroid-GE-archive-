#include "Actor.h"

#include <iostream>

<<<<<<< HEAD
Transform transform;
bool enbaled = true;

void Actor::printTransform()
{
	std::cout << "PositionX: " << transform.position.x << "PositionY: " << transform.position.y << "PositionZ: " << transform.position.z << std::endl;
	std::cout << "ScaleX: " << transform.scale.x << "ScaleY: " << transform.scale.y << "ScaleZ: " << transform.scale.z << std::endl;
}

void Actor::Draw()
{
	printf("draw from actor");
=======
Class_Transform Transform;


void Actor::printTransform()
{
	std::cout << "PositionX: " << Transform.position.x << "PositionY: " << Transform.position.y << "PositionZ: " << Transform.position.z << std::endl;
	std::cout << "ScaleX: " << Transform.scale.x << "ScaleY: " << Transform.scale.y << "ScaleZ: " << Transform.scale.z << std::endl;
>>>>>>> ed1bdf2babff7727a9f89eb22a35a8e9126ad568
}
