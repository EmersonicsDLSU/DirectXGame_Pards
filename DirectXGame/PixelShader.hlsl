Texture2D Texture: register(t0);
sampler TextureSampler: register(s0);

struct PS_INPUT
{
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD0;
};

cbuffer constant_transform: register(b0)
{
	row_major float4x4 m_world;
	row_major float4x4 m_view;
	row_major float4x4 m_proj;
	unsigned int m_time;
};

cbuffer constant_texture: register(b0)
{
	float alpha;
};

float4 psmain(PS_INPUT input) : SV_TARGET
{
	float3 pixelColor = Texture.Sample(TextureSampler,input.texcoord);
	//return Texture.Sample(TextureSampler, input.texcoord);
	return float4(pixelColor, alpha);
}