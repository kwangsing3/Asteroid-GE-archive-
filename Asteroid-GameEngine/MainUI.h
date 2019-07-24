#ifndef MAINUI_H
#define MAINUI_H

#include <Units/Actor.h>
#include <iostream>
struct SelectObject
{
	Actor* _actor;
	int _index=0;
	bool Is_selected = false;
	bool Is_renaming = false;
	SelectObject* next = NULL;
};

class InspectorManager
{
public:

	static SelectObject *cur_SelectObject;
	static InspectorManager _InspectorManager;
	static void Deletecur_actor(SelectObject* cur_actor);
	static void Renamecur_actor(SelectObject* cur_actor);
	InspectorManager()
	{
		

	}

	
	void ShowInspector(SelectObject *actor);
	void ListInspectorCur();
};



namespace MyImGui
{
	
	void ShowMyImGUIDemoWindow(bool *p_open, unsigned int *width, unsigned int *height, unsigned int textureColorbuffer);




	

}













#endif // ! MAINUI_H
