#include "BBoxRender.h"
#include <string>

using namespace std;

BBoxRender::BBoxRender()
{
	device = 0;
	deviceContext = 0;
	VS = 0;
	PS = 0;
	buffer = 0;
	matrixBuffer = 0;
	vertLayout = 0;
}

BBoxRender::~BBoxRender()
{
	VS->Release();
	PS->Release();
	//buffer->Release();
	//matrixBuffer->Release();	
	//vertLayout->Release();
}

void BBoxRender::Init(ID3D11Device * device, ID3D11DeviceContext * deviceContext)
{
	this->device = device;
	this->deviceContext = deviceContext;

	ID3D10Blob * VSBuffer, *PSBuffer, *errorBlob;
	HRESULT result;
	result = D3DX11CompileFromFile(L"simpleShader.fx", 0, 0, "VS", "vs_5_0", 0, 0, 0, &VSBuffer, NULL, 0);
	result = D3DX11CompileFromFile(L"simpleShader.fx", 0, 0, "PS", "ps_5_0", 0, 0, 0, &PSBuffer, &errorBlob, 0);

	result = device->CreateVertexShader(VSBuffer->GetBufferPointer(), VSBuffer->GetBufferSize(), NULL, &VS);
	result = device->CreatePixelShader(PSBuffer->GetBufferPointer(), PSBuffer->GetBufferSize(), NULL, &PS);

	D3D11_INPUT_ELEMENT_DESC layout;
	ZeroMemory(&layout, sizeof(layout));
	layout.SemanticName = "POSITION";
	layout.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	layout.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	result = device->CreateInputLayout(&layout, 3, VSBuffer->GetBufferPointer(), VSBuffer->GetBufferSize(), &vertLayout);
}

void BBoxRender::Update(vector<ModelClass::BoundingBox> & bboxes, vector<D3DXMATRIX> & worldMatrices, D3DXMATRIX view, D3DXMATRIX proj)
{
	static unsigned int lastSize = 0;

	// Update buffers if the number of bounding boxes have changed.
	if (lastSize != bboxes.size())
	{
		vertices.clear();
		for (unsigned int i = 0; i < bboxes.size(); i++)
		{
			BBoxToVertices(bboxes[i]);
		}
	
		if (lastSize < bboxes.size())
		{
			if (buffer)
				buffer->Release();
			if (matrixBuffer)
				matrixBuffer->Release();

			D3D11_SUBRESOURCE_DATA bufferData;
			ZeroMemory(&bufferData, sizeof(bufferData));
			bufferData.pSysMem = vertices.data();

			D3D11_BUFFER_DESC bufferDesc;
			ZeroMemory(&bufferDesc, sizeof(bufferDesc));
			bufferDesc.ByteWidth = sizeof(float)* vertices.size();
			bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			HRESULT result = device->CreateBuffer(&bufferDesc, &bufferData, &buffer);

			for (int i = 0; i < worldMatrices.size(); i++)
			{
				matrices.push_back(worldMatrices[i]);
				matrices.push_back(view);
				matrices.push_back(proj);
			}
			D3D11_SUBRESOURCE_DATA  matrixData;
			ZeroMemory(&matrixData, sizeof(matrixData));
			matrixData.pSysMem = matrices.data();

			D3D11_BUFFER_DESC matrixBufferDesc;
			ZeroMemory(&matrixBufferDesc, sizeof(matrixBufferDesc));
			matrixBufferDesc.ByteWidth = sizeof(D3DXMATRIX) * matrices.size();
			matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			result = device->CreateBuffer(&matrixBufferDesc, &matrixData, &matrixBuffer);
		}
		else
		{
			deviceContext->UpdateSubresource(buffer, 0, NULL, vertices.data(), 1, vertices.size());

			D3D11_MAPPED_SUBRESOURCE matrixData;
			matrixData.pData = matrices.data();
			matrixData.DepthPitch = 0;
			matrixData.RowPitch = sizeof(D3DXMATRIX) * 4;

			deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE, 0, &matrixData);
			memcpy(matrixBuffer, matrices.data(), matrices.size());
			deviceContext->Unmap(matrixBuffer, 0);
		}
	}
	lastSize = bboxes.size();
}

void BBoxRender::Draw()
{
	deviceContext->VSSetShader(VS, 0, 0);
	deviceContext->PSSetShader(PS, 0, 0);
	deviceContext->IASetInputLayout(vertLayout);
	UINT stride = sizeof(float) * 3;
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	
	for (int i = 0; i < vertices.size() / 14.0f; i++)
	{
		deviceContext->Draw(14, i * 14);
	}
}

void BBoxRender::BBoxToVertices(ModelClass::BoundingBox bbox)
{
	float length = bbox.max.x - bbox.min.x;
	float height = bbox.max.y - bbox.min.y;
	float depth = bbox.max.z - bbox.min.z;

	// Data to draw a box with a single trianglestrip
	// http://www.cs.umd.edu/gvil/papers/av_ts.pdf

	// 4
	vertices.push_back(bbox.max.x);
	vertices.push_back(bbox.max.y);
	vertices.push_back(bbox.max.z);

	// 3
	vertices.push_back(bbox.max.x);
	vertices.push_back(bbox.max.y);
	vertices.push_back(bbox.max.z - depth);

	// 7
	vertices.push_back(bbox.max.x);
	vertices.push_back(bbox.max.y - height);
	vertices.push_back(bbox.max.z);

	// 8
	vertices.push_back(bbox.min.x + length);
	vertices.push_back(bbox.min.y);
	vertices.push_back(bbox.min.z);

	// 5
	vertices.push_back(bbox.min.x);
	vertices.push_back(bbox.min.y);
	vertices.push_back(bbox.min.z);

	// 3
	vertices.push_back(bbox.max.x);
	vertices.push_back(bbox.max.y);
	vertices.push_back(bbox.max.z - depth);

	// 1
	vertices.push_back(bbox.min.x);
	vertices.push_back(bbox.min.y + height);
	vertices.push_back(bbox.min.z);
	
	// 4
	vertices.push_back(bbox.max.x);
	vertices.push_back(bbox.max.y);
	vertices.push_back(bbox.max.z);

	// 2
	vertices.push_back(bbox.max.x - length);
	vertices.push_back(bbox.max.y);
	vertices.push_back(bbox.max.z);

	// 7
	vertices.push_back(bbox.max.x);
	vertices.push_back(bbox.max.y - height);
	vertices.push_back(bbox.max.z);

	// 6
	vertices.push_back(bbox.min.x);
	vertices.push_back(bbox.min.y);
	vertices.push_back(bbox.min.z + depth);

	// 5
	vertices.push_back(bbox.min.x);
	vertices.push_back(bbox.min.y);
	vertices.push_back(bbox.min.z);

	// 2
	vertices.push_back(bbox.max.x - length);
	vertices.push_back(bbox.max.y);
	vertices.push_back(bbox.max.z);

	// 1
	vertices.push_back(bbox.min.x);
	vertices.push_back(bbox.min.y + height);
	vertices.push_back(bbox.min.z);

	/* Data to draw a box with indexed trianglelist
	vertices.push_back(bbox.min.x);
	vertices.push_back(bbox.min.y);
	vertices.push_back(bbox.min.z);

	vertices.push_back(bbox.min.x + length);
	vertices.push_back(bbox.min.y);
	vertices.push_back(bbox.min.z);

	vertices.push_back(bbox.min.x);
	vertices.push_back(bbox.min.y + height);
	vertices.push_back(bbox.min.z);

	vertices.push_back(bbox.min.x);
	vertices.push_back(bbox.min.y);
	vertices.push_back(bbox.min.z + depth);

	vertices.push_back(bbox.max.x);
	vertices.push_back(bbox.max.y);
	vertices.push_back(bbox.max.z);

	vertices.push_back(bbox.max.x - length);
	vertices.push_back(bbox.max.y);
	vertices.push_back(bbox.max.z);

	vertices.push_back(bbox.max.x);
	vertices.push_back(bbox.max.y - height);
	vertices.push_back(bbox.max.z);

	vertices.push_back(bbox.max.x);
	vertices.push_back(bbox.max.y);
	vertices.push_back(bbox.max.z - depth);

	int offset = vertices.size();
	// First face
	indices.push_back(1 + offset);
	indices.push_back(2 + offset);
	indices.push_back(3 + offset);

	indices.push_back(2 + offset);
	indices.push_back(3 + offset);
	indices.push_back(8 + offset);

	// Second face
	indices.push_back(2 + offset);
	indices.push_back(7 + offset);
	indices.push_back(8 + offset);

	indices.push_back(7 + offset);
	indices.push_back(8 + offset);
	indices.push_back(5 + offset);

	// Third face
	indices.push_back(7 + offset);
	indices.push_back(4 + offset);
	indices.push_back(5 + offset);

	indices.push_back(4 + offset);
	indices.push_back(5 + offset);
	indices.push_back(6 + offset);

	// Fourth face
	indices.push_back(4 + offset);
	indices.push_back(1 + offset);
	indices.push_back(6 + offset);

	indices.push_back(1 + offset);
	indices.push_back(6 + offset);
	indices.push_back(3 + offset);

	// Fifth face
	indices.push_back(3 + offset);
	indices.push_back(8 + offset);
	indices.push_back(6 + offset);

	indices.push_back(8 + offset);
	indices.push_back(6 + offset);
	indices.push_back(5 + offset);

	// Sixth face
	indices.push_back(1 + offset);
	indices.push_back(2 + offset);
	indices.push_back(4 + offset);

	indices.push_back(2 + offset);
	indices.push_back(4 + offset);
	indices.push_back(7 + offset);
	*/
}