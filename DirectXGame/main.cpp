#include "AppWindow.h"
#include "InputSystem.h"
#include "GameObjectManager.h"
#include "PrimitiveCreation.h"

int main()
{
	try
	{
		// initialize our GraphicsEngine
		GraphicsEngine::create();
		// initialize our InputSystem
		InputSystem::create();
		// initialize our GameObjectManager
		GameObjectManager::create();
		// initialize our PrimitiveCreation
		PrimitiveCreation::create();
	}
	catch (...) { return -1; }
	{
		try
		{
			AppWindow app;
			while (app.isRun());
		}
		catch (...) {
			InputSystem::release();
			GraphicsEngine::release();
			GameObjectManager::release();
			PrimitiveCreation::release();
			return -1;
		}
	}

	/* NOTE:
	 * The order of release should be based on the order of creation.
	 * Will follow the First in, Last out principle.
	 */
	InputSystem::release();
	GraphicsEngine::release();
	GameObjectManager::release();
	PrimitiveCreation::release();

	return 0;
}