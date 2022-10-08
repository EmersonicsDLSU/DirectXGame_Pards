#pragma once
#include "Prerequisites.h"


class AGameObject
{
public:
	AGameObject();
	virtual ~AGameObject();

public:
	virtual void Update(float deltaTime, void* buffer) = 0;
	virtual void Draw(const VertexShaderPtr& m_vs, const PixelShaderPtr& m_ps, const BlenderPtr& m_blender);
};

