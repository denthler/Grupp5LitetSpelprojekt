#ifndef _RESOURCEMANAGER_H_
#define _RESOURCEMANAGER_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <d3d11.h>
#include <D3DX10math.h>

#include "LevelImporter.h"
#include "AssetImporter.h"

#include "model.h"


struct VertexTypeT
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 texture;	
	D3DXVECTOR3 tangent;
};

struct Mesh
{
	std::string type;
	ID3D11Buffer* m_vertexBuffer;
	std::vector<D3DXMATRIX> transforms;
	std::vector<ModelClass::BoundingBox> bBox;
	int vCount;
};

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	bool						LoadLevel(std::string level, ID3D11Device* DContext);
private:
	void						CreateMesh(ID3D11Device* DContext, int index);

public:
	std::vector<Mesh>			meshes;
private:
	LevelImporter				lImporter;
	AssetImporter				aImporter;
};

#endif