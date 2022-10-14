#include "AGameObject.h"
#include "Vector3D.h"
#include "Matrix4x4.h"
#include <math.h>

AGameObject::AGameObject(ObjectTypes type) : ObjectType(type)
{
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

double AGameObject::GetDistance(const Matrix4x4& camera)
{
	Vector3D cam = { camera.m_mat[0][0], camera.m_mat[1][1] ,camera.m_mat[2][2] };
	cam.m_x = m_position.m_x - cam.m_x;
	cam.m_y = m_position.m_y - cam.m_y;
	cam.m_z = m_position.m_z - cam.m_z;

	return sqrt(pow(cam.m_x, 2) + pow(cam.m_y, 2) + pow(cam.m_z, 2));
}


