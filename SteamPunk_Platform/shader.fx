////////////////////////////////////////////////////////////////////////////////
// Filename: shader.fx
////////////////////////////////////////////////////////////////////////////////

/////////////
// GLOBALS //
/////////////
Texture2D shaderTexture;
Texture2D normalMap;
Texture2D shadowMap;
SamplerState SampleType;

cbuffer gMaterial
{
	float4 difColor;
	float4 ambColor;
	//float3 pad;
	int hasTexture;
	int hasNormal;
	int pad[2];
};


/*cbuffer LightBuffer	/// PointLight
{
	float4 diffuse;
	float4 pos;
	float3 att;
	float range;
	float4 ambient;
};*/

cbuffer LightBuffer	/// DirectionalLight
{
	float4 diffuse;
	float3 direction;
	float pad1;
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

	matrix viewMatrixShadow;
	matrix projectionMatrixShadow;
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
	//float3 lightPos1 : TEXCOORD1;
	float4 posLightView : POSLV;
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
	
	//output.lightPos1.xyz = pos.xyz - worldPosition.xyz; 

	output.posLightView = mul( float4(input.position, 1), worldMatrix );
    output.posLightView = mul( output.posLightView, viewMatrixShadow );
    output.posLightView = mul( output.posLightView, projectionMatrixShadow );

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

	//output.lightPos1.xyz = pos.xyz - worldPosition.xyz;

	output.posLightView = mul( float4(posL, 1), worldMatrix );
    output.posLightView = mul( output.posLightView, viewMatrixShadow );
    output.posLightView = mul( output.posLightView, projectionMatrixShadow );

	return output;
}

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader Shadow
////////////////////////////////////////////////////////////////////////////////
PixelInputType VSShadow(VertexInputTypeAni input)
{
	PixelInputType output;

	output.position = mul(float4(input.position, 1.0f), (float4x4)worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	return output;
}

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader Animation Shadow
////////////////////////////////////////////////////////////////////////////////
PixelInputType VSANIShadow(VertexInputTypeAni input)
{
	PixelInputType output;

	float weights[3] = { 0.0f, 0.0f, 0.0f };
	weights[0] = input.Weight.x;
	weights[1] = input.Weight.y;
	weights[2] = input.Weight.z;

	float3 posL = float3(0.0f, 0.0f, 0.0f);

	if (weights[0] <= 0)
	{
		posL = input.position;
	}	

	for (int x = 0; x < 3; x++)
	{
		posL += weights[x] * mul(BoneTransforms[input.BoneIndex[x]], float4(input.position, 1.0f)).xyz;
	}

	output.position = mul(float4(posL, 1.0f), (float4x4)worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

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

	/*float test = 1.0f - ((input.position.z / input.position.w) * 50);
	if(test > 1.0f)
		test = 1.0f;*/

	if(hasTexture != 0)
	{
		textureColor = shaderTexture.Sample(SampleType, input.tex);
	}

	bumpNormal = input.normal;
	if(hasNormal != 0)
	{
		float3 normalMapSample = normalMap.Sample(SampleType, input.tex).rgb;
		float3 normalT = 2.0f*normalMapSample - 1.0f;

		float3 T = normalize(input.tangent - dot(input.tangent, bumpNormal)*bumpNormal);
		float3 B = cross(bumpNormal, T);
		float3x3 TBN = float3x3(T, B, bumpNormal);

		bumpNormal = mul(normalT, TBN);
	}

	//float d = length(input.lightPos1);

	float4 finalAmbient = ambColor * ambient * 	textureColor;

	/*if(d > range)
	{ 
		return finalAmbient; 
		//return float4(1.0f, 1.0f, 1.0f, 1.0f);	
	}*/

	//input.lightPos1 = normalize(input.lightPos1);
	
	lightIntensity1 = saturate(dot(bumpNormal, -direction));
	color += saturate(diffuse * textureColor); 	
	color *= lightIntensity1; 
	color = saturate(color + finalAmbient);	

	input.posLightView.xy /= input.posLightView.w; 
 
	float2 smTex = float2(0.5f*input.posLightView.x, -0.5f*input.posLightView.y) + 0.5f;

	input.posLightView.z /= input.posLightView.w;

	float shadowCoeff = shadowMap.Sample(SampleType, smTex).r;

	if(shadowCoeff + 0.00001f < input.posLightView.z)
	{
		shadowCoeff = 0.7f;
	}
	else
		shadowCoeff = 1;

	color *= shadowCoeff;

	return color;
}

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
		SetVertexShader(CompileShader(vs_5_0, VSShadow()));
		SetPixelShader(NULL);
	}
	pass P3
	{
		SetVertexShader(CompileShader(vs_5_0, VSANIShadow()));
		SetPixelShader(NULL);
	}
}