#include "Actor.h"

#include <iostream>

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
}
