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

struct GameObject
{
	D3DXMATRIX world;
	ModelClass::BoundingBox bBox;
};

struct meshStruct
{
	std::vector<GameObject> objectData;
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
<<<<<<< HEAD
	void Draw(ID3D11DeviceContext*, Render*, D3DXMATRIX, ID3D11ShaderResourceView*, PointLightClass*, ModelClass::Material*);
=======
	void Draw(ID3D11DeviceContext*, Render*, D3DXMATRIX, ID3D11ShaderResourceView*, PointLightClass*, ModelClass::Material);
	void Update(D3DXVECTOR3 playerPosition, std::vector<ModelClass::BoundingBox>& bb);
	//Get Boundingboxes
>>>>>>> origin/Updated-BBoxes
private:
	void OrganizeLevel(std::vector<GameObject>&);
	std::vector<meshStruct> objects;
	std::vector<ID3D11Buffer*> buffers;
};
#endif
