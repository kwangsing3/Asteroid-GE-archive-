//
//  DirectionalLight.hpp
//  Asteroid-GameEngine
//
//  Created by KS on 2019/11/20.
//

#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include <Component/Component.hpp>



class DirectionalLight :public Component
{
public:

	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;

	DirectionalLight(Actor* _ac)
	{
		_actor = _ac;

		Ambient = { 1.0f,1.0f,1.0f };
		Diffuse = { 0.4f,0.4f,0.4f };
		Specular = { 0.5f,0.5f,0.5f };
		enabled = true;
	}
	void SaveFile(pugi::xml_node* _node) override;
	void OpenFile(pugi::xml_node* _node) override;
	void Copy(Actor* _actor) override;
};

#endif /* DirectionalLight_hpp */
