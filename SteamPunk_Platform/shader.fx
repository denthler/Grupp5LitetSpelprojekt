////////////////////////////////////////////////////////////////////////////////
// Filename: shader.fx
////////////////////////////////////////////////////////////////////////////////

/////////////
// GLOBALS //
/////////////
Texture2D shaderTexture;
Texture2D shadowTexture;
Texture2D normalMap;
SamplerState SampleType;
SamplerState SampleShadowType;

cbuffer gMaterial
{
	float4 difColor;
	float4 ambColor;
	//float3 pad;
	int hasTexture;
	int hasNormal;
	int pad[2];
};

cbuffer ShadowGen
{
	matrix shadowMat;
};

cbuffer LightBuffer
{
	float4 diffuse;
	float4 pos;
	float3 att;
	float range;
	float4 ambient;
};

//Material gMaterial;

cbuffer AnimationMatrixBuffer
{
	matrix BoneTransforms[30];
};

cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float2 tex : TEXCOORD0;
	float3 tangent : TANGENT;
	float3 lightPos1 : TEXCOORD1;
	float4 lightClipSpace : TEXCOORD2;
};

/*struct VertexInputType
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 tex : TEXCOORD0;	
	float3 tangent : TANGENT;
};*/

struct VertexInputTypeAni
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 tex : TEXCOORD0;
	float3 tangent : TANGENT;
	float3 Weight : WEIGHT;
	uint3 BoneIndex : BONEINDEX;
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
float4 pointShadowVS(float4 pos : POSITION) : SV_Position
{
	pos.w = 1.0f; 



	return mul(pos, shadowMat);
}
PixelInputType VS(VertexInputTypeAni input)
{
	PixelInputType output;
	float4 worldPosition;

	output.position = mul(float4(input.position, 1.0f), (float4x4)worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.tex = input.tex;

	output.normal = mul(input.normal, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);

	output.tangent = mul(input.tangent, (float3x3)worldMatrix);

	worldPosition = mul(float4(input.position, 1.0f), (float4x4)worldMatrix);
	
	output.lightPos1.xyz = pos.xyz - worldPosition.xyz; 	

	//output.lightClipSpace = mul(input.position, (float4x4)worldMatrix);
	output.lightClipSpace = mul(float4(input.position, 1.0f), (float4x4)shadowMat);

	return output;
}

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader Animation
////////////////////////////////////////////////////////////////////////////////
PixelInputType VSANI(VertexInputTypeAni input)
{
	PixelInputType output;
	float4 worldPosition;

	float weights[3] = { 0.0f, 0.0f, 0.0f };
	weights[0] = input.Weight.x;
	weights[1] = input.Weight.y;
	weights[2] = input.Weight.z;

	float3 posL = float3(0.0f, 0.0f, 0.0f);
	float3 normalL = float3(0.0f, 0.0f, 0.0f);
	float3 tangentL = float3(0.0f, 0.0f, 0.0f);

	if (weights[0] == 0)
	{
		posL = input.position;
		normalL = input.normal;
		tangentL = input.tangent;
	}	

	for (int x = 0; x < 3; x++)
	{
		posL += weights[x] * mul(BoneTransforms[input.BoneIndex[x]], float4(input.position, 1.0f)).xyz;
		normalL += weights[x] * mul(BoneTransforms[input.BoneIndex[x]], float4(input.normal, 0.0f)).xyz;
		tangentL += weights[x] * mul(BoneTransforms[input.BoneIndex[x]], float4(input.tangent, 0.0f)).xyz;
	}

	output.position = mul(float4(posL, 1.0f), (float4x4)worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.tex = input.tex;

	output.normal = mul(normalL, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);

	output.tangent = mul(tangentL, (float3x3)worldMatrix);

	worldPosition = mul(float4(posL, 1.0f), (float4x4)worldMatrix);

	output.lightPos1.xyz = pos.xyz - worldPosition.xyz;

	//output.lightClipSpace = mul(input.position, (float4x4)worldMatrix);
	output.lightClipSpace = mul(float4(posL, 1.0f), (float4x4)shadowMat);

	return output;
}

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 PS(PixelInputType input) : SV_TARGET
{

	float4 textureColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	float lightIntensity1;
	float4 color = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float3 bumpNormal;

	//float3 uvw = float3(input.tex, input.PrimID%4);

	bumpNormal = input.normal;
	if(hasTexture != 0)
	{
		textureColor = shaderTexture.Sample(SampleType, input.tex);
	}
	if(hasNormal != 0)
	{
		float3 normalMapSample = normalMap.Sample(SampleType, input.tex).rgb;
		float3 normalT = 2.0f*normalMapSample - 1.0f;

		float3 T = normalize(input.tangent - dot(input.tangent, bumpNormal)*bumpNormal);
		float3 B = cross(bumpNormal, T);
		float3x3 TBN = float3x3(T, B, bumpNormal);

		bumpNormal = mul(normalT, TBN);
	}

	//textureColor = shaderTexture.Sample(SampleType, 	//input.tex);
	float d = length(input.lightPos1);

	float4 finalAmbient = ambColor * ambient * 	textureColor;
	
	if(d > range)
	{ 
		return finalAmbient; 
		//return float4(1.0f, 1.0f, 1.0f, 1.0f);	
	}

	input.lightPos1 = normalize(input.lightPos1);

	//?
	//input.lightPos1 = -input.lightPos1;
	float2 projLight;
	projLight.x = input.lightClipSpace.x / input.lightClipSpace.w / 2.0f + 0.5f;
	projLight.y = -input.lightClipSpace.y / input.lightClipSpace.w / 2.0f + 0.5f;

	float lightDepth = 0.0f;
	float depth = 1.0f;

	if ((projLight.x <= 1.0f) && (projLight.x >= 0.0f) && (projLight.y <= 1.0f) && (projLight.y >= 0.0f))
	{
		if ((projLight.x == projLight.x) && (projLight.y == projLight.y))
		{
			depth = shadowTexture.Sample(SampleShadowType,
				projLight).r;
			//color = float4(depth, depth, depth, 1.0f);
			lightDepth = input.lightClipSpace.z / input.lightClipSpace.w;
			lightDepth -= 0.001f;
		}
	}

	if (lightDepth < depth)
	{

		lightIntensity1 = saturate(dot(bumpNormal, input.lightPos1));

		color += saturate(diffuse * textureColor);
		// * difColor

		color *= lightIntensity1;
		color /= att.x + (att.y * d) + (att.z * (d*d));
		color = saturate(color + finalAmbient);
		//color = float4(1.0f, 0.0f, 0.0f, 1.0f);
	}
	else
	{
		//color += saturate(diffuse * textureColor);
		color = saturate(color + (finalAmbient * 0.5f));
		//color = float4(1.0f, 0.0f, 0.0f, 1.0f);
	}
	
	
	//Test
	//if((diffuse .x == 0.0f) && 
	//(diffuse .y == 0.0f))
	//color = shaderTexture.Sample(SampleType,input.tex);

	//color = float4(0.0f, 0.0f, depth, 1.0f);
	return color;
}

////////////////////////////////////////////////////////////////////////////////
// Geometry Shader
////////////////////////////////////////////////////////////////////////////////



technique11 ShaderTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetPixelShader( CompileShader( ps_5_0, PS() ) );
    }
	pass P1
	{
		SetVertexShader(CompileShader(vs_5_0, VSANI()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
	pass P2
	{
		SetVertexShader(CompileShader(vs_5_0, pointShadowVS()));
		SetGeometryShader(NULL);
		SetPixelShader(NULL);

	}
}