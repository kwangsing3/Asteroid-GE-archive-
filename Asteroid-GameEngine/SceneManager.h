#ifndef SHADER_LIST_H
#define SHADER_LIST_H
#include <shader_m.h>
#include <vector>
#include <Units/Actor.h>
#include <ADD_Component.h>
#include <Component/DirectionalLight.h>
#include <Component/PointLight.h>
#include "Xml/pugixml.hpp"
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
		Shader _SimpleShader("Shader/SimpleVertexShader.vs", "Shader/SimpleFragmentShader.fs");
		Shader _StandardShader("Shader/StandardVertexShader.vs", "Shader/StandardFragmentShader.fs");
		SceneManager::vec_ShaderProgram.push_back(_SimpleShader);
		SceneManager::vec_ShaderProgram.push_back(_StandardShader);
	}
private:

};






#endif // !SHADER_LIST_H


