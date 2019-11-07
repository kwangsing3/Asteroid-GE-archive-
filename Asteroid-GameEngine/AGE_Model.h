#ifndef AGE_MODEL_H
#define AGE_MODEL_H
#include <AGE_Assert.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include <vector>

#include <AGE_Mesh.h>
#include <AGE_SkeletalMesh.h>
enum struct Shape { Plane, Cube, Sphere, Capsule, Cylinder, _Model, DEBUG };

class AGE_Model    
{
protected:
	const aiScene* _aiScene;
	Assimp::Importer importer;
private:
	std::string directory;
	/*void CreateShape(Shape _sha)
	{
		this->_shape = _sha;
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
		/*this->_ModelPath = DefaultShapePath;                 //要確認是否是載入過的模型交給ADD_Component 解決
		for (int i = 0; i < SceneManager::ModelList.size(); i++)                                               
		{
			if (SceneManager::ModelList[i]->path == DefaultShapePath)
			{
				this->_meshes = SceneManager::ModelList[i]->_meshes;
				return;
			}
		}
		loadModel(DefaultShapePath);
	}*/
	void CreateShape(std::string _path)
	{
		this->_shape = Shape::_Model;
		this->_ModelPath = _path;
		
		loadModel(_path);
	}
	void loadModel(std::string const& path);
	Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
	SkeletalMesh* processSkeletalMesh(aiMesh* mesh, const aiScene* scene);
	void processNode(aiNode* node, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);


	
public:
	Shape _shape = Shape::DEBUG;
	std::vector<Mesh*> _meshes;
	std::vector<Texture> textures_loaded;
	bool HasBone = false;
	std::string _ModelPath;
	AGE_Model() { /*AGE_PRINTCONSLE("Careful, this should been created with no thing put in")*/};
/*	AGE_Model(Shape _sha) { CreateShape(_sha); };*/
	AGE_Model(std::string _path) { CreateShape(_path); };
	AGE_Model(std::vector<Mesh*> _mesh) { _meshes = _mesh; };
};





#endif // !AGE_MODEL_H
