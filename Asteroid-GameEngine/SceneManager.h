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
struct Render_Struct
{
	Meshrender* _meshrender;
	unsigned int amount = 0;
	std::vector<Transform*> transformList;
};

class SceneManager     //SceneManager分兩個部分，管理遊戲場景物件   以及場景檔案的相關操作
{
public:
	//管理遊戲場景物件
	static std::vector<Actor*> Objects;
	static std::vector<Render_Struct*> vec_ObjectsToRender;
	static std::vector<DirectionalLight*> vec_DirectionlLight;
	static std::vector<PointLight*> vec_PointLight;
	static std::vector<Shader*> vec_ShaderProgram;
	static bool NeedReload;
	//載入檔案
	static void OpenFile();
	static void SaveFile();
	static void NewScene();
	
	SceneManager()
	{
		CreateShader();
	}
	void CheckReload()
	{
		if (!NeedReload) return;
		this->NeedReload = false;
		CreateShader();
	}

	static void DrawScene(bool _drawShadow, unsigned int _dp=NULL);
	static void AddToRenderPipeline(Meshrender* _mrender);

private:
	void CreateShader()
	{
		if (vec_ShaderProgram.size() > 0)
			vec_ShaderProgram.clear();

		/*0*/Shader* _SimpleShader = new Shader("Shader/SimpleVertexShader.vs", "Shader/SimpleFragmentShader.fs");
		/*1*/Shader* _StandardShader = new Shader("Shader/StandardVertexShader.vs", "Shader/StandardFragmentShader.fs");
		/*2*/Shader* _DepthShader = new Shader("Shader/shadow_mapping_depth.vs", "Shader/shadow_mapping_depth.fs", "Shader/point_shadows_depth.gs");
		/*3*/Shader* _ShadowdShader = new Shader("Shader/shadow_mapping.vs", "Shader/shadow_mapping.fs");
		/*4*/Shader* _ModelShader = new Shader("Shader/ModelShader.vs", "Shader/ModelShader.fs");    //目前的最主要Shader
		SceneManager::vec_ShaderProgram.push_back(_SimpleShader);
		SceneManager::vec_ShaderProgram.push_back(_StandardShader);
		SceneManager::vec_ShaderProgram.push_back(_DepthShader);
		SceneManager::vec_ShaderProgram.push_back(_ShadowdShader);
		SceneManager::vec_ShaderProgram.push_back(_ModelShader);

		//  先暫時放這裡  影子相關
		SceneManager::vec_ShaderProgram[4]->use();
		SceneManager::vec_ShaderProgram[4]->setInt("diffuseTexture", 0);
		SceneManager::vec_ShaderProgram[4]->setInt("depthMap", 1);
	}
	static glm::vec3 lightPos;
};






#endif // !SHADER_LIST_H


