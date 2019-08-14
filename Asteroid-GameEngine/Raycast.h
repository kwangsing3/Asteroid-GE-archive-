#ifndef RAYCAST_H
#define RAYCAST_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Raycast
{
public:
	static glm::vec3 hitposition;
	Raycast() {}
	static glm::vec3 GetWorldPosition(float alpha);
	static glm::vec3 GetScreenPosition(glm::vec3 _pos);
	static glm::vec3 GetRaycastVector();
};



#endif // !RAYCAST_H


