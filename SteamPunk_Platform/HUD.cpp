#include <vector>
#include "HUD.h"

HUD::HUD(ID3D11Device* device, ID3D11DeviceContext * deviceContext, HWND hwnd, D3DXMATRIX proj, HINSTANCE hInstance, PlatformManager * platformManager)
{
	this->device = device;
	this->deviceContext = deviceContext;

	// Render a texture
	ID3D10Blob * textureVSBuffer, *texturePSBuffer;
	HRESULT result;
	result = D3DX11CompileFromFile(L"texture.fx", 0, 0, "VShader", "vs_5_0", 0, 0, 0, &textureVSBuffer, NULL, 0);
	result = D3DX11CompileFromFile(L"texture.fx", 0, 0, "PShader", "ps_5_0", 0, 0, 0, &texturePSBuffer, NULL, 0);

	result = device->CreateVertexShader(textureVSBuffer->GetBufferPointer(), textureVSBuffer->GetBufferSize(), NULL, &textureVS);
	result = device->CreatePixelShader(texturePSBuffer->GetBufferPointer(), texturePSBuffer->GetBufferSize(), NULL, &texturePS);

	deviceContext->VSSetShader(textureVS, 0, 0);
	deviceContext->PSSetShader(texturePS, 0, 0);

	std::vector<float> cogData;
	// Each vertex requires 3 floats for position, and 2 floats for texcoordinates.
	// Each cog consists of 6 vertices.
	for (int i = 0; i < platformManager->gearsTotal; i++)
	{
		// Each cog texture is drawn next to each other vertically.
		// The offset is 0.125 in screen cordinates.
		float offset = i * 0.125f;
		cogData.push_back(-1.0f + offset); cogData.push_back(0.875f); cogData.push_back(0.0f);
		cogData.push_back(0.0f); cogData.push_back(1.0f);
		cogData.push_back(-1.0f + offset); cogData.push_back(1.0f); cogData.push_back(0.0f);
		cogData.push_back(0.0f); cogData.push_back(0.0f);
		cogData.push_back(-0.875f + offset); cogData.push_back(0.875f); cogData.push_back(0.0f);
		cogData.push_back(1.0f); cogData.push_back(1.0f);

		cogData.push_back(-0.875f + offset); cogData.push_back(1.0f); cogData.push_back(0.0f);
		cogData.push_back(1.0f); cogData.push_back(0.0f);
		cogData.push_back(-1.0f + offset); cogData.push_back(1.0f); cogData.push_back(0.0f);
		cogData.push_back(0.0f); cogData.push_back(0.0f);
		cogData.push_back(-0.875f + offset); cogData.push_back(0.875f); cogData.push_back(0.0f);
		cogData.push_back(1.0f); cogData.push_back(1.0f);
	}

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = cogData.data();

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = sizeof(float) * cogData.size();
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&bufferDesc, &vertexBufferData, &quadVertBuffer);
	UINT stride = sizeof(float) * (3 + 2);
	UINT offset = 0;
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
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	result = D3DX11CreateShaderResourceViewFromFile(device, L"Resources/Textures/cog_full.png", NULL, NULL, &cog_fullTexture, NULL);
	result = D3DX11CreateShaderResourceViewFromFile(device, L"Resources/Textures/cog_empty.png", NULL, NULL, &cog_emptyTexture, NULL);

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
	deviceContext->PSSetShaderResources(0, 1, &cog_fullTexture);
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
	this->platformManager = platformManager;
}

HUD::~HUD()
{

}

void HUD::Update()
{
	
}

void HUD::Draw()
{
	// Cog cog_fullTexture
	float blendFactor[] = { 0.75f, 0.75f, 0.75f, 1.0f };
	deviceContext->OMSetBlendState(blendState, blendFactor, 0xffffffff);
	deviceContext->VSSetShader(textureVS, 0, 0);
	deviceContext->PSSetShader(texturePS, 0, 0);
	deviceContext->PSSetShaderResources(0, 1, &cog_fullTexture);
	deviceContext->PSSetSamplers(0, 1, &samplerState);
	UINT stride = sizeof(float) * (3 + 2);
	UINT offset = 0;
	deviceContext->IASetInputLayout(quadLayout);
	deviceContext->IASetVertexBuffers(0, 1, &quadVertBuffer, &stride, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	int gearsFound = platformManager->gearsFound;
	int gearsMissing = platformManager->gearsTotal - gearsFound;

	deviceContext->Draw(6 * gearsFound, 0);
	deviceContext->PSSetShaderResources(0, 1, &cog_emptyTexture);	
	deviceContext->Draw(6 * gearsMissing, 6 * gearsFound);

	// Reset blend state
	deviceContext->OMSetBlendState(0, 0, 0xffffffff);
}