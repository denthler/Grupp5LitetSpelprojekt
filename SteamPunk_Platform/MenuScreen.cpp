#include "MenuScreen.h"
static int cogs = 2;
MenuScreen::MenuScreen(ID3D11Device* device, ID3D11DeviceContext * deviceContext, HWND hwnd, D3DXMATRIX proj, HINSTANCE hInstance)
{
	// Triangle
	HRESULT result;
	ID3D10Blob * VS_Buffer, *PS_Buffer;
	result = D3DX11CompileFromFile(L"effect.fx", 0, 0, "VShader", "vs_5_0", 0, 0, 0, &VS_Buffer, 0, 0);
	result = D3DX11CompileFromFile(L"effect.fx", 0, 0, "PShader", "ps_5_0", 0, 0, 0, &PS_Buffer, 0, 0);

	result = device->CreateVertexShader(VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), NULL, &VS);
	result = device->CreatePixelShader(PS_Buffer->GetBufferPointer(), PS_Buffer->GetBufferSize(), NULL, &PS);

	deviceContext->VSSetShader(VS, 0, 0);
	deviceContext->PSSetShader(PS, 0, 0);

	float triangleData[] =
	{
		-0.875f, 1.0f, 0.0f,
		-1.0f, 0.75f, 0.0f,
		-0.75f, 0.75f, 0.0f
	};

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = triangleData;

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = sizeof(float)* 3 * 3;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&bufferDesc, &vertexBufferData, &triangleVertBuffer);
	UINT stride = sizeof(float)* 3;
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &triangleVertBuffer, &stride, &offset);

	D3D11_INPUT_ELEMENT_DESC layout;
	layout.SemanticName = "POSITION";
	layout.SemanticIndex = 0;
	layout.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	layout.InputSlot = 0;
	layout.AlignedByteOffset = 0;
	layout.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout.InstanceDataStepRate = 0;

	result = device->CreateInputLayout(&layout, 1, VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), &vertLayout);

	deviceContext->IASetInputLayout(vertLayout);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = 800;
	viewport.Height = 640;

	deviceContext->RSSetViewports(1, &viewport);

	this->device = device;
	this->deviceContext = deviceContext;

	// Render a texture
	ID3D10Blob * textureVSBuffer, *texturePSBuffer;
	result = D3DX11CompileFromFile(L"texture.fx", 0, 0, "VShader", "vs_5_0", 0, 0, 0, &textureVSBuffer, NULL, 0);
	result = D3DX11CompileFromFile(L"texture.fx", 0, 0, "PShader", "ps_5_0", 0, 0, 0, &texturePSBuffer, NULL, 0);

	result = device->CreateVertexShader(textureVSBuffer->GetBufferPointer(), textureVSBuffer->GetBufferSize(), NULL, &textureVS);
	result = device->CreatePixelShader(texturePSBuffer->GetBufferPointer(), texturePSBuffer->GetBufferSize(), NULL, &texturePS);

	deviceContext->VSSetShader(textureVS, 0, 0);
	deviceContext->PSSetShader(texturePS, 0, 0);


	float quadData[] =
	{
		0.75f, 0.75f, 0.0f,
		0.0f, 1.0f,
		0.75f, 1.0f, 0.0f,
		0.0f, 0.0f,
		0.875f, 0.875f, 0.0f,
		1.0f, 1.0f,		
		0.875f, 1.0f, 0.0f,
		1.0f, 0.0f,

		0.875f, 0.875f, 0.0f,
		0.0f, 1.0f,
		0.875f, 1.0f, 0.0f,
		0.0f, 0.0f,
		1.0f, 0.875f, 0.0f,
		1.0f, 1.0f,		
		1.0f, 1.0f, 0.0f,
		1.0f, 0.0f,
	};

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = quadData;

	bufferDesc.ByteWidth = sizeof(float)* (3 + 2) * cogs * 4;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&bufferDesc, &vertexBufferData, &quadVertBuffer);
	stride = sizeof(float)* (3 + 2);
	offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &quadVertBuffer, &stride, &offset);

	D3D11_INPUT_ELEMENT_DESC texturefxLayout[2];
	texturefxLayout[0].SemanticName = "POSITION";
	texturefxLayout[0].SemanticIndex = 0;
	texturefxLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	texturefxLayout[0].InputSlot = 0;
	texturefxLayout[0].AlignedByteOffset = 0;
	texturefxLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	texturefxLayout[0].InstanceDataStepRate = 0;

	texturefxLayout[1].SemanticName = "TEXCOORD";
	texturefxLayout[1].SemanticIndex = 0;
	texturefxLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	texturefxLayout[1].InputSlot = 0;
	texturefxLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	texturefxLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	texturefxLayout[1].InstanceDataStepRate = 0;

	result = device->CreateInputLayout(texturefxLayout, 2, textureVSBuffer->GetBufferPointer(), textureVSBuffer->GetBufferSize(), &quadLayout);
	deviceContext->IASetInputLayout(quadLayout);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	result = D3DX11CreateShaderResourceViewFromFile(device, L"C:/Users/Martin/Desktop/LuryADO.png", NULL, NULL, &texture, NULL);

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	result = device->CreateSamplerState(&samplerDesc, &samplerState);
	deviceContext->PSSetShaderResources(0, 1, &texture);
	deviceContext->PSSetSamplers(0, 1, &samplerState);

	D3D11_RENDER_TARGET_BLEND_DESC rtbd;
	ZeroMemory(&rtbd, sizeof(rtbd));
	rtbd.BlendEnable = true;
	rtbd.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	rtbd.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	rtbd.BlendOp = D3D11_BLEND_OP_ADD;
	rtbd.SrcBlendAlpha = D3D11_BLEND_INV_DEST_ALPHA;
	rtbd.DestBlendAlpha = D3D11_BLEND_ONE;
	rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.RenderTarget[0] = rtbd;

	device->CreateBlendState(&blendDesc, &blendState);
}

MenuScreen::~MenuScreen()
{

}

void MenuScreen::Update()
{
	
}

void MenuScreen::Draw()
{
	// Triangle
	deviceContext->VSSetShader(VS, 0, 0);
	deviceContext->PSSetShader(PS, 0, 0);
	UINT stride = sizeof(float)* 3;
	UINT offset = 0;
	deviceContext->IASetInputLayout(vertLayout);
	deviceContext->IASetVertexBuffers(0, 1, &triangleVertBuffer, &stride, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->Draw(3, 0);

	// Cog texture
	float blendFactor[] = { 0.75f, 0.75f, 0.75f, 1.0f };
	deviceContext->OMSetBlendState(blendState, blendFactor, 0xffffffff);
	deviceContext->VSSetShader(textureVS, 0, 0);
	deviceContext->PSSetShader(texturePS, 0, 0);
	deviceContext->PSSetShaderResources(0, 1, &texture);
	deviceContext->PSSetSamplers(0, 1, &samplerState);
	stride = sizeof(float)* (3 + 2);
	offset = 0;
	deviceContext->IASetInputLayout(quadLayout);
	deviceContext->IASetVertexBuffers(0, 1, &quadVertBuffer, &stride, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	for (int i = 0; i < cogs; i++)
	{
		deviceContext->Draw(4, 4*i);
	}
	

	// Reset blend state
	deviceContext->OMSetBlendState(0, 0, 0xffffffff);
}