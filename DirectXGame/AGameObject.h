#pragma once
#include "Prerequisites.h"
#include "Vector3D.h"

class Matrix4x4;
class AppWindow;

enum ObjectTypes
{
	CUBE = 0,
	MESH
};

class AGameObject
{
public:
	AGameObject(ObjectTypes type);
	virtual ~AGameObject();
public:
	ObjectTypes ObjectType;
public:
	virtual void Update(float deltaTime, AppWindow* app_window) = 0;
	virtual void Draw(const VertexShaderPtr& m_vs, const PixelShaderPtr& m_ps, const BlenderPtr& m_blender);
public:
	void SetTransform(Vector3D position = { 0,0,0 }, Vector3D scale = { 1,1,1 }, Vector3D rotation = { 0,0,0 });
	double GetDistance(const Matrix4x4& camera);
protected:
	Vector3D m_position;
	Vector3D m_scale;
	Vector3D m_rotation;
};

