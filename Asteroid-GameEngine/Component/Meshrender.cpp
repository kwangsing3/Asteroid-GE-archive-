#include <Component/Meshrender.h>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif // !STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glm/gtx/quaternion.hpp>

#include <Units/Camera.h>
#include <Window.h>
#include <shader_m.h>

#include <World.h>
#include <Units/Actor.h>
std::vector<ModelStruct> Meshrender::ModelList;
extern Camera _editorCamera;
extern World* _MainWorld;


void Meshrender::SaveFile(pugi::xml_node* _node)
{
	if (_node == NULL || this->_actor->meshrender == NULL) return;
	_node->append_attribute("meshrender") = 1;
	pugi::xml_node _n = _node->append_child("MeshRender");
	_n.append_attribute("VertexColorX") = this->_actor->meshrender->VertexColor.x;
	_n.append_attribute("VertexColorY") = this->_actor->meshrender->VertexColor.y;
	_n.append_attribute("VertexColorZ") = this->_actor->meshrender->VertexColor.z;
}
void Meshrender::OpenFile(pugi::xml_node* _node)
{
	if (_node == NULL || this->_actor->meshrender == NULL) return;
	pugi::xml_node _n = _node->child("MeshRender");
	this->VertexColor = glm::vec3(_n.attribute("VertexColorX").as_float(), _n.attribute("VertexColorY").as_float(), _n.attribute("VertexColorZ").as_float());
}
void Meshrender::Copy(Actor* _actor)
{
	if (_actor == NULL || _actor->meshrender == NULL) return;
	this->enabled = _actor->meshrender->enabled;
	//-----Component-----
	this->_needdebug = _actor->meshrender->_needdebug;
	this->_visable = _actor->meshrender->_visable;
	this->VertexColor = _actor->meshrender->VertexColor;
	this->_shape = _actor->meshrender->_shape;
	this->_mode = _actor->meshrender->_mode;
	/*  Model Data */
	this->textures_loaded= _actor->meshrender->textures_loaded;
	this->meshes = _actor->meshrender->meshes;
	this->directory = _actor->meshrender->directory;
	this->_Mat4model = _actor->meshrender->_Mat4model;
	this->Model_path = _actor->meshrender->Model_path;

	
	/*  Model Data */
}

/*void Meshrender::Draw(Shader* _shader, bool _renderShadow,Transform* _trans)
{
	if (!this->_visable)  return;
	if (_shader==NULL) { std::cout << "Meshrender Shader Pass failed" << std::endl; return; }

	//Shader* _shader = _renderShadow ? _ShadowShader:_StandardShader;
	_shader->use();
	lightPos = SceneManager::vec_DirectionlLight.size() > 0 ? SceneManager::vec_DirectionlLight[0]->_actor->transform->position : glm::vec3(0, 5, 0);
	if (_renderShadow)
	{
		/*
			紀錄一下  目前光影只會對第一個Directional Ligiht做反應，照理來說應該有更好的解法，雖然有興趣，不過因為先完善完整功能更重要，所以先放著   最佳展示角度Y要-0.3f~0.3f
		
		///lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		/*shadowProj = glm::perspective(glm::radians(90.0f), (float)1024 / (float)1024, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
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
			_shader->setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
	}
	else
	{
		_shader->setFloat("material.shininess", 32.0f);
		
		//Directional Light
		for (int i = 0; i < Light_Length; i++)
		{
			if (i + 1 > SceneManager::vec_DirectionlLight.size())
			{
				_shader->setVec3("dirLight[" + std::to_string(i) + "].direction", glm::vec3(0, 0, 0));
				_shader->setVec3("dirLight[" + std::to_string(i) + "].ambient", glm::vec3(0, 0, 0));
				_shader->setVec3("dirLight[" + std::to_string(i) + "].diffuse", glm::vec3(0, 0, 0));
				_shader->setVec3("dirLight[" + std::to_string(i) + "].specular", glm::vec3(0, 0, 0));
				continue;
			}
			_shader->setVec3("dirLight[" + std::to_string(i) + "].direction", SceneManager::vec_DirectionlLight[i]->_actor->transform->rotation);
			_shader->setVec3("dirLight[" + std::to_string(i) + "].ambient", SceneManager::vec_DirectionlLight[i]->Ambient);
			_shader->setVec3("dirLight[" + std::to_string(i) + "].diffuse", SceneManager::vec_DirectionlLight[i]->Diffuse);
			_shader->setVec3("dirLight[" + std::to_string(i) + "].specular", SceneManager::vec_DirectionlLight[i]->Specular);
		}
		//Point Light
		for (int i = 0; i < Light_Length; i++)
		{
			if (i + 1 > SceneManager::vec_PointLight.size())
			{
				_shader->setVec3("pointLights[" + std::to_string(i) + "].position", glm::vec3(0, 0, 0));
				_shader->setVec3("pointLights[" + std::to_string(i) + "].ambient", glm::vec3(0, 0, 0));
				_shader->setVec3("pointLights[" + std::to_string(i) + "].diffuse", glm::vec3(0, 0, 0));
				_shader->setVec3("pointLights[" + std::to_string(i) + "].specular", glm::vec3(0, 0, 0));
				_shader->setFloat("pointLights[" + std::to_string(i) + "].constant", 0);
				_shader->setFloat("pointLights[" + std::to_string(i) + "].linear", 0);
				_shader->setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0);
				continue;
			}
			_shader->setVec3("pointLights[" + std::to_string(i) + "].position", SceneManager::vec_PointLight[i]->_actor->transform->position);
			_shader->setVec3("pointLights[" + std::to_string(i) + "].ambient", SceneManager::vec_PointLight[i]->Ambient);
			_shader->setVec3("pointLights[" + std::to_string(i) + "].diffuse", SceneManager::vec_PointLight[i]->Diffuse);
			_shader->setVec3("pointLights[" + std::to_string(i) + "].specular", SceneManager::vec_PointLight[i]->Specular);
			_shader->setFloat("pointLights[" + std::to_string(i) + "].constant", SceneManager::vec_PointLight[i]->Constant);
			_shader->setFloat("pointLights[" + std::to_string(i) + "].linear", SceneManager::vec_PointLight[i]->linear);
			_shader->setFloat("pointLights[" + std::to_string(i) + "].quadratic", SceneManager::vec_PointLight[i]->quadratic);
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
		_shader->setVec3("Color", this->VertexColor.x, this->VertexColor.y, this->VertexColor.z);
	}

	//_shader.setVec3("viewPos", _editorCamera.transform.position);
	// 共通
	_shader->setMat4("projection", _editorCamera.Projection);
	_shader->setMat4("view", _editorCamera.GetViewMatrix());
	_Mat4model = glm::mat4(1.0f);
	_Mat4model = glm::translate(_Mat4model, glm::vec3(_trans->position.x, _trans->position.y, _trans->position.z));

	EulerAngles = glm::vec3(glm::radians(_trans->rotation.x), glm::radians(-_trans->rotation.y), glm::radians(_trans->rotation.z));
	glm::mat4 RotationMatrix = glm::toMat4(glm::quat(EulerAngles));
	_Mat4model = _Mat4model * RotationMatrix;
	_Mat4model = glm::scale(_Mat4model, glm::vec3(_trans->scale.x, _trans->scale.y, _trans->scale.z));
	_shader->setMat4("model", _Mat4model);
	
	///Shadow
	_shader->setFloat("far_plane", far_plane);
	_shader->setVec3("viewPos", _editorCamera.transform.position);
	_shader->setVec3("lightPos", lightPos);
	// 共通

	/// Draw Pipeline
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(*_shader);
}*/
//  --------------------------------- Mouse Collision --------------------------------- 
#include <World.h>
//btCollisionShape* colShape;
void Meshrender::CreateMouseCollision()
{
	_MainWorld->InitPhysics = true;

	btVector3 localInertia(0, 0, 0);
	//create a dynamic rigidbody
	if (colShape == NULL)
	{
		colShape = new btBoxShape(btVector3(this->_actor->transform->scale.x, this->_actor->transform->scale.y, this->_actor->transform->scale.z));
		_MainWorld->m_collisionShapes.push_back(colShape);
	}

	btScalar mass(0);
	/// Create Dynamic Objects
	btTransform startTransform;
	startTransform.setIdentity();
	btQuaternion quat;
	//quat.setEuler(-glm::radians(this->_actor->transform->rotation.y), glm::radians(this->_actor->transform->rotation.x), glm::radians(this->_actor->transform->rotation.z));//or quat.setEulerZYX depending on the ordering you want
	quat.setEulerZYX(glm::radians(this->_actor->transform->rotation.z), glm::radians(-this->_actor->transform->rotation.y), glm::radians(this->_actor->transform->rotation.x));
	startTransform.setRotation(quat);

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);
	if (isDynamic)
		colShape->calculateLocalInertia(mass, localInertia);
	startTransform.setOrigin(btVector3(this->_actor->transform->position.x, this->_actor->transform->position.y, this->_actor->transform->position.z));
	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	 myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);

	body = new btRigidBody(rbInfo);
	body->setCenterOfMassTransform(startTransform);
	
	
	int Collision_flag = _needdebug ? 4 : btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT;
	body->setCollisionFlags(Collision_flag);

	int _group = 1;
	int _mask = 1;
	body->_ActorInBullet = this->_actor;
	_MainWorld->m_dynamicsWorld->addRigidBody(body, _group, _mask);
	
	//World::dynamicsWorld->updateSingleAabb(body);
}
void Meshrender::UpdateCollision()
{
	SceneManager::NeedInitedDraw = true;
	if (this->body == NULL) return;

	if (!_MainWorld->_PlayMode)
	{
		_MainWorld->deleteRigidBody(this->body);
		CreateMouseCollision();
	}
	else
	{
		btTransform initialTransform;
		initialTransform.setOrigin(btVector3(this->_actor->transform->position.x, this->_actor->transform->position.y, this->_actor->transform->position.z));
		btQuaternion quat;
		quat.setEulerZYX(glm::radians(this->_actor->transform->rotation.z), glm::radians(-this->_actor->transform->rotation.y), glm::radians(this->_actor->transform->rotation.x));
		initialTransform.setRotation(quat);
		this->body->setWorldTransform(initialTransform);
		this->myMotionState->setWorldTransform(initialTransform);
		_MainWorld->m_dynamicsWorld->updateSingleAabb(this->body);
	}
	
}

void Meshrender::SetVisable(bool _bool)
{
	this->_visable = _bool;
	SceneManager::UpdateRenderPipeline(this);
}


void Meshrender::ReSetCollisionFlag()
{
	int Collision_flag = _needdebug ? 4 : btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT;
	this->body->setCollisionFlags(Collision_flag);
}







//  --------------------------------- Model Loading --------------------------------- 
void Meshrender::loadModel(string const & path)
{
	// read file via ASSIMP
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return;
	}
	// retrieve the directory path of the filepath
	directory = path.substr(0, path.find_last_of('/'));

	// process ASSIMP's root node recursively
	processNode(scene->mRootNode, scene);

	ModelStruct _NewModel;
	_NewModel.path = path;
	_NewModel._meshes = this->meshes;
	ModelList.push_back(_NewModel);
}
void Meshrender::processNode(aiNode* node, const aiScene* scene)
{
	// process each mesh located at the current node
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// the node object only contains indices to index the actual objects in the scene. 
		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}

}
Mesh Meshrender::processMesh(aiMesh* mesh, const aiScene* scene)
{
	// data to fill
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	// Walk through each of the mesh's vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
		// positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;
		// normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;
		// texture coordinates
		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		// tangent
		vector.x = mesh->mTangents[i].x;
		vector.y = mesh->mTangents[i].y;
		vector.z = mesh->mTangents[i].z;
		vertex.Tangent = vector;
		// bitangent
		vector.x = mesh->mBitangents[i].x;
		vector.y = mesh->mBitangents[i].y;
		vector.z = mesh->mBitangents[i].z;
		vertex.Bitangent = vector;
		vertices.push_back(vertex);
	}
	// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	// process materials
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
	// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
	// Same applies to other texture as the following list summarizes:
	// diffuse: texture_diffuseN
	// specular: texture_specularN
	// normal: texture_normalN

	// 1. diffuse maps
	vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// 2. specular maps
	vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	// 3. normal maps
	std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	// 4. height maps
	std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	// return a mesh object created from the extracted mesh data
	return Mesh(vertices, indices, textures);
}
vector<Texture> Meshrender::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
	vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}
	if (mat->GetTextureCount(type) < 1)
	{
		Texture texture;
		aiString str("DefaultColor.jpg");
		texture.id = TextureFromFile(str.C_Str(), this->directory);
		texture.type = typeName;
		texture.path = str.C_Str();
		textures.push_back(texture);
		textures_loaded.push_back(texture);
	}

	return textures;
}
unsigned int Meshrender::TextureFromFile(const char * path, const string & directory)
{
	string filename = string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

