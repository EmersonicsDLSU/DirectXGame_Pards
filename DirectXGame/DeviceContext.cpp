#include "DeviceContext.h"
#include "SwapChain.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Texture.h"
#include <exception>

#include "Blender.h"

DeviceContext::DeviceContext(ID3D11DeviceContext* device_context, RenderSystem* system) : m_system(system), m_device_context(device_context)
{
}

void DeviceContext::clearRenderTargetColor(const SwapChainPtr& swap_chain, float red, float green, float blue, float alpha)
{
	FLOAT clear_color[] = { red,green,blue,alpha };
	// sets all the elements in a render target to one value which clears the render target view.
	m_device_context->ClearRenderTargetView(swap_chain->m_rtv, clear_color);
	// sets all the elements in a depth-stencil target to one value which clears the depth-stencil view.
	m_device_context->ClearDepthStencilView(swap_chain->m_dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	// binds one or more render targets and the depth-stencil buffer to the output-merger stage.
	m_device_context->OMSetRenderTargets(1, &swap_chain->m_rtv, swap_chain->m_dsv);
}

void DeviceContext::setVertexBuffer(const VertexBufferPtr& vertex_buffer)
{
	// the size of our vertex type
	UINT stride = vertex_buffer->m_size_vertex;
	// starting point of our buffer
	UINT offset = 0;
	// Bind an array of vertex buffers to the input-assembler stage.
	m_device_context->IASetVertexBuffers(0, 1, &vertex_buffer->m_buffer, &stride, &offset);
	m_device_context->IASetInputLayout(vertex_buffer->m_layout);
}

void DeviceContext::setIndexBuffer(const IndexBufferPtr& index_buffer)
{
	m_device_context->IASetIndexBuffer(index_buffer->m_buffer, DXGI_FORMAT_R32_UINT, 0);
}

void DeviceContext::setBlender(const BlenderPtr& blender)
{
	float blendFactors[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	// binds a blend state object to the output merger stage of the pipeline
	m_device_context->OMSetBlendState(blender->pp_blend_state_, blendFactors, 0xFFFFFFFF);
}

// draw the list of triangles on the screen
void DeviceContext::drawTriangleList(UINT vertex_count, UINT start_vertex_index)
{
	// our data will contain a list of primitive triangles
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// Draw non-indexed, non-instanced primitives.
	m_device_context->Draw(vertex_count, start_vertex_index);
}

// draw the triangles' indices on the screen
void DeviceContext::drawIndexedTriangleList(UINT index_count, UINT start_vertex_index, UINT start_index_location)
{
	// our data will contain a list of primitive triangles
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// Draw non-indexed, non-instanced primitives.
	m_device_context->DrawIndexed(index_count, start_index_location, start_vertex_index);
}

// draw the strip of triangles on the screen
void DeviceContext::drawTriangleStrip(UINT vertex_count, UINT start_vertex_index)
{
	// our data will contain a strip of primitive triangles
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	// Draw non-indexed, non-instanced primitives.
	m_device_context->Draw(vertex_count, start_vertex_index);
}

// sets on which area of the view target we want to draw
void DeviceContext::setViewportSize(UINT width, UINT height)
{
	// viewport structure of the render target view port
	D3D11_VIEWPORT vp = {};
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	m_device_context->RSSetViewports(1, &vp);
}

void DeviceContext::setVertexShader(const VertexShaderPtr& vertex_shader)
{
	// Set a vertex shader to the device.
	m_device_context->VSSetShader(vertex_shader->m_vs, nullptr, 0);
}

void DeviceContext::setPixelShader(const PixelShaderPtr& pixel_shader)
{
	// Set a pixel shader to the device.
	m_device_context->PSSetShader(pixel_shader->m_ps, nullptr, 0);
}

// binds the texture to the graphics pipeline in the VertexShader
void DeviceContext::setTexture(const VertexShaderPtr& vertex_shader, const TexturePtr& texture)
{
	m_device_context->VSSetShaderResources(0, 1, &texture->m_shader_res_view);
}

// binds the texture to the graphics pipeline in the PixelShader
void DeviceContext::setTexture(const PixelShaderPtr& pixel_shader, const TexturePtr& texture)
{
	m_device_context->PSSetShaderResources(0, 1, &texture->m_shader_res_view);
}

void DeviceContext::setConstantBuffer(const VertexShaderPtr& vertex_shader, const ConstantBufferPtr& buffer)
{
	m_device_context->VSSetConstantBuffers(0, 1, &buffer->m_buffer);
}

void DeviceContext::setConstantBuffer(const PixelShaderPtr& pixel_shader, const ConstantBufferPtr& buffer)
{
	m_device_context->PSSetConstantBuffers(0, 1, &buffer->m_buffer);
}

DeviceContext::~DeviceContext()
{
	m_device_context->Release();
}
