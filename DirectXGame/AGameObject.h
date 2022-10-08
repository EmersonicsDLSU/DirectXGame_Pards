#pragma once
#include "Prerequisites.h"


class AGameObject
{
public:
	AGameObject();
	virtual ~AGameObject();

public:
	virtual void Update(float deltaTime, void* buffer) = 0;
	virtual void Draw(VertexShaderPtr m_vs, PixelShaderPtr m_ps, BlenderPtr m_blender);
};

