//
//PlatformManager.h
//
#ifndef _PlatformManager_H_
#define _PlatformManager_H_

#include <vector>
#include <D3D11.h>
#include <D3DX10math.h>
#include "Render.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Platform.h"
#include "CogWheel.h"
#include "Door.h"

/*
struct GameObject
{
	D3DXMATRIX world;
	ModelClass::BoundingBox bBox;
};
*/
struct meshStruct
{
	std::vector<GameObject*> objectData;
	std::vector<int> bufferIndices;
	int vCount;
};

class PlatformManager
{
public:
	PlatformManager();
	PlatformManager(const PlatformManager&);
	~PlatformManager();

	void CreateLevel(std::vector<Mesh>&);
	void Draw(ID3D11DeviceContext*, Render*, D3DXMATRIX, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ModelClass::Material);
	void DrawShadow(ID3D11DeviceContext*, Render*);
	void Update(D3DXVECTOR3 playerPosition, std::vector<ModelClass::BoundingBox>& bb, float gT, bool);
	//Get Boundingboxes
	int gearsFound;
	int gearsTotal;
	int GetGearCount();

	bool endLevel;
	bool endGame;

	std::vector<D3DXMATRIX> GetWorldMatrices()
	{
		std::vector<D3DXMATRIX> matrices;
		for (int i = 0; i < objects.size(); i++)
		{
			for (int j = 0; j < objects[i].objectData.size(); j++)
			{
				matrices.push_back(objects[i].objectData[j]->getWorld());
			}			
		}
		return matrices;
	}

private:
	void OrganizeLevel(std::vector<GameObject>&);
	std::vector<meshStruct> objects;
	std::vector<ID3D11Buffer*> buffers;
	std::vector<ID3D11ShaderResourceView*> textureMap;
	std::vector<ID3D11ShaderResourceView*> normalMap;

	ModelClass* mechanic;
	bool mechanicLoaded;
};
#endif
