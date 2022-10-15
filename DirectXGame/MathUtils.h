#pragma once
#include <iostream>
#include <memory>
#include <ostream>

class MathUtils
{
public:
	MathUtils()
	{
	}

	static float randomFloat(float x, float y)
	{
		return x + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (y - x)));
	}
		
	~MathUtils()
	{
	}
	
};
