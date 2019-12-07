//
//  Transform.hpp
//  Asteroid-GameEngine
//
//  Created by KS on 2019/11/20.
//

#ifndef CLASS_TRANSFORM_H
#define CLASS_TRANSFORM_H
#include <Component/Component.hpp>


class btTransform;
class Actor;
class Transform:public Component_Interface
{
public:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    std::string name;
    
    Transform()
    {
        position = glm::vec3(0.0f);
        rotation = glm::vec3(0.0f);
        scale = glm::vec3(1.0f);
        name =(char*)"New Actor";
        enabled = true;
    }
    Transform(Actor* actor)
    {
        _actor = actor;
        position = glm::vec3(0.0f);
        rotation = glm::vec3(0.0f);
        scale = glm::vec3(1.0f);
        name = (char*)"New Actor";
        enabled = true;
    }
    void SaveFile( pugi::xml_node* _node) override;
    void OpenFile( pugi::xml_node* _node) override;
    void Copy(Component_Interface* _information) override;
    void Inspector() override;
    void MoveEvent() {};
    void Translate(glm::vec3 _pos);
    void Translate(glm::mat4 _pos);
    void Rotate(glm::vec3 _rot);
    //void Rotate(glm::Quaternion _qu);
    void Scale(glm::vec3 _scal);
    void MoveByPhysics(btTransform* _trans);
     
};


#endif // !TRANSFORM_H
