#include "ObjectCreation.h"
#include "ObjectManager.h"


ObjectCreation::ObjectCreation()
{
}

ObjectCreation::~ObjectCreation()
{
}

bool ObjectCreation::release()
{
	return true;
}

ObjectCreation* ObjectCreation::get()
{
	static ObjectCreation creation;
	return &creation;
}

void ObjectCreation::InitializeVertices(void* shader_byte_code, size_t size_shader)
{
	list shape =
	{
		//X - Y - Z
		{-0.3f,-0.3f,0.0f,   0,1,0}, // POS1
		{-0.3f,0.3f,0.0f,    0,1,0}, // POS2
		{ 0.3f,0.3f,0.0f,    0,1,0}, // POS3

		{ 0.3f,0.3f,0.0f,   0,1,0}, // POS1
		{ 0.3f,-0.3f,0.0f,   0,1,0}, // POS2
		{ -0.3f,-0.3f,0.0f,   0,1,0}, // POS3
	};
	// initialize a new AGameobject
	AGameobject* object = new AGameobject();
	ObjectManager::get()->objectList.push_back(object);
	// gets the size of our array
	UINT size_list = ARRAYSIZE(shape);
	// load the recently pass gameobject
	ObjectManager::get()->objectList[ObjectManager::get()->objectList.size() - 1]->load(
		shape, sizeof(vertex), size_list, shader_byte_code, size_shader);

	list shape1 =
	{
		//X - Y - Z
		{-0.2f - 0.6f,-0.2f - 0.6f,0.0f,   1,0,0}, // POS1
		{-0.2f - 0.6f,0.2f - 0.6f,0.0f,    1,0,0}, // POS2
		{ 0.2f - 0.6f,0.2f - 0.6f,0.0f,    1,0,0}, // POS3

		{ 0.2f - 0.6f,0.2f - 0.6f,0.0f,   1,0,0}, // POS1
		{ 0.2f - 0.6f,-0.2f - 0.6f,0.0f,   1,0,0}, // POS2
		{ -0.2f - 0.6f,-0.2f - 0.6f,0.0f,   1,0,0} // POS3

	};
	// initialize a new AGameobject
	AGameobject* object1 = new AGameobject();
	ObjectManager::get()->objectList.push_back(object1);
	// gets the size of our array
	UINT size_list1 = ARRAYSIZE(shape1);
	// load the recently pass gameobject
	ObjectManager::get()->objectList[ObjectManager::get()->objectList.size() - 1]->load(
		shape1, sizeof(vertex), size_list1, shader_byte_code, size_shader);

	list shape2 =
	{
		//X - Y - Z
		{-0.2f + 0.6f,-0.2f + 0.6f,0.0f,   1,0,0}, // POS1
		{-0.2f + 0.6f,0.2f + 0.6f,0.0f,    0,1,0}, // POS2
		{ 0.2f + 0.6f,0.2f + 0.6f,0.0f,    0,0,1}, // POS3

		{ 0.2f + 0.6f,0.2f + 0.6f,0.0f,   0,0,1}, // POS1
		{ 0.2f + 0.6f,-0.2f + 0.6f,0.0f,   1,0.68f,0.26f}, // POS2
		{ -0.2f + 0.6f,-0.2f + 0.6f,0.0f,   1,0,0} // POS3
	};
	// initialize a new AGameobject
	AGameobject* object2 = new AGameobject();
	ObjectManager::get()->objectList.push_back(object2);
	// gets the size of our array
	UINT size_list2 = ARRAYSIZE(shape2);
	// load the recently pass gameobject
	ObjectManager::get()->objectList[ObjectManager::get()->objectList.size() - 1]->load(
		shape2, sizeof(vertex), size_list2, shader_byte_code, size_shader);

	
}




