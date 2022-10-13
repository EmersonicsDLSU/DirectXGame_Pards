#pragma once
#include "AGameObject.h"
#include "Prerequisites.h"

class AppWindow;

class Cube : public AGameObject
{
public:
	Cube();
	~Cube();
public:
	void Update(float deltaTime, AppWindow* app_window) override;
	void Draw(const VertexShaderPtr& m_vs, const PixelShaderPtr& m_ps, const BlenderPtr& m_blender) override;
public:
	void SetTexture(const wchar_t* tex_path);
	void SetAlpha(float alpha);
	float GetAlpha();
private:
	TexturePtr m_tex;
	VertexBufferPtr m_vb;
	IndexBufferPtr m_ib;
	ConstantBufferPtr m_cb;
	ConstantBufferPtr m_cb_texture;
private:
	float alpha = 1.0f;
private:
	friend class AppWindow;
};

