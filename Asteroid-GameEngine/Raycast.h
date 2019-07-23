#ifndef RAYCAST_H
#define RAYCAST_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Raycast
{
public:
	static glm::vec3 hitposition;
	Raycast() {}
	static glm::vec3 GetWorldPosition();
	
};



#endif // !RAYCAST_H


