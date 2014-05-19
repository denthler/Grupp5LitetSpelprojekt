#include "Menu.h"

Menu::Menu(ID3D11Device* device)
{
	offset = 1.0f;
	notMoving = true;
	pause = false;
	aImporter.LoadAsset("meny_cogwheel_");
	CreateVertexBuffer(device);

	D3DXMatrixIdentity(&mesh.world_Matrix);
}

Menu::~Menu()
{

}

void Menu::CreateVertexBuffer(ID3D11Device* device)
{
	for (int i = 0; i < aImporter.meshes.size(); i++)
	{
		MenuMesh newMesh;
		std::vector<VertexTypeT> tempVertices;

		for (int j = 0; j < aImporter.meshes[i]->faceIndices.size(); j++)
			for (int x = 0; x < aImporter.meshes[i]->faceIndices[j].vertex.size(); x++)
			{
				VertexTypeT newVert;
				newVert.position = aImporter.meshes[i]->vertices[aImporter.meshes[i]->faceIndices[j].vertex[x]];
				newVert.normal = aImporter.meshes[i]->normals[aImporter.meshes[i]->faceIndices[j].normal[x]];
				newVert.texture = aImporter.meshes[i]->uv[aImporter.meshes[i]->faceIndices[j].uv[x]];
				newVert.tangent = aImporter.meshes[i]->uvTangents[aImporter.meshes[i]->faceIndices[j].tangent[x]];
				tempVertices.push_back(newVert);
			}

		newMesh.vCount = tempVertices.size();

		std::string file_Path;
		if (aImporter.meshes[i]->textureMap.length() > 0)
		{
			file_Path = "Resources/Textures/" + aImporter.meshes[i]->textureMap;
			texture.Initialize(device, file_Path);
			newMesh.textureMap = texture.GetTexture();
		}
		else
			newMesh.textureMap = NULL;
		if (aImporter.meshes[i]->normalMap.length() > 0)
		{
			file_Path = "Resources/Textures/" + aImporter.meshes[i]->normalMap;
			texture.Initialize(device, file_Path);
			newMesh.normalMap = texture.GetTexture();
		}
		else
			newMesh.normalMap = NULL;

		D3D11_BUFFER_DESC vBD;
		memset(&vBD, 0, sizeof(vBD));
		vBD.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vBD.CPUAccessFlags = 0;
		vBD.Usage = D3D11_USAGE_DEFAULT;
		vBD.MiscFlags = 0;
		vBD.ByteWidth = sizeof(VertexTypeT)* tempVertices.size();
		D3D11_SUBRESOURCE_DATA pData;
		pData.pSysMem = &tempVertices[0];
		device->CreateBuffer(&vBD, &pData, &newMesh.m_vertexBuffer);

		mesh = newMesh;
	}
}

void Menu::Pause()
{
	if(notMoving && !pause)
	{
		notMoving = false;
		pause = true;
	}
	else if(notMoving && pause)
	{
		notMoving = false;
		pause = false;
	}
}

void Menu::Update(D3DXMATRIX playerPos, bool Rotated, D3DXVECTOR3 worldAxis)
{
	UpdatePosition(playerPos, Rotated, worldAxis);

	if (offset != 0.0f && pause)
	{
		if (offset > 0.0f)
		{
			offset -= 0.01f;
		}
		else
		{
			offset = 0.0f;
			notMoving = true;
		}
	}
	else if (offset != 1.0f && !pause)
	{
		if (offset < 1.0f)
		{
			offset += 0.01f;
		}
		else
		{
			offset = 1.0f;
			notMoving = true;
		}
	}
}

void Menu::Draw(ID3D11DeviceContext* deviceContext, Render* render, D3DXMATRIX viewMatrix, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* normal, ModelClass::Material mat)
{
	unsigned int stride;
	unsigned int offset;
	std::vector<D3DMATRIX> temp;

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	stride = sizeof(VertexTypeT);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &mesh.m_vertexBuffer, &stride, &offset);

	if (mesh.textureMap)
	{
		texture = mesh.textureMap;
		mat.hasTexture = true;
	}
	else
	{
		texture = 0;
		mat.hasTexture = false;
	}

	if (mesh.normalMap)
	{
		normal = mesh.normalMap;
		mat.hasNormal = true;
	}
	else
	{
		normal = 0;
		mat.hasNormal = false;
	}

	bool result = render->UpdateRender(deviceContext, mesh.world_Matrix, viewMatrix, texture, normal, mat, temp);
	render->Draw(deviceContext, mesh.vCount, 0);
}

void Menu::UpdatePosition(D3DXMATRIX playerPos, bool Rotated, D3DXVECTOR3 worldAxis)
{
	D3DXVECTOR3 temp;
	temp.x = playerPos._41;
	temp.y = playerPos._42;
	temp.z = playerPos._43 -35;

	playerPos._41 = 0.0f;
	playerPos._42 = 0.0f;
	playerPos._43 = 0.0f;

	if (Rotated)
	{
		if (worldAxis.y > 0.0f)
		{
			D3DXMatrixRotationY(&playerPos, 0.0f);
		}
		else if (worldAxis.y < 0.0f)
		{
			D3DXMatrixRotationZ(&playerPos, D3DX_PI);
		}
		else if (worldAxis.x < 0.0f)
		{
			D3DXMatrixRotationZ(&playerPos, D3DX_PI / 2);
		}
		else if (worldAxis.x > 0.0f)
		{
			D3DXMatrixRotationZ(&playerPos, -D3DX_PI / 2);
		}		
	}

	if (worldAxis.y > 0.0f)
	{
		temp.x -= 10.0f + offset * 10;
	}
	else if (worldAxis.y < 0.0f)
	{
		temp.x += 10.0f + offset * 10;
	}
	else if (worldAxis.x < 0.0f)
	{
		temp.y -= 10.0f + offset * 10;
	}
	else if (worldAxis.x > 0.0f)
	{
		temp.y += 10.0f + offset * 10;
	}	

	playerPos._41 = temp.x;
	playerPos._42 = temp.y;
	playerPos._43 = temp.z;

	mesh.world_Matrix = playerPos;
}