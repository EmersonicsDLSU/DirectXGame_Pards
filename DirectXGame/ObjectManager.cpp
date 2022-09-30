#include "ObjectManager.h"

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
}

bool ObjectManager::release()
{
	std::vector<AGameobject*>::iterator i;
	for (i = objectList.begin(); i != objectList.end(); ++i)
	{
		static_cast<AGameobject*>(*i)->release();
		delete *i;
	}
	objectList.clear();
	return true;
}

ObjectManager* ObjectManager::get()
{
	static ObjectManager manager;
	return &manager;
}
