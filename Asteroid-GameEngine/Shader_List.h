#ifndef SHADER_LIST_H
#define SHADER_LIST_H
#include <shader_m.h>
#include <vector>



class Shader_List
{
public:
	static std::vector<Shader> vec_ShaderProgram;

	Shader_List()
	{
		CreateShader();
	}
private:
	void CreateShader()
	{
		Shader _StandardShader("Shader/VertexShader.vs", "Shader/FragmentShader.fs");
		Shader _StandardShader2("Shader/VertexShader.vs", "Shader/FragmentShader.fs");
		Shader_List::vec_ShaderProgram.push_back(_StandardShader);
		Shader_List::vec_ShaderProgram.push_back(_StandardShader2);
	}
};






#endif // !SHADER_LIST_H


