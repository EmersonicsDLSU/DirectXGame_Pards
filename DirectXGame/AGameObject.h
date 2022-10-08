#pragma once
#include "Prerequisites.h"

class AGameObject
{
public:
	AGameObject();
	virtual ~AGameObject();

public:
	virtual void Initialize() = 0;

public:
	virtual void Draw();
};

