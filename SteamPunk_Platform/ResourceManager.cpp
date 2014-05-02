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

		std::string type = lImporter.types[i]->type;
		std::string tempSubString = type.substr(0, 2);

		if (tempSubString != "p_" && tempSubString != "e_")
			CreateStaticMeshes(DContext, i);
		else
			CreateAnimatedMeshes(DContext, i);
	}
	return true;
}

void ResourceManager::CreateStaticMeshes(ID3D11Device* DContext, int index)
{
	HRESULT hr;
	for (int i = 0; i < aImporter.meshes.size(); i++)
	{
		Mesh newMesh;
		std::vector<VertexTypeT> tempVertices;
		
		newMesh.type = lImporter.types[index]->type;
		std::string tempSubString = newMesh.type.substr(0, 2);

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

		ModelClass::BoundingBox bBox;
		bBox.min = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		bBox.max = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		for (int j = 0; j < tempVertices.size(); j++)
		{
			bBox.max.x = max(bBox.max.x, tempVertices[j].position.x);
			bBox.min.x = min(bBox.min.x, tempVertices[j].position.x);

			bBox.max.y = max(bBox.max.y, tempVertices[j].position.y);
			bBox.min.y = min(bBox.min.y, tempVertices[j].position.y);

			bBox.max.z = max(bBox.max.z, tempVertices[j].position.z);
			bBox.min.z = min(bBox.min.z, tempVertices[j].position.z);
		}
			
		ModelClass::BoundingBox tempBox;
		for (int j = 0; j < lImporter.types[index]->transforms.size(); j++)
		{
			newMesh.transforms.push_back(lImporter.types[index]->transforms[j]);
			D3DXVECTOR4 tempVec;
			D3DXVec3Transform(&tempVec, &bBox.max, &newMesh.transforms[j]);
			tempBox.max.x = tempVec.x;
			tempBox.max.y = tempVec.y;
			tempBox.max.z = tempVec.z;
			D3DXVec3Transform(&tempVec, &bBox.min, &newMesh.transforms[j]);
			tempBox.min.x = tempVec.x;
			tempBox.min.y = tempVec.y;
			tempBox.min.z = tempVec.z;

			if ((tempBox.min.x > tempBox.max.x))
			{
				tempBox.min.x = tempBox.max.x;
				tempBox.max.x = tempVec.x;
			}
			if ((tempBox.min.y > tempBox.max.y))
			{
				tempBox.min.y = tempBox.max.y;
				tempBox.max.y = tempVec.y;
			}
			if ((tempBox.min.z > tempBox.max.z))
			{
				tempBox.min.z = tempBox.max.z;
				tempBox.max.z = tempVec.z;
			}

			newMesh.bBox.push_back(tempBox);
		}		

		newMesh.vCount = tempVertices.size();

		std::string file_Path;
		if (aImporter.meshes[i]->textureMap.length() > 0)
		{
			file_Path = "Resources/Textures/" + aImporter.meshes[i]->textureMap;
			texture.Initialize(DContext, file_Path);
			newMesh.textureMap = texture.GetTexture();
		}
		else
			newMesh.textureMap = NULL;
		if (aImporter.meshes[i]->normalMap.length() > 0)
		{
			file_Path = "Resources/Textures/" + aImporter.meshes[i]->normalMap;
			texture.Initialize(DContext, file_Path);
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
		hr = (DContext->CreateBuffer(&vBD, &pData, &newMesh.m_vertexBuffer));

		meshes.push_back(newMesh);
	}
}

void ResourceManager::CreateAnimatedMeshes(ID3D11Device* DContext, int index)
{
	HRESULT hr;
	for (int i = 0; i < aImporter.meshes.size(); i++)
	{
		Mesh newMesh;
		std::vector<VertexTypeAni> tempVertices;

		newMesh.type = lImporter.types[index]->type;
		std::string tempSubString = newMesh.type.substr(0, 2);

		struct weightTemp
		{
			std::vector<BYTE> indecies;
			std::vector<float> weights;
		};
		std::vector<weightTemp> weightVertex;
		weightVertex.resize(aImporter.meshes[i]->vertices.size());

		for (int j = 0; j < aImporter.meshes[i]->clusters.size(); j++)
			for (int x = 0; x < aImporter.meshes[i]->clusters[j].index.size(); x++)
			{
				weightVertex[aImporter.meshes[i]->clusters[j].index[x]].indecies.push_back(j);
				weightVertex[aImporter.meshes[i]->clusters[j].index[x]].weights.push_back(aImporter.meshes[i]->clusters[j].weight[x]);
			}

		for (int j = 0; j < aImporter.meshes[i]->faceIndices.size(); j++)
			for (int x = 0; x < aImporter.meshes[i]->faceIndices[j].vertex.size(); x++)
			{
				VertexTypeAni newVert;
				newVert.position = aImporter.meshes[i]->vertices[aImporter.meshes[i]->faceIndices[j].vertex[x]];
				newVert.normal = aImporter.meshes[i]->normals[aImporter.meshes[i]->faceIndices[j].normal[x]];
				newVert.texture = aImporter.meshes[i]->uv[aImporter.meshes[i]->faceIndices[j].uv[x]];
				newVert.tangent = aImporter.meshes[i]->uvTangents[aImporter.meshes[i]->faceIndices[j].tangent[x]];

				for (int y = 0; y < 3; y++)
				{
					if (weightVertex[aImporter.meshes[i]->faceIndices[j].vertex[x]].indecies.size() > y)
					{
						newVert.BoneIndex[y] = weightVertex[aImporter.meshes[i]->faceIndices[j].vertex[x]].indecies[y];
						newVert.Weight[y] = weightVertex[aImporter.meshes[i]->faceIndices[j].vertex[x]].weights[y];
					}
					else
					{
						newVert.BoneIndex[y] = 0;
						newVert.Weight[y] = 0.0f;
					}
				}

				tempVertices.push_back(newVert);
			}

		for (int j = 0; j < tempVertices.size(); j++)
		{
			float sum = 0.0f;
			for (int y = 0; y < 3; y++)
			{
				sum += tempVertices[j].Weight[y];
			}
			if (sum != 0.0f)
				for (int y = 0; y < 3; y++)
				{
					tempVertices[j].Weight[y] /= sum;
				}
		}

		ModelClass::BoundingBox bBox;
		bBox.min = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		bBox.max = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		for (int j = 0; j < tempVertices.size(); j++)
		{
			bBox.max.x = max(bBox.max.x, tempVertices[j].position.x);
			bBox.min.x = min(bBox.min.x, tempVertices[j].position.x);

			bBox.max.y = max(bBox.max.y, tempVertices[j].position.y);
			bBox.min.y = min(bBox.min.y, tempVertices[j].position.y);

			bBox.max.z = max(bBox.max.z, tempVertices[j].position.z);
			bBox.min.z = min(bBox.min.z, tempVertices[j].position.z);
		}

		for (int j = 0; j < lImporter.types[index]->transforms.size(); j++)
		{
			newMesh.transforms.push_back(lImporter.types[index]->transforms[j]);
			newMesh.bBox.push_back(bBox);
		}

		newMesh.vCount = tempVertices.size();

		std::string file_Path;
		if (aImporter.meshes[i]->textureMap.length() > 0)
		{
			file_Path = "Resources/Textures/" + aImporter.meshes[i]->textureMap;
			texture.Initialize(DContext, file_Path);
			newMesh.textureMap = texture.GetTexture();
		}
		else
			newMesh.textureMap = NULL;
		if (aImporter.meshes[i]->normalMap.length() > 0)
		{
			file_Path = "Resources/Textures/" + aImporter.meshes[i]->normalMap;
			texture.Initialize(DContext, file_Path);
			newMesh.normalMap = texture.GetTexture();
		}
		else
			newMesh.normalMap = NULL;

		newMesh.animationSets.resize(1);		// detta ska ändras så man kan ha fler animations sets

		for (int j = 0; j < aImporter.meshes[i]->keyFrames.size(); j++)
		{
			KeyFramesAni tempKeyFrame;
			for (int x = 0; x < aImporter.meshes[i]->clusters.size(); x++)
			{
				D3DXMATRIX tempMatrix;
				D3DXMATRIX bindPoseInverse = aImporter.meshes[i]->clusters[x].bindPoseTransform;
				D3DXMATRIX geometricTransform = aImporter.meshes[i]->clusters[x].geometricTransform;
				D3DXMATRIX boneT = aImporter.meshes[i]->keyFrames[j].boneTransforms[x];

				float bindPoseDet = D3DXMatrixDeterminant(&bindPoseInverse);

				D3DXMatrixInverse(&bindPoseInverse, &bindPoseDet, &bindPoseInverse);
				D3DXMatrixMultiply(&tempMatrix, &bindPoseInverse, &geometricTransform);
				D3DXMatrixMultiply(&tempMatrix, &tempMatrix, &boneT);
				
				tempKeyFrame.boneTransforms.push_back(tempMatrix);
			}
			newMesh.animationSets[0].keyFrames.push_back(tempKeyFrame);
		}

		D3D11_BUFFER_DESC vBD;
		memset(&vBD, 0, sizeof(vBD));
		vBD.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vBD.CPUAccessFlags = 0;
		vBD.Usage = D3D11_USAGE_DEFAULT;
		vBD.MiscFlags = 0;
		vBD.ByteWidth = sizeof(VertexTypeAni)* tempVertices.size();
		D3D11_SUBRESOURCE_DATA pData;
		pData.pSysMem = &tempVertices[0];
		hr = (DContext->CreateBuffer(&vBD, &pData, &newMesh.m_vertexBuffer));

		if (tempSubString == "p_")
			player = newMesh;
		else if (tempSubString == "e_")
			enemys.push_back(newMesh);
	}
}