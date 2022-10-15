#include "Cube.h"

#include <iostream>

#include "AppWindow.h"
#include "ConstantBuffer.h"
#include "ContantBufferTypes.h"
#include "DeviceContext.h"
#include "EngineTime.h"
#include "GraphicsEngine.h"
#include "IndexBuffer.h"
#include "Mesh.h"
#include "PrimitiveCreation.h"

Cube::Cube(std::string name, ObjectTypes type) : AGameObject(name, type)
{
	// Set the object type
	ObjectType = ObjectTypes::CUBE;

	// list of all the vertex in the 3D Cube
	vertex vertex_list[] =
	{
		//X - Y - Z
		//FRONT FACE
		{Vector3D(-0.5f,-0.5f,-0.5f),    Vector3D(1,0,0),  Vector3D(0.2f,0,0) },
		{Vector3D(-0.5f,0.5f,-0.5f),    Vector3D(1,1,0), Vector3D(0.2f,0.2f,0) },
		{ Vector3D(0.5f,0.5f,-0.5f),   Vector3D(1,1,0),  Vector3D(0.2f,0.2f,0) },
		{ Vector3D(0.5f,-0.5f,-0.5f),     Vector3D(1,0,0), Vector3D(0.2f,0,0) },

		//BACK FACE
		{ Vector3D(0.5f,-0.5f,0.5f),    Vector3D(0,1,0), Vector3D(0,0.2f,0) },
		{ Vector3D(0.5f,0.5f,0.5f),    Vector3D(0,1,1), Vector3D(0,0.2f,0.2f) },
		{ Vector3D(-0.5f,0.5f,0.5f),   Vector3D(0,1,1),  Vector3D(0,0.2f,0.2f) },
		{ Vector3D(-0.5f,-0.5f,0.5f),     Vector3D(0,1,0), Vector3D(0,0.2f,0) }

	};
	
	UINT size_list = ARRAYSIZE(vertex_list);

	// list of all the triangle index with their vertex compositions
	// this index list should reflect the vertex list
	unsigned int index_list[] =
	{
		//FRONT SIDE
		0,1,2,  //FIRST TRIANGLE
		2,3,0,  //SECOND TRIANGLE
		//BACK SIDE
		4,5,6,
		6,7,4,
		//TOP SIDE
		1,6,5,
		5,2,1,
		//BOTTOM SIDE
		7,0,3,
		3,4,7,
		//RIGHT SIDE
		3,2,5,
		5,4,3,
		//LEFT SIDE
		7,6,1,
		1,0,7
	};

	UINT size_index_list = ARRAYSIZE(index_list);
	// create IB
	m_ib = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer
	(index_list, size_index_list);

	// gets the byte code and size of the vertex shader
	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	// access the VertexShader.hlsl and compile
	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	// create VB
	m_vb = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(
		vertex_list,
		sizeof(vertex), size_list,
		shader_byte_code, size_shader);

	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();
	// create CB
	constant_transform cc;
	cc.m_time = 0;
	m_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant_transform));
	// Texture update
	constant_texture cc_texture;
	cc_texture.object_type = ObjectType;
	m_cb_texture = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc_texture, sizeof(constant_texture));
	
}

Cube::~Cube()
{

}

void Cube::Update(float deltaTime, AppWindow* app_window)
{
	// transform update
	constant_transform cc;

	deltaScale += EngineTime::getDeltaTime() * speed;

	// objects matrix
	Matrix4x4 temp;

	cc.m_world.setIdentity();

	// creates a translation animation
	temp.setIdentity();
	temp.setRotationX(deltaScale);
	cc.m_world *= temp;
	temp.setIdentity();
	temp.setRotationY(deltaScale);
	cc.m_world *= temp;
	temp.setIdentity();
	temp.setRotationZ(m_rotation.m_z);
	cc.m_world *= temp;
	temp.setScale(m_scale);
	temp.setTranslation(m_position);
	// Transformation of matrices; Note that order is important
	cc.m_world *= temp;

	// creating the camera matrix
	Matrix4x4 world_cam;
	world_cam.setIdentity();
	// set the transform rotation X of the object
	temp.setIdentity();
	temp.setRotationX(app_window->m_rot_x);
	// make the object relative to the camera
	world_cam *= temp;
	// set the transform rotation Y of the object
	temp.setIdentity();
	temp.setRotationY(app_window->m_rot_y);
	// make the object relative to the camera
	world_cam *= temp;

	// moving or setting the camera position in the z or x axis
	Vector3D new_pos = app_window->m_world_cam.getTranslation() + world_cam.getZDirection() * (app_window->m_forward * 0.3f);
	new_pos = new_pos + world_cam.getXDirection() * (app_window->m_rightward * 0.3f);
	world_cam.setTranslation(new_pos);
	// save the newly transformed world_cam to the world_cam from the constant buffer
	app_window->m_world_cam = world_cam;
	// convert camera matrix to view matrix
	world_cam.inverse();
	// change the view matrix from our constant buffer to the world/camera matrix
	cc.m_view = world_cam;

	// width and height of the screen
	int width = (app_window->getClientWindowRect().right - app_window->getClientWindowRect().left) / 300.0f;
	int height = (app_window->getClientWindowRect().bottom - app_window->getClientWindowRect().left) / 300.0f;

	// setting for the orthographic projection
	cc.m_proj.setOrthoLH
	(
		width,
		height,
		-40.0f,
		40.0f
	);
	// setting the perspective projection
	//cc.m_proj.setPerspectiveFovLH(1.57f, ((float)width / (float)height), 0.1f, 100.0f);

	m_cb->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);

	// Texture update
	constant_texture cc_texture;
	cc_texture.alpha = alpha;
	cc_texture.object_type = ObjectType;
	m_cb_texture->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc_texture);
}

void Cube::Draw(const VertexShaderPtr& m_vs, const PixelShaderPtr& m_ps, const BlenderPtr& m_blender)
{
	// for the transform
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(m_vs, m_cb);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb);

	// for the texture
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb_texture);

	//SET DEFAULT SHADER IN THE GRAPHICS PIPELINE TO BE ABLE TO DRAW
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(m_ps);

	//GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setTexture(m_ps, m_tex);

	//SET THE VERTICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(m_vb);
	//SET THE INDICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(m_ib);
	// FINALLY DRAW THE TRIANGLE
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(
		m_ib->getSizeIndexList(), 0, 0);
	//SET THE BLENDING
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setBlender(m_blender);
}

void Cube::SetMesh(const wchar_t* tex_path)
{
	m_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(tex_path);
	m_ib = m_mesh.get()->getIndexBuffer();
	m_vb = m_mesh.get()->getVertexBuffer();
	// Texture update
	constant_texture cc_texture;
	ObjectType = ObjectTypes::MESH;
	cc_texture.object_type = ObjectType;
	m_cb_texture = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc_texture, sizeof(constant_texture));
}

void Cube::SetTexture(const wchar_t* tex_path)
{
	// assign the texture file to the Texture pointer by passing the its path in the file
	//m_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(tex_path);
}

void Cube::SetAlpha(float alpha)
{
	this->alpha = alpha;
}

float Cube::GetAlpha()
{
	return alpha;
}

void Cube::SetSpeed(float f)
{
	speed = f;
}


