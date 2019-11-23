//
//  Actor.hpp
//  Asteroid-GameEngine
//
//  Created by KS on 2019/11/20.
//

#ifndef ACTOR_H
#define ACTOR_H

#include <Transform.hpp>
#include <DirectionalLight.hpp>
#include <PointLight.hpp>
#include <Meshrender.hpp>
#include <BoxCollision.hpp>

class Actor
{
public:

	Transform* transform;
	DirectionalLight* _Dirlight;
	PointLight* _PointLight;
	Meshrender* meshrender;
	BoxCollision* boxcollision;
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
