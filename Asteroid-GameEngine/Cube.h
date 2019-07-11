#ifndef CUBE_H
#define CUBE_H
#include <Actor.h>
#include<Component/Transform.h>


class Cube:public Actor
{
public:
	
	void Draw() override;
	
	unsigned int VBO, VAO;
	unsigned int _Texture;


	Cube() 
	{
		CreateCube();
	};
private:
	void CreateCube();
	unsigned int LoadTexture(const char* path);
};



#endif // !CUBE_H




