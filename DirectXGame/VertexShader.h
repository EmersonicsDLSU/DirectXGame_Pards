#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

/*
 *A vertex shader is a program written in HLSL, executed on the GPU, which
 *inputs a vertex and outputs a vertex. Every drawn vertex goes through the
 *vertex shader. This enables the programmer to do specialized work on a per
 *vertex basis to achieve various rendering effects. The values output from
 *the vertex shader are passed on to the next stage in the pipeline.
 */
class VertexShader
{
public:
	VertexShader(const void* shader_byte_code, size_t byte_code_size, RenderSystem* system);
	~VertexShader();
private:
	ID3D11VertexShader* m_vs;
	RenderSystem* m_system = nullptr;
private:
	friend class RenderSystem;
	friend class DeviceContext;
};