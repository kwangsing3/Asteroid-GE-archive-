#ifndef SHADER_LIST_H
#define SHADER_LIST_H
#include <shader_m.h>
#include <vector>
#include <glm/glm.hpp>
#include <ADD_Component.h>
#include <AGE_Model.h>
class Actor;
class Meshrender;
class DirectionalLight;
class PointLight;
class Transform;
class Mesh;

struct Render_Struct
{
	Meshrender* _meshrender;
	bool Drawing = true;
	unsigned int amount = 0;
	unsigned int DrawingAmount = 0;
	std::vector<Transform*> transformList;
	std::vector<Transform*> DrawingtransformList;
	std::vector<bool*> _visableList;
};
struct ModelLoadStruct
{
	std::string path;
	AGE_Model* _model;
	ModelLoadStruct(std::string _pat, AGE_Model* _mod)
	{
		path = _pat; 
		_model = _mod;
	}
};
enum class RenderShadowType { Normal, DiectionalLight, PointLight };


class SceneManager     //SceneManager分兩個部分，管理遊戲場景物件   以及場景檔案的相關操作
{
public:
	//管理遊戲場景物件
	std::vector<Actor*> Objects;
	std::vector<Render_Struct*> vec_ObjectsToRender_Instancing;
	std::vector<Meshrender*> vec_ObjectsToRender;
	std::vector<DirectionalLight*> vec_DirectionlLight;
	std::vector<PointLight*> vec_PointLight;
	std::vector<Shader*> vec_ShaderProgram;
	bool NeedReloadShader = true;
	bool NeedInitedDraw;
	ADD_Component* _ADDManager;

	std::vector<ModelLoadStruct*> ModelList;

	//載入檔案
	void OpenFile();
	void OpenFile(int _index);
	void SaveFile();
	void NewScene();
	void CheckReloadShader()
	{
		if (!NeedReloadShader) return;
		this->NeedReloadShader = false;
		CreateShader();
	}


	std::string _FilePAth;
	SceneManager() 
	{ 
		CheckReloadShader(); 
		_ADDManager = new ADD_Component(this);
	}
	

	void DrawScene(bool _drawShadow, unsigned int _dp=NULL);
	void DrawScene(RenderShadowType _RTypw);



	//static void DrawScene();   //this is test draw scene, only has simple drawing.
	void AddToRenderPipeline_Instancing(Meshrender* _mrender);
	void AddToRenderPipeline(Meshrender* _mrender);
	void UpdateRenderPipeline(Meshrender * _mrender);
private:
	void CreateShader()
	{
		if (!vec_ShaderProgram.empty())
			vec_ShaderProgram.clear();

		/*0*/vec_ShaderProgram.push_back(new Shader("Shader/SimpleDrawShader.vs", "Shader/SimpleDrawShader.fs"));
		/*1*/vec_ShaderProgram.push_back(new Shader("Shader/StandardVertexShader.vs", "Shader/StandardFragmentShader.fs"));
		/*2*/vec_ShaderProgram.push_back(new Shader("Shader/shadow_mapping_depth.vs", "Shader/shadow_mapping_depth.fs", "Shader/point_shadows_depth.gs"));
		/*3*/vec_ShaderProgram.push_back(new Shader("Shader/shadow_mapping.vs", "Shader/shadow_mapping.fs"));
		/*4*/vec_ShaderProgram.push_back(new Shader("Shader/ModelShader.vs", "Shader/ModelShader.fs")); //目前的最主要Shader
		/*5*/vec_ShaderProgram.push_back(new Shader("Shader/ModelShader_ins.vs", "Shader/ModelShader_ins.fs"));    //目前的最主要Shader instance
		/*6*/vec_ShaderProgram.push_back(new Shader("Shader/SimpleTextureShader_Instancing.vs", "Shader/SimpleTextureShader_Instancing.fs"));    //目前的最主要Shader

		//  先暫時放這裡  影子相關
		vec_ShaderProgram[4]->use();
		vec_ShaderProgram[4]->setInt("diffuseTexture", 0);
		vec_ShaderProgram[4]->setInt("depthMap", 1);
		vec_ShaderProgram[4]->setInt("shadowMap", 1);
		vec_ShaderProgram[5]->use();
		vec_ShaderProgram[5]->setInt("diffuseTexture", 0);
		vec_ShaderProgram[5]->setInt("depthMap", 1);
		vec_ShaderProgram[5]->setInt("shadowMap", 1);
	}
	//static glm::vec3 lightPos;
	void InitDrawPipline();
	void Draw_Instancing(bool _drawShadow, unsigned int _dp);
	void Draw_Normal(bool _drawShadow, unsigned int _dp);
};






#endif // !SHADER_LIST_H


