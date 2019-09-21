#include <Component/Component.h>
//#include <Xml/pugixml.hpp>
//#include <Units/Actor.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

std::vector<int> Component::vec_ID;

unsigned Component::GetTimeSeed()
{
	return (unsigned)time(NULL);
}
