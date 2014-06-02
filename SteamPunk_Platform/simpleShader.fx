////////////////////////////////////////////////////////////////////////////////
// Filename: simpleShader.fx
////////////////////////////////////////////////////////////////////////////////

float4 color = float4(1.0, 0.0f, 0.0f, 1.0f);

cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
	float3 position : POSITION;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType VS(VertexInputType input)
{
	PixelInputType output;
	output.position = mul(float4(input.position, 1.0f), (float4x4)worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	
	return output;
}

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 PS(PixelInputType input) : SV_TARGET
{
	float4 output = float4(color);
	return output;
}