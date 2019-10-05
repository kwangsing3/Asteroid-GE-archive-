#include <Component/DirectionalLight.h>
#include <Units/Actor.h>
//#include <Xml/pugixml.hpp>
void DirectionalLight::SaveFile(pugi::xml_node* _node)
{
	if (_node == NULL|| this->_actor->_Dirlight==NULL) return;
	_node->append_attribute("_Dirlight") = 1;
	pugi::xml_node Amb = _node->append_child("Ambient");
	Amb.append_attribute("X") = this->_actor->_Dirlight->Ambient.x;
	Amb.append_attribute("Y") = this->_actor->_Dirlight->Ambient.y;
	Amb.append_attribute("Z") = this->_actor->_Dirlight->Ambient.z;
	pugi::xml_node Dif = _node->append_child("Diffuse");
	Dif.append_attribute("X") = this->_actor->_Dirlight->Diffuse.x;
	Dif.append_attribute("Y") = this->_actor->_Dirlight->Diffuse.y;
	Dif.append_attribute("Z") = this->_actor->_Dirlight->Diffuse.z;
	pugi::xml_node Spe = _node->append_child("Specular");
	Spe.append_attribute("X") = this->_actor->_Dirlight->Specular.x;
	Spe.append_attribute("Y") = this->_actor->_Dirlight->Specular.y;
	Spe.append_attribute("Z") = this->_actor->_Dirlight->Specular.z;
}

void DirectionalLight::OpenFile(pugi::xml_node* _node)
{
	if (_node == NULL || this->_actor->_Dirlight == NULL) return;
	
	pugi::xml_node Amb = _node->child("Ambient");
	this->_actor->_Dirlight->Ambient.x = Amb.attribute("X").as_float();
	this->_actor->_Dirlight->Ambient.y = Amb.attribute("Y").as_float();
	this->_actor->_Dirlight->Ambient.z = Amb.attribute("Z").as_float();
	pugi::xml_node Dif = _node->child("Diffuse");
	this->_actor->_Dirlight->Diffuse.x = Dif.attribute("X").as_float();
	this->_actor->_Dirlight->Diffuse.y = Dif.attribute("Y").as_float();
	this->_actor->_Dirlight->Diffuse.z = Dif.attribute("Z").as_float();
	pugi::xml_node Spe = _node->child("Specular");
	this->_actor->_Dirlight->Specular.x = Spe.attribute("X").as_float();
	this->_actor->_Dirlight->Specular.y = Spe.attribute("Y").as_float();
	this->_actor->_Dirlight->Specular.z = Spe.attribute("Z").as_float();
}

void DirectionalLight::Copy(Actor * _actor)
{
	if (_actor == NULL || _actor->_Dirlight == NULL) return;
	this->enabled = _actor->_Dirlight->enabled;
	//-----Component-----
	this->Ambient = _actor->_Dirlight->Ambient;
	this->Diffuse = _actor->_Dirlight->Diffuse;
	this->Specular = _actor->_Dirlight->Specular;

}
