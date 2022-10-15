#pragma once
#include "Prerequisites.h"
#include "Vector2D.h"
#include "Vector3D.h"

struct vertex
{
	Vector3D position;
	Vector3D color;
	Vector3D color1;
};

class PrimitiveCreation
{
public:
	PrimitiveCreation();
	~PrimitiveCreation();

private:
	void* cube_vertex_list();
	void* cube_index_list();

public:
	static PrimitiveCreation* get();
	static void create();
	static void release();

private:
	static PrimitiveCreation* m_instance;
	friend class Cube;
};

