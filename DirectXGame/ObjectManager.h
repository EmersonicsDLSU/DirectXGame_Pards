#pragma once
#include <iostream>
#include <vector>
#include "AGameobject.h"

class ObjectCreation;
class AppWindow;

class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();
	//Release all the resources loaded
	bool release();
	static ObjectManager* get();
private:
	std::vector<AGameobject*> objectList;
	friend class ObjectCreation;
	friend class AppWindow;
};

