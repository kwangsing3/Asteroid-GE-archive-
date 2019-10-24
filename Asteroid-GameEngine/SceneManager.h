#ifndef SHADER_LIST_H
#define SHADER_LIST_H
#include <shader_m.h>
#include <vector>
#include <glm/glm.hpp>

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
	std::vector<Mesh*> _meshes;
	std::string path;
	ModelLoadStruct(std::string _pat, std::vector<Mesh*> _mesh)
	{
		path = _pat; _meshes = _mesh;
	}

};



class SceneManager     //SceneManager分兩個部分，管理遊戲場景物件   以及場景檔案的相關操作
{
public:
	//管理遊戲場景物件
	static std::vector<Actor*> Objects;
	static std::vector<Render_Struct*> vec_ObjectsToRender_Instancing;
	static std::vector<Meshrender*> vec_ObjectsToRender;
	static std::vector<DirectionalLight*> vec_DirectionlLight;
	static std::vector<PointLight*> vec_PointLight;
	static std::vector<Shader*> vec_ShaderProgram;
	static bool NeedReloadShader;
	//載入檔案
	static void OpenFile();
	static void OpenFile(int _index);
	static void SaveFile();
	static void NewScene();
	void CheckReloadShader()
	{
		if (!NeedReloadShader) return;
		this->NeedReloadShader = false;
		CreateShader();
	}
	static bool NeedInitedDraw;
	static std::vector<ModelLoadStruct*> ModelList;
	static std::string _FilePAth;
	SceneManager()
	{
		CreateShader();
	}
	

	static void DrawScene(bool _drawShadow, unsigned int _dp=NULL);
	//static void DrawScene();   //this is test draw scene, only has simple drawing.
	static void AddToRenderPipeline_Instancing(Meshrender* _mrender);
	static void AddToRenderPipeline(Meshrender* _mrender);
	static void UpdateRenderPipeline(Meshrender * _mrender);
private:
	void CreateShader()
	{
		if (vec_ShaderProgram.size() > 0)
			vec_ShaderProgram.clear();

		/*0*/Shader* _SimpleShader = new Shader("Shader/SimpleDrawShader.vs", "Shader/SimpleDrawShader.fs");
		/*1*/Shader* _StandardShader = new Shader("Shader/StandardVertexShader.vs", "Shader/StandardFragmentShader.fs");
		/*2*/Shader* _DepthShader = new Shader("Shader/shadow_mapping_depth.vs", "Shader/shadow_mapping_depth.fs", "Shader/point_shadows_depth.gs");
		/*3*/Shader* _ShadowdShader = new Shader("Shader/shadow_mapping.vs", "Shader/shadow_mapping.fs");
		/*4*/Shader* _ModelShader = new Shader("Shader/ModelShader.vs", "Shader/ModelShader.fs");    //目前的最主要Shader
		/*5*/Shader* _ModelShader_instancing = new Shader("Shader/ModelShader_ins.vs", "Shader/ModelShader_ins.fs");    //目前的最主要Shader instance
		/*6*/Shader* _Simple_TextureShader = new Shader("Shader/SimpleTextureShader_Instancing.vs", "Shader/SimpleTextureShader_Instancing.fs");    //目前的最主要Shader
		SceneManager::vec_ShaderProgram.push_back(_SimpleShader);
		SceneManager::vec_ShaderProgram.push_back(_StandardShader);
		SceneManager::vec_ShaderProgram.push_back(_DepthShader);
		SceneManager::vec_ShaderProgram.push_back(_ShadowdShader);
		SceneManager::vec_ShaderProgram.push_back(_ModelShader);
		SceneManager::vec_ShaderProgram.push_back(_ModelShader_instancing);
		SceneManager::vec_ShaderProgram.push_back(_Simple_TextureShader);

		//  先暫時放這裡  影子相關
		SceneManager::vec_ShaderProgram[4]->use();
		SceneManager::vec_ShaderProgram[4]->setInt("diffuseTexture", 0);
		SceneManager::vec_ShaderProgram[4]->setInt("depthMap", 1);
		SceneManager::vec_ShaderProgram[4]->setInt("shadowMap", 1);
		SceneManager::vec_ShaderProgram[5]->use();
		SceneManager::vec_ShaderProgram[5]->setInt("diffuseTexture", 0);
		SceneManager::vec_ShaderProgram[5]->setInt("depthMap", 1);
		SceneManager::vec_ShaderProgram[5]->setInt("shadowMap", 1);
	}
	//static glm::vec3 lightPos;
	static void InitDrawPipline();
	static void Draw_Instancing(bool _drawShadow, unsigned int _dp);
	static void Draw_Normal(bool _drawShadow, unsigned int _dp);
};






#endif // !SHADER_LIST_H


