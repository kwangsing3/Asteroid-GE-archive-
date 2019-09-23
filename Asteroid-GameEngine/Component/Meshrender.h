#ifndef MESHRENDER_H
#define MESHRENDER_H
#include <Component/Component.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <mesh.h>



class btRigidBody;
class btCollisionShape;
class Mesh;
enum Shape {Plane ,Cube ,Sphere ,Capsule ,Cylinder, _Model, NONE};

struct ModelStruct
{
	std::vector<Mesh> _meshes;
	std::string path;
};
enum RenderMode { RMode2D, RMode3D };


class Shader;
class Meshrender :public Component
{
public:
	
	glm::vec3 VertexColor;
	Shape _shape;
	RenderMode _mode;
	//---------------------------------

	//unsigned int Texture;
	virtual void Draw(Shader* _shader,bool _renderShadow);

	bool _needdebug = false;
	bool _visable = true;
	
	/*  Model Data */
	std::vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	std::vector<Mesh> meshes;
	std::string directory;
	glm::mat4 _Mat4model;
	static std::vector<ModelStruct> ModelList;
	
	/*  Model Data */
	btRigidBody* body;
	btCollisionShape* colShape;
	Meshrender() { std::cout<<"error: it couldn't be add without constructors "; }
	Meshrender(Actor* _a, Shape _shape)
	{
		_actor = _a;
		this->_mode = RenderMode(1);
		this->enabled = true;
		this->CreateShape(_shape);
		this->VertexColor = glm::vec3(1, 1, 1);
		_Mat4model = glm::mat4(1.0f);
		if(_shape!=NONE)
			CreateMouseCollision();
		
	}
	Meshrender(Actor* _a, std::string _path)
	{
		_actor = _a;
		_Mat4model = glm::mat4(1.0f);
		this->_mode = RenderMode(1);
		this->enabled = true;
		this->CreateShape(_path);
		this->VertexColor = glm::vec3(1, 1, 1);
		
		
	}
	void SaveFile( pugi::xml_node* _node) override;
	void OpenFile( pugi::xml_node* _node) override;

	void CreateShape(std::string _path)
	{
		this->_shape = _Model;
		for (int i = 0; i < ModelList.size(); i++)
		{
			if (ModelList[i].path == _path)
			{
				meshes = ModelList[i]._meshes;
				return;
			}
		}

		loadModel(_path);
	}
	void CreateShape(Shape _shape)
	{
		this->_shape = _shape;
		

		std::string DefaultShapePath;
		switch (_shape)
		{
		case Plane:
			DefaultShapePath = "";
			break;
		case Cube:
			DefaultShapePath = "ExampleModel/Cube.obj";
			break;
		case Sphere:
			DefaultShapePath = "ExampleModel/Sphere.obj";
			break;
		case Capsule:
			DefaultShapePath = "";
			break;
		case Cylinder:
			DefaultShapePath = "ExampleModel/Cylinder.obj";
			break;
		default:
			return;
			break;
		}
		for (int i = 0; i < ModelList.size(); i++)
		{
			if (ModelList[i].path == DefaultShapePath)
			{
				meshes = ModelList[i]._meshes;
				return;
			}
		}
		loadModel(DefaultShapePath);
		//CreateMouseCollision();
	}
	virtual void UpdateCollision();
	
private:

	void CreateMouseCollision();

	void loadModel(string const &path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	// checks all material textures of a given type and loads the textures if they're not loaded yet.   the required info is returned as a Texture struct.
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
	unsigned int TextureFromFile(const char *path, const string &directory);
	
	// Draw Pipeline Variable
	float near_plane = 1.0f;
	float far_plane = 25.0f;
	glm::mat4 shadowProj, lightView;
	glm::mat4 lightSpaceMatrix;
	glm::vec3 lightPos;
	
	int Light_Length = 3;

	glm::vec3 EulerAngles;
	//glm::quat MyQuaternion;
	//void CreatePivotVollision();
};

#endif //MESHRENDER_H


