#include "AppWindow.h"
#include <Windows.h>

#include "Cube.h"
#include "Matrix4x4.h"
#include "InputSystem.h"
#include "EngineTime.h"
#include "GameObjectManager.h"
#include "PrimitiveCreation.h"
#include <vector>

#include "MathUtils.h"
#include "PassRender.h"
#include "Mesh.h"


AppWindow::AppWindow()
{
}

AppWindow::~AppWindow()
{
}

void AppWindow::onCreate()
{
	Window::onCreate();

	// subscribe this class to the InputSystem
	InputSystem::get()->addListener(this);
	// hides the cursor
	InputSystem::get()->showCursor(false);

	RECT rc = this->getClientWindowRect();
	m_swap_chain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(
		this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	// set the camera transform position -2 away from the object
	m_world_cam.setTranslation(Vector3D(0, 0, -2));

	for (int i = 0; i < 100; ++i)
	{
		float x = MathUtils::randomFloat(0, 2.5f);
		float y = MathUtils::randomFloat(0, 2);

		// instantiate a cube and texture
		std::string name = "cube" + std::to_string(i);
		Cube* cube = new Cube(name, ObjectTypes::CUBE);
		cube->SetSpeed(MathUtils::randomFloat(-4.0f, 4.0f));
		cube->SetScale(Vector3D{ 0.25f, 0.25f, 0.25f });
		cube->SetPosition(Vector3D{ x, y, 0 });
		AGameObjectPtr temp_ptr(cube);
		GameObjectManager::get()->objectList.push_back(temp_ptr);
	}

#define SWITCH 2
#if SWITCH == 0 // First demo; no alpha blending yet
	wood_obj->SetTransform();
	coat_obj->SetTransform(Vector3D{ 0,0,-2.0f });
	dynamic_cast<Cube*>(wood_obj)->SetAlpha(1.0f);
	dynamic_cast<Cube*>(coat_obj)->SetAlpha(1.0f);
	// add the objects to our manager
	GameObjectManager::get()->objectList.push_back(temp_ptr);
	GameObjectManager::get()->objectList.push_back(temp_ptr2);
#elif SWITCH == 1 // Demonstrate with no PassRender
	wood_obj->SetTransform();
	coat_obj->SetTransform(Vector3D{ 0,0,-2.0f });
	dynamic_cast<Cube*>(wood_obj)->SetAlpha(1.0f);
	dynamic_cast<Cube*>(coat_obj)->SetAlpha(0.5f);
	// add the objects to our manager
	GameObjectManager::get()->objectList.push_back(temp_ptr2);
	GameObjectManager::get()->objectList.push_back(temp_ptr);
#elif SWITCH == 2 // Demonstrate with PassRender
#endif

	// gets the byte code and size of the vertex shader
	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::get()->getPixelShaderByteCodeAndSize(&shader_byte_code, &size_shader);
	// after a successful compiling, create the vertex buffer then
	m_vs = GraphicsEngine::get()->getRenderSystem()->createVertexShader(shader_byte_code, size_shader);

	// access the PixelShader.hlsl and compile
	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	// create blenderPtr
	m_blender = GraphicsEngine::get()->getRenderSystem()->createBlender();

}

float randomFloat(float x, float y)
{
	srand(time(NULL));
	return rand() % 3;
}

// updating our constant buffers
void AppWindow::update()
{
	// Call each object's constant buffer in the scene
	std::vector<AGameObjectPtr>::iterator i;
	for (i = GameObjectManager::get()->objectList.begin(); i != GameObjectManager::get()->objectList.end(); ++i)
	{
		//std::static_pointer_cast<Cube>(*i)->m_cb->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);
		if ((*i) != nullptr)
			(*i)->Update(EngineTime::getDeltaTime(), this);
	}

}

void AppWindow::onUpdate()
{
	Window::onUpdate();

	// run the update for the InputSystem
	InputSystem::get()->update();

	//CLEAR THE RENDER TARGET 
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain,
		1.0f, 1.0f, 1.0f, 1);
	//SET VIEWPORT OF RENDER TARGET IN WHICH WE HAVE TO DRAW
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	update();

#define PASS 1
#if PASS == 0 // Demonstrate with no PassRender
	// Call each object in the scene
	std::vector<AGameObjectPtr>::iterator i;
	for (i = GameObjectManager::get()->objectList.begin(); i != GameObjectManager::get()->objectList.end(); ++i)
	{
		(*i)->Draw(m_vs, m_ps, m_blender);
	}
#elif PASS == 1 // Demonstrate with PassRender
	PassRender<OpaqueFilterPolicy, FrontToBackPolicy> opaquePass;
	opaquePass.Render(m_vs, m_ps, m_blender, m_world_cam);

	PassRender<TransparencyFilterPolicy, BackToFrontPolicy> transparencyPass;
	transparencyPass.Render(m_vs, m_ps, m_blender, m_world_cam);
#endif

	m_swap_chain->present(true);
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
}

void AppWindow::onFocus()
{
	// subscribe this class to the InputSystem
	InputSystem::get()->addListener(this);
}

void AppWindow::onKillFocus()
{
	InputSystem::get()->removeListener(this);
}

#define NAVIGATE_SPEED 5.0F

// InputListener virtual method definitions
void AppWindow::onKeyDown(int key)
{
	if (key == 'W')
	{
		//m_rot_x += 0.707f * m_delta_time;
		m_forward = 1.0f * EngineTime::getDeltaTime() * NAVIGATE_SPEED;
	}
	else if (key == 'S')
	{
		//m_rot_x -= 0.707f * m_delta_time;
		m_forward = -1.0f * EngineTime::getDeltaTime() * NAVIGATE_SPEED;
	}
	else if (key == 'A')
	{
		//m_rot_y += 0.707f * m_delta_time;
		m_rightward = -1.0f * EngineTime::getDeltaTime() * NAVIGATE_SPEED;
	}
	else if (key == 'D')
	{
		//m_rot_y -= 0.707f * m_delta_time;
		m_rightward = 1.0f * EngineTime::getDeltaTime() * NAVIGATE_SPEED;
	}
}

void AppWindow::onKeyUp(int key)
{
	// stops the camera
	m_forward = 0.0f;
	m_rightward = 0.0f;
}

void AppWindow::onMouseMove(const Point& mouse_pos)
{
	// width and height of the screen
	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);

	m_rot_x += (mouse_pos.m_y - (height / 2.0f)) * EngineTime::getDeltaTime() * 0.1f;
	m_rot_y += (mouse_pos.m_x - (width / 2.0f)) * EngineTime::getDeltaTime() * 0.1f;

	InputSystem::get()->setCursorPosition(Point((int)(width / 2.0f), (int)(height / 2.0f)));
}

void AppWindow::onLeftMouseDown(const Point& delta_mouse_pos)
{
	m_scale_cube = 0.5f;
}

void AppWindow::onLeftMouseUp(const Point& delta_mouse_pos)
{
	m_scale_cube = 1.0f;
}

void AppWindow::onRightMouseDown(const Point& delta_mouse_pos)
{
	m_scale_cube = 2.0f;
}

void AppWindow::onRightMouseUp(const Point& delta_mouse_pos)
{
	m_scale_cube = 1.0f;
}

