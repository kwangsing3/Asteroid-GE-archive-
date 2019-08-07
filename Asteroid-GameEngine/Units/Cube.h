#ifndef CUBE_H
#define CUBE_H

#include <Component/Component.h>
#include <Component/Transform.h>
#include <vector>
#include <Component/Meshrender.h>



class Cube:public Meshrender
{
	
public:
	enum RenderMode { Mode_2D, Mode_3D };
	RenderMode _mode;

	void Draw() override;
	//Transform* transform;
	unsigned int VBO, VAO;
	unsigned int VBO2D, VAO2D;
	unsigned int _Texture;
	std::vector<glm::vec3> Vectices_Debug;
	std::vector<glm::vec3> Worldvectices_Debug;
	std::vector<glm::vec3> Spacevectices_Debug;
	Cube(RenderMode mode)
	{
		_mode = mode;
		transform = new Transform();
		enabled = true;
		CreateCube();
		//transform->name = (char*)"Cube";
	}
	Cube(int m)
	{
		_mode = RenderMode(m);
		transform = new Transform();
		enabled = true;
		CreateCube();
		//transform->name = (char*)"Cube";
	}
	Cube()
	{
		_mode = RenderMode(1);
		transform = new Transform();
		enabled = true;
		CreateCube();
		//transform->name = (char*)"Cube";
	}
private:
	void CreateCube();
	unsigned int LoadTexture(const char* path);
	void UpdateWorldVertices();
	
};



#endif // !CUBE_H




