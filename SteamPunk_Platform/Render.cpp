////////////////////////////////////////////////////////////////////////////////
// Filename: lightshaderclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Render.h"

Render::Render()
{
	effect = 0;
	eTech = 0;
	samplerVariable = 0;
	cbAniMatrixBuffer = 0;
	cbMatrixBuffer = 0;
	cbMatrix = 0;
	cbAniMatrix = 0;
	shaderResourceView = 0;
	normalMapShaderResourceView = 0;
	shadowMapShaderResourceView = 0;
	shadowMap = 0;
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

bool SphereCollision(const D3DXPLANE& plane, float radius, D3DXVECTOR3 center)
{
	int i;
	
	if (D3DXPlaneDot(&plane, &D3DXVECTOR4(center.x, center.y, center.z, 1.0f)) < -radius)
	{
		return false;
	}
	
	return true;
}

bool Render::InsideFrustum(D3DXVECTOR3 min, D3DXVECTOR3 max)
{
	D3DXVECTOR3 extense = (max - min) / 2.0f;

	D3DXVECTOR3 center = min + extense;

	int count = 0;
	for (int i = 0; i < 6; i++)
	{
		D3DXVECTOR3 Normal = D3DXVECTOR3(plane[0].a, plane[0].b, plane[0].c);
		D3DXVec3Normalize(&Normal, &Normal);
		Normal *= 3.0f;

		float radius = abs(Normal.x * extense.x) + abs(Normal.y * extense.y) + abs(Normal.z * extense.z);

		if (SphereCollision(plane[i], radius, center))
			count++;
	}
	/*
	int count = 0;
	for (int i = 0; i < 6; i++)
	{
		if ((D3DXPlaneDotCoord(&plane[i], &min) > 0.0f) || (D3DXPlaneDotCoord(&plane[i], &max) > 0.0f))
			count++;
	}
	*/
	return count == 6;
	
}

void Render::UpdateFrustum(D3DXMATRIX view, D3DXMATRIX proj)
{
	D3DXMATRIX m2;

	m2 = view * proj;

	plane[0].a = m2[3] + m2[0];
	plane[0].b = m2[7] + m2[4];
	plane[0].c = m2[11] + m2[8];
	plane[0].d = m2[15] + m2[12];
	D3DXPlaneNormalize(&plane[0], &plane[0]);

	plane[1].a = m2[3] - m2[0];
	plane[1].b = m2[7] - m2[4];
	plane[1].c = m2[11] - m2[8];
	plane[1].d = m2[15] - m2[12];
	D3DXPlaneNormalize(&plane[1], &plane[1]);

	plane[2].a = m2[3] + m2[1];
	plane[2].b = m2[7] + m2[5];
	plane[2].c = m2[11] + m2[9];
	plane[2].d = m2[15] + m2[13];
	D3DXPlaneNormalize(&plane[2], &plane[2]);

	plane[3].a = m2[3] - m2[1];
	plane[3].b = m2[7] - m2[5];
	plane[3].c = m2[11] - m2[9];
	plane[3].d = m2[15] - m2[13];
	D3DXPlaneNormalize(&plane[3], &plane[3]);

	plane[4].a = m2[3] + m2[2];
	plane[4].b = m2[7] + m2[6];
	plane[4].c = m2[11] + m2[10];
	plane[4].d = m2[15] + m2[14];
	D3DXPlaneNormalize(&plane[4], &plane[4]);

	plane[5].a = m2[3] - m2[2];
	plane[5].b = m2[7] - m2[6];
	plane[5].c = m2[11] - m2[10];
	plane[5].d = m2[15] - m2[14];
	D3DXPlaneNormalize(&plane[5], &plane[5]);
}

bool Render::Initialize(ID3D11Device* device, HWND hwnd, WCHAR* filename, D3DXMATRIX proj)
{
	HRESULT result;
	ID3D10Blob* compiledShader;
	D3D11_INPUT_ELEMENT_DESC vertexLayout[6];
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC animationMatrixBufferDesc;
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
	cbAniMatrix = effect->GetConstantBufferByName("AnimationMatrixBuffer");
	lightConstantBuffer = effect->GetConstantBufferByName("LightBuffer");
	shaderResourceView = effect->GetVariableByName("shaderTexture")->AsShaderResource();
	normalMapShaderResourceView = effect->GetVariableByName("normalMap")->AsShaderResource();
	shadowMapShaderResourceView = effect->GetVariableByName("shadowMap")->AsShaderResource();
	samplerVariable = effect->GetVariableByName("SampleType")->AsSampler();
	Mat = effect->GetConstantBufferByName("gMaterial");

	vertexLayout[0].SemanticName = "POSITION";
	vertexLayout[0].SemanticIndex = 0;
	vertexLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	vertexLayout[0].InputSlot = 0;
	vertexLayout[0].AlignedByteOffset = 0;
	vertexLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	vertexLayout[0].InstanceDataStepRate = 0;

	/*vertexLayout[1].SemanticName = "TEXCOORD";
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
	vertexLayout[2].InstanceDataStepRate = 0;*/

	vertexLayout[1].SemanticName = "NORMAL";
	vertexLayout[1].SemanticIndex = 0;
	vertexLayout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	vertexLayout[1].InputSlot = 0;
	vertexLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	vertexLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	vertexLayout[1].InstanceDataStepRate = 0;

	vertexLayout[2].SemanticName = "TEXCOORD";
	vertexLayout[2].SemanticIndex = 0;
	vertexLayout[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	vertexLayout[2].InputSlot = 0;
	vertexLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	vertexLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	vertexLayout[2].InstanceDataStepRate = 0;

	vertexLayout[3].SemanticName = "TANGENT";
	vertexLayout[3].SemanticIndex = 0;
	vertexLayout[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	vertexLayout[3].InputSlot = 0;
	vertexLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	vertexLayout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	vertexLayout[3].InstanceDataStepRate = 0;

	vertexLayout[4].SemanticName = "WEIGHT";
	vertexLayout[4].SemanticIndex = 0;
	vertexLayout[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	vertexLayout[4].InputSlot = 0;
	vertexLayout[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	vertexLayout[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	vertexLayout[4].InstanceDataStepRate = 0;

	vertexLayout[5].SemanticName = "BONEINDEX";
	vertexLayout[5].SemanticIndex = 0;
	vertexLayout[5].Format = DXGI_FORMAT_R8G8B8A8_UINT;
	vertexLayout[5].InputSlot = 0;
	vertexLayout[5].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	vertexLayout[5].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	vertexLayout[5].InstanceDataStepRate = 0;

	D3DX11_PASS_DESC passDesc;
    eTech->GetPassByIndex(0)->GetDesc(&passDesc);
	result = device->CreateInputLayout(vertexLayout, 6, passDesc.pIAInputSignature, 
		passDesc.IAInputSignatureSize, &layout);

	if(FAILED(result))
	{
		return false;
	}

	compiledShader->Release();
	compiledShader = 0;

	animationMatrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	animationMatrixBufferDesc.ByteWidth = sizeof(AnimationMatrixBufferType);
	animationMatrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	animationMatrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	animationMatrixBufferDesc.MiscFlags = 0;
	animationMatrixBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&animationMatrixBufferDesc, NULL, &cbAniMatrixBuffer);
	if (FAILED(result))
	{
		return false;
	}
	
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
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;//D3D11_TEXTURE_ADDRESS_WRAP
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 1;
	samplerDesc.BorderColor[1] = 1;
	samplerDesc.BorderColor[2] = 1;
	samplerDesc.BorderColor[3] = 1;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	result = device->CreateSamplerState(&samplerDesc, &sampleState);
	if(FAILED(result))
	{
		return false;
	}

	//ShadowMap
	D3DXMatrixOrthoLH(&projectionMatrixShadow, 100.0f, 100.0f,1.0f ,500.0f );
	D3DXMatrixTranspose(&projectionMatrixShadow, &projectionMatrixShadow);	

	D3DXMatrixIdentity(&viewMatrixShadow);

	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 cameraPos(50.0f, 80.0f, -50.0f);

	D3DXVECTOR3 lightDirection = target - cameraPos;
	D3DXVec3Normalize(&lightDirection, &lightDirection);
	D3DXVECTOR3 right;
	D3DXVECTOR3 up;

	D3DXVec3Cross(&right ,&lightDirection ,&D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	D3DXVec3Cross(&up ,&right, &lightDirection);

	D3DXMatrixLookAtLH(&viewMatrixShadow, &cameraPos, &target, &up);

	/*viewMatrixShadow._22 = 0.0f;
	viewMatrixShadow._23 = -1.0f;
	viewMatrixShadow._32 = 1.0f;
	viewMatrixShadow._33 = 0.0f;
	viewMatrixShadow._43 = 100.0f;*/

	D3DXMatrixTranspose(&viewMatrixShadow, &viewMatrixShadow);	

	directionalLight = new DirectionalLightClass();

	directionalLight->SetDirection(lightDirection.x, lightDirection.y, lightDirection.z); 
	directionalLight->SetAmbientColor(0.4f, 0.4f, 0.4f, 1.0f);
	directionalLight->SetDiffuseColor(0.7f, 0.7f, 0.7f, 1.0f);

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
	if (normalMapShaderResourceView)
	{
		normalMapShaderResourceView = 0;
	}
	if (shadowMapShaderResourceView)
	{
		shadowMapShaderResourceView = 0;
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

	if (cbAniMatrixBuffer)
	{
		cbAniMatrixBuffer->Release();
		cbAniMatrixBuffer = 0;
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

	if (plane)
	{
		delete[] plane;
	}
}

void Render::Draw(ID3D11DeviceContext* deviceContext, int indexCount, int technique)
{
	deviceContext->IASetInputLayout(layout);

	samplerVariable->SetSampler(0, sampleState);

	D3DX11_TECHNIQUE_DESC techDesc;
    eTech->GetDesc( &techDesc );
	eTech->GetPassByIndex(technique)->Apply(0, deviceContext);
        
	deviceContext->Draw(indexCount, 0); 
}



bool Render::UpdateRender(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix,
	D3DXMATRIX viewMatrix, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* normalMap, ModelClass::Material mat, std::vector<D3DMATRIX> boneTransforms)
{

	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	LightBufferType* dataPtr2;
	AnimationMatrixBufferType* dataPtr3;
	MaterialType* dataPtr4;
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
	//dataPtr->view = viewMatrixShadow;
	dataPtr->projection = projectionMatrix;
	//dataPtr->projection = projectionMatrixShadow;

	dataPtr->viewShadow = viewMatrixShadow;
	dataPtr->projectionShadow = projectionMatrixShadow;

	deviceContext->Unmap(cbMatrixBuffer, 0);

	bufferNumber = 0;

	cbMatrix->SetConstantBuffer(cbMatrixBuffer);


	result = deviceContext->Map(cbAniMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0,
		&mappedResource);
	if (FAILED(result))
	{
		return false;
	}
	dataPtr3 = (AnimationMatrixBufferType*)mappedResource.pData;

	for (int x = 0; x < boneTransforms.size(); x++)
	{
		dataPtr3->boneTransform[x] = boneTransforms[x];
	}

	deviceContext->Unmap(cbAniMatrixBuffer, 0);

	bufferNumber = 2;
	cbAniMatrix->SetConstantBuffer(cbAniMatrixBuffer);


	shaderResourceView->SetResource(texture);
	normalMapShaderResourceView->SetResource(normalMap);
	shadowMapShaderResourceView->SetResource(shadowMap);
	
	result = deviceContext->Map(lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	dataPtr2 = (LightBufferType*)mappedResource.pData;

	dataPtr2->ambientColor = directionalLight->GetAmbientColor();
	dataPtr2->diffuseColor = directionalLight->GetDiffuseColor();
	dataPtr2->lightDirection = directionalLight->GetDirection();
	
	deviceContext->Unmap(lightBuffer, 0);

	bufferNumber = 1;

	lightConstantBuffer->SetConstantBuffer(lightBuffer);

	result = deviceContext->Map(materialBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	dataPtr4 = (MaterialType*)mappedResource.pData;

	dataPtr4->ambColor = mat.ambColor;
	dataPtr4->difColor = mat.difColor;
	dataPtr4->hasTexture = mat.hasTexture;
	dataPtr4->hasNormal = mat.hasNormal;

   	deviceContext->Unmap(materialBuffer, 0);	

	Mat->SetConstantBuffer(materialBuffer);

	return true;
}

bool Render::UpdateRenderShadow(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, std::vector<D3DMATRIX> boneTransforms)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	LightBufferType* dataPtr2;
	AnimationMatrixBufferType* dataPtr3;
	unsigned int bufferNumber;
		
	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);

	result = deviceContext->Map(cbMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, 
		&mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	dataPtr = (MatrixBufferType*)mappedResource.pData;

	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrixShadow;
	dataPtr->projection = projectionMatrixShadow;

	deviceContext->Unmap(cbMatrixBuffer, 0);

	bufferNumber = 0;

	cbMatrix->SetConstantBuffer(cbMatrixBuffer);


	result = deviceContext->Map(cbAniMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0,
		&mappedResource);
	if (FAILED(result))
	{
		return false;
	}
	dataPtr3 = (AnimationMatrixBufferType*)mappedResource.pData;

	for (int x = 0; x < boneTransforms.size(); x++)
	{
		dataPtr3->boneTransform[x] = boneTransforms[x];
	}

	deviceContext->Unmap(cbAniMatrixBuffer, 0);

	bufferNumber = 2;
	cbAniMatrix->SetConstantBuffer(cbAniMatrixBuffer);

	return true;
}

void Render::setLightPosition(D3DXVECTOR3 playerPos)
{
	D3DXVECTOR4 newPos = D3DXVECTOR4(-playerPos.x - 50, -playerPos.y - 80.0f, playerPos.z + 40, 0.0f);

	D3DXMatrixTranspose(&viewMatrixShadow, &viewMatrixShadow);	
	D3DXVec4Transform(&newPos, &newPos, &viewMatrixShadow);

	viewMatrixShadow._41 = newPos.x;
	viewMatrixShadow._42 = newPos.y;
	viewMatrixShadow._43 = newPos.z;

	D3DXMatrixTranspose(&viewMatrixShadow, &viewMatrixShadow);	
}
