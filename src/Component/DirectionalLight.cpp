//
//  DirectionalLight.cpp
//  Asteroid-GameEngine
//
//  Created by KS on 2019/11/20.
//

#include <Component/DirectionalLight.hpp>
#include <Actor.hpp>
//#include <Xml/pugixml.hpp>
void DirectionalLight::SaveFile(pugi::xml_node* _node)
{
	if (_node == NULL) return;
	_node->append_attribute("_Dirlight") = 1;
	pugi::xml_node Amb = _node->append_child("Ambient");
	Amb.append_attribute("X") = this->Ambient.x;
	Amb.append_attribute("Y") = this->Ambient.y;
	Amb.append_attribute("Z") = this->Ambient.z;
	pugi::xml_node Dif = _node->append_child("Diffuse");
	Dif.append_attribute("X") = this->Diffuse.x;
	Dif.append_attribute("Y") = this->Diffuse.y;
	Dif.append_attribute("Z") = this->Diffuse.z;
	pugi::xml_node Spe = _node->append_child("Specular");
	Spe.append_attribute("X") = this->Specular.x;
	Spe.append_attribute("Y") = this->Specular.y;
	Spe.append_attribute("Z") = this->Specular.z;
}

void DirectionalLight::OpenFile(pugi::xml_node* _node)
{
	if (_node == NULL) return;

	pugi::xml_node Amb = _node->child("Ambient");
	this->Ambient.x = Amb.attribute("X").as_float();
	this->Ambient.y = Amb.attribute("Y").as_float();
	this->Ambient.z = Amb.attribute("Z").as_float();
	pugi::xml_node Dif = _node->child("Diffuse");
	this->Diffuse.x = Dif.attribute("X").as_float();
	this->Diffuse.y = Dif.attribute("Y").as_float();
	this->Diffuse.z = Dif.attribute("Z").as_float();
	pugi::xml_node Spe = _node->child("Specular");
	this->Specular.x = Spe.attribute("X").as_float();
	this->Specular.y = Spe.attribute("Y").as_float();
	this->Specular.z = Spe.attribute("Z").as_float();
}

void DirectionalLight::Copy(Component_Interface* _information)
{
	if (_information == NULL) return;




}
