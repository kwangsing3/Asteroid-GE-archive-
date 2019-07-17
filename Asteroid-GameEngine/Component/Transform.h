#ifndef CLASS_TRANSFORM_H
#define CLASS_TRANSFORM_H
#include<Component/Component.h>
#include <glm/glm.hpp>
#include <iostream>
class Transform:public Component
{
public:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	char* name;
	bool enabled;
	Transform()
	{
		position = glm::vec3(0.0f);
		rotation = glm::vec3(0.0f);
		scale = glm::vec3(1.0f);
		name =(char*)"New Actor";
		enabled = true;
	}





};

#endif // !TRANSFORM_H
