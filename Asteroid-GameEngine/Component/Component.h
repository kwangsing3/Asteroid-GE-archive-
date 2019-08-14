#ifndef COMPONENT_H
#define COMPONENT_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
class Component
{

public:
	bool enabled;
	int ID;
	static std::vector<int> vec_ID;
	virtual void Draw() {};
	Component()
	{
		enabled = false;	
		// Random ID
		unsigned seed;
		seed = (unsigned)time(NULL); // 取得時間序列
		srand(seed); // 以時間序列當亂數種子

		do
		{
			//ID = randint(9999 - 1000 + 1) + 1000;
			ID = rand();
	    } while (ReadVector(ID));
		Component::vec_ID.push_back(ID);
	}
private:
	
	bool ReadVector(int _id)
	{
		for (std::vector<int>::iterator it_i = Component::vec_ID.begin(); it_i != Component::vec_ID.end(); ++it_i)
		{
			if (*it_i == _id)
				return true;
		}
		return false;
	}

};



#endif // !COMPONENT_H
