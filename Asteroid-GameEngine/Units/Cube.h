#ifndef CUBE_H
#define CUBE_H

#include <Component/Component.h>
#include <Component/Transform.h>

class Cube:public Component
{
public:
	
	void Draw() override;
	Transform* transform;
	unsigned int VBO, VAO;
	unsigned int _Texture;


	Cube() 
	{
		CreateCube();
		transform = new Transform();
		enabled = true;
		//transform->name = (char*)"Cube";
	}
private:
	void CreateCube();
	unsigned int LoadTexture(const char* path);
};



#endif // !CUBE_H




