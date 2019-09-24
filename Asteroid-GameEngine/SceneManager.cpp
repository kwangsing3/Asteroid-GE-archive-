#include <SceneManager.h>

#include "Xml/pugixml.hpp"

#include <Units/Actor.h>
#include <Component/DirectionalLight.h>
#include <Component/PointLight.h>
#include <Component/Meshrender.h>
#include <Collision/BoxCollision.h>
#include <World.h>
#include <Window.h>
#include <ADD_Component.h>
std::vector<Shader*> SceneManager::vec_ShaderProgram;
std::vector<DirectionalLight*> SceneManager::vec_DirectionlLight;
std::vector<PointLight*> SceneManager::vec_PointLight;
std::vector<Render_Struct*> SceneManager::vec_ObjectsToRender;
std::vector<Actor*> SceneManager::Objects;
glm::vec3 SceneManager::lightPos;
bool SceneManager::NeedReload = false;
extern World* _MainWorld;

const char* gloabl_test = "It's me, Mario";
void SceneManager::OpenFile()
{
	SceneManager::NewScene();
	
	std::cout << "Open File Function" << std::endl;
	pugi::xml_document _doc;
	std::ifstream _XMLstream;
	try
	{
		_XMLstream.open("ExampleProject/Scene1.AstGamEng");
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

	doc.save_file("ExampleProject/Scene1.AstGamEng", "\t", pugi::format_no_escapes, pugi::encoding_utf8);
}

void SceneManager::NewScene()
{
	vec_DirectionlLight.clear();
	vec_PointLight.clear();
	vec_ObjectsToRender.clear();
	Objects.clear();

	_MainWorld->exitPhysics();
	_MainWorld->initPhysics();
	_MainWorld->_PlayMode = false;
	delete _MainWorld->_piv;
	_MainWorld->_piv =new _Pivot(new Actor());
	
	
	//ADD_Component::Add_Pivot(ADD_Component::Add_Actor());
}
void SceneManager::AddToRenderPipeline(Meshrender * _mrender)
{
	for (int i = 0; i < vec_ObjectsToRender.size(); i++)
	{
		if (vec_ObjectsToRender[i]->_meshrender->Model_path == _mrender->Model_path)
		{
			vec_ObjectsToRender[i]->amount += 1;
			vec_ObjectsToRender[i]->transformList.push_back(_mrender->_actor->transform);
			return;
		}
	}

	Render_Struct* _rs = new Render_Struct();
	_rs->amount += 1;
	_rs->transformList.push_back(_mrender->_actor->transform);
	_rs->_meshrender = _mrender;
	vec_ObjectsToRender.push_back(_rs);
}
void SceneManager::DrawScene(bool _drawShadow)
{

	lightPos = SceneManager::vec_DirectionlLight.size() > 0 ? SceneManager::vec_DirectionlLight[0]->_actor->transform->position : glm::vec3(0, 5, 0);
	float far_plane = 25.0f;
	if (_drawShadow)
	{
		vec_ShaderProgram[2]->use();
		float near_plane = 1.0f;	
		for (int y = 0; y < vec_ObjectsToRender.size(); y++)   //模型種類的數量
		{
			for (int NowT = 0; NowT < vec_ObjectsToRender[y]->amount; NowT++)
			{
				
				// Draw Shadow
				if (!vec_ObjectsToRender[y]->_meshrender->_visable) continue;
				if (vec_ShaderProgram[2] == NULL) { std::cout << "Meshrender Shader Pass failed" << std::endl; return; }

				glm::mat4 shadowProj, lightView;
				glm::mat4 lightSpaceMatrix;
				shadowProj = glm::perspective(glm::radians(90.0f), (float)1024 / (float)1024, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
				std::vector<glm::mat4> shadowTransforms;
				if (SceneManager::vec_DirectionlLight.size() > 0)  //目前只有Directional Light有效果
				{
					shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
					shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
					shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
					shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
					shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
					shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
				}
				lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
				lightSpaceMatrix = shadowProj * lightView;    //這行應該跟Directional light 有關
				// render scene from light's point of view
				for (unsigned int i = 0; i < shadowTransforms.size(); ++i)
					vec_ShaderProgram[2]->setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);

				// 共通
				vec_ShaderProgram[2]->setMat4("projection", _editorCamera.Projection);
				vec_ShaderProgram[2]->setMat4("view", _editorCamera.GetViewMatrix());
				glm::mat4 _Mat4model = glm::mat4(1.0f);
				Transform _trans(*vec_ObjectsToRender[y]->transformList[NowT]);     // 這裡要依照渲染順序改Transform  

				_Mat4model = glm::translate(_Mat4model, glm::vec3(_trans.position.x, _trans.position.y, _trans.position.z));
				glm::vec3 EulerAngles = glm::vec3(glm::radians(_trans.rotation.x), glm::radians(-_trans.rotation.y), glm::radians(_trans.rotation.z));
				glm::mat4 RotationMatrix = glm::toMat4(glm::quat(EulerAngles));
				_Mat4model = _Mat4model * RotationMatrix;
				_Mat4model = glm::scale(_Mat4model, glm::vec3(_trans.scale.x, _trans.scale.y, _trans.scale.z));
				vec_ShaderProgram[2]->setMat4("model", _Mat4model);
				///Shadow
				vec_ShaderProgram[2]->setFloat("far_plane", far_plane);
				vec_ShaderProgram[2]->setVec3("viewPos", _editorCamera.transform.position);
				vec_ShaderProgram[2]->setVec3("lightPos", lightPos);
				// 共通
				/// Draw Pipeline
				for (unsigned int i = 0; i < vec_ObjectsToRender[y]->_meshrender->meshes.size(); i++)
					vec_ObjectsToRender[y]->_meshrender->meshes[i].Draw(*vec_ShaderProgram[2]);
			}
		}
	}
	else
	{
		int Light_Length = 3;
		vec_ShaderProgram[4]->use();
		for (int y = 0; y < vec_ObjectsToRender.size(); y++)
		{
			for (int NowT = 0; NowT < vec_ObjectsToRender[y]->amount; NowT++)
			{
				//vec_ObjectsToRender[y]->_meshrender->Draw(_drawShadow ? vec_ShaderProgram[2] : vec_ShaderProgram[4], _drawShadow); continue;
				// Draw Normally
				if (!vec_ObjectsToRender[y]->_meshrender->_visable) continue;
				if (vec_ShaderProgram[4] == NULL) { std::cout << "Meshrender Shader Pass failed" << std::endl; return; }
				
				// Light Setting
				{
					vec_ShaderProgram[4]->setFloat("material.shininess", 32.0f);
					//Directional Light
					for (int i = 0; i < Light_Length; i++)
					{
						if (i + 1 > SceneManager::vec_DirectionlLight.size())
						{
							vec_ShaderProgram[4]->setVec3("dirLight[" + std::to_string(i) + "].direction", glm::vec3(0, 0, 0));
							vec_ShaderProgram[4]->setVec3("dirLight[" + std::to_string(i) + "].ambient", glm::vec3(0, 0, 0));
							vec_ShaderProgram[4]->setVec3("dirLight[" + std::to_string(i) + "].diffuse", glm::vec3(0, 0, 0));
							vec_ShaderProgram[4]->setVec3("dirLight[" + std::to_string(i) + "].specular", glm::vec3(0, 0, 0));
							continue;
						}
						vec_ShaderProgram[4]->setVec3("dirLight[" + std::to_string(i) + "].direction", SceneManager::vec_DirectionlLight[i]->_actor->transform->rotation);
						vec_ShaderProgram[4]->setVec3("dirLight[" + std::to_string(i) + "].ambient", SceneManager::vec_DirectionlLight[i]->Ambient);
						vec_ShaderProgram[4]->setVec3("dirLight[" + std::to_string(i) + "].diffuse", SceneManager::vec_DirectionlLight[i]->Diffuse);
						vec_ShaderProgram[4]->setVec3("dirLight[" + std::to_string(i) + "].specular", SceneManager::vec_DirectionlLight[i]->Specular);
					}
					//Point Light
					for (int i = 0; i < Light_Length; i++)
					{
						if (i + 1 > SceneManager::vec_PointLight.size())
						{
							vec_ShaderProgram[4]->setVec3("pointLights[" + std::to_string(i) + "].position", glm::vec3(0, 0, 0));
							vec_ShaderProgram[4]->setVec3("pointLights[" + std::to_string(i) + "].ambient", glm::vec3(0, 0, 0));
							vec_ShaderProgram[4]->setVec3("pointLights[" + std::to_string(i) + "].diffuse", glm::vec3(0, 0, 0));
							vec_ShaderProgram[4]->setVec3("pointLights[" + std::to_string(i) + "].specular", glm::vec3(0, 0, 0));
							vec_ShaderProgram[4]->setFloat("pointLights[" + std::to_string(i) + "].constant", 0);
							vec_ShaderProgram[4]->setFloat("pointLights[" + std::to_string(i) + "].linear", 0);
							vec_ShaderProgram[4]->setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0);
							continue;
						}
						vec_ShaderProgram[4]->setVec3("pointLights[" + std::to_string(i) + "].position", SceneManager::vec_PointLight[i]->_actor->transform->position);
						vec_ShaderProgram[4]->setVec3("pointLights[" + std::to_string(i) + "].ambient", SceneManager::vec_PointLight[i]->Ambient);
						vec_ShaderProgram[4]->setVec3("pointLights[" + std::to_string(i) + "].diffuse", SceneManager::vec_PointLight[i]->Diffuse);
						vec_ShaderProgram[4]->setVec3("pointLights[" + std::to_string(i) + "].specular", SceneManager::vec_PointLight[i]->Specular);
						vec_ShaderProgram[4]->setFloat("pointLights[" + std::to_string(i) + "].constant", SceneManager::vec_PointLight[i]->Constant);
						vec_ShaderProgram[4]->setFloat("pointLights[" + std::to_string(i) + "].linear", SceneManager::vec_PointLight[i]->linear);
						vec_ShaderProgram[4]->setFloat("pointLights[" + std::to_string(i) + "].quadratic", SceneManager::vec_PointLight[i]->quadratic);
						
					}
					// spotLight
						/*_shader.setVec3("spotLight.position", Window::_editorCamera.transform.position);
						_shader.setVec3("spotLight.direction", Window::_editorCamera.Front);
						_shader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
						_shader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
						_shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
						_shader.setFloat("spotLight.constant", 1.0f);
						_shader.setFloat("spotLight.linear", 0.09);
						_shader.setFloat("spotLight.quadratic", 0.032);
						_shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
						_shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));*/
				}
				vec_ShaderProgram[4]->setVec3("Color", vec_ObjectsToRender[y]->_meshrender->VertexColor.x, vec_ObjectsToRender[y]->_meshrender->VertexColor.y, vec_ObjectsToRender[y]->_meshrender->VertexColor.z);
				// 共通
				vec_ShaderProgram[4]->setMat4("projection", _editorCamera.Projection);
				vec_ShaderProgram[4]->setMat4("view", _editorCamera.GetViewMatrix());
				glm::mat4 _Mat4model = glm::mat4(1.0f);
				Transform _trans(*vec_ObjectsToRender[y]->transformList[NowT]);     // 這裡要依照渲染順序改Transform  

				_Mat4model = glm::translate(_Mat4model, glm::vec3(_trans.position.x, _trans.position.y, _trans.position.z));
				glm::vec3 EulerAngles = glm::vec3(glm::radians(_trans.rotation.x), glm::radians(-_trans.rotation.y), glm::radians(_trans.rotation.z));
				glm::mat4 RotationMatrix = glm::toMat4(glm::quat(EulerAngles));
				_Mat4model = _Mat4model * RotationMatrix;
				_Mat4model = glm::scale(_Mat4model, glm::vec3(_trans.scale.x, _trans.scale.y, _trans.scale.z));
				vec_ShaderProgram[4]->setMat4("model", _Mat4model);
				///Shadow
				vec_ShaderProgram[4]->setFloat("far_plane", far_plane);
				vec_ShaderProgram[4]->setVec3("viewPos", _editorCamera.transform.position);
				vec_ShaderProgram[4]->setVec3("lightPos", lightPos);
				// 共通

				for (unsigned int i = 0; i < vec_ObjectsToRender[y]->_meshrender->meshes.size(); i++)
					vec_ObjectsToRender[y]->_meshrender->meshes[i].Draw(*vec_ShaderProgram[4]);
			}
		}
	}
	
}


