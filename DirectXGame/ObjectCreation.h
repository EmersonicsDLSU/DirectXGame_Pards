#pragma once
#include <iostream>
#include <vector>
#include "AGameobject.h"

struct vec3
{
	float x, y, z;
};

struct vertex
{
	vec3 position;
	vec3 color;
};

typedef vertex list[];

class AppWindow;
class AGameobject;

class ObjectCreation
{
public:
	ObjectCreation();
	~ObjectCreation();
	//Release all the resources loaded
	bool release();
	static ObjectCreation* get();
	void InitializeVertices(void* shader_byte_code, size_t size_shader);
private:
	friend class AppWindow;
	friend class AGameobject;
};

