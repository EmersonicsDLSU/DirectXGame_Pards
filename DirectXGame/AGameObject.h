#pragma once
#include "Prerequisites.h"


class AppWindow;

class AGameObject
{
public:
	AGameObject();
	virtual ~AGameObject();

public:
	virtual void Update(float deltaTime, AppWindow* app_window) = 0;
	virtual void Draw(const VertexShaderPtr& m_vs, const PixelShaderPtr& m_ps, const BlenderPtr& m_blender);
};

