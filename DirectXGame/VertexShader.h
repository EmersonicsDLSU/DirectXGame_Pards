#pragma once
#include <d3d11.h>


class GraphicsEngine;
class DeviceContext;

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
	VertexShader();
	void release();
	~VertexShader();
private:
	bool init(const void* shader_byte_code, size_t byte_code_size);
private:
	ID3D11VertexShader* m_vs;
private:
	friend class GraphicsEngine;
	friend class DeviceContext;
};