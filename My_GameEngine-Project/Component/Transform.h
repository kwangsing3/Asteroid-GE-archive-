#ifndef CLASS_TRANSFORM_H
#define CLASS_TRANSFORM_H
#include<Component/Component.h>
#include <glm/glm.hpp>
#include <iostream>
<<<<<<< HEAD
class Transform:Component
=======
class Class_Transform:Component
>>>>>>> ed1bdf2babff7727a9f89eb22a35a8e9126ad568
{
public:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	std::string name;

<<<<<<< HEAD
	Transform()
=======
	Class_Transform()
>>>>>>> ed1bdf2babff7727a9f89eb22a35a8e9126ad568
	{
		position = glm::vec3(0.0f);
		rotation = glm::vec3(0.0f);
		scale = glm::vec3(1.0f);
		name ="New Actor";
	}





};

#endif // !TRANSFORM_H
