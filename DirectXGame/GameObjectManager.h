#pragma once
#include <vector>
#include "Prerequisites.h"

class GameObjectManager
{
public:
	GameObjectManager();
	~GameObjectManager();
	//Release all the resources loaded
	bool release();
	static GameObjectManager* get();
private:
	std::vector<AGameObject*> objectList;
};

