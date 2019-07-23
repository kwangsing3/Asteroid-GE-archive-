#ifndef MAINUI_H
#define MAINUI_H

#include <Units/Actor.h>

class InspectorManager
{
public:
	static Actor *cur_actor;
	static InspectorManager _InspectorManager;
	InspectorManager()
	{
		

	}

	void Deletecur_actor();
	void ShowInspector(Actor *actor);
	void ListInspectorCur();
};



namespace MyImGui
{
	
	void ShowMyImGUIDemoWindow(bool *p_open, unsigned int *width, unsigned int *height, unsigned int textureColorbuffer);




	

}













#endif // ! MAINUI_H
