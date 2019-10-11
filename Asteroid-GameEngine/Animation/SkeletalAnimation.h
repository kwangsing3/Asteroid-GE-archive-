#ifndef SKELETALANIMATION
#define SKELETALANIMATION

#include <assimp/scene.h>
#include <iostream>
#include <vector>

struct Animation
{
	std::string Name;
	double Duration;
	std::vector<aiNodeAnim*> _Channel;
	Animation(std::string _n,double _d, std::vector<aiNodeAnim*> _c)
	{
		Name = _n;
		Duration = _d;
		_Channel = _c;
	}

};


class SkeletalAnimation
{

public:
	bool HasAnimation = false;
	std::vector<Animation*> _Animations;
	SkeletalAnimation(const aiScene* _scene)
	{
		this->HasAnimation = _scene->HasAnimations();
		for (int i = 0; i < _scene->mNumAnimations; i++)
		{
			std::vector<aiNodeAnim*> _cc;
			for (int x = 0; x < _scene->mAnimations[i]->mNumChannels; x++)
			{
				_cc.push_back(_scene->mAnimations[i]->mChannels[x]);
			}


			Animation* _na = new Animation(_scene->mAnimations[i]->mName.data,_scene->mAnimations[i]->mDuration, _cc);
			_Animations.push_back(_na);
			
		}
		
		
	}

};


#endif // !SKELETALANIMATION
