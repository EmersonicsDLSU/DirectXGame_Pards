// this function will be executed on each vertex of the triangles to render
/* SV_POSITION - indicates to the graphics pipeline that the output of our vertex shader
will contain the final transformed vertex position in the screen space coordinate.*/
float4 vsmain( float4 position : POSITION ) : SV_POSITION
{
	// translating vertices
	if (position.y > 0 && position.y < 1)
	{
		position.x += 0.25f;
	}
	if (position.y > -1 && position.y < 0 && position.x > 0 && position.x < 1)
	{
		position.y += 0.25f;
	}
	if (position.y > 0 && position.y < 1 && position.x > -1 && position.x < 0)
	{
		position.y -= 0.25f;
	}
	return position;
}