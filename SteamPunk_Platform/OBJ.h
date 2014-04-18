#pragma once

#include <vector>
#include <string>
#include <DirectXMath.h>

struct VertexData
{
	DirectX::XMFLOAT3 point;
	DirectX::XMFLOAT2 texCoord;
	DirectX::XMFLOAT3 normal;

	VertexData()
	{

	}

	VertexData(DirectX::XMFLOAT3 point, DirectX::XMFLOAT2 texCoord, DirectX::XMFLOAT3 normal)
	{
		this->point = point;
		this->texCoord = texCoord;
		this->normal = normal;
	}
};

struct ObjectMaterialData
{
	std::string Name;
	unsigned int Illum;
	float	Kd[3];
	float	Ka[3];
	float	Tf[3];
	std::string	map_Kd;
	std::string bump;
	std::string disp;
	std::string occlusion;
	float	Ni;

	ObjectMaterialData()
	{
		Name = "none";
		map_Kd = "none";
		bump = "none";
		disp = "none";
		occlusion = "none";
	}
};

struct Group
{
	std::string name;
	std::vector<VertexData> vertices;
	ObjectMaterialData materialData;
};

class Object
{
	std::vector<Group> groups;
	
	//GLuint bufferHandle[1], VAOHandle[1];
	
public:
	//~Object();

	void Init();
	//void Draw();
	void LoadMaterialData(std::string filename, std::vector<ObjectMaterialData> & data);
	void LoadFromObjFile(std::string dir, std::string filename);

	ObjectMaterialData GetGroupMaterialData(int groupIndex) { return groups[groupIndex].materialData; };
	int GetVertexCount(int groupIndex) { return groups[groupIndex].vertices.size(); };
	int GetVertexDataSize(int groupIndex) { return groups[groupIndex].vertices.size() * sizeof(VertexData); };
	const VertexData * GetVertexData(int groupIndex) { return groups[groupIndex].vertices.data(); };
};