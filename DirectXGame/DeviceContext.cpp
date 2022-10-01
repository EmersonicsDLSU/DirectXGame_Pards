#include "DeviceContext.h"
#include "SwapChain.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include <exception>

DeviceContext::DeviceContext(ID3D11DeviceContext* device_context, RenderSystem* system) : m_system(system), m_device_context(device_context)
{
}

void DeviceContext::clearRenderTargetColor(SwapChainPtr swap_chain, float red, float green, float blue, float alpha)
{
	FLOAT clear_color[] = { red,green,blue,alpha };
	// sets all the elements in a render target to one value which clears the render target view.
	m_device_context->ClearRenderTargetView(swap_chain->m_rtv, clear_color);
	// binds one or more render targets and the depth-stencil buffer to the output-merger stage.
	m_device_context->OMSetRenderTargets(1, &swap_chain->m_rtv, NULL);
}

void DeviceContext::setVertexBuffer(VertexBufferPtr vertex_buffer)
{
	// the size of our vertex type
	UINT stride = vertex_buffer->m_size_vertex;
	// starting point of our buffer
	UINT offset = 0;
	// Bind an array of vertex buffers to the input-assembler stage.
	m_device_context->IASetVertexBuffers(0, 1, &vertex_buffer->m_buffer, &stride, &offset);
	m_device_context->IASetInputLayout(vertex_buffer->m_layout);
}

void DeviceContext::setIndexBuffer(IndexBufferPtr index_buffer)
{
	m_device_context->IASetIndexBuffer(index_buffer->m_buffer, DXGI_FORMAT_R32_UINT, 0);
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

void DeviceContext::setVertexShader(VertexShaderPtr vertex_shader)
{
	// Set a vertex shader to the device.
	m_device_context->VSSetShader(vertex_shader->m_vs, nullptr, 0);
}

void DeviceContext::setPixelShader(PixelShaderPtr pixel_shader)
{
	// Set a pixel shader to the device.
	m_device_context->PSSetShader(pixel_shader->m_ps, nullptr, 0);
}

void DeviceContext::setConstantBuffer(VertexShaderPtr vertex_shader, ConstantBufferPtr buffer)
{
	m_device_context->VSSetConstantBuffers(0, 1, &buffer->m_buffer);
}

void DeviceContext::setConstantBuffer(PixelShaderPtr pixel_shader, ConstantBufferPtr buffer)
{
	m_device_context->PSSetConstantBuffers(0, 1, &buffer->m_buffer);
}

DeviceContext::~DeviceContext()
{
	m_device_context->Release();
}
