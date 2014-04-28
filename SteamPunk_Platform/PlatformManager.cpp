#include "PlatformManager.h"

PlatformManager::PlatformManager()
{

}

PlatformManager::~PlatformManager()
{

}

PlatformManager::PlatformManager(const PlatformManager& other)
{

}

void PlatformManager::CreateLevel(std::vector<Mesh>& meshes)
{
	std::string tempString;

	for (int i = 0; i < meshes.size(); i++)
	{
		tempString = meshes[i].type;
		std::string tempSubString = tempString.substr(0, 2);

		buffers.push_back(meshes[i].m_vertexBuffer);

		//BBox tempBox = meshes[i].BoundingBox;

		meshStruct newMesh;
		newMesh.vCount = meshes[i].vCount;

		//if (tempSubString.c_str() == "bg")
		//{
			for (int j = 0; j < meshes[i].transforms.size(); j++)
			{
				GameObject newObject;
				newObject.world = meshes[i].transforms[j];
				newObject.bBox = meshes[i].bBox[j];
				newMesh.bufferIndices.push_back(i);
				newMesh.objectData.push_back(newObject);
			}
		/*}
		else
		{
			for (int j = 0; j < meshes[i].transforms.size(); j++)
			{
				Platform newPlatform;
				newPlatform.position = meshes[i].transforms[j];
				newPlatform.BoundingBox = meshes[i].BoundingBox;
				tempObjects.push_back(newPlatform); 
			}
		}*/

		objects.push_back(newMesh);
	}
}

void PlatformManager::Update(D3DXVECTOR3 playerPosition, std::vector<ModelClass::BoundingBox>& bb)
{

	//std::vector<BoundingBox> bb;
	for (int i = 0; i < objects.size(); i++)
	{
		for (int j = 0; j < objects[i].objectData.size(); j++)
		{
			bb.push_back(objects[i].objectData[j].bBox);
		}
	}
	
}

void PlatformManager::Draw(ID3D11DeviceContext* deviceContext, Render* render, D3DXMATRIX viewMatrix, ID3D11ShaderResourceView* texture, PointLightClass* lightStruct, ModelClass::Material* mat)
{
	unsigned int stride;
	unsigned int offset;

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	stride = sizeof(VertexTypeT);
	offset = 0;
	for (int i = 0; i < objects.size(); i++)
	{
		for (int j = 0; j < objects[i].objectData.size(); j++)
		{
			deviceContext->IASetVertexBuffers(0, 1, &buffers[objects[i].bufferIndices[j]], &stride, &offset);

			bool result = render->UpdateRender(deviceContext, objects[i].objectData[j].world, viewMatrix, texture, lightStruct, mat);
			render->Draw(deviceContext, objects[i].vCount);
		}
	}	
}


