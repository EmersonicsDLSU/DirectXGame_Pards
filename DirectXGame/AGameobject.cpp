#include "AGameobject.h"


AGameobject::AGameobject()
{
}

AGameobject::~AGameobject()
{
}

void AGameobject::load(void* list_vertices, UINT size_vertex, UINT size_list, void* shader_byte_code, size_t size_byte_shader)
{
	m_vb = GraphicsEngine::get()->createVertexBuffer();

	// drawing  object
	m_vb->load(list_vertices, size_vertex, size_list, shader_byte_code, size_byte_shader);
}

void AGameobject::update()
{
	//SET THE VERTICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb);

	// FINALLY DRAW THE TRIANGLE
	GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleList(m_vb->getSizeVertexList(), 0);
}

bool AGameobject::release()
{
	m_vb->release();
	return true;
}
