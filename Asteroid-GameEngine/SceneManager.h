#ifndef SHADER_LIST_H
#define SHADER_LIST_H
#include <shader_m.h>
#include <vector>
#include <Units/Actor.h>
#include <ADD_Component.h>
#include <Component/DirectionalLight.h>
#include <Component/PointLight.h>

#include <Component/Meshrender.h>
#include <Collision/BoxCollision.h>
class SceneManager     //SceneManager分兩個部分，管理遊戲場景物件   以及場景檔案的相關操作
{
public:
	//管理遊戲場景物件
	static std::vector<Actor*> Objects;
	static std::vector<Meshrender*> vec_ObjectsToRender;
	static std::vector<DirectionalLight*> vec_DirectionlLight;
	static std::vector<PointLight*> vec_PointLight;
	static std::vector<Shader> vec_ShaderProgram;

	//載入檔案
	static void OpenFile();
	static void SaveFile();
	static void NewScene();

	SceneManager()
	{
		/*0*/Shader _SimpleShader("Shader/SimpleVertexShader.vs", "Shader/SimpleFragmentShader.fs");
		/*1*/Shader _StandardShader("Shader/StandardVertexShader.vs", "Shader/StandardFragmentShader.fs");
		/*2*/Shader _DepthShader("Shader/shadow_mapping_depth.vs", "Shader/shadow_mapping_depth.fs", "Shader/point_shadows_depth.gs");
		/*3*/Shader _ShadowdShader("Shader/shadow_mapping.vs", "Shader/shadow_mapping.fs");
		/*4*/Shader _ModelShader("Shader/ModelShader.vs", "Shader/ModelShader.fs");
		SceneManager::vec_ShaderProgram.push_back(_SimpleShader);
		SceneManager::vec_ShaderProgram.push_back(_StandardShader);
		SceneManager::vec_ShaderProgram.push_back(_DepthShader);
		SceneManager::vec_ShaderProgram.push_back(_ShadowdShader);
		SceneManager::vec_ShaderProgram.push_back(_ModelShader);
	}
private:

};






#endif // !SHADER_LIST_H


