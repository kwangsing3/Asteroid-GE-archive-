//
//  Component.cpp
//  Asteroid-GameEngine
//
//  Created by KS on 2019/11/20.
//

#include <Component.hpp>
#include <Actor.hpp>
#include <time.h>

std::vector<int> Component::vec_ID;

unsigned Component::GetTimeSeed()
{
    return (unsigned)time(NULL);
}
