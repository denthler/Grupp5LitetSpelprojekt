Texture2D g_Texture;

SamplerState TextureSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct vInput
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD;
};

struct pInput
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD;
};

pInput VShader(vInput input)
{
	pInput output;
	output.position = float4(input.position, 1.0f);
	output.texCoord = input.texCoord;
	return output;
}

float4 PShader(pInput input) : SV_TARGET
{
	float3 rgb = g_Texture.Sample(TextureSampler, input.texCoord);
	return float4(rgb, 1.0f);
	//return float4(1.0f, 0.5f, 0.5f, 1.0f);
}

technique11 tech
{
	pass p
	{
		SetVertexShader(CompileShader(vs_5_0, VShader()));
		SetPixelShader(CompileShader(ps_5_0, PShader()));
	}
}
