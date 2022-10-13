#include "Mesh.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <locale>
#include <codecvt>

#include "ContantBufferTypes.h"
#include "GraphicsEngine.h"
#include "VertexMesh.h"
#include "AppWindow.h"

Mesh::Mesh(const wchar_t* full_path) : Resource(full_path)
{
	// Set the object type
	ObjectType = ObjectTypes::MESH;

	// data structure for the list of vertices, texture coordinates, and so on..
	tinyobj::attrib_t attribs;
	// group of meshes stored in the obj file
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	// warning exceptions for functions
	std::string warn;
	std::string err;

	// convert const wchar_t into simple string type
	std::string inputfile = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(full_path);

	// parse the object model into a data structure
	bool res = tinyobj::LoadObj(&attribs, &shapes, &materials, &warn, &err, inputfile.c_str());

	if (!err.empty())
		throw std::exception("Mesh not created successfully");

	if (!res)
		throw std::exception("Mesh not created successfully");

	if (shapes.size() > 1)
		throw std::exception("Mesh not created successfully");

	std::vector<VertexMesh> list_vertices;
	std::vector<unsigned int> list_indices;

	// get all the retrieved data and process them
	// iterate all of the shapes
	for (size_t s = 0; s < shapes.size(); s++)
	{
		size_t index_offset = 0;
		// reserve the size of our vector in order to speed up the insertion
		list_vertices.reserve(shapes[s].mesh.indices.size());
		list_indices.reserve(shapes[s].mesh.indices.size());

		// iterate all of the face
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
		{
			// for each phase we have to get the number of vertices that compose it
			unsigned char num_face_verts = shapes[s].mesh.num_face_vertices[f];

			// iterate all of those vertices
			for (unsigned char v = 0; v < num_face_verts; v++)
			{
				// indices of each shape
				tinyobj::index_t index = shapes[s].mesh.indices[index_offset + v];

				// vertex coordinates
				tinyobj::real_t vx = attribs.vertices[index.vertex_index * 3 + 0];
				tinyobj::real_t vy = attribs.vertices[index.vertex_index * 3 + 1];
				tinyobj::real_t vz = attribs.vertices[index.vertex_index * 3 + 2];

				// texture coordinates
				tinyobj::real_t tx = attribs.texcoords[index.texcoord_index * 2 + 0];
				tinyobj::real_t ty = attribs.texcoords[index.texcoord_index * 2 + 1];

				// passing the attributes to our vertex mesh; then push it to the vector
				VertexMesh vertex(Vector3D(vx, vy, vz), Vector2D(tx, ty));
				list_vertices.push_back(vertex);
				// passing the attributes to our index mesh; then push it to the vector
				list_indices.push_back(index_offset + v);
			}

			index_offset += num_face_verts;
		}
	}

	// gets the byte code and size of the vertex shader
	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::get()->getVertexMeshLayoutShaderByteCodeAndSize(&shader_byte_code, &size_shader);

	// create the index buffer
	m_ib = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(&list_indices[0],
		(UINT)list_indices.size());

	// create a vertex buffer
	m_vb = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(&list_vertices[0],
		sizeof(VertexMesh), (UINT)list_vertices.size(),
		shader_byte_code, (UINT)size_shader);

	// create CB
	constant_transform cc;
	cc.m_time = 0;
	m_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant_transform));
	// Texture update
	constant_texture cc_texture;
	m_cb_texture = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc_texture, sizeof(constant_texture));

}

Mesh::~Mesh()
{
}

void Mesh::Update(float deltaTime, AppWindow* app_window)
{
	// transform update
	constant_transform cc;

	// objects matrix
	Matrix4x4 temp;

	cc.m_world.setIdentity();

	// creates a translation animation
	temp.setIdentity();
	temp.setRotationX(m_rotation.m_x);
	temp.setIdentity();
	temp.setRotationY(m_rotation.m_y);
	temp.setIdentity();
	temp.setRotationZ(m_rotation.m_z);
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
	// setting the perspective projection
	cc.m_proj.setPerspectiveFovLH(1.57f, ((float)width / (float)height), 0.1f, 100.0f);

	m_cb->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);

	// Texture update
	constant_texture cc_texture;
	cc_texture.alpha = alpha;
	m_cb_texture->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc_texture);
}

void Mesh::Draw(const VertexShaderPtr& m_vs, const PixelShaderPtr& m_ps, const BlenderPtr& m_blender)
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

const VertexBufferPtr& Mesh::getVertexBuffer()
{
	return m_vb;
}

const IndexBufferPtr& Mesh::getIndexBuffer()
{
	return  m_ib;
}

void Mesh::SetTexture(const wchar_t* tex_path)
{
	// assign the texture file to the Texture pointer by passing the its path in the file
	m_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(tex_path);

}

void Mesh::SetAlpha(float alpha)
{
	this->alpha = alpha;
}

float Mesh::GetAlpha()
{
	return alpha;
}

