////////////////////////////////////////////////////////////////////////////////
// Filename: shader.fx
////////////////////////////////////////////////////////////////////////////////

/////////////
// GLOBALS //
/////////////
Texture2D shaderTexture;
SamplerState SampleType;

cbuffer gMaterial
{
	
	float4 difColor;
	float4 ambColor;
	float3 pad;
	bool hasTexture;
	
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

cbuffer LightPositionBuffer
{
	float4 lightPosition[1];
};

cbuffer LightColorBuffer
{
	float4 diffuseColor[1];
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
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 lightPos1 : TEXCOORD1;
	
};

struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType VS(VertexInputType input)
{
	PixelInputType output;
	float4 worldPosition;

	input.position.w = 1.0f;

	output.position = mul(input.position, (float4x4)worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.tex = input.tex;

	output.normal = mul(input.normal, (float3x3)worldMatrix);

	output.normal = normalize(output.normal);

	worldPosition = mul(input.position, (float4x4)worldMatrix);
	
	output.lightPos1.xyz = pos.xyz - worldPosition.xyz; 	


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

	//float3 uvw = float3(input.tex, input.PrimID%4);

	if(hasTexture)
	{
		textureColor = shaderTexture.Sample(SampleType, 			      input.tex);
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
	
	lightIntensity1 = saturate(dot(input.normal, 	input.lightPos1));

	color += saturate(diffuse * textureColor); 
     // * difColor
	
	color *= lightIntensity1; 
	color /= att.x + (att.y * d) + (att.z * (d*d));

	color = saturate(color + finalAmbient); 
	
	//Test
	//if((diffuse .x == 0.0f) && 
	//(diffuse .y == 0.0f))
	//color = shaderTexture.Sample(SampleType,input.tex);


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
	//SetGeometryShader( CompileShader( gs_5_0, GS() ));
	SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS() ) );
    }
}