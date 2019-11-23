//
//  Actor.hpp
//  Asteroid-GameEngine
//
//  Created by KS on 2019/11/20.
//

#ifndef ACTOR_H
#define ACTOR_H

#include <Component/Transform.hpp>
#include <Component/DirectionalLight.hpp>
#include <Component/PointLight.hpp>
#include <Component/Meshrender.hpp>
#include <Component/BoxCollision.hpp>

class Actor
{
public:

	Transform* transform = NULL;
	DirectionalLight* _Dirlight = NULL;
	PointLight* _PointLight = NULL;
	Meshrender* meshrender = NULL;
	BoxCollision* boxcollision = NULL;
	bool enabled;

	void removeActor();
	//bool IsSelected;

	Actor()
	{
		transform = new Transform();
		transform->_actor = this;
		transform->name = (char*)"Actor";
		enabled = true;



	}


private:

};



#endif /* Actor_hpp */
