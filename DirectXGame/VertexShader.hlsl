// this function will be executed on each vertex of the triangles to render
/* SV_POSITION - indicates to the graphics pipeline that the output of our vertex shader
will contain the final transformed vertex position in the screen space coordinate.*/
struct VS_INPUT
{
	float4 position: POSITION;
	float3 color: COLOR;
};

struct VS_OUTPUT
{
	float4 position: SV_POSITION;
	float3 color: COLOR;
};


VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.position = input.position;
	output.color = input.color;

	return output;
}