#pragma once
#include "VertexBuffer.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include "ObjectCreation.h"

class ObjectCreation;

class AGameobject
{
public:
	AGameobject();
	~AGameobject();
	void load(void* list_vertices, UINT size_vertex, UINT size_list, void* shader_byte_code, size_t size_byte_shader);
	void update();
	bool release();
private:
	VertexBuffer* m_vb;
	friend class ObjectCreation;
};

