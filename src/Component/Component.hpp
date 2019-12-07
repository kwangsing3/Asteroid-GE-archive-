//
//  Component.hpp
//  Asteroid-GameEngine
//
//  Created by KS on 2019/11/20.
//

#ifndef COMPONENT_H
#define COMPONENT_H


#include <vector>
#include <glm/glm.hpp>
#include <pugixml.hpp>
#include <GraphicEngine/imgui.h>


class Actor;
class Component_Interface
{

public:

    bool enabled;
    int ID;
    Actor* _actor;
    
    static std::vector<int> vec_ID;    // 放在這裡不好看  應該要歸類在別的地方
    virtual void SaveFile(pugi::xml_node* _node) = 0;
    virtual void OpenFile( pugi::xml_node* _node) = 0;
    virtual void Copy(Component_Interface* _information) = 0;
    virtual void Inspector() = 0;  //每幀呼叫，用來顯示
    virtual void MoveEvent() = 0;  //Translate Event, if needed
    Component_Interface()
    {
        enabled = false;
        do
        {
            ID = rand();   //ID = randint(9999 - 1000 + 1) + 1000;
        } while (ReadVector(ID));
        Component_Interface::vec_ID.push_back(ID);
    }
private:
    unsigned GetTimeSeed();
    bool ReadVector(int _id)
    {
        for (int i=0; i<vec_ID.size();i++)
        {
            if (vec_ID[i]== _id)
                return true;
        }
        return false;
    }

};

#endif /* Component_hpp */
