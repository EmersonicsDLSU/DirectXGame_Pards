#include "Plane.h"

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
#include "Camera.h"

Plane::Plane(std::string name, ObjectTypes type) : AGameObject(name, type)
{
	// Set the object type
	ObjectType = ObjectTypes::PLANE;

	// Position Coords
	Vector3D position_list[] =
	{
		{ Vector3D(-0.5f,-0.5f,-0.5f)},
		{ Vector3D(-0.5f,0.5f,-0.5f) },
		{ Vector3D(0.5f,0.5f,-0.5f) },
		{ Vector3D(0.5f,-0.5f,-0.5f)},

		//BACK FACE
		{ Vector3D(0.5f,-0.5f,0.5f) },
		{ Vector3D(0.5f,0.5f,0.5f) },
		{ Vector3D(-0.5f,0.5f,0.5f)},
		{ Vector3D(-0.5f,-0.5f,0.5f) }
	};

	// UV Coords
	Vector2D texcoord_list[] =
	{
		{ Vector2D(0.0f,0.0f) },
		{ Vector2D(0.0f,1.0f) },
		{ Vector2D(1.0f,0.0f) },
		{ Vector2D(1.0f,1.0f) }
	};

	// list of all the vertex in the 3D Cube
	vertex vertex_list[] =
	{
		//X - Y - Z
		//FRONT FACE
		{ position_list[0],texcoord_list[1] },
		{ position_list[1],texcoord_list[0] },
		{ position_list[2],texcoord_list[2] },
		{ position_list[3],texcoord_list[3] },


		{ position_list[4],texcoord_list[1] },
		{ position_list[5],texcoord_list[0] },
		{ position_list[6],texcoord_list[2] },
		{ position_list[7],texcoord_list[3] },


		{ position_list[1],texcoord_list[1] },
		{ position_list[6],texcoord_list[0] },
		{ position_list[5],texcoord_list[2] },
		{ position_list[2],texcoord_list[3] },

		{ position_list[7],texcoord_list[1] },
		{ position_list[0],texcoord_list[0] },
		{ position_list[3],texcoord_list[2] },
		{ position_list[4],texcoord_list[3] },

		{ position_list[3],texcoord_list[1] },
		{ position_list[2],texcoord_list[0] },
		{ position_list[5],texcoord_list[2] },
		{ position_list[4],texcoord_list[3] },

		{ position_list[7],texcoord_list[1] },
		{ position_list[6],texcoord_list[0] },
		{ position_list[1],texcoord_list[2] },
		{ position_list[0],texcoord_list[3] }


	};

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
		8,9,10,
		10,11,8,
		//BOTTOM SIDE
		12,13,14,
		14,15,12,
		//RIGHT SIDE
		16,17,18,
		18,19,16,
		//LEFT SIDE
		20,21,22,
		22,23,20
	};

	UINT size_list = ARRAYSIZE(vertex_list);

	UINT size_index_list = ARRAYSIZE(index_list);

	// create IB
	m_ib = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer
	(index_list, size_index_list);

	// gets the byte code and size of the vertex shader
	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::get()->getPixelShaderByteCodeAndSize(&shader_byte_code, &size_shader);

	// create VB
	m_vb = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(
		vertex_list,
		sizeof(vertex), size_list,
		shader_byte_code, size_shader);

	// create CB
	constant_transform cc;
	cc.m_time = 0;
	m_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant_transform));
	// Texture update
	constant_texture cc_texture;
	cc_texture.object_type = ObjectType;
	m_cb_texture = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc_texture, sizeof(constant_texture));

	SetScale(5.0f, 0.1f, 5.0f);
}

Plane::~Plane()
{

}

void Plane::Update(float deltaTime, AppWindow* app_window)
{
	// transform update
	constant_transform cc;

	Matrix4x4 allMatrix; allMatrix.setIdentity();
	Matrix4x4 translationMatrix; translationMatrix.setIdentity();  translationMatrix.setTranslation(GetLocalPosition());
	Matrix4x4 scaleMatrix; scaleMatrix.setScale(GetLocalScale());
	Vector3D rotation = GetLocalRotation();
	Matrix4x4 zMatrix; zMatrix.setRotationZ(rotation.m_z);
	Matrix4x4 xMatrix; xMatrix.setRotationX(rotation.m_x);
	Matrix4x4 yMatrix; yMatrix.setRotationY(rotation.m_y);

	//Scale --> Rotate --> Transform as recommended order.
	Matrix4x4 rotMatrix; rotMatrix.setIdentity();
	rotMatrix = rotMatrix.MultiplyTo(zMatrix.MultiplyTo(yMatrix.MultiplyTo(xMatrix)));
	allMatrix = allMatrix.MultiplyTo(scaleMatrix.MultiplyTo(rotMatrix));
	allMatrix = allMatrix.MultiplyTo(translationMatrix);
	cc.m_world = allMatrix;
	
	Matrix4x4 cameraMatrix = dynamic_cast<Camera*>(&*app_window->m_camera)->GetCamViewMatrix();
	cc.m_view = cameraMatrix;

	// width and height of the screen
	int width = (app_window->getClientWindowRect().right - app_window->getClientWindowRect().left) / 300.0f;
	int height = (app_window->getClientWindowRect().bottom - app_window->getClientWindowRect().left) / 300.0f;

#define VIEW 1
#if VIEW == 0
	// setting for the orthographic projection
	cc.m_proj.setOrthoLH
	(
		width,
		height,
		-40.0f,
		40.0f
	);
#elif VIEW == 1
	// setting the perspective projection
	float aspectRatio = (float)width / (float)height;
	cc.m_proj.setPerspectiveFovLH(aspectRatio, aspectRatio, 0.1f, 100.0f);
#endif

	m_cb->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);

	// Texture update
	constant_texture cc_texture;
	cc_texture.alpha = alpha;
	cc_texture.object_type = ObjectType;
	m_cb_texture->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc_texture);
}

void Plane::Draw(const VertexShaderPtr& m_vs, const PixelShaderPtr& m_ps, const BlenderPtr& m_blender)
{
	// for the transform
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(m_vs, m_cb);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb);

	// for the texture
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb_texture);

	//SET DEFAULT SHADER IN THE GRAPHICS PIPELINE TO BE ABLE TO DRAW
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(m_ps);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setTexture(m_ps, m_tex);

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

void Plane::SetMesh(const wchar_t* tex_path)
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

void Plane::SetTexture(const wchar_t* tex_path)
{
	// assign the texture file to the Texture pointer by passing the its path in the file
	m_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(tex_path);
}

void Plane::SetAlpha(float alpha)
{
	this->alpha = alpha;
}

float Plane::GetAlpha()
{
	return alpha;
}


