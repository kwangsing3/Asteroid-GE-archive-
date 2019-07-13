#ifndef SHADER_LIST_H
#define SHADER_LIST_H
#include <shader_m.h>
#include <vector>
#include <Units/Actor.h>
#include <Units/DirectionalLight.h>
#include <Units/PointLight.h>
class SceneManager
{
public:
	static std::vector<Actor*> Objects;
	static std::vector<Actor*> vec_ObjectsToRender;
	static std::vector<DirectionalLight*> vec_DirectionlLight;
	static std::vector<PointLight*> vec_PointLight;

	static std::vector<Shader> vec_ShaderProgram;


	SceneManager()
	{
		CreateShader();
	}
private:
	void CreateShader()
	{
		Shader _SimpleShader("Shader/SimpleVertexShader.vs", "Shader/SimpleFragmentShader.fs");
		Shader _StandardShader("Shader/StandardVertexShader.vs", "Shader/StandardFragmentShader.fs");
		SceneManager::vec_ShaderProgram.push_back(_SimpleShader);
		SceneManager::vec_ShaderProgram.push_back(_StandardShader);

	}
};






#endif // !SHADER_LIST_H


