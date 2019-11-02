#include <SceneManager.h>

#include <Xml/include/pugixml.hpp>


#include <Component/DirectionalLight.h>
#include <Component/PointLight.h>
#include <Component/Meshrender.h>
#include <Component/BoxCollision.h>
#include <Units/Actor.h>
#include <World.h>
#include <Window.h>

#include <AGE_Mesh.h>

std::string _Examples_List[5] = 
{
	"EXampleProject/Basic Example.AstGamEng",
	"EXampleProject/Constraints.AstGamEng",
	"EXampleProject/Motorized Hinge.AstGamEng",
	"EXampleProject/Rolling Friction.AstGamEng",
	"EXampleProject/TestHinge Torque.AstGamEng",
};


//*********************************
//Render Request
//*********************************
float near_plane = 1.0f;
float far_plane = 7.5f;
glm::mat4 shadowProj = glm::mat4(1.0f), lightView = glm::mat4(1.0f);
glm::mat4 lightSpaceMatrix = glm::mat4(1.0f);
glm::vec3 lightPos = glm::vec3(1.0f);
Shader* _CurrentShader;
extern World* _MainWorld;


/*std::vector<Actor*> SceneManager::Objects;
std::vector<Shader*> SceneManager::vec_ShaderProgram;
std::vector<DirectionalLight*> SceneManager::vec_DirectionlLight;
std::vector<PointLight*> SceneManager::vec_PointLight;
std::vector<Render_Struct*> SceneManager::vec_ObjectsToRender_Instancing;
std::vector<Meshrender*> SceneManager::vec_ObjectsToRender;

std::vector<ModelLoadStruct*> SceneManager::ModelList;

//glm::vec3 SceneManager::lightPos;
bool SceneManager::NeedReloadShader = false;
bool SceneManager::NeedInitedDraw = true;

std::string SceneManager::_FilePAth;*/
void SceneManager::OpenFile(int _index) 
{
	if (_index > _Examples_List->size() - 1 || _index == -1) return;
	_FilePAth = _Examples_List[_index];  
	OpenFile(); 
}
void SceneManager::OpenFile()
{
	SceneManager::NewScene();
	std::cout << "Open File Function" << std::endl;
	if (_FilePAth.empty()) return;

	pugi::xml_document _doc;
	std::ifstream _XMLstream;
	try
	{
		_XMLstream.open(_FilePAth);
		_doc.load(_XMLstream);
	}
	catch (const std::exception&)
	{
		std::cout << "Failed to open file" << std::endl;
		return;
	}
	pugi::xml_node _root = _doc.child("Scene");

	if (_editorCamera.enabled)
	{
		_editorCamera.OpenFile(&_root);
	}
	for (pugi::xml_node tool = _root.first_child().next_sibling(); tool; tool = tool.next_sibling())
	{

		Actor* _Actor = _ADDManager->Add_Actor();

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
			_ADDManager->Add_DirectionalLight(_Actor)->OpenFile(&tool);
			_check++;
		}
		if (tool.attribute("_PointLight").as_int())                                 //光線類的OpenFile詳細數值 還沒有製作
		{
			_ADDManager->Add_PointLight(_Actor)->OpenFile(&tool);
			_check++;
		}
		if (tool.attribute("meshrender").as_int())
		{
			_ADDManager->Add_Meshrender(_Actor, tool.child("MeshRender").attribute("_path").as_string())->OpenFile(&tool);
			_check++;
		}
		if (tool.attribute("_BoxCollision").as_int())
		{
			_ADDManager->Add_BoxCollision(_Actor)->OpenFile(&tool);    //調試用
			_check++;
		}
		if (_check != _componentSize) { std::cout << _char << ": Component_size error" << std::endl; }
	}


	_XMLstream.close();
}
void SceneManager::SaveFile()
{
	if (_FilePAth.empty()) _FilePAth = _Examples_List[3];

	pugi::xml_document doc;
	//增加說明
	pugi::xml_node decl = doc.prepend_child(pugi::node_declaration);
	decl.append_attribute("version") = "1.0";
	decl.append_attribute("encoding") = "UTF-8";
	//增加節點
	pugi::xml_node root = doc.append_child("Scene");
	//給節點增加屬性，並賦值
	root.append_attribute("name") = "test";

	//*************************************************
	// Save Camera
	//*************************************************
	if (_editorCamera.enabled)
	{
		_editorCamera.SaveFile(&root);
	}
	//*************************************************
	// Save Objects 
	//*************************************************
	int component_size = 0;
	for (int i = 0; i < Objects.size(); i++)
	{
		pugi::xml_node _cur = root.append_child("Objects");
		_cur.append_attribute("name") = Objects[i]->transform->name.c_str();
		//_cur.append_attribute("ID") = Objects[i]->ID;

		//過濾坐標軸
		if (Objects[i]->meshrender != NULL && Objects[i]->meshrender->_model->_shape == Shape::DEBUG)
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




	doc.save_file(_FilePAth.c_str(), "\t", pugi::format_no_escapes, pugi::encoding_utf8);
	std::cout << "Save File Function" << std::endl;
}
void SceneManager::NewScene()
{
	_MainWorld->_PlayMode = false;
	_MainWorld->depose_init_PhysicsProgress();
	_MainWorld->exitPhysics();
	vec_DirectionlLight.clear();
	vec_PointLight.clear();
	vec_ObjectsToRender_Instancing.clear();
	vec_ObjectsToRender.clear();
	Objects.clear();

	delete _MainWorld->_piv;
	_MainWorld->_piv =new _Pivot(new Actor());	

	_MainWorld->initPhysics();
}
void SceneManager::AddToRenderPipeline_Instancing(Meshrender * _mrender)
{
	NeedInitedDraw = true;
	for (int i = 0; i < vec_ObjectsToRender_Instancing.size(); i++)
	{
		if (vec_ObjectsToRender_Instancing[i]->_meshrender->_model->_ModelPath == _mrender->_model->_ModelPath)
		{
			vec_ObjectsToRender_Instancing[i]->amount += 1;
			vec_ObjectsToRender_Instancing[i]->transformList.push_back(_mrender->_actor->transform);
			vec_ObjectsToRender_Instancing[i]->_visableList.push_back(&_mrender->_visable);
			return;
		}
	}
	Render_Struct* _rs = new Render_Struct();
	_rs->amount += 1;
	_rs->transformList.push_back(_mrender->_actor->transform);
	_rs->_meshrender = _mrender;
	_rs->_visableList.push_back(&_mrender->_visable);
	vec_ObjectsToRender_Instancing.push_back(_rs);
}
void SceneManager::AddToRenderPipeline(Meshrender * _mrender)
{
	NeedInitedDraw = true;
	vec_ObjectsToRender.push_back(_mrender);
}
void SceneManager::UpdateRenderPipeline(Meshrender * _mrender)
{
	NeedInitedDraw = true;
	for (int i = 0; i < vec_ObjectsToRender_Instancing.size(); i++)
	{
		if (vec_ObjectsToRender_Instancing[i]->_meshrender->_model->_ModelPath == _mrender->_model->_ModelPath)
		{
			for (int x = 0; x < vec_ObjectsToRender_Instancing[i]->transformList.size(); x++)
			{
				if (vec_ObjectsToRender_Instancing[i]->transformList[x] == _mrender->_actor->transform)
				{
					vec_ObjectsToRender_Instancing[i]->_visableList[x] = &_mrender->_visable;
					return;
				}
			}
		}
	}
}
void SceneManager::InitDrawPipline()
{
	if (!NeedInitedDraw) return;
	NeedInitedDraw = false;	
	//_MainWorld->InitPhysics = true;
	//Clear Render_List
	{
		for (int y = 0; y < vec_ObjectsToRender_Instancing.size(); y++)
		{
			vec_ObjectsToRender_Instancing[y]->DrawingAmount = 0;
			vec_ObjectsToRender_Instancing[y]->DrawingtransformList.clear();
		}
	}
	//*********************************
	//Instancing Init Transform
	//*********************************
	for (int y = 0; y < vec_ObjectsToRender_Instancing.size(); y++)
	{
		vec_ObjectsToRender_Instancing[y]->DrawingAmount = 0;
		for (int x = 0; x < vec_ObjectsToRender_Instancing[y]->amount; x++)
		{
			if (*vec_ObjectsToRender_Instancing[y]->_visableList[x] == true)
			{
				vec_ObjectsToRender_Instancing[y]->DrawingAmount += 1;
				vec_ObjectsToRender_Instancing[y]->DrawingtransformList.push_back(vec_ObjectsToRender_Instancing[y]->transformList[x]);
			}
		}
			if (vec_ObjectsToRender_Instancing[y]->DrawingAmount < 1)
		{
			vec_ObjectsToRender_Instancing[y]->Drawing = false;
			continue;
		}
		vec_ObjectsToRender_Instancing[y]->Drawing = true;
		glm::mat4* modelMatrices;
		modelMatrices = new glm::mat4[vec_ObjectsToRender_Instancing[y]->DrawingAmount];
		for (unsigned int i = 0; i < vec_ObjectsToRender_Instancing[y]->DrawingAmount; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			Transform* _trans = vec_ObjectsToRender_Instancing[y]->DrawingtransformList[i];
			model = glm::translate(model, glm::vec3(_trans->position.x, _trans->position.y, _trans->position.z));
			// 2. rotation: add random rotation around a (semi)randomly picked rotation axis vector
			glm::quat MyQuaternion;
			glm::vec3 EulerAngles(glm::radians(_trans->rotation.x), glm::radians(-_trans->rotation.y), glm::radians(_trans->rotation.z));
			MyQuaternion = glm::quat(EulerAngles);
			glm::mat4 RotationMatrix = glm::toMat4(MyQuaternion);
			model = model * RotationMatrix;
			// 3. scale: Scale between 0.05 and 0.25f
			model = glm::scale(model, glm::vec3(_trans->scale));
			// 4. now add to list of matrices
			modelMatrices[i] = model;
		}
		unsigned int buffer = 0;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, vec_ObjectsToRender_Instancing[y]->DrawingAmount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
		//Bind to Vertex Array
		for (unsigned int i = 0; i < vec_ObjectsToRender_Instancing[y]->_meshrender->_model->_meshes.size(); i++)
		{
			unsigned int VAO = vec_ObjectsToRender_Instancing[y]->_meshrender->_model->_meshes[i]->VAO;
			glBindVertexArray(VAO);
			// set attribute pointers for matrix (4 times vec4)
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
			glEnableVertexAttribArray(7);
			glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
			glEnableVertexAttribArray(8);
			glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);
			glVertexAttribDivisor(7, 1);
			glVertexAttribDivisor(8, 1);
			glBindVertexArray(0);
		}
	}

}
void SceneManager::SetUpShader()
{
	this->NeedInitedShader = false;
	//*********************************
	//Resend Light information to shader
	//*********************************
	for (int i = 0; i < vec_ShaderProgram.size(); i++)
	{
		vec_ShaderProgram[i]->use();
		vec_ShaderProgram[i]->setMat4("projection", _editorCamera.Projection);
	}





	_CurrentShader = vec_ShaderProgram[4];
	_CurrentShader->use();

	_CurrentShader->setFloat("far_plane", far_plane);
	_CurrentShader->setInt("diffuseTexture", 2);
	_CurrentShader->setInt("depthMap", 1);
	_CurrentShader->setInt("shadowMap", 1);
	_CurrentShader->setInt("material.diffuse", 0);
	_CurrentShader->setInt("material.specular", 1);
	int Light_Length = 3;
	if (!vec_DirectionlLight.empty())
	{
		lightPos = this->vec_DirectionlLight[0]->_actor->transform->rotation;
		//lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
		shadowProj = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, near_plane, 1000.0f);
		lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = shadowProj * lightView;
		_CurrentShader->setInt("DebugRenderType", (int)_DebugRenderType);
		_CurrentShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
		_CurrentShader->setVec3("lightPos", lightPos);
		//DirectionalLight
		for (int i = 0; i < Light_Length; i++)
		{
			if (i + 1 > SceneManager::vec_DirectionlLight.size())
			{
				_CurrentShader->setVec3("dirLight[" + std::to_string(i) + "].direction", glm::vec3(0, 0, 0));
				_CurrentShader->setVec3("dirLight[" + std::to_string(i) + "].ambient", glm::vec3(0, 0, 0));
				_CurrentShader->setVec3("dirLight[" + std::to_string(i) + "].diffuse", glm::vec3(0, 0, 0));
				_CurrentShader->setVec3("dirLight[" + std::to_string(i) + "].specular", glm::vec3(0, 0, 0));
				continue;
			}
			_CurrentShader->setVec3("dirLight[" + std::to_string(i) + "].direction", vec_DirectionlLight[i]->_actor->transform->rotation);
			_CurrentShader->setVec3("dirLight[" + std::to_string(i) + "].ambient", vec_DirectionlLight[i]->Ambient);
			_CurrentShader->setVec3("dirLight[" + std::to_string(i) + "].diffuse", vec_DirectionlLight[i]->Diffuse);
			_CurrentShader->setVec3("dirLight[" + std::to_string(i) + "].specular", vec_DirectionlLight[i]->Specular);
		}
	}
	if (!vec_PointLight.empty())
	{
		//Point Light
		for (int i = 0; i < Light_Length; i++)
		{
			if (i + 1 > SceneManager::vec_PointLight.size())
			{
				_CurrentShader->setVec3("pointLights[" + std::to_string(i) + "].position", glm::vec3(0, 0, 0));
				_CurrentShader->setVec3("pointLights[" + std::to_string(i) + "].ambient", glm::vec3(0, 0, 0));
				_CurrentShader->setVec3("pointLights[" + std::to_string(i) + "].diffuse", glm::vec3(0, 0, 0));
				_CurrentShader->setVec3("pointLights[" + std::to_string(i) + "].specular", glm::vec3(0, 0, 0));
				_CurrentShader->setFloat("pointLights[" + std::to_string(i) + "].constant", 0);
				_CurrentShader->setFloat("pointLights[" + std::to_string(i) + "].linear", 0);
				_CurrentShader->setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0);
				continue;
			}
			_CurrentShader->setVec3("pointLights[" + std::to_string(i) + "].position", SceneManager::vec_PointLight[i]->_actor->transform->position);
			_CurrentShader->setVec3("pointLights[" + std::to_string(i) + "].ambient", SceneManager::vec_PointLight[i]->Ambient);
			_CurrentShader->setVec3("pointLights[" + std::to_string(i) + "].diffuse", SceneManager::vec_PointLight[i]->Diffuse);
			_CurrentShader->setVec3("pointLights[" + std::to_string(i) + "].specular", SceneManager::vec_PointLight[i]->Specular);
			_CurrentShader->setFloat("pointLights[" + std::to_string(i) + "].constant", SceneManager::vec_PointLight[i]->Constant);
			_CurrentShader->setFloat("pointLights[" + std::to_string(i) + "].linear", SceneManager::vec_PointLight[i]->linear);
			_CurrentShader->setFloat("pointLights[" + std::to_string(i) + "].quadratic", SceneManager::vec_PointLight[i]->quadratic);
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
	}
	_CurrentShader->setFloat("material.shininess", 32.0f);
	_CurrentShader->setBool("Use_Light", !(vec_DirectionlLight.empty() && vec_PointLight.empty()));
}



bool Use_Light = true;

void SceneManager::DrawScene(RenderShadowType _RType)
{
	//if (vec_ObjectsToRender.empty() && vec_ObjectsToRender_Instancing.empty()) return;
	if (NeedInitedDraw)InitDrawPipline();
	if (NeedInitedShader)SetUpShader();

	switch (_RType)
	{
	case RenderShadowType::Normal:
		_CurrentShader = vec_ShaderProgram[4];
		_CurrentShader->use();
		_CurrentShader->setVec3("viewPos", _editorCamera.transform.position);
		break;
	case RenderShadowType::DirectionalLight:
		_CurrentShader = vec_ShaderProgram[3];
		_CurrentShader->use();
		_CurrentShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
		break;
	case RenderShadowType::PointLight:
		_CurrentShader = vec_ShaderProgram[2];
		_CurrentShader->use();
		break;
	default:
		AGE_PRINTCONSLE("Passing unknown RenderType");
		AGE_ASSERT("false");
		break;
	}
	
	if (!vec_ObjectsToRender.empty())
	{
		_CurrentShader->setBool("Use_Instance", false);
		for (int i = 0; i < vec_ObjectsToRender.size(); i++)
		{
			vec_ObjectsToRender[i]->Draw(_CurrentShader);
		}
	}

	if (_RType != RenderShadowType::Normal) return;                          //// 先不為Instance Draw 製作陰影，之後會為Instnace Draw來製作烘焙功能，所以這行以後再拿掉
	if (!vec_ObjectsToRender_Instancing.empty())     
	{
		_CurrentShader->setBool("Use_Instance", true);
		_CurrentShader->setMat4("view", _editorCamera.GetViewMatrix());
		for (int y = 0; y < vec_ObjectsToRender_Instancing.size(); y++)
		{
			if (!vec_ObjectsToRender_Instancing[y]->Drawing) continue;
			_CurrentShader->setVec3("Color", vec_ObjectsToRender_Instancing[y]->_meshrender->VertexColor.x, vec_ObjectsToRender_Instancing[y]->_meshrender->VertexColor.y, vec_ObjectsToRender_Instancing[y]->_meshrender->VertexColor.z);

			{
				unsigned int diffuseNr = 1;
				unsigned int specularNr = 1;
				unsigned int normalNr = 1;
				unsigned int heightNr = 1;
				for (unsigned int i = 0; i < vec_ObjectsToRender_Instancing[y]->_meshrender->_model->textures_loaded.size(); i++)
				{
					glActiveTexture(GL_TEXTURE2 + i); // active proper texture unit before binding
					// retrieve texture number (the N in diffuse_textureN)
					std::string number;
					std::string name = vec_ObjectsToRender_Instancing[y]->_meshrender->_model->textures_loaded[i].type;
					if (name == "texture_diffuse")
						number = std::to_string(diffuseNr++);
					else if (name == "texture_specular")
						number = std::to_string(specularNr++); // transfer unsigned int to stream
					else if (name == "texture_normal")
						number = std::to_string(normalNr++); // transfer unsigned int to stream
					else if (name == "texture_height")
						number = std::to_string(heightNr++); // transfer unsigned int to stream
															 // now set the sampler to the correct texture unit
					glUniform1i(glGetUniformLocation(vec_ShaderProgram[4]->ID, (name + number).c_str()), i);
					// and finally bind the texture
					glBindTexture(GL_TEXTURE_2D, vec_ObjectsToRender_Instancing[y]->_meshrender->_model->textures_loaded[i].id);
				}
			}
			for (unsigned int xi = 0; xi < vec_ObjectsToRender_Instancing[y]->_meshrender->_model->_meshes.size(); xi++)
			{
				glBindVertexArray(vec_ObjectsToRender_Instancing[y]->_meshrender->_model->_meshes[xi]->VAO);
				glDrawElementsInstanced(GL_TRIANGLES, vec_ObjectsToRender_Instancing[y]->_meshrender->_model->_meshes[xi]->indices.size(), GL_UNSIGNED_INT, 0, vec_ObjectsToRender_Instancing[y]->DrawingAmount);
			}
		}
	}
	glBindVertexArray(0);

}

void SceneManager::vec_SpecializedDraw()
{
	vec_ShaderProgram[0]->use();
//	_Croodinate->_actor->transform->Scale(glm::vec3(4));
	_Croodinate->Draw(vec_ShaderProgram[0]);
}


//已沒有再使用 為了方便製作 instance而刻意留著
void Draw_Normal(bool _drawShadow, unsigned int _dp = NULL)
{


}
void Draw_Instancing(bool _drawShadow, unsigned int _dp)                       
{
	/*if (vec_ObjectsToRender_Instancing.empty()) return;
	
	float far_plane = 25.0f;
	bool Use_Light = true;
	if (_drawShadow)
	{
		if (vec_ShaderProgram[2] == NULL) { std::cout << "Meshrender Shader Pass failed" << std::endl; return; }
		vec_ShaderProgram[2]->use();
		float near_plane = 1.0f;
		for (int y = 0; y < vec_ObjectsToRender_Instancing.size(); y++)   //模型種類的數量
		{
			// Draw Shadow
			glm::mat4 shadowProj, lightView;
			glm::mat4 lightSpaceMatrix = glm::mat4(1.0f) ;
			lightPos = glm::vec3(0.0f);
			if (!vec_DirectionlLight.empty())
			{
				far_plane = 7.5f;
				shadowProj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
				lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
				lightSpaceMatrix = shadowProj * lightView;
			}

			if (!vec_PointLight.empty())                                                             
			{

				far_plane = 25.0f;
				lightPos = vec_PointLight[0]->_actor->transform->position;  //目前只有第一個點光源有效
				shadowProj = glm::perspective(glm::radians(90.0f), (float)1024 / (float)1024, near_plane, far_plane);
				std::vector<glm::mat4> shadowTransforms;
				if (SceneManager::vec_PointLight.size() > 0)
				{
					shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
					shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
					shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
					shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
					shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
					shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
				}
				// render scene from light's point of view
				for (unsigned int i = 0; i < shadowTransforms.size(); ++i)
					vec_ShaderProgram[2]->setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
			}
			///Shadow
			vec_ShaderProgram[2]->setFloat("far_plane", far_plane);
			vec_ShaderProgram[2]->setVec3("lightPos", lightPos);
			vec_ShaderProgram[2]->setMat4("lightSpaceMatrix", lightSpaceMatrix);

			/// Draw Pipeline
			for (unsigned int xi = 0; xi < vec_ObjectsToRender_Instancing[y]->_meshrender->_model->_meshes.size(); xi++)
			{
				glBindVertexArray(vec_ObjectsToRender_Instancing[y]->_meshrender->_model->_meshes[xi]->VAO);
				//glBindTexture(GL_TEXTURE_CUBE_MAP, _dp);    //這個綁陰影的動作很醜，還能夠優化
				glDrawElementsInstanced(GL_TRIANGLES, vec_ObjectsToRender_Instancing[y]->_meshrender->_model->_meshes[xi]->indices.size(), GL_UNSIGNED_INT, 0, vec_ObjectsToRender_Instancing[y]->DrawingAmount);
				glBindVertexArray(0);	
			}
		}
		return;
	}
	else
	{
		int Shader_index = 5;
		int Light_Length = 3;
		vec_ShaderProgram[Shader_index]->use();
		for (int y = 0; y < vec_ObjectsToRender_Instancing.size(); y++)
		{
			if (!vec_ObjectsToRender_Instancing[y]->Drawing) continue;
			if (vec_ObjectsToRender_Instancing[y]->_meshrender->_model->_shape == Shape::DEBUG)
			{
				if (_MainWorld->_piv != NULL) _MainWorld->_piv->Draw(vec_ShaderProgram[0], false);
				continue;
			}

			if (vec_ShaderProgram[Shader_index] == NULL) { std::cout << "Meshrender Shader Pass failed" << std::endl; return; }

			vec_ShaderProgram[Shader_index]->use();
			vec_ShaderProgram[Shader_index]->setMat4("projection", _editorCamera.Projection);
			vec_ShaderProgram[Shader_index]->setMat4("view", _editorCamera.GetViewMatrix());
			// Light Setting
			vec_ShaderProgram[Shader_index]->setFloat("material.shininess", 32.0f);
			if(Use_Light)
			{			
				//Directional Light
				for (int i = 0; i < Light_Length; i++)
				{
					if (i + 1 > SceneManager::vec_DirectionlLight.size())
					{
						vec_ShaderProgram[Shader_index]->setVec3("dirLight[" + std::to_string(i) + "].direction", glm::vec3(0, 0, 0));
						vec_ShaderProgram[Shader_index]->setVec3("dirLight[" + std::to_string(i) + "].ambient", glm::vec3(0, 0, 0));
						vec_ShaderProgram[Shader_index]->setVec3("dirLight[" + std::to_string(i) + "].diffuse", glm::vec3(0, 0, 0));
						vec_ShaderProgram[Shader_index]->setVec3("dirLight[" + std::to_string(i) + "].specular", glm::vec3(0, 0, 0));
						continue;
					}
					vec_ShaderProgram[Shader_index]->setVec3("dirLight[" + std::to_string(i) + "].direction", SceneManager::vec_DirectionlLight[i]->_actor->transform->rotation);
					vec_ShaderProgram[Shader_index]->setVec3("dirLight[" + std::to_string(i) + "].ambient", SceneManager::vec_DirectionlLight[i]->Ambient);
					vec_ShaderProgram[Shader_index]->setVec3("dirLight[" + std::to_string(i) + "].diffuse", SceneManager::vec_DirectionlLight[i]->Diffuse);
					vec_ShaderProgram[Shader_index]->setVec3("dirLight[" + std::to_string(i) + "].specular", SceneManager::vec_DirectionlLight[i]->Specular);
				}
				//Point Light
				for (int i = 0; i < Light_Length; i++)
				{
					if (i + 1 > SceneManager::vec_PointLight.size())
					{
						vec_ShaderProgram[Shader_index]->setVec3("pointLights[" + std::to_string(i) + "].position", glm::vec3(0, 0, 0));
						vec_ShaderProgram[Shader_index]->setVec3("pointLights[" + std::to_string(i) + "].ambient", glm::vec3(0, 0, 0));
						vec_ShaderProgram[Shader_index]->setVec3("pointLights[" + std::to_string(i) + "].diffuse", glm::vec3(0, 0, 0));
						vec_ShaderProgram[Shader_index]->setVec3("pointLights[" + std::to_string(i) + "].specular", glm::vec3(0, 0, 0));
						vec_ShaderProgram[Shader_index]->setFloat("pointLights[" + std::to_string(i) + "].constant", 0);
						vec_ShaderProgram[Shader_index]->setFloat("pointLights[" + std::to_string(i) + "].linear", 0);
						vec_ShaderProgram[Shader_index]->setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0);
						continue;
					}
					vec_ShaderProgram[Shader_index]->setVec3("pointLights[" + std::to_string(i) + "].position", SceneManager::vec_PointLight[i]->_actor->transform->position);
					vec_ShaderProgram[Shader_index]->setVec3("pointLights[" + std::to_string(i) + "].ambient", SceneManager::vec_PointLight[i]->Ambient);
					vec_ShaderProgram[Shader_index]->setVec3("pointLights[" + std::to_string(i) + "].diffuse", SceneManager::vec_PointLight[i]->Diffuse);
					vec_ShaderProgram[Shader_index]->setVec3("pointLights[" + std::to_string(i) + "].specular", SceneManager::vec_PointLight[i]->Specular);
					vec_ShaderProgram[Shader_index]->setFloat("pointLights[" + std::to_string(i) + "].constant", SceneManager::vec_PointLight[i]->Constant);
					vec_ShaderProgram[Shader_index]->setFloat("pointLights[" + std::to_string(i) + "].linear", SceneManager::vec_PointLight[i]->linear);
					vec_ShaderProgram[Shader_index]->setFloat("pointLights[" + std::to_string(i) + "].quadratic", SceneManager::vec_PointLight[i]->quadratic);

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
					_shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
			}
			vec_ShaderProgram[Shader_index]->setVec3("Color", vec_ObjectsToRender_Instancing[y]->_meshrender->VertexColor.x, vec_ObjectsToRender_Instancing[y]->_meshrender->VertexColor.y, vec_ObjectsToRender_Instancing[y]->_meshrender->VertexColor.z);
			vec_ShaderProgram[Shader_index]->setBool("Use_Light", Use_Light);
			///Shadow
			vec_ShaderProgram[Shader_index]->setFloat("far_plane", far_plane);
			vec_ShaderProgram[Shader_index]->setVec3("viewPos", _editorCamera.transform.position);
			vec_ShaderProgram[Shader_index]->setVec3("lightPos", lightPos);

			{
				unsigned int diffuseNr = 1;
				unsigned int specularNr = 1;
				unsigned int normalNr = 1;
				unsigned int heightNr = 1;
				for (unsigned int i = 0; i < vec_ObjectsToRender_Instancing[y]->_meshrender->_model->textures_loaded.size(); i++)
				{
					glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
					// retrieve texture number (the N in diffuse_textureN)
					std::string number;
					std::string name = vec_ObjectsToRender_Instancing[y]->_meshrender->_model->textures_loaded[i].type;
					if (name == "texture_diffuse")
						number = std::to_string(diffuseNr++);
					else if (name == "texture_specular")
						number = std::to_string(specularNr++); // transfer unsigned int to stream
					else if (name == "texture_normal")
						number = std::to_string(normalNr++); // transfer unsigned int to stream
					else if (name == "texture_height")
						number = std::to_string(heightNr++); // transfer unsigned int to stream
															 // now set the sampler to the correct texture unit
					glUniform1i(glGetUniformLocation(vec_ShaderProgram[4]->ID, (name + number).c_str()), i);
					// and finally bind the texture
					glBindTexture(GL_TEXTURE_2D, vec_ObjectsToRender_Instancing[y]->_meshrender->_model->textures_loaded[i].id);
				}

			}

			for (unsigned int xi = 0; xi < vec_ObjectsToRender_Instancing[y]->_meshrender->_model->_meshes.size(); xi++)
			{
				glBindVertexArray(vec_ObjectsToRender_Instancing[y]->_meshrender->_model->_meshes[xi]->VAO);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_CUBE_MAP, _dp);    //這個綁陰影的動作很醜，還能夠優化
				glDrawElementsInstanced(GL_TRIANGLES, vec_ObjectsToRender_Instancing[y]->_meshrender->_model->_meshes[xi]->indices.size(), GL_UNSIGNED_INT, 0, vec_ObjectsToRender_Instancing[y]->DrawingAmount);
				glBindVertexArray(0);
				glActiveTexture(GL_TEXTURE0);
			}
		}
	}*/
}


