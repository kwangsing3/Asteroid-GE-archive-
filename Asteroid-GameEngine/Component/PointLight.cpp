#include <Component/PointLight.h>
#include <Units/Actor.h>

void PointLight::SaveFile(pugi::xml_node _node)
{
	if (_node == NULL || this->_actor->_PointLight == NULL) return;
	_node.append_attribute("_PointLight") = 1;
	pugi::xml_node Amb = _node.append_child("Ambient");
	Amb.append_attribute("X") = this->_actor->_PointLight->Ambient.x;
	Amb.append_attribute("Y") = this->_actor->_PointLight->Ambient.y;
	Amb.append_attribute("Z") = this->_actor->_PointLight->Ambient.z;
	pugi::xml_node Dif = _node.append_child("Diffuse");
	Dif.append_attribute("X") = this->_actor->_PointLight->Diffuse.x;
	Dif.append_attribute("Y") = this->_actor->_PointLight->Diffuse.y;
	Dif.append_attribute("Z") = this->_actor->_PointLight->Diffuse.z;
	pugi::xml_node Spe = _node.append_child("Specular");
	Spe.append_attribute("X") = this->_actor->_PointLight->Specular.x;
	Spe.append_attribute("Y") = this->_actor->_PointLight->Specular.y;
	Spe.append_attribute("Z") = this->_actor->_PointLight->Specular.z;


	_node.append_child("Constant").append_attribute("Constant") = this->_actor->_PointLight->Constant;
	_node.append_child("linear").append_attribute("linear") = this->_actor->_PointLight->linear;
	_node.append_child("quadratic").append_attribute("quadratic") = this->_actor->_PointLight->quadratic;
}

void PointLight::OpenFile(pugi::xml_node _node)
{
	if (_node == NULL || this->_actor->_PointLight == NULL) return;
}
