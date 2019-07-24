#ifndef COMPONENT_H
#define COMPONENT_H

class Component
{

public:
	bool enabled;
	virtual void Draw() {};
	Component()
	{
		enabled = false;	
	}

};



#endif // !COMPONENT_H
