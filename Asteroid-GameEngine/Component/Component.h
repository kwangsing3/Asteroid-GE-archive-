﻿#ifndef COMPONENT_H
#define COMPONENT_H


#include <vector>
#include <glm/glm.hpp>
#include <Xml/pugixml.hpp>

class Actor;


class Component
{

public:

	bool enabled;
	int ID;
	Actor* _actor;
	
	static std::vector<int> vec_ID;    // 放在這裡不安全  應該要歸類在別的地方

	virtual void SaveFile( pugi::xml_node* _node) {}
	virtual void OpenFile( pugi::xml_node* _node) {}

	Component()
	{
		enabled = false;	

		do
		{
			//ID = randint(9999 - 1000 + 1) + 1000;
			ID = rand();
	    } while (ReadVector(ID));
		Component::vec_ID.push_back(ID);
	}
private:
	unsigned GetTimeSeed();
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
