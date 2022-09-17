#include "DeviceContext.h"
#include "SwapChain.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"

DeviceContext::DeviceContext(ID3D11DeviceContext* device_context) :m_device_context(device_context)
{
}

void DeviceContext::clearRenderTargetColor(SwapChain* swap_chain, float red, float green, float blue, float alpha)
{
	FLOAT clear_color[] = { red,green,blue,alpha };
	// sets all the elements in a render target to one value which clears the render target view.
	m_device_context->ClearRenderTargetView(swap_chain->m_rtv, clear_color);
	// binds one or more render targets and the depth-stencil buffer to the output-merger stage.
	m_device_context->OMSetRenderTargets(1, &swap_chain->m_rtv, NULL);
}

void DeviceContext::setVertexBuffer(VertexBuffer* vertex_buffer)
{
	// the size of our vertex type
	UINT stride = vertex_buffer->m_size_vertex;
	// starting point of our buffer
	UINT offset = 0;
	// Bind an array of vertex buffers to the input-assembler stage.
	m_device_context->IASetVertexBuffers(0, 1, &vertex_buffer->m_buffer, &stride, &offset);
	m_device_context->IASetInputLayout(vertex_buffer->m_layout);
}

// draw the list of triangles on the screen
void DeviceContext::drawTriangleList(UINT vertex_count, UINT start_vertex_index)
{
	// our data will contain a list of primitive triangles
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// Draw non-indexed, non-instanced primitives.
	m_device_context->Draw(vertex_count, start_vertex_index);
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

void DeviceContext::setVertexShader(VertexShader* vertex_shader)
{
	// Set a vertex shader to the device.
	m_device_context->VSSetShader(vertex_shader->m_vs, nullptr, 0);
}

void DeviceContext::setPixelShader(PixelShader* pixel_shader)
{
	m_device_context->PSSetShader(pixel_shader->m_ps, nullptr, 0);
}

void DeviceContext::setConstantBuffer(VertexShader* vertex_shader, ConstantBuffer* buffer)
{
	m_device_context->VSSetConstantBuffers(0, 1, &buffer->m_buffer);
}

void DeviceContext::setConstantBuffer(PixelShader* pixel_shader, ConstantBuffer* buffer)
{
	m_device_context->PSSetConstantBuffers(0, 1, &buffer->m_buffer);
}


bool DeviceContext::release()
{
	m_device_context->Release();
	delete this;
	return true;
}

DeviceContext::~DeviceContext()
{
}