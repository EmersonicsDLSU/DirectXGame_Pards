#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class Blender
{
public:
	Blender(RenderSystem* system);
	~Blender();
private:
	ID3D11BlendState* pp_blend_state_;
	RenderSystem* m_system = nullptr;
private:
	friend class DeviceContext;
};

