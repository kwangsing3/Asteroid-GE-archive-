//
//  PointLight.cpp
//  Asteroid-GameEngine
//
//  Created by KS on 2019/11/20.
//

#include <Component/PointLight.hpp>
#include <Actor.hpp>
//#include <Xml/pugixml.hpp>

void PointLight::SaveFile(pugi::xml_node* _node)
{
	if (_node == NULL || this->_actor->_PointLight == NULL) return;
	_node->append_attribute("_PointLight") = 1;
	pugi::xml_node Amb = _node->append_child("Ambient");
	Amb.append_attribute("X") = this->_actor->_PointLight->Ambient.x;
	Amb.append_attribute("Y") = this->_actor->_PointLight->Ambient.y;
	Amb.append_attribute("Z") = this->_actor->_PointLight->Ambient.z;
	pugi::xml_node Dif = _node->append_child("Diffuse");
	Dif.append_attribute("X") = this->_actor->_PointLight->Diffuse.x;
	Dif.append_attribute("Y") = this->_actor->_PointLight->Diffuse.y;
	Dif.append_attribute("Z") = this->_actor->_PointLight->Diffuse.z;
	pugi::xml_node Spe = _node->append_child("Specular");
	Spe.append_attribute("X") = this->_actor->_PointLight->Specular.x;
	Spe.append_attribute("Y") = this->_actor->_PointLight->Specular.y;
	Spe.append_attribute("Z") = this->_actor->_PointLight->Specular.z;


	_node->append_child("Constant").append_attribute("Constant") = this->_actor->_PointLight->Constant;
	_node->append_child("linear").append_attribute("linear") = this->_actor->_PointLight->linear;
	_node->append_child("quadratic").append_attribute("quadratic") = this->_actor->_PointLight->quadratic;
}

void PointLight::OpenFile(pugi::xml_node* _node)
{
	if (_node == NULL || this->_actor->_PointLight == NULL) return;

	pugi::xml_node Amb = _node->child("Ambient");
	this->_actor->_PointLight->Ambient.x = Amb.attribute("X").as_float();
	this->_actor->_PointLight->Ambient.y = Amb.attribute("Y").as_float();
	this->_actor->_PointLight->Ambient.z = Amb.attribute("Z").as_float();
	pugi::xml_node Dif = _node->child("Diffuse");
	this->_actor->_PointLight->Diffuse.x = Dif.attribute("X").as_float();
	this->_actor->_PointLight->Diffuse.y = Dif.attribute("Y").as_float();
	this->_actor->_PointLight->Diffuse.z = Dif.attribute("Z").as_float();
	pugi::xml_node Spe = _node->child("Specular");
	this->_actor->_PointLight->Specular.x = Spe.attribute("X").as_float();
	this->_actor->_PointLight->Specular.y = Spe.attribute("Y").as_float();
	this->_actor->_PointLight->Specular.z = Spe.attribute("Z").as_float();


}

void PointLight::Copy(Actor* _actor)
{
	if (_actor == NULL || _actor->_PointLight == NULL) return;
	this->enabled = _actor->_PointLight->enabled;
	//-----Component-----
	this->Ambient = _actor->_PointLight->Ambient;
	this->Diffuse = _actor->_PointLight->Diffuse;
	this->Specular = _actor->_PointLight->Specular;

	this->Constant = _actor->_PointLight->Constant;
	this->linear = _actor->_PointLight->linear;
	this->quadratic = _actor->_PointLight->quadratic;

}
