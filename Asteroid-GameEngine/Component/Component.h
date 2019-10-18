#ifndef COMPONENT_H
#define COMPONENT_H


#include <vector>
#include <glm/glm.hpp>
#include <Xml/include/pugixml.hpp>

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
	virtual void Copy(Actor* _actor) {}
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
		for (int i=0; i<vec_ID.size();i++)
		{
			if (vec_ID[i]== _id)
				return true;
		}
		return false;
	}

};



#endif // !COMPONENT_H
