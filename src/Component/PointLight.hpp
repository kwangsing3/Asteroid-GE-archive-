//
//  PointLight.hpp
//  Asteroid-GameEngine
//
//  Created by KS on 2019/11/20.
//
#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include <Component/Component.hpp>



class PointLight :public Component_Interface
{
public:


	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;
	float Constant;
	float linear;
	float quadratic;

	PointLight(Actor* _ac)
	{
		_actor = _ac;

		enabled = true;
		Ambient = { 0.05f, 0.05f, 0.05f };
		Diffuse = { 0.8f, 0.8f, 0.8f };
		Specular = { 1.0f, 1.0f, 1.0f };
		Constant = 1.0f;
		linear = 0.09f;
		quadratic = 0.032f;
	}
	void SaveFile(pugi::xml_node* _node) override;
	void OpenFile(pugi::xml_node* _node) override;
	void Copy(Component_Interface* _information) override;
	void Inspector() override;
	void MoveEvent() {};
};




#endif // !DIRECTIONALLIGHT_H



