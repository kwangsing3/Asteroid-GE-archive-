//
//  Component.cpp
//  Asteroid-GameEngine
//
//  Created by KS on 2019/11/20.
//

#include <Component/Component.hpp>
#include <Actor.hpp>
#include <time.h>

std::vector<int> Component_Interface::vec_ID;

unsigned Component_Interface::GetTimeSeed()
{
    return (unsigned)time(NULL);
}
