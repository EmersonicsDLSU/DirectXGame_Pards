#pragma once
#include <string>

#include "Matrix4x4.h"
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
	AGameObject(std::string name ,ObjectTypes type);
	virtual ~AGameObject();
public:
	ObjectTypes ObjectType;
public:
	virtual void Update(float deltaTime, AppWindow* app_window) = 0;
	virtual void Draw(const VertexShaderPtr& m_vs, const PixelShaderPtr& m_ps, const BlenderPtr& m_blender);
public:
	void SetTransform(Vector3D position, Vector3D scale, Vector3D rotation);

	void SetPosition(float x, float y, float z);
	void SetPosition(Vector3D pos);
	Vector3D GetLocalPosition();

	void SetScale(float x, float y, float z);
	void SetScale(Vector3D scale);
	Vector3D GetLocalScale();

	void SetRotation(float x, float y, float z);
	void SetRotation(Vector3D rot);
	Vector3D GetLocalRotation();

	double GetDistance(const Matrix4x4& camera);
	std::string GetName();
protected:
	std::string name;
	Vector3D m_position;
	Vector3D m_scale;
	Vector3D m_rotation;
	Matrix4x4 m_matrix;
};

