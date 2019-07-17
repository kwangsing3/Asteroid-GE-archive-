#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include <Component/Transform.h>
#include <Component/Component.h>


class PointLight:public Component
{
public:
	
	Transform* transform;
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;
	float Constant;
	float linear;
	float quadratic;
	PointLight()
	{
		transform = new Transform();
		transform->name = (char*) "PointLight";
		enabled = true;
		Ambient = { 0.05f, 0.05f, 0.05f };
		Diffuse = { 0.8f, 0.8f, 0.8f };
		Specular = { 1.0f, 1.0f, 1.0f };
		Constant = 1.0f;
		linear = 0.09f;
		quadratic = 0.032f;
	}
	
};




#endif // !DIRECTIONALLIGHT_H



