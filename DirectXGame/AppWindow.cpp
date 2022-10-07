/*MIT License

C++ 3D Game Tutorial Series (https://github.com/PardCode/CPP-3D-Game-Tutorial-Series)

Copyright (c) 2019-2022, PardCode

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/


#include "AppWindow.h"

#include <iostream>
#include <Windows.h>

#include "EngineTime.h"
#include "Vector3D.h"
#include "Matrix4x4.h"

struct vertex
{
	Vector3D position;
	Vector3D position1;
	Vector3D color;
	Vector3D color1;
};

// constant buffer; this is updated per frame
__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	float m_time;
};


AppWindow::AppWindow()
{
}

float AppWindow::lerp(float start, float end, float delta)
{
	return start * (1.0f - delta) + end * (delta);
}

// updating our constant buffer
void AppWindow::updateQuadPosition()
{

	constant cc;
	
	static float tick = 0.0f;
	tick += EngineTime::getDeltaTime();
	cc.m_time = tick;
	

#define SWITCH 1
#if SWITCH == 0 
	static float tick = 0.0f;
	static float speed = 0.5f;
	tick += EngineTime::getDeltaTime() / 2.0f;
	speed = lerp(0.01f, 2.0f, (sin(tick) + 1.0f) / 2.0f);
	// increments our vertex position
	m_delta_pos += EngineTime::getDeltaTime() * speed;

	Matrix4x4 temp;
	m_delta_scale += EngineTime::getDeltaTime() / 0.15f;
	// creates a scale animation
	cc.m_world.setScale(Vector3D::lerp(Vector3D(0.5, 0.5, 0), Vector3D(1.0f, 1.0f, 0), (sin(m_delta_scale) + 1.0f) / 2.0f));
	// creates a translation animation
	temp.setTranslation(Vector3D::lerp(Vector3D(-0.5f, -0.5f, 0), Vector3D(0.5f, 0.5f, 0), (sin(m_delta_pos) + 1.0f) / 2.0f));

	//std::cout << "DeltaPos: " << m_delta_pos << " -> " << (sin(m_delta_pos) + 1.0f) / 2.0f << "\n";

	// Transformation of matrices; Note that order is important
	cc.m_world *= temp;
#elif SWITCH == 1
	m_delta_pos += EngineTime::getDeltaTime() * 1.0F;
	/*
	if (m_delta_pos > 1.0f)
		m_delta_pos = 0;
	*/

	Matrix4x4 temp;
	cc.m_world.setScale(Vector3D(1.0f, 1.0f, 0));
	temp.setTranslation(Vector3D(0.0f, 0.0f, 0));
	cc.m_world *= temp;
	
#endif
	

	cc.m_view.setIdentity();
	cc.m_proj.setOrthoLH
	(
		(this->getClientWindowRect().right - this->getClientWindowRect().left) / 400.0f,
		(this->getClientWindowRect().bottom - this->getClientWindowRect().top) / 400.0f,
		-4.0f,
		4.0f
	);


	m_cb->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);
}


AppWindow::~AppWindow()
{
}

void AppWindow::onCreate()
{
	Window::onCreate();
	GraphicsEngine::get()->init();
	m_swap_chain = GraphicsEngine::get()->createSwapChain();

	RECT rc = this->getClientWindowRect();
	m_swap_chain->init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

#define SAMPLE 1
#if SAMPLE == 0
	vertex list[] =
	{
		//X - Y - Z
		{Vector3D(-0.5f,-0.5f,0.0f),    Vector3D(-0.32f,-0.11f,0.0f),   Vector3D(0,0,0), Vector3D(0,1,0) }, // POS1
		{Vector3D(-0.5f,0.5f,0.0f),     Vector3D(-0.11f,0.78f,0.0f),   Vector3D(1,1,0), Vector3D(0,1,1) }, // POS2
		{ Vector3D(0.5f,-0.5f,0.0f),     Vector3D(0.75f,-0.73f,0.0f), Vector3D(0,0,1),  Vector3D(1,0,0) },// POS2
		{ Vector3D(0.5f,0.5f,0.0f),     Vector3D(0.88f,0.77f,0.0f),    Vector3D(1,1,1), Vector3D(0,0,1) }
	};
#elif SAMPLE == 1
	vertex list[] =
	{
		//X - Y - Z
		{Vector3D(-0.2f,-0.1f,0.0f),    Vector3D(-0.7f,-0.9f,0.0f),   Vector3D(0,1,0), Vector3D(1,0,0) }, // POS1
		{Vector3D(-0.1f,0.8f,0.0f),     Vector3D(-0.9f,0.2f,0.0f),   Vector3D(1,1,0), Vector3D(1,1,0) }, // POS2
		{ Vector3D(0.1f,-0.7f,0.0f),     Vector3D(0.95f,-0.3f,0.0f), Vector3D(1,0,0),  Vector3D(0,0,1) },// POS2
		{ Vector3D(0.80f,0.80f,0.0f),     Vector3D(-0.7f,-0.9f,0.0f),    Vector3D(0,0,1), Vector3D(1,1,1) }
	};
#endif

	m_vb = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(list);

	// gets the byte code and size of the vertex shader
	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	// access the VertexShader.hlsl and compile
	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	// after a successful compiling, create the vertex buffer then
	m_vs = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);
	// drawing  object
	m_vb->load(list, sizeof(vertex), size_list, shader_byte_code, size_shader);

	GraphicsEngine::get()->releaseCompiledShader();

	// access the PixelShader.hlsl and compile
	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->releaseCompiledShader();

	constant cc;
	cc.m_time = 0;

	m_cb = GraphicsEngine::get()->createConstantBuffer();
	m_cb->load(&cc, sizeof(constant));

}

void AppWindow::onUpdate()
{
	Window::onUpdate();
	//CLEAR THE RENDER TARGET 
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain,
		0, 0.3f, 0.4f, 1);
	//SET VIEWPORT OF RENDER TARGET IN WHICH WE HAVE TO DRAW
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	updateQuadPosition();

	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_vs, m_cb);
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb);

	//SET DEFAULT SHADER IN THE GRAPHICS PIPELINE TO BE ABLE TO DRAW
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(m_ps);


	//SET THE VERTICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb);

	// FINALLY DRAW THE TRIANGLE
	GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(m_vb->getSizeVertexList(), 0);
	m_swap_chain->present(true);

}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_vb->release();
	m_swap_chain->release();
	m_vs->release();
	m_ps->release();
	GraphicsEngine::get()->release();
}
