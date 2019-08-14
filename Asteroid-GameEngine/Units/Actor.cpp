#include <Units/Actor.h>

#include <iostream>

Transform transform;
bool enbaled = true;


void Actor::Draw()
{
	printf("draw from actor");
}
void Actor::translate(glm::vec3 position)
{
	this->transform->position = position;
}