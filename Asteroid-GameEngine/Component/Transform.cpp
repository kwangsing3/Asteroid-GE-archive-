#include <Component/Transform.h>
#include <Units/Actor.h>
//#include <glm/gtx/quaternion.hpp>
//#include <Math/Math.h>

//#include <World.h>
//#include <Xml/pugixml.hpp>
void Transform::SaveFile(pugi::xml_node* _node)
{
	if (_node == NULL|| this->_actor->transform==NULL) return;
	pugi::xml_node _childPos = _node->append_child("Position");
	_childPos.append_attribute("x") = this->_actor->transform->position.x;
	_childPos.append_attribute("y") = this->_actor->transform->position.y;
	_childPos.append_attribute("z") = this->_actor->transform->position.z;
	pugi::xml_node _childRot = _node->append_child("Rotation");
	_childRot.append_attribute("x") = this->_actor->transform->rotation.x;
	_childRot.append_attribute("y") = this->_actor->transform->rotation.y;
	_childRot.append_attribute("z") = this->_actor->transform->rotation.z;
	pugi::xml_node _childSca = _node->append_child("Scale");
	_childSca.append_attribute("x") = this->_actor->transform->scale.x;
	_childSca.append_attribute("y") = this->_actor->transform->scale.y;
	_childSca.append_attribute("z") = this->_actor->transform->scale.z;
}

void Transform::OpenFile( pugi::xml_node* _node)
{
	if (_node == NULL || this->_actor->transform == NULL) return;
	this->position.x = _node->child("Position").attribute("x").as_float();
	this->position.y = _node->child("Position").attribute("y").as_float();
	this->position.z = _node->child("Position").attribute("z").as_float();
	this->rotation.x = _node->child("Rotation").attribute("x").as_float();
	this->rotation.y = _node->child("Rotation").attribute("y").as_float();
	this->rotation.z = _node->child("Rotation").attribute("z").as_float();
	this->scale.x = _node->child("Scale").attribute("x").as_float();
	this->scale.y = _node->child("Scale").attribute("y").as_float();
	this->scale.z = _node->child("Scale").attribute("z").as_float();
}

void Transform::Translate(glm::vec3 _pos)
{
	this->position= _pos;
	if (this->_actor->meshrender != NULL) this->_actor->meshrender->UpdateCollision();
	if (this->_actor->boxcollision != NULL) this->_actor->boxcollision->UpdateCollision();
}

void Transform::Rotate(glm::vec3 _rot)
{
	this->rotation = _rot;
	if (this->_actor->meshrender != NULL) this->_actor->meshrender->UpdateCollision();
	/*if (!World::_PlayMode)
	{
		if (this->_actor->boxcollision != NULL) this->_actor->boxcollision->UpdateCollision();
	}*/
	
}

/*void Transform::Rotate(glm::Quaternion _qu);
{
	EulerAngles _EA = Math::ToEulerAngles(_qu);
	glm::vec3 _rot(glm::degrees(_EA.roll), glm::degrees(-_EA.pitch) , glm::degrees(_EA.yaw));

	this->Rotate(_rot);
	if (this->_actor->meshrender != NULL) this->_actor->meshrender->UpdateCollision();
	//if (this->_actor->boxcollision != NULL) this->_actor->boxcollision->UpdateCollision();
}*/

void Transform::Scale(glm::vec3 _scal)
{
	this->scale = _scal;
	if (this->_actor->meshrender != NULL) this->_actor->meshrender->UpdateCollision();
	if (this->_actor->boxcollision != NULL) this->_actor->boxcollision->UpdateCollision();
}

