#include "AppWindow.h"
#include <Windows.h>

#include "Cube.h"
#include "Matrix4x4.h"
#include "InputSystem.h"
#include "EngineTime.h"
#include "GameObjectManager.h"
#include "PrimitiveCreation.h"
#include <vector>


AppWindow::AppWindow()
{
}

// updating our constant buffer
void AppWindow::update()
{
	constant cc;
	cc.m_time = ::GetTickCount();

	// increments our vertex position
	m_delta_pos += EngineTime::getDeltaTime() / 10.0f;
	if (m_delta_pos > 1.0f)
		m_delta_pos = 0;

	// objects matrix
	Matrix4x4 temp;

	m_delta_scale += EngineTime::getDeltaTime() / 0.55f;

	// creates a scale animation
	//cc.m_world.setScale(Vector3D::lerp(Vector3D(0.5, 0.5, 0), Vector3D(1.0f, 1.0f, 0), (sin(m_delta_scale) + 1.0f) / 2.0f));
	// creates a translation animation
	//temp.setTranslation(Vector3D::lerp(Vector3D(-1.5f, -1.5f, 0), Vector3D(1.5f,1.5f, 0), m_delta_pos));

	// Transformation of matrices; Note that order is important
	//cc.m_world *= temp;

	/*
	cc.m_world.setScale(Vector3D(m_scale_cube, m_scale_cube, m_scale_cube));

	// rotates it in the Z-axis
	temp.setIdentity();
	temp.setRotationZ(0.0f);
	cc.m_world *= temp;

	// rotates it in the Y-axis
	temp.setIdentity();
	temp.setRotationY(m_rot_y);
	cc.m_world *= temp;

	// rotates it in the X-axis
	temp.setIdentity();
	temp.setRotationX(m_rot_x);
	cc.m_world *= temp;
	*/

	cc.m_world.setIdentity();

	// creating the camera matrix
	Matrix4x4 world_cam;
	world_cam.setIdentity();
	// set the transform rotation X of the object
	temp.setIdentity();
	temp.setRotationX(m_rot_x);
	// make the object relative to the camera
	world_cam *= temp;
	// set the transform rotation Y of the object
	temp.setIdentity();
	temp.setRotationY(m_rot_y);
	// make the object relative to the camera
	world_cam *= temp;
	// moving or setting the camera position in the z or x axis
	Vector3D new_pos = m_world_cam.getTranslation() + world_cam.getZDirection() * (m_forward * 0.3f);
	 new_pos = new_pos + world_cam.getXDirection() * (m_rightward * 0.3f);
	// set the camera transform position -2 away from the object
	world_cam.setTranslation(new_pos);
	// save the newly transformed world_cam to the world_cam from the contant buffer
	m_world_cam = world_cam;
	// convert camera matrix to view matrix
	world_cam.inverse();
	// change the view matrix from our constant buffer to the world/camera matrix
	cc.m_view = world_cam;

	/*cc.m_proj.setOrthoLH
	(
		(this->getClientWindowRect().right - this->getClientWindowRect().left) / 300.0f,
		(this->getClientWindowRect().bottom - this->getClientWindowRect().top) / 300.0f,
		-4.0f,
		4.0f
	);*/

	// width and height of the screen
	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left) / 300.0f;
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().left) / 300.0f;
	// setting the perspective projection
	cc.m_proj.setPerspectiveFovLH(1.57f, ((float)width / (float)height), 0.1f, 100.0f);
	
	// Call each object's constant buffer in the scene
	/*
	std::vector<AGameObjectPtr>::iterator i;
	for (i = GameObjectManager::get()->objectList.begin(); i != GameObjectManager::get()->objectList.end(); ++i)
	{
		//std::static_pointer_cast<Cube>(*i)->m_cb->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);
		std::cout << "Sample: " << std::static_pointer_cast<Cube>(*i)->sample << "\n";
		//(*i)->Update(EngineTime::getDeltaTime(), &cc);
	}
	*/
	for (auto i : GameObjectManager::get()->objectList)
	{
		i->Update(EngineTime::getDeltaTime(), &cc);
	}
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

	// assign the texture file to the Texture pointer by passing the its path in the file

	RECT rc = this->getClientWindowRect();
	m_swap_chain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(
		this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);


	m_world_cam.setTranslation(Vector3D(0, 0, -2));

	// gets the byte code and size of the vertex shader
	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	// access the VertexShader.hlsl and compile
	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	// after a successful compiling, create the vertex buffer then
	m_vs = GraphicsEngine::get()->getRenderSystem()->createVertexShader(shader_byte_code, size_shader);

	// instantiate a cube and texture
	CubePtr cube_0(new Cube(shader_byte_code, size_shader, L"Assets\\Textures\\wood.jpg", 'A'));

	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	// access the PixelShader.hlsl and compile
	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	// create blenderPtr
	m_blender = GraphicsEngine::get()->getRenderSystem()->createBlender();

}

void AppWindow::onUpdate()
{
	Window::onUpdate();

	// run the update for the InputSystem
	InputSystem::get()->update();

	//CLEAR THE RENDER TARGET 
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain,
		0, 0.3f, 0.4f, 1);
	//SET VIEWPORT OF RENDER TARGET IN WHICH WE HAVE TO DRAW
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	update();

	// Call each object in the scene
	/*
	std::vector<AGameObjectPtr>::iterator i;
	for (i = GameObjectManager::get()->objectList.begin(); i != GameObjectManager::get()->objectList.end(); ++i)
	{
		static_cast<AGameObjectPtr>(*i)->Draw(m_vs, m_ps, m_blender);
	}
	*/
	for (auto i : GameObjectManager::get()->objectList)
	{
		i->Draw(m_vs, m_ps, m_blender);
	}

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

// InputListener virtual method definitions
void AppWindow::onKeyDown(int key)
{
	if (key == 'W')
	{
		//m_rot_x += 0.707f * m_delta_time;
		m_forward = 1.0f;
	}
	else if (key == 'S')
	{
		//m_rot_x -= 0.707f * m_delta_time;
		m_forward = -1.0f;
	}
	else if (key == 'A')
	{
		//m_rot_y += 0.707f * m_delta_time;
		m_rightward = -1.0f;
	}
	else if (key == 'D')
	{
		m_rot_y -= 0.707f * EngineTime::getDeltaTime();
		m_rightward = 1.0f;
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

