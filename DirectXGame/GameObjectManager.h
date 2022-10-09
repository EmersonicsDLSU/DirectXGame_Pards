#pragma once
#include <vector>
#include "Prerequisites.h"

class AppWindow;

class GameObjectManager
{
public:
	GameObjectManager();
	~GameObjectManager();

public:
	static GameObjectManager* get();
	static void create();
	static void release();
	std::vector<AGameObjectPtr>& getObjectList();
	
private:
	std::vector<AGameObjectPtr> objectList;
	static GameObjectManager* m_instance;
	friend class AppWindow;
	friend class Cube;
};
