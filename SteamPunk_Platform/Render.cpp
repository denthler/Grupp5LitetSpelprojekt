////////////////////////////////////////////////////////////////////////////////
// Filename: lightshaderclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Render.h"

Render::Render()
{
	effect = 0;
	eTech = 0;
	samplerVariable = 0;
	cbMatrixBuffer = 0;
	cbMatrix = 0;
	shaderResourceView = 0;
	sampleState = 0;
	lightConstantBuffer = 0;
	cbMatrix = 0;
	lightBuffer = 0;
	Mat = 0;
	materialBuffer = 0;

}

Render::Render(const Render& other)
{
}

Render::~Render()
{
}

bool Render::Initialize(ID3D11Device* device, HWND hwnd, WCHAR* filename, D3DXMATRIX proj)
{
	HRESULT result;
	ID3D10Blob* compiledShader;
	D3D11_INPUT_ELEMENT_DESC vertexLayout[3];
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC lightBufferDesc;
	D3D11_BUFFER_DESC materialBufferDesc;
	compiledShader = 0;
	ID3D10Blob* errorMessage;

	projectionMatrix = proj;
	D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

	result = D3DX11CompileFromFile(filename, NULL, NULL, NULL, "fx_5_0", 0,
		0, NULL, &compiledShader, &errorMessage, NULL);

	if(FAILED(result))
	{
		if(errorMessage)
		{
			char* compileErrors;
			unsigned long bufferSize, i;
			ofstream fout;

			compileErrors = (char*)(errorMessage->GetBufferPointer());

			bufferSize = errorMessage->GetBufferSize();

			fout.open("shader-error.txt");

			for(i = 0; i < bufferSize; i++)
			{
				fout << compileErrors[i];
			}

			fout.close();

			errorMessage->Release();
			errorMessage = 0;

			MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", 
			filename, MB_OK);
			return false;
			}
		else
			MessageBox(hwnd, filename, L"Missing shader file", MB_OK);
		return false;
	}

	result = D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), 
		0, device, &effect);

	if(FAILED(result))
	{
		return false;
	}

	eTech = effect->GetTechniqueByName("ShaderTech");
	
	cbMatrix = effect->GetConstantBufferByName("MatrixBuffer");
	lightConstantBuffer = effect->GetConstantBufferByName("LightBuffer");
	shaderResourceView = effect->GetVariableByName("shaderTexture")->AsShaderResource();
	samplerVariable = effect->GetVariableByName("SampleType")->AsSampler();
	Mat = effect->GetConstantBufferByName("gMaterial");

	vertexLayout[0].SemanticName = "POSITION";
	vertexLayout[0].SemanticIndex = 0;
	vertexLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	vertexLayout[0].InputSlot = 0;
	vertexLayout[0].AlignedByteOffset = 0;
	vertexLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	vertexLayout[0].InstanceDataStepRate = 0;

	vertexLayout[1].SemanticName = "TEXCOORD";
	vertexLayout[1].SemanticIndex = 0;
	vertexLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	vertexLayout[1].InputSlot = 0;
	vertexLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	vertexLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	vertexLayout[1].InstanceDataStepRate = 0;

	vertexLayout[2].SemanticName = "NORMAL";
	vertexLayout[2].SemanticIndex = 0;
	vertexLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	vertexLayout[2].InputSlot = 0;
	vertexLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	vertexLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	vertexLayout[2].InstanceDataStepRate = 0;


	D3DX11_PASS_DESC passDesc;
    eTech->GetPassByIndex(0)->GetDesc(&passDesc);
	result = device->CreateInputLayout(vertexLayout, 3, passDesc.pIAInputSignature, 
		passDesc.IAInputSignatureSize, &layout);

	if(FAILED(result))
	{
		return false;
	}

	compiledShader->Release();
	compiledShader = 0;
	
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&matrixBufferDesc, NULL, &cbMatrixBuffer);
	if(FAILED(result))
	{
		return false;
	}
	
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&lightBufferDesc, NULL, &lightBuffer);
	if(FAILED(result))
	{
		return false;
	}


	materialBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	materialBufferDesc.ByteWidth = sizeof(MaterialType);
	materialBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	materialBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	materialBufferDesc.MiscFlags = 0;
	materialBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&materialBufferDesc, NULL, &materialBuffer);
	if(FAILED(result))
	{
		return false;
	}

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	result = device->CreateSamplerState(&samplerDesc, &sampleState);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}

void Render::CleanShader()
{
	if(materialBuffer)
	{
		materialBuffer->Release();
		materialBuffer = 0;
	}
	if(shaderResourceView)
	{
		shaderResourceView = 0;
	}
	if(lightBuffer)
	{
		lightBuffer->Release();
		lightBuffer = 0;
	}

	if(sampleState)
	{
		sampleState->Release();
		sampleState = 0;
	}

	if(cbMatrixBuffer)
	{
		cbMatrixBuffer->Release();
		cbMatrixBuffer = 0;
	}

	if(layout)
	{
		layout->Release();
		layout = 0;
	}

	if(effect)
	{
		effect->Release();
		effect = 0;
	}

}

void Render::Draw(ID3D11DeviceContext* deviceContext, int indexCount)
{


	deviceContext->IASetInputLayout(layout);

	samplerVariable->SetSampler(0, sampleState);

	D3DX11_TECHNIQUE_DESC techDesc;
    eTech->GetDesc( &techDesc );
    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
        eTech->GetPassByIndex(p)->Apply(0, deviceContext);
        
		deviceContext->DrawIndexed(indexCount, 0, 0);
    }
}



bool Render::UpdateRender(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix,
	D3DXMATRIX viewMatrix, ID3D11ShaderResourceView* texture, PointLightClass* lightStruct, ModelClass::Material* mat)
{

	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	LightBufferType* dataPtr2;
	MaterialType* dataPtr3;
	unsigned int bufferNumber;

	
	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);	

	result = deviceContext->Map(cbMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, 
		&mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	dataPtr = (MatrixBufferType*)mappedResource.pData;

	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	deviceContext->Unmap(cbMatrixBuffer, 0);

	bufferNumber = 0;

	cbMatrix->SetConstantBuffer(cbMatrixBuffer);

	shaderResourceView->SetResource(texture);
	
	result = deviceContext->Map(lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	dataPtr2 = (LightBufferType*)mappedResource.pData;

	dataPtr2->ambientColor = lightStruct->GetAmbientColor();
	dataPtr2->diffuseColor = lightStruct->GetDiffuseColor();
	dataPtr2->lightAtt = lightStruct->GetAttenuation();
	dataPtr2->lightPosition = lightStruct->GetPosition();
	dataPtr2->range = lightStruct->GetRange();
	
	deviceContext->Unmap(lightBuffer, 0);

	bufferNumber = 1;

	lightConstantBuffer->SetConstantBuffer(lightBuffer);

	result = deviceContext->Map(materialBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	dataPtr3 = (MaterialType*)mappedResource.pData;

	dataPtr3->ambColor = mat->ambColor;
	dataPtr3->difColor = mat->difColor;
	dataPtr3->hasTexture = mat->hasTexture;
	dataPtr3->padding = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	deviceContext->Unmap(materialBuffer, 0);	

	Mat->SetConstantBuffer(materialBuffer);

	return true;
}




