#include "MenuScreen.h"

MenuScreen::MenuScreen(ID3D11Device* device, ID3D11DeviceContext * deviceContext, HWND hwnd, D3DXMATRIX proj, HINSTANCE hInstance)
{	
	// Drawing a simple triangle on the screen. 
	HRESULT result;
	ID3D10Blob * VS_Buffer, * PS_Buffer;
	result = D3DX11CompileFromFile(L"effect.fx", 0, 0, "VShader", "vs_5_0", 0, 0, 0, &VS_Buffer, 0, 0);
	result = D3DX11CompileFromFile(L"effect.fx", 0, 0, "PShader", "ps_5_0", 0, 0, 0, &PS_Buffer, 0, 0);

	result = device->CreateVertexShader(VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), NULL, &VS);
	result = device->CreatePixelShader(PS_Buffer->GetBufferPointer(), PS_Buffer->GetBufferSize(), NULL, &PS);
	
	deviceContext->VSSetShader(VS, 0, 0);
	deviceContext->PSSetShader(PS, 0, 0);

	float triangleData[] =
	{
		-0.875f, 1.0f, 0.0f,
		-1.0f,  0.75f, 0.0f,
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

	triangleVertBuffer;
	result = device->CreateBuffer(&bufferDesc, &vertexBufferData, &triangleVertBuffer);
	UINT stride = sizeof(float) * 3;
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
	
	ID3D11InputLayout* vertLayout;
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
	ID3D11ShaderResourceView * texture;
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

	ID3D11SamplerState * samplerState;
	result = device->CreateSamplerState(&samplerDesc, &samplerState);
}

MenuScreen::~MenuScreen()
{

}

void MenuScreen::Update()
{
	
}

void MenuScreen::Draw()
{
	deviceContext->VSSetShader(VS, 0, 0);
	deviceContext->PSSetShader(PS, 0, 0);
	UINT stride = sizeof(float)* 3;
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &triangleVertBuffer, &stride, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->Draw(3, 0);
}