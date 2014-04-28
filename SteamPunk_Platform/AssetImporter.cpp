#include "AssetImporter.h"

AssetImporter::AssetImporter()
{
	file_Path = "Resources/Assets/";
}

AssetImporter::~AssetImporter()
{

}

bool AssetImporter::LoadAsset(std::string asset)
{
	meshes.clear();
	file_Path = "Resources/Assets/";

	std::ifstream inFile;
	asset += ".SPA";
	file_Path += asset;

	inFile.open(file_Path);
	if (!inFile.is_open())
	{
		std::wstring debug = L"Could not open: " + std::wstring(asset.begin(), asset.end());
		OutputDebugString(debug.c_str());
		return false;
	}

	std::string str;
	while (!inFile.eof())
	{
		inFile >> str;

		if (str == "mesh")
		{
			currentMesh = new MeshData;
			std::string temp;
			getline(inFile, temp);
			meshes.push_back(currentMesh);
		}
		else if (str == "v")
		{
			D3DXVECTOR3 temp;
			inFile >> temp.x >> temp.y >> temp.z;
			currentMesh->vertices.push_back(temp);
		}
		else if (str == "vn")
		{
			D3DXVECTOR3 temp;
			inFile >> temp.x >> temp.y >> temp.z;
			currentMesh->normals.push_back(temp);
		}
		else if (str == "uv")
		{
			D3DXVECTOR2 temp;
			inFile >> temp.x >> temp.y;
			currentMesh->uv.push_back(temp);
		}
		else if (str == "vt")
		{
			D3DXVECTOR3 temp;
			inFile >> temp.x >> temp.y >> temp.z;
			currentMesh->uvTangents.push_back(temp);
		}
		else if (str == "f")
		{
			FaceInfo newFace;
			std::string tri;
			std::string split;

			for (int t = 0; t < 3; ++t)
			{
				inFile >> tri;
				for (int i = 0; i < 4; ++i)
				{
					split = tri.substr(0, tri.find("/"));
					int cOff = (int)tri.find("/");
					if (cOff > 0)
						tri = tri.substr(cOff + 1);

					if (i == 0) { newFace.vertex.push_back(atoi(split.c_str())); }
					if (i == 1) { newFace.normal.push_back(atoi(split.c_str())); }
					if (i == 2) { newFace.uv.push_back(atoi(split.c_str())); }
					if (i == 3) { newFace.tangent.push_back(atoi(split.c_str())); }
				}
			}
			currentMesh->faceIndices.push_back(newFace);
		}
	}

	inFile.close();
	return true;
}