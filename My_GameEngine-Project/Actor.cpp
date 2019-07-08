#include "Actor.h"

#include <iostream>

Class_Transform Transform;


void Actor::printTransform()
{
	std::cout << "PositionX: " << Transform.position.x << "PositionY: " << Transform.position.y << "PositionZ: " << Transform.position.z << std::endl;
	std::cout << "ScaleX: " << Transform.scale.x << "ScaleY: " << Transform.scale.y << "ScaleZ: " << Transform.scale.z << std::endl;
}
