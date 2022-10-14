#pragma once
#include "AGameObject.h"
#include "Resource.h"
#include "Prerequisites.h"

class AppWindow;

class Mesh : public Resource
{
public:
	Mesh(const wchar_t* full_path);
	~Mesh();

	const VertexBufferPtr& getVertexBuffer();
	const IndexBufferPtr& getIndexBuffer();
private:
	VertexBufferPtr m_vb;
	IndexBufferPtr m_ib;
private:
	friend class DeviceContext;
};
