#include "GameObjectManager.h"

GameObjectManager* GameObjectManager::m_instance = nullptr;

GameObjectManager::GameObjectManager()
{
}

GameObjectManager::~GameObjectManager()
{
}

GameObjectManager* GameObjectManager::get()
{
	return m_instance;
}

void GameObjectManager::create()
{
	if (GameObjectManager::m_instance)
		throw std::exception("GameObjectManager already created");
	GameObjectManager::m_instance = new GameObjectManager();
}

void GameObjectManager::release()
{
	if (!GameObjectManager::m_instance)
		return;
	delete GameObjectManager::m_instance;
}

std::vector<AGameObjectPtr>& GameObjectManager::getObjectList()
{
	return objectList;
}
