cbuffer matrices
{
	float4x4 world;
	float4x4 view;
	float4x4 projection;
};

struct vInput
{
	float3 position : POSITION;
};

struct pInput
{
	float4 position : SV_POSITION;
};

pInput VShader(vInput input)
{
	pInput output;
	output.position = float4(input, 1.0f);
	return output;
	/*
	pInput output;
	
	output.position = mul(float4(input.position, 1.0f), world);
	output.position = mul(output.position, view);
	output.position = mul(output.position, projection);

	return output;
	*/
}

float4 PShader(pInput input) : SV_TARGET
{

	return float4(1.0f, 0.5f, 0.5f, 1.0f);
}

technique11 tech
{
	pass p
	{
		SetVertexShader(CompileShader(vs_5_0, VShader()));
		SetPixelShader(CompileShader(ps_5_0, PShader()));
	}
}
