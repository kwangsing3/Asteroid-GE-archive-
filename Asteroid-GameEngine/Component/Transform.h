#ifndef CLASS_TRANSFORM_H
#define CLASS_TRANSFORM_H
#include<Component/Component.h>



class Transform:public Component
{
public:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	char* name;
	bool enabled;
	Transform()
	{
		position = glm::vec3(0.0f);
		rotation = glm::vec3(0.0f);
		scale = glm::vec3(1.0f);
		name =(char*)"New Actor";
		enabled = true;
	}
	void SaveFile( pugi::xml_node* _node) override;
	void OpenFile( pugi::xml_node* _node) override;
	void Translate(glm::vec3 _pos);
	void Rotate(glm::vec3 _rot);
	//void Rotate(glm::Quaternion _qu);
	void Scale(glm::vec3 _scal);
	 
};


#endif // !TRANSFORM_H
