
#pragma once
#include <d3d11.h>

class DeviceContext;

/*
 * Because indices need to be accessed by the GPU, they
 * need to be placed in a special resource structure: an
 * index buffer. Creating an index buffer is very similar
 * to creating a vertex buffer, except that the index buffer
 * stores indices instead of vertices.*/
class IndexBuffer
{
public:
	IndexBuffer();
	bool load(void* list_indices, UINT size_list);
	UINT getSizeIndexList();
	bool release();
	~IndexBuffer();
private:
	UINT m_size_list;
private:
	ID3D11Buffer* m_buffer;
private:
	friend class DeviceContext;
};