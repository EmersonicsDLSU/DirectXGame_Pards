#include "PrimitiveCreation.h"

#include "AppWindow.h"

PrimitiveCreation* PrimitiveCreation::m_instance = nullptr;

PrimitiveCreation::PrimitiveCreation()
{

}

PrimitiveCreation::~PrimitiveCreation()
{
}

void* PrimitiveCreation::cube_vertex_list()
{
	return nullptr;
}

void* PrimitiveCreation::cube_index_list()
{
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

	return &index_list;
}

PrimitiveCreation* PrimitiveCreation::get()
{
	return m_instance;
}

void PrimitiveCreation::create()
{
	if (PrimitiveCreation::m_instance)
		throw std::exception("PrimitiveCreation already created");
	PrimitiveCreation::m_instance = new PrimitiveCreation();
}

void PrimitiveCreation::release()
{
	if (!PrimitiveCreation::m_instance)
		return;
	delete PrimitiveCreation::m_instance;
}
