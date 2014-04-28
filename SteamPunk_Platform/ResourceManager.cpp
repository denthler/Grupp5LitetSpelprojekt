#include "ResourceManager.h"

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{

}

bool ResourceManager::LoadLevel(std::string level, ID3D11Device* DContext)
{
	lImporter.LoadLevel(level);

	for (int i = 0; i < lImporter.types.size(); i++)
	{
		aImporter.LoadAsset(lImporter.types[i]->type);
		CreateMesh(DContext, i);
	}
	meshes;
	return true;
}

void ResourceManager::CreateMesh(ID3D11Device* DContext, int index)
{
	HRESULT hr;
	for (int i = 0; i < aImporter.meshes.size(); i++)
	{
		Mesh newMesh;
		std::vector<VertexTypeT> tempVertices;

		newMesh.type = lImporter.types[index]->type;

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

		for (int j = 0; j < lImporter.types[index]->transforms.size(); j++)
			newMesh.transforms.push_back(lImporter.types[index]->transforms[j]);

		newMesh.bBox.min = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		newMesh.bBox.max = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		for (int j = 0; j < tempVertices.size(); j++)
		{
			newMesh.bBox.max.x = max(newMesh.bBox.max.x, tempVertices[j].position.x);
			newMesh.bBox.min.x = min(newMesh.bBox.min.x, tempVertices[j].position.x);

			newMesh.bBox.max.y = max(newMesh.bBox.max.y, tempVertices[j].position.y);
			newMesh.bBox.min.y = min(newMesh.bBox.min.y, tempVertices[j].position.y);

			newMesh.bBox.max.z = max(newMesh.bBox.max.z, tempVertices[j].position.z);
			newMesh.bBox.min.z = min(newMesh.bBox.min.z, tempVertices[j].position.z);
		}

		newMesh.vCount = tempVertices.size();

		D3D11_BUFFER_DESC vBD;
		memset(&vBD, 0, sizeof(vBD));
		vBD.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vBD.CPUAccessFlags = 0;
		vBD.Usage = D3D11_USAGE_DEFAULT;
		vBD.MiscFlags = 0;
		vBD.ByteWidth = sizeof(VertexTypeT)* tempVertices.size();
		D3D11_SUBRESOURCE_DATA pData;
		pData.pSysMem = &tempVertices[0];
		hr = (DContext->CreateBuffer(&vBD, &pData, &newMesh.m_vertexBuffer));
		meshes.push_back(newMesh);
	}
}