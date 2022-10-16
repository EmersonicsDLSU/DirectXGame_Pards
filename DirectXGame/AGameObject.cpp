#include "AGameObject.h"
#include "Vector3D.h"
#include "Matrix4x4.h"
#include <math.h>

AGameObject::AGameObject(std::string name, ObjectTypes type) : name(name), ObjectType(type)
{
	m_position = Vector3D::zero();
	m_scale = Vector3D::one();
	m_rotation = Vector3D::zero();
}

AGameObject::~AGameObject()
{
}

void AGameObject::Draw(const VertexShaderPtr& m_vs, const PixelShaderPtr& m_ps, const BlenderPtr& m_blender)
{

}

void AGameObject::SetTransform(Vector3D position, Vector3D scale, Vector3D rotation)
{
	m_position = position;
	m_scale = scale;
	m_rotation = rotation;
}

void AGameObject::SetPosition(float x, float y, float z)
{
	m_position = Vector3D{ x,y,z };
}

void AGameObject::SetPosition(Vector3D pos)
{
	m_position = pos;
}

Vector3D AGameObject::GetLocalPosition()
{
	return m_position;
}

void AGameObject::SetScale(float x, float y, float z)
{
	m_scale = Vector3D{ x,y,z };
}

void AGameObject::SetScale(Vector3D scale)
{
	m_scale = scale;
}

Vector3D AGameObject::GetLocalScale()
{
	return m_scale;
}

void AGameObject::SetRotation(float x, float y, float z)
{
	m_rotation = Vector3D{ x,y,z };
}

void AGameObject::SetRotation(Vector3D rot)
{
	m_rotation = rot;
}

Vector3D AGameObject::GetLocalRotation()
{
	return m_rotation;
}

double AGameObject::GetDistance(const Matrix4x4& camera)
{
	Vector3D cam = { camera.m_mat[3][2], camera.m_mat[3][2] ,camera.m_mat[3][2] };
	Vector3D temp;
	temp.m_x = cam.m_x - m_position.m_x;
	temp.m_y = cam.m_y - m_position.m_y;
	temp.m_z = cam.m_z - m_position.m_z;

	return sqrt(pow(temp.m_x, 2) + pow(temp.m_y, 2) + pow(temp.m_z, 2));
}

std::string AGameObject::GetName()
{
	return name;
}

Matrix4x4 AGameObject::GetMatrix()
{
	return m_matrix;
}


