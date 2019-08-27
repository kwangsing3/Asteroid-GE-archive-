#ifndef RAYCAST_H
#define RAYCAST_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Raycast
{
	
public:
	static float X_pos;
	static float Y_pos;

	static glm::vec3 MousePosInWorld;
	Raycast() {

	}
	static glm::vec3 GetWorldPosition(float alpha);
	static glm::vec3 GetScreenPosition(glm::vec3 _pos);

	static void SetMousePosition(float xpos,float ypos)
	{
		X_pos = xpos; Y_pos = ypos;
	}
};



#endif // !RAYCAST_H


