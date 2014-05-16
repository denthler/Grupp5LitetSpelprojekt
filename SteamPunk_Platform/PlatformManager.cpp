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
	endLevel = false;
	std::string tempString;

	objects.clear();
	buffers.clear();
	textureMap.clear();
	normalMap.clear();
	gearsFound = 0;
	gearsTotal = 0;

	for (int i = 0; i < meshes.size(); i++)
	{
		tempString = meshes[i].type;
		std::string tempSubString = tempString.substr(0, 2);

		buffers.push_back(meshes[i].m_vertexBuffer);
		textureMap.push_back(meshes[i].textureMap);
		normalMap.push_back(meshes[i].normalMap);

		//BBox tempBox = meshes[i].BoundingBox;

		meshStruct newMesh;
		newMesh.vCount = meshes[i].vCount;

		if (tempSubString == "bg")
		{
			//if (tempSubString.c_str() == "bg")
			//{
			for (int j = 0; j < meshes[i].transforms.size(); j++)
			{
				GameObject* newObject = new GameObject(meshes[i].transforms[j], meshes[i].bBox[j], true, GameObject::ObjectType::Background);
				//newObject.world = meshes[i].transforms[j];
				//newObject.bBox = meshes[i].bBox[j];
				newMesh.bufferIndices.push_back(i);
				newMesh.objectData.push_back(newObject);
			}
		}
		else if (tempSubString == "pf")
		{
			for (int j = 0; j < meshes[i].transforms.size(); j++)
			{
				Platform* newPlatform = new Platform(meshes[i].transforms[j], meshes[i].bBox[j], false, GameObject::ObjectType::Platform);
				//newPlatform.position = meshes[i].transforms[j];
				//newPlatform.BoundingBox = meshes[i].BoundingBox;
				//tempObjects.push_back(newPlatform); 
				newMesh.bufferIndices.push_back(i);
				newMesh.objectData.push_back(newPlatform);
			}
		}
		else if (tempSubString == "go")
		{
			tempSubString = meshes[i].type;
			tempSubString = tempSubString.substr(0, 4);

			if (tempSubString == "go_c")
				for (int j = 0; j < meshes[i].transforms.size(); j++)
				{
					Gear* newGear = new Gear(meshes[i].transforms[j], meshes[i].bBox[j], false, GameObject::ObjectType::Gear);
					//newPlatform.position = meshes[i].transforms[j];
					//newPlatform.BoundingBox = meshes[i].BoundingBox;
					//tempObjects.push_back(newPlatform); 
					newMesh.bufferIndices.push_back(i);
					newMesh.objectData.push_back(newGear);
					gearsTotal++;
				}
			else if (tempSubString == "go_d")
				for (int j = 0; j < meshes[i].transforms.size(); j++)
				{
					Door* newDoor = new Door(meshes[i].transforms[j], meshes[i].bBox[j], false, GameObject::ObjectType::Door);
					newMesh.bufferIndices.push_back(i);
					newMesh.objectData.push_back(newDoor);
				}
		}
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
			switch (objects[i].objectData[j]->GetType())
			{
				case GameObject::ObjectType::Background :
				{

					break;
				}
				case GameObject::ObjectType::Platform :
				{
					bb.push_back(objects[i].objectData[j]->getBoundingBox());
					break;
				}
				case GameObject::ObjectType::Gear :
				{
					float length = D3DXVec3Length(&(playerPosition - objects[i].objectData[j]->GetPosition()));
					if (length < 1.0f)
					{
						objects[i].objectData.erase(objects[i].objectData.begin() + j);
						gearsFound++;
					}
					else
						objects[i].objectData[j]->Update();

					break;
				}
				case GameObject::ObjectType::Door :
				{
					float length = D3DXVec3Length(&(playerPosition - objects[i].objectData[j]->GetPosition()));
					if (length < 1.0f)
					{
						endLevel = true;
					}

					break;
				}
			}
			//if (!objects[i].objectData[j]->IsBackground())
				//bb.push_back(objects[i].objectData[j]->getBoundingBox());
		}
	}
	
}

void PlatformManager::Draw(ID3D11DeviceContext* deviceContext, Render* render, D3DXMATRIX viewMatrix, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* normal, ModelClass::Material mat)
{
	unsigned int stride;
	unsigned int offset;
	std::vector<D3DMATRIX> temp;

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	stride = sizeof(VertexTypeT);
	offset = 0;
	for (int i = 0; i < objects.size(); i++)
	{
		for (int j = 0; j < objects[i].objectData.size(); j++)
		{
			
			if (render->InsideFrustum(objects[i].objectData[j]->getBoundingBox().min, objects[i].objectData[j]->getBoundingBox().max))
			{
				deviceContext->IASetVertexBuffers(0, 1, &buffers[objects[i].bufferIndices[j]], &stride, &offset);

				if (textureMap[objects[i].bufferIndices[j]])
				{
					texture = textureMap[objects[i].bufferIndices[j]];
					mat.hasTexture = true;
				}
				else
				{
					texture = 0;
					mat.hasTexture = false;
				}

				if (normalMap[objects[i].bufferIndices[j]])
				{
					normal = normalMap[objects[i].bufferIndices[j]];
					mat.hasNormal = true;
				}
				else
				{
					normal = 0;
					mat.hasNormal = false;
				}

				bool result = render->UpdateRender(deviceContext, objects[i].objectData[j]->getWorld(), viewMatrix, texture, normal, mat, temp);
				render->Draw(deviceContext, objects[i].vCount, 0);
			}
		}
	}	
}

void PlatformManager::DrawShadow(ID3D11DeviceContext* deviceContext, Render* render)
{
	unsigned int stride;
	unsigned int offset;
	std::vector<D3DMATRIX> temp;

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	stride = sizeof(VertexTypeT);
	offset = 0;
	for (int i = 0; i < objects.size(); i++)
	{
		for (int j = 0; j < objects[i].objectData.size(); j++)
		{			
			if (render->InsideFrustum(objects[i].objectData[j]->getBoundingBox().min, objects[i].objectData[j]->getBoundingBox().max))
			{
				deviceContext->IASetVertexBuffers(0, 1, &buffers[objects[i].bufferIndices[j]], &stride, &offset);

				bool result = render->UpdateRenderShadow(deviceContext, objects[i].objectData[j]->getWorld(), temp);
				render->Draw(deviceContext, objects[i].vCount, 2);
			}
		}
	}	
}

int PlatformManager::GetGearCount()
{
	int count = 0;
	for (int i = 0; i < objects.size(); i++)
	{
		for (int j = 0; j < objects[i].objectData.size(); j++)
		{
			if (objects[i].objectData[j]->GetType() == GameObject::ObjectType::Gear)
			{
				count++;
			}
		}
	}
	return count;
}
