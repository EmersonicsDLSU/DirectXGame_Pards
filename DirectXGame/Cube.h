#pragma once
#include "AGameObject.h"

class Cube : public AGameObject
{
public:
	void Initialize() override;

private:
	VertexBufferPtr* m_vb;
	IndexBufferPtr* m_ib;
	ConstantBufferPtr* m_cb;
};

