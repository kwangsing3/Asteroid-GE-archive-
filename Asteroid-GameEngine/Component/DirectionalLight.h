#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H


#include <Component/Transform.h>
#include <Component/Component.h>

class DirectionalLight:public Component
{
public:
	
	Transform* transform;
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;
	
	DirectionalLight()
	{
		transform = new Transform();
		transform->name = (char*) "DirectionalLight";
		transform->rotation ={-0.2f, -1.0f, -0.3f};
		Ambient = {1.0f,1.0f,1.0f};
		Diffuse = {0.4f,0.4f,0.4f};
		Specular = {0.5f,0.5f,0.5f};
		enabled = true;
	}
	
};




#endif // !DIRECTIONALLIGHT_H



