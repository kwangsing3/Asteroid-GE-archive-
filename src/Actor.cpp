//
//  Actor.hpp
//  Asteroid-GameEngine
//
//  Created by KS on 2019/11/20.
//
#include <Actor.hpp>
#include <Component/Component.hpp>
#include <Component/Transform.hpp>
void Actor::inited()
{
	enabled = true;
	transform = new Transform(this);
	this->_Components.push_back(transform);
}
