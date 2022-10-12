#include "Blender.h"
#include <exception>
#include <iostream>

#include "RenderSystem.h"

Blender::Blender(RenderSystem* system) : m_system(system)
{
	D3D11_RENDER_TARGET_BLEND_DESC rt_blend_desc;
	ZeroMemory(&rt_blend_desc, sizeof(rt_blend_desc));
	
	rt_blend_desc.BlendEnable = 1;
	rt_blend_desc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	rt_blend_desc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	rt_blend_desc.BlendOp = D3D11_BLEND_OP_ADD;
	rt_blend_desc.SrcBlendAlpha = D3D11_BLEND_ZERO;
	rt_blend_desc.DestBlendAlpha = D3D11_BLEND_ONE;
	rt_blend_desc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rt_blend_desc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	D3D11_BLEND_DESC blend_desc = {};
	blend_desc.AlphaToCoverageEnable = 0;
	blend_desc.IndependentBlendEnable = 0;
	blend_desc.RenderTarget[0] = rt_blend_desc;

	if (FAILED(system->m_d3d_device->CreateBlendState(&blend_desc, &pp_blend_state_)))
	{
			throw std::exception("Blender not created successfully");
	}

	std::cout << "Blender iw Initialize" << std::endl;
}

Blender::~Blender()
{
	pp_blend_state_->Release();
	std::cout << "Blender is Release" << std::endl;
}
