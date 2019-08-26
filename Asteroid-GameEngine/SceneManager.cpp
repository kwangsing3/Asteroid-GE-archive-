#include <SceneManager.h>
#include <World.h>


std::vector<Shader> SceneManager::vec_ShaderProgram;
std::vector<DirectionalLight*> SceneManager::vec_DirectionlLight;
std::vector<PointLight*> SceneManager::vec_PointLight;
std::vector<Meshrender*> SceneManager::vec_ObjectsToRender;
std::vector<Actor*> SceneManager::Objects;
                        

void SceneManager::OpenFile()
{
	SceneManager::NewScene();

	std::cout << "Open File Function"<<std::endl;
	pugi::xml_document _doc;
	std::ifstream _XMLstream;
	try
	{
		_XMLstream.open("ExampleProject/Scene2.AstGamEng");
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
		_Actor->transform->name = (char*) _char->c_str();
		//_Actor->ID = tool.attribute("ID").as_int();
		//transform
		_Actor->transform->position.x = tool.child("Position").attribute("x").as_float();
		_Actor->transform->position.y = tool.child("Position").attribute("y").as_float();
		_Actor->transform->position.z = tool.child("Position").attribute("z").as_float();
		_Actor->transform->rotation.x = tool.child("Rotation").attribute("x").as_float();
		_Actor->transform->rotation.y = tool.child("Rotation").attribute("y").as_float();
		_Actor->transform->rotation.z = tool.child("Rotation").attribute("z").as_float();
		_Actor->transform->scale.x = tool.child("Scale").attribute("x").as_float();
		_Actor->transform->scale.y = tool.child("Scale").attribute("y").as_float();
		_Actor->transform->scale.z = tool.child("Scale").attribute("z").as_float();
		// Component
		int _componentSize = tool.attribute("Component_size").as_int();
		int _check = 0;
		
		if (tool.attribute("_Dirlight").as_int())
		{
			ADD_Component::Add_DirectionalLight(_Actor);
			_check++;
		}
		if (tool.attribute("_PointLight").as_int())
		{
			ADD_Component::Add_PointLight(_Actor);
			_check++;
		}
		if (tool.attribute("meshrender").as_int())
		{
			ADD_Component::Add_Cube(_Actor);    
			pugi::xml_node _n = tool.child("MeshRender");
			if (_Actor->meshrender != NULL)
			{
				_Actor->meshrender->VertexColor = glm::vec3(_n.attribute("VertexColorX").as_float(), _n.attribute("VertexColorY").as_float(), _n.attribute("VertexColorZ").as_float());
			}
			else
			{
				std::cout << "error:In Open File";
			}
			_Actor->meshrender->ResetDynamic();
			_check++;
		}
		if (tool.attribute("_BoxCollision").as_int())
		{
			ADD_Component::Add_BoxCollision(_Actor);    //調試用
			float _f = tool.child("BoxCollision").attribute("Mass").as_float();
			_Actor->boxcollision->_Mass = _f;
			
			_check++;
		}
		if (_check != _componentSize) { std::cout<< _char <<": Component_size error"<<std::endl; }
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
		//_cur.append_attribute("_index") =i;
		component_size = 0;
		if (Objects[i]->transform != NULL)
		{
			pugi::xml_node _childPos = _cur.append_child("Position");
			_childPos.append_attribute("x")= Objects[i]->transform->position.x;
			_childPos.append_attribute("y") = Objects[i]->transform->position.y;
			_childPos.append_attribute("z") = Objects[i]->transform->position.z;
			pugi::xml_node _childRot = _cur.append_child("Rotation");
			_childRot.append_attribute("x") = Objects[i]->transform->rotation.x;
			_childRot.append_attribute("y") = Objects[i]->transform->rotation.y;
			_childRot.append_attribute("z") = Objects[i]->transform->rotation.z;
			pugi::xml_node _childSca = _cur.append_child("Scale");
			_childSca.append_attribute("x") = Objects[i]->transform->scale.x;
			_childSca.append_attribute("y") = Objects[i]->transform->scale.y;
			_childSca.append_attribute("z") = Objects[i]->transform->scale.z;
		}
		if (Objects[i]->meshrender != NULL)
		{
			_cur.append_attribute("meshrender") = 1;
			pugi::xml_node _n=_cur.append_child("MeshRender");
			_n.append_attribute("VertexColorX") = Objects[i]->meshrender->VertexColor.x;
			_n.append_attribute("VertexColorY") = Objects[i]->meshrender->VertexColor.y;
			_n.append_attribute("VertexColorZ") = Objects[i]->meshrender->VertexColor.z;
			component_size++;
		}
		if (Objects[i]->_Dirlight != NULL)
		{	
			_cur.append_attribute("_Dirlight") = 1;
			pugi::xml_node Amb=_cur.append_child("Ambient");
			Amb.append_attribute("X")= Objects[i]->_Dirlight->Ambient.x;
			Amb.append_attribute("Y") = Objects[i]->_Dirlight->Ambient.y;
			Amb.append_attribute("Z") = Objects[i]->_Dirlight->Ambient.z;
			pugi::xml_node Dif = _cur.append_child("Diffuse");
			Dif.append_attribute("X") = Objects[i]->_Dirlight->Diffuse.x;
			Dif.append_attribute("Y") = Objects[i]->_Dirlight->Diffuse.y;
			Dif.append_attribute("Z") = Objects[i]->_Dirlight->Diffuse.z;
			pugi::xml_node Spe = _cur.append_child("Specular");
			Spe.append_attribute("X") = Objects[i]->_Dirlight->Specular.x;
			Spe.append_attribute("Y") = Objects[i]->_Dirlight->Specular.y;
			Spe.append_attribute("Z") = Objects[i]->_Dirlight->Specular.z;
			component_size++;
		}
		if (Objects[i]->_PointLight != NULL)
		{
			_cur.append_attribute("_PointLight") = 1;
			pugi::xml_node Amb = _cur.append_child("Ambient");
			Amb.append_attribute("X") = Objects[i]->_PointLight->Ambient.x;
			Amb.append_attribute("Y") = Objects[i]->_PointLight->Ambient.y;
			Amb.append_attribute("Z") = Objects[i]->_PointLight->Ambient.z;
			pugi::xml_node Dif = _cur.append_child("Diffuse");
			Dif.append_attribute("X") = Objects[i]->_PointLight->Diffuse.x;
			Dif.append_attribute("Y") = Objects[i]->_PointLight->Diffuse.y;
			Dif.append_attribute("Z") = Objects[i]->_PointLight->Diffuse.z;
			pugi::xml_node Spe = _cur.append_child("Specular");
			Spe.append_attribute("X") = Objects[i]->_PointLight->Specular.x;
			Spe.append_attribute("Y") = Objects[i]->_PointLight->Specular.y;
			Spe.append_attribute("Z") = Objects[i]->_PointLight->Specular.z;

			
			_cur.append_child("Constant").append_attribute("Constant") = Objects[i]->_PointLight->Constant;
			_cur.append_child("linear").append_attribute("linear")= Objects[i]->_PointLight->linear;
			_cur.append_child("quadratic").append_attribute("quadratic") = Objects[i]->_PointLight->quadratic;
			component_size++;
		}
		if (Objects[i]->boxcollision != NULL)
		{
			_cur.append_attribute("_BoxCollision") = 1;
			_cur.append_child("BoxCollision").append_attribute("Mass") = Objects[i]->boxcollision->_Mass;
			component_size++;
		}

		
		_cur.append_attribute("Component_size") = component_size;
	}
	
	root.append_attribute("Objects_Size") = Objects.size();
	

	doc.save_file("ExampleProject/Scene2.AstGamEng", "\t", pugi::format_no_escapes, pugi::encoding_utf8);
	
}

void SceneManager::NewScene()
{
	vec_DirectionlLight.clear();
	vec_PointLight.clear();
	vec_ObjectsToRender.clear();
	
	
	Objects.clear();
	World::dynamicsWorld->getCollisionObjectArray().clear();
}
