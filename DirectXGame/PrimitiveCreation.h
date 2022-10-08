#pragma once
#include "Prerequisites.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "Matrix4x4.h"

struct vertex
{
	Vector3D position;
	Vector2D texcoord;
};

// constant buffer; this is updated per frame
__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	unsigned int m_time;
};

class PrimitiveCreation
{
public:
	PrimitiveCreation();
	~PrimitiveCreation();

private:
	void CreateCubeDefault();
	vertex cube_vertex_list[24];
	unsigned int cube_index_list[36];

public:
	static PrimitiveCreation* get();
	static void create();
	static void release();

private:
	static PrimitiveCreation* m_instance;
	friend class Cube;
};

