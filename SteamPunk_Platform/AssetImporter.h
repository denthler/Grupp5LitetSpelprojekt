#ifndef _ASSETIMPORTER_H_
#define _ASSETIMPORTER_H_

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include <D3DX10math.h>

struct FaceInfo
{
	std::vector<int> vertex;
	std::vector<int> normal;
	std::vector<int> uv;
	std::vector<int> tangent;
};

struct MeshData
{
	std::vector<FaceInfo> faceIndices;
	std::vector<D3DXVECTOR3> vertices;
	std::vector<D3DXVECTOR2> uv;

	std::vector<D3DXVECTOR3> normals;
	std::vector<D3DXVECTOR3> uvTangents;
};

class AssetImporter
{
public:
	AssetImporter();
	~AssetImporter();

	bool						LoadAsset(std::string asset);
	std::vector<MeshData*>		meshes;
private:
	std::string					file_Path;
	MeshData*					currentMesh;
};

#endif