#ifndef WORLD_H
#define WORLD_H
#include <iostream>
#include <AGE_Assert.h>
#include <SceneManager.h>


class World
{
public:
    World()
    {
        _SceneManager = new SceneManager();
        
    }
    SceneManager* _SceneManager;
    
};






#endif // !WORLD_H
