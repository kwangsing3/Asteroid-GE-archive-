//
//  Actor.hpp
//  Asteroid-GameEngine
//
//  Created by KS on 2019/11/20.
//

#ifndef ACTOR_H
#define ACTOR_H
#include <vector>


class Component_Interface;
class Transform;
class Actor
{
public:


	bool enabled;
	std::vector<Component_Interface*> _Components;
	Transform* transform = NULL;
	void removeActor() {}

	Actor()
	{
		inited();
	}


private:
	void inited();
};



#endif /* Actor_hpp */
