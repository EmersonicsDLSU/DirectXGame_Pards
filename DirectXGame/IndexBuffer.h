
#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

/*
 * Because indices need to be accessed by the GPU, they
 * need to be placed in a special resource structure: an
 * index buffer. Creating an index buffer is very similar
 * to creating a vertex buffer, except that the index buffer
 * stores indices instead of vertices.*/
class IndexBuffer
{
public:
	IndexBuffer(void* list_indices, UINT size_list, RenderSystem* system);
	UINT getSizeIndexList();
	~IndexBuffer();
private:
	UINT m_size_list;
private:
	ID3D11Buffer* m_buffer;
	RenderSystem* m_system = nullptr;
private:
	friend class DeviceContext;
};