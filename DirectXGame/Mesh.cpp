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
	
}

Mesh::~Mesh()
{
}

const VertexBufferPtr& Mesh::getVertexBuffer()
{
	return m_vb;
}

const IndexBufferPtr& Mesh::getIndexBuffer()
{
	return  m_ib;
}
