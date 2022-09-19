#pragma once
#include <d3d11.h>

class DeviceContext;

// constant buffer is a block of data which can store different kinds of data types.
// In the case of game engine, this is use to share data with the shaders(i.e. changing world view projection matrix).
// C++ application can communicate with the shader and update the values in the constant buffers the shader uses.
class ConstantBuffer
{
public:
	ConstantBuffer();
	bool load(void* buffer, UINT size_buffer);
	void update(DeviceContext* context, void* buffer);
	bool release();
	~ConstantBuffer();
private:
	ID3D11Buffer* m_buffer;
private:
	friend class DeviceContext;
};