#include "AGameObject.h"

#include "Vector3D.h"

AGameObject::AGameObject()
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