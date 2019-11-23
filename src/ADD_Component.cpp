#include <AGE_Assert.hpp>

#include<ADD_Component.hpp>
#include <Actor.hpp>
#include <Meshrender.hpp>
#include <DirectionalLight.hpp>
#include <PointLight.hpp>
#include <BoxCollision.hpp>

#include <SceneManager.hpp>

bool use_Instance = false;

Actor * ADD_Component::Add_Actor()
{
	Actor* _Actor = new Actor();
	_OwnedSceneManager->Objects.push_back(_Actor);
	return _Actor;
}

Meshrender* ADD_Component::Add_Meshrender(Actor* _actor, Shape _sha)
{
	std::string DefaultShapePath;
	switch (_sha)
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
		AGE_ASSERT(false);
		break;
	}
	return Add_Meshrender(_actor, DefaultShapePath);
}
Meshrender* ADD_Component::Add_Meshrender(Actor* _actor, std::string _path)  // ADD Meshrender的時候檢查一下模型是否有先加載過
{
	_OwnedSceneManager->NeedInitedDraw = true;
	Actor* _ac = _actor == NULL ? Add_Actor() : _actor;

	/// If Found in ModelList then Copy
	for (int i = 0; i < _OwnedSceneManager->ModelList.size(); i++)
	{
		if (_OwnedSceneManager->ModelList[i]->path == _path)
		{
			Meshrender* _mesh = new Meshrender(_ac);
			_mesh->_model = _OwnedSceneManager->ModelList[i]->_model;
			_mesh->CreateMouseCollision();
			_ac->meshrender = _mesh;

			if (!use_Instance || _mesh->_model->HasBone)
				_OwnedSceneManager->AddToRenderPipeline(_mesh);
			else
				_OwnedSceneManager->AddToRenderPipeline_Instancing(_mesh);
			return _mesh;
		}
	}
	/// If not Found then create new ModelList
	Meshrender* _mesh = new Meshrender(_ac, _path);
	_OwnedSceneManager->ModelList.push_back(new ModelLoadStruct(_path, _mesh->_model));
	_ac->meshrender = _mesh;

	if (!use_Instance || _mesh->_model->HasBone)
		_OwnedSceneManager->AddToRenderPipeline(_mesh);
	else
		_OwnedSceneManager->AddToRenderPipeline_Instancing(_mesh);

	return _mesh;
}
Meshrender* ADD_Component::ADD_CustomMesh(float* vertexs, unsigned int _Length)   //或許以後可以加上其他分配方式，不過目前只能分配成線
{
	Actor* _ac = new Actor();
	Meshrender* _meshrender = new Meshrender(_ac);
	std::vector<Vertex> _newVer;
	std::vector<unsigned int> _newUint;

	for (int i = 0; i < _Length; i = i + 3)
	{
		Vertex _Nvertex;
		_Nvertex.Position = glm::vec3(vertexs[i], vertexs[i + 1], vertexs[i + 2]);
		_newVer.push_back(_Nvertex);
	}

	std::vector<Mesh*> _newMeshVec;
	_newMeshVec.push_back(new Mesh(_newVer));
	_meshrender->_model = new AGE_Model(_newMeshVec);
	_ac->meshrender = _meshrender;
	// 不應該加入到 Draw Pipeline 中  畢竟會繪製兩次  有另外獨立到SceneManager 中繪製
	return _meshrender;
}









DirectionalLight* ADD_Component::Add_DirectionalLight(Actor* _actor)
{
	Actor* _ac = _actor == NULL ? Add_Actor() : _actor;
	DirectionalLight* _DirLight = new DirectionalLight(_actor);
	//	_ac->transform->name = (char*)"Directional Light";
	_OwnedSceneManager->vec_DirectionlLight.push_back(_DirLight);
	_ac->_Dirlight = _DirLight;
	//_ac->transform->rotation = { -0.2f, -1.0f, -0.3f };
	return _DirLight;
}
PointLight* ADD_Component::Add_PointLight(Actor* _actor)
{
	Actor* _ac = _actor == NULL ? Add_Actor() : _actor;
	PointLight* _PoLight = new PointLight(_actor);
	//_ac->transform->name = (char*)"Point Light";
	_OwnedSceneManager->vec_PointLight.push_back(_PoLight);
	_ac->_PointLight = _PoLight;
	return _PoLight;
}

BoxCollision* ADD_Component::Add_BoxCollision(Actor* _actor, float mass)
{
	Actor* _ac = _actor == NULL ? Add_Actor() : _actor;
	if (_ac->meshrender == NULL) Add_Meshrender(_ac, Shape::Cube);
	BoxCollision* _box = new BoxCollision(_actor, mass);
	//_ac->transform->name = (char*)"Cube";
	_ac->boxcollision = _box;

	return _box;
}

Actor* ADD_Component::Copy_Actor(Actor* _actor)
{
	if (_actor == NULL) return NULL;
	Actor* _ac = Add_Actor();
	_ac->transform->Copy(_actor);

	if (_actor->meshrender != NULL)
		Add_Meshrender(_ac, _actor->meshrender->_model->_shape)->Copy(_actor);
	if (_actor->_Dirlight != NULL)
		Add_DirectionalLight(_ac)->Copy(_actor);
	if (_actor->_PointLight != NULL)
		Add_PointLight(_ac)->Copy(_actor);
	if (_actor->boxcollision != NULL)
		Add_BoxCollision(_ac, _actor->boxcollision->_Mass)->Copy(_actor);


	return _ac;
}


