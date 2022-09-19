#pragma once
#include <d3d11.h>


class GraphicsEngine;
class DeviceContext;

/*
 * A pixel shader is a program written in HLSL, executed on the GPU,
 * which inputs interpolated vertex data and outputs a color value.
 * As a hardware optimization, it is possible that a pixel fragment
 * is rejected by the pipeline before making it to the pixel shader
 * (e.g., early-z rejection). Pixel shaders enable the programmer to
 * do specialized work on a per pixel basis to achieve various
 * rendering effects. The values output from the pixelshader are
 * passed on to the next stage in the pipeline.
 */
class PixelShader
{
public:
	PixelShader();
	void release();
	~PixelShader();
private:
	bool init(const void* shader_byte_code, size_t byte_code_size);
private:
	ID3D11PixelShader* m_ps;
private:
	friend class GraphicsEngine;
	friend class DeviceContext;
};