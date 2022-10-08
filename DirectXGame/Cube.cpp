#include "Cube.h"

#include "ConstantBuffer.h"
#include "DeviceContext.h"
#include "GameObjectManager.h"
#include "GraphicsEngine.h"
#include "IndexBuffer.h"
#include "PrimitiveCreation.h"

Cube::Cube(void* shader_byte_code, size_t size_shader, const wchar_t* file_path, char sample) : sample(sample)
{
	// assign the texture file to the Texture pointer by passing the its path in the file
	m_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(file_path);
	// create VB
	UINT size_index_list = ARRAYSIZE(PrimitiveCreation::get()->cube_index_list);
	m_ib = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer
	(PrimitiveCreation::get()->cube_index_list, size_index_list);

	// create VB
	UINT size_list = ARRAYSIZE(PrimitiveCreation::get()->cube_vertex_list);
	m_vb = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(
		PrimitiveCreation::get()->cube_vertex_list, 
		sizeof(vertex), size_list, 
		shader_byte_code, size_shader);

	// create CB
	constant cc;
	cc.m_time = 0;
	m_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));

	// add this object to our manager
	AGameObject* tempo = this;
	AGameObjectPtr temp(tempo);
	GameObjectManager::get()->objectList.push_back(temp);
}

Cube::~Cube()
{

}

void Cube::Update(float deltaTime, void* buffer)
{
	m_cb->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), buffer);
}

void Cube::Draw(VertexShaderPtr m_vs, PixelShaderPtr m_ps, BlenderPtr m_blender)
{
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(m_vs, m_cb);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb);

	//SET DEFAULT SHADER IN THE GRAPHICS PIPELINE TO BE ABLE TO DRAW
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(m_ps);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setTexture(m_ps, m_tex);

	//SET THE VERTICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(m_vb);
	//SET THE INDICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(m_ib);
	//SET THE BLENDING
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setBlender(m_blender);
	// FINALLY DRAW THE TRIANGLE
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(m_ib->getSizeIndexList(), 0, 0);
}

