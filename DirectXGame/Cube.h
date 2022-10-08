#pragma once
#include "AGameObject.h"

class Cube : public AGameObject
{
public:
	Cube(void* shader_byte_code, size_t size_shader, const wchar_t* file_path, char sample);
	~Cube();
public:
	void Update(float deltaTime, void* buffer) override;
	void Draw(VertexShaderPtr m_vs, PixelShaderPtr m_ps, BlenderPtr m_blender) override;
	char sample;
private:
	TexturePtr m_tex;
	VertexBufferPtr m_vb;
	IndexBufferPtr m_ib;
	ConstantBufferPtr m_cb;
	float ticks = 0.0f;
	float deltaPos = 0.0f;
	float deltaTime = 0.0f;
	float speed = 10.0f;
	friend class AppWindow;
};

