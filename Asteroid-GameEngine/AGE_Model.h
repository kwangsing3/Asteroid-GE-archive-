#ifndef AGE_MODEL_H
#define AGE_MODEL_H

#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include <vector>
#include <SceneManager.h>
#include <AGE_Mesh.h>
#include <SkeletalMesh.h>

enum struct Shape{ Plane, Cube, Sphere, Capsule, Cylinder, _Model, DEBUG };

class AGE_Model    //
{
protected:
	const aiScene* _aiScene;
	Assimp::Importer importer;
private:
	std::string directory;
	void CreateShape(Shape _sha)
	{
		this->_shape = _shape;
		std::string DefaultShapePath;
		switch (_shape)
		{
		case Shape::Plane:
			DefaultShapePath = "";
			break;
		case Shape::Cube:
			DefaultShapePath = "ExampleModel/Cube.obj";
			break;
		case Shape::Sphere:
			DefaultShapePath = "ExampleModel/Sphere.obj";
			break;
		case Shape::Capsule:
			DefaultShapePath = "";
			break;
		case Shape::Cylinder:
			DefaultShapePath = "ExampleModel/Cylinder.obj";
			break;
		default:
			return;
			break;
		}
		this->_ModelPath = DefaultShapePath;
		for (int i = 0; i < SceneManager::ModelList.size(); i++)
		{
			if (SceneManager::ModelList[i]->path == DefaultShapePath)
			{
				this->_meshes = SceneManager::ModelList[i]->_meshes;
				return;
			}
		}
		loadModel(DefaultShapePath);
	}
	void CreateShape(std::string _path)
	{
		this->_shape = Shape::_Model;
		this->_ModelPath = _path;
		for (int i = 0; i < SceneManager::ModelList.size(); i++)
		{
			if (SceneManager::ModelList[i]->path == _path)
			{
				this->_meshes = SceneManager::ModelList[i]->_meshes;
				return;
			}
		}
		loadModel(_path);
	}
	void loadModel(std::string const& path);
	Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
	SkeletalMesh* processSkeletalMesh(aiMesh* mesh, const aiScene* scene);
	void processNode(aiNode* node, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);


	std::vector<Texture> textures_loaded;
public:
	Shape _shape = Shape::DEBUG;
	std::vector<Mesh*> _meshes;
	std::string _ModelPath;
	AGE_Model() {};
	AGE_Model(Shape _sha) { CreateShape(_sha); };
	AGE_Model(std::string _path) { CreateShape(_path); };

};





#endif // !AGE_MODEL_H
