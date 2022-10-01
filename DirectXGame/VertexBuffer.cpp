#include "VertexBuffer.h"
#include "RenderSystem.h"
#include <exception>


VertexBuffer::VertexBuffer(void* list_vertices, UINT size_vertex, UINT size_list, 
	void* shader_byte_code, size_t size_byte_shader, 
	RenderSystem* system) : m_system(system), m_layout(0), m_buffer(0)
{
	// describe a buffer resource
	D3D11_BUFFER_DESC buff_desc = {};
	// checks if our buffer is accessible by CPU and GPU; default means it could be written by both CPU and GPU
	buff_desc.Usage = D3D11_USAGE_DEFAULT;
	buff_desc.ByteWidth = size_vertex * size_list;
	// we want our buffer to be bind as a vertex buffer
	buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buff_desc.CPUAccessFlags = 0;
	buff_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA init_data = {};
	// passing the list of references of the vertices to the 'pSysMemory'
	init_data.pSysMem = list_vertices;

	m_size_vertex = size_vertex;
	m_size_list = size_list;

	if (FAILED(m_system->m_d3d_device->CreateBuffer(&buff_desc, &init_data, &m_buffer)))
	{
		throw std::exception("VertexBuffer not created successfully");
	}

	// defines the attribute of our vertex buffer type
	// contains all the information about the attributes that will compose our vertex type
	// the layout for our vertex list[] in AppWindow
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		//SEMANTIC NAME - SEMANTIC INDEX - FORMAT - INPUT SLOT - ALIGNED BYTE OFFSET - INPUT SLOT CLASS - INSTANCE DATA STEP RATE
		{"POSITION", 0,  DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,D3D11_INPUT_PER_VERTEX_DATA ,0},
		{ "COLOR", 0,  DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,D3D11_INPUT_PER_VERTEX_DATA ,0 },
		{ "COLOR", 1,  DXGI_FORMAT_R32G32B32_FLOAT, 0, 24,D3D11_INPUT_PER_VERTEX_DATA ,0 }
	};

	UINT size_layout = ARRAYSIZE(layout);

	if (FAILED(m_system->m_d3d_device->CreateInputLayout(layout, size_layout, shader_byte_code, size_byte_shader, &m_layout)))
	{
		throw std::exception("InputLayout not created successfully");
	}

}

UINT VertexBuffer::getSizeVertexList()
{
	return this->m_size_list;
}

VertexBuffer::~VertexBuffer()
{
	m_layout->Release();
	m_buffer->Release();
}
