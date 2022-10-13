#include "GraphicsEngine.h"
#include "RenderSystem.h"
#include <exception>
#include <iostream>

GraphicsEngine* GraphicsEngine::m_engine = nullptr;

GraphicsEngine::GraphicsEngine()
{
	try
	{
		// instantiate our RenderSystem
		m_render_system = new RenderSystem();
	}
	catch (...)
	{
		throw std::exception("GraphicsEngine not created successfully");
	}
	try
	{
		// instantiate our TextureManager
		m_tex_manager = new TextureManager();
	}
	catch (...)
	{
		throw std::exception("TextureManager not created successfully");
	}
	try
	{
		// instantiate our TextureManager
		m_mesh_manager = new MeshManager();
	}
	catch (...)
	{
		throw std::exception("MeshManager not created successfully");
	}

	// gets the byte code and size of the vertex shader
	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	// access the VertexShader.hlsl and compile
	m_render_system->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	// copy the bytecode into our public field(primitive and shader byte codes)
	::memcpy(m_primitive_byte_code, shader_byte_code, size_shader);
	// set the primitive size
	m_primitive_size = size_shader;
	m_render_system->releaseCompiledShader();
	// access the VertexMeshLayoutShader.hlsl and compile
	m_render_system->compileVertexShader(L"VertexMeshLayoutShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	// copy the bytecode into our public field(layout and shader byte codes)
	::memcpy(m_mesh_layout_byte_code, shader_byte_code, size_shader);
	// set the layout size
	m_mesh_layout_size = size_shader;
	m_render_system->releaseCompiledShader();
}

GraphicsEngine::~GraphicsEngine()
{
	GraphicsEngine::m_engine = nullptr;
	delete m_mesh_manager;
	delete m_tex_manager;
	delete m_render_system;
}

RenderSystem* GraphicsEngine::getRenderSystem()
{
	return m_render_system;
}

TextureManager* GraphicsEngine::getTextureManager()
{
	return m_tex_manager;
}

MeshManager* GraphicsEngine::getMeshManager()
{
	return m_mesh_manager;
}

void GraphicsEngine::getVertexMeshLayoutShaderByteCodeAndSize(void** byte_code, size_t* size)
{
	*byte_code = m_mesh_layout_byte_code;
	*size = m_mesh_layout_size;
}

void GraphicsEngine::getPixelShaderByteCodeAndSize(void** byte_code, size_t* size)
{
	*byte_code = m_primitive_byte_code;
	*size = m_primitive_size;
}


GraphicsEngine* GraphicsEngine::get()
{
	return m_engine;
}

void GraphicsEngine::create()
{
	if (GraphicsEngine::m_engine) 
		throw std::exception("GraphicsEngine already created");
	GraphicsEngine::m_engine = new GraphicsEngine();
}

void GraphicsEngine::release()
{
	if (!GraphicsEngine::m_engine)
		return;
	delete GraphicsEngine::m_engine;
}
