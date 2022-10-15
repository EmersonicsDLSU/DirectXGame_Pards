// this function will be executed on each vertex of the triangles to render
/* SV_POSITION - indicates to the graphics pipeline that the output of our vertex shader
will contain the final transformed vertex position in the screen space coordinate.*/

// what is the structure of our vertex input
struct VS_INPUT
{
	float4 position: POSITION;
	float3 color: COLOR;
	float3 color1: COLOR1;
};

// what is the structure of our vertex output
struct VS_OUTPUT
{
	float4 position: SV_POSITION;
	float3 color: COLOR;
	float3 color1: COLOR1;
};


cbuffer constant_transform: register(b0)
{
	row_major float4x4 m_world;
	row_major float4x4 m_view;
	row_major float4x4 m_proj;
	unsigned int m_time;
};


// what should we do with our input, and place it as a value for our output
VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	//	output.position = lerp(input.position, input.position1, (float)((sin((float)(m_time / (float)1000.0f)) + 1.0f) / 2.0f));

	//WORLD SPACE - multiplies(mul) our position to the world matrix
	output.position = mul(input.position, m_world);
	//VIEW SPACE - multiplies(mul) our position to the view matrix
	output.position = mul(output.position, m_view);
	//SCREEN SPACE - multiplies(mul) our position to the screen/projection matrix
	output.position = mul(output.position, m_proj);

	output.color = input.color;
	output.color1 = input.color1;
	return output;
}