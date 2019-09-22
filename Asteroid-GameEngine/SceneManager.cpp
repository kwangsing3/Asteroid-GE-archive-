#include <SceneManager.h>

#include "Xml/pugixml.hpp"

#include <Units/Actor.h>
#include <Component/DirectionalLight.h>
#include <Component/PointLight.h>
#include <Component/Meshrender.h>
#include <Collision/BoxCollision.h>
#include <World.h>

#include <ADD_Component.h>
std::vector<Shader> SceneManager::vec_ShaderProgram;
std::vector<DirectionalLight*> SceneManager::vec_DirectionlLight;
std::vector<PointLight*> SceneManager::vec_PointLight;
std::vector<Meshrender*> SceneManager::vec_ObjectsToRender;
std::vector<Actor*> SceneManager::Objects;


void SceneManager::OpenFile()
{
	SceneManager::NewScene();
	
	std::cout << "Open File Function" << std::endl;
	pugi::xml_document _doc;
	std::ifstream _XMLstream;
	try
	{
		_XMLstream.open("ExampleProject/BasicExample.AstGamEng");
		_doc.load(_XMLstream);
	}
	catch (const std::exception&)
	{
		std::cout << "Failed to open file" << std::endl;
		return;
	}
	pugi::xml_node _root = _doc.child("Scene");

	for (pugi::xml_node tool = _root.first_child(); tool; tool = tool.next_sibling())
	{

		Actor* _Actor = ADD_Component::Add_Actor();

		// name
		std::string* _char = new std::string();
		*_char = tool.attribute("name").value();
		_Actor->transform->name = (char*)_char->c_str();
		//_Actor->ID = tool.attribute("ID").as_int();
		//Transform
		_Actor->transform->OpenFile(&tool);

		// Component
		int _componentSize = tool.attribute("Component_size").as_int();
		int _check = 0;

		if (tool.attribute("_Dirlight").as_int())
		{
			ADD_Component::Add_DirectionalLight(_Actor)->OpenFile(&tool);
			_check++;
		}
		if (tool.attribute("_PointLight").as_int())                                 //光線類的OpenFile詳細數值 還沒有製作
		{
			ADD_Component::Add_PointLight(_Actor)->OpenFile(&tool);
			_check++;
		}
		if (tool.attribute("meshrender").as_int())
		{
			ADD_Component::Add_Meshrender(_Actor,Cube)->OpenFile(&tool);
			_check++;
		}
		if (tool.attribute("_BoxCollision").as_int())
		{
			ADD_Component::Add_BoxCollision(_Actor)->OpenFile(&tool);    //調試用
			_check++;
		}
		if (_check != _componentSize) { std::cout << _char << ": Component_size error" << std::endl; }
	}


	_XMLstream.close();
}

void SceneManager::SaveFile()
{
	std::cout << "Save File Function" << std::endl;
	pugi::xml_document doc;
	//增加說明
	pugi::xml_node decl = doc.prepend_child(pugi::node_declaration);
	decl.append_attribute("version") = "1.0";
	decl.append_attribute("encoding") = "UTF-8";
	//增加節點
	pugi::xml_node root = doc.append_child("Scene");
	//給節點增加屬性，並賦值
	root.append_attribute("name") = "test";

	

	int component_size = 0;
	for (int i = 0; i < Objects.size(); i++)
	{
		pugi::xml_node _cur = root.append_child("Objects");
		_cur.append_attribute("name") = Objects[i]->transform->name;
		//_cur.append_attribute("ID") = Objects[i]->ID;

		//過濾坐標軸
		if (Objects[i]->meshrender != NULL && Objects[i]->meshrender->_shape == NONE)
			continue;

		component_size = 0;
		if (Objects[i]->transform != NULL)
		{
			Objects[i]->transform->SaveFile(&_cur);
		}
		if (Objects[i]->meshrender != NULL)
		{
			Objects[i]->meshrender->SaveFile(&_cur);
			component_size++;
		}
		if (Objects[i]->_Dirlight != NULL)
		{
			Objects[i]->_Dirlight->SaveFile(&_cur);
			component_size++;
		}
		if (Objects[i]->_PointLight != NULL)
		{
			Objects[i]->_PointLight->SaveFile(&_cur);
			component_size++;
		}
		if (Objects[i]->boxcollision != NULL)
		{
			Objects[i]->boxcollision->SaveFile(&_cur);
			component_size++;
		}
		_cur.append_attribute("Component_size") = component_size;
	}
	root.append_attribute("Objects_Size") = Objects.size();

	doc.save_file("ExampleProject/BasicExample.AstGamEng", "\t", pugi::format_no_escapes, pugi::encoding_utf8);
}

void SceneManager::NewScene()
{
	vec_DirectionlLight.clear();
	vec_PointLight.clear();
	vec_ObjectsToRender.clear();
	Objects.clear();

	Window::_Mainworld->exitPhysics();
	Window::_Mainworld->initPhysics();
	World::_PlayMode = false;
	delete World::_piv;
	World::_piv =new _Pivot(new Actor());
	
	
	//ADD_Component::Add_Pivot(ADD_Component::Add_Actor());
}
