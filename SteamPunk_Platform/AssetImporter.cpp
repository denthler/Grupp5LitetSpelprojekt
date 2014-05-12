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

	currentMesh = new MeshData;
	meshes.push_back(currentMesh);
	while (!inFile.eof())
	{
		inFile >> str;

		if (str == "mesh")
		{
			std::string temp;
			getline(inFile, temp);
		}
		else if (str == "v")
		{
			D3DXVECTOR3 temp;
			inFile >> temp.x >> temp.y >> temp.z;
			temp.z *= -1.0f;
			currentMesh->vertices.push_back(temp);
		}
		else if (str == "vn")
		{
			D3DXVECTOR3 temp;
			inFile >> temp.x >> temp.y >> temp.z;
			temp.z *= -1.0f;
			currentMesh->normals.push_back(temp);
		}
		else if (str == "uv")
		{
			D3DXVECTOR2 temp;
			inFile >> temp.x >> temp.y;
			temp.y = 1.0f - temp.y;
			currentMesh->uv.push_back(temp);
		}
		else if (str == "vt")
		{
			D3DXVECTOR3 temp;
			inFile >> temp.x >> temp.y >> temp.z;
			temp.z *= -1.0f;
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
		else if (str == "mattexturemap")
		{
			inFile >> currentMesh->textureMap;
		}
		else if (str == "matnormalmap")
		{
			inFile >> currentMesh->normalMap;
		}
		else if (str == "cluster")
		{
			skinCluster newCluster;

			inFile >> str;
			while (str != "clusterweight")
			{
				newCluster.index.push_back(atoi(str.c_str()));
				inFile >> str;
			}

			inFile >> str;
			while (str != "clusterbindpose")
			{
				newCluster.weight.push_back(atof(str.c_str()));
				inFile >> str;
			}

			for (int y = 0; y < 4; y++)
				for (int x = 0; x < 4; x++)
					inFile >> newCluster.bindPoseTransform.m[y][x];

			inFile >> str;
			for (int y = 0; y < 4; y++)
				for (int x = 0; x < 4; x++)
					inFile >> newCluster.geometricTransform.m[y][x];

			currentMesh->clusters.push_back(newCluster);
		}
		else if (str == "animationSet")
		{
			currentAnimSet = new AnimationSets;
			inFile >> currentAnimSet->name;
			currentMesh->animSets.push_back(currentAnimSet);
		}
		else if (str == "skey")
		{
			KeyFrames newKeyFrame;

			inFile >> newKeyFrame.time;

			inFile >> str;
			for (int i = 0; i < currentMesh->clusters.size(); i++)
			{
				D3DMATRIX temp;
				for (int y = 0; y < 4; y++)
					for (int x = 0; x < 4; x++)
						inFile >> temp.m[y][x];
				newKeyFrame.boneTransforms.push_back(temp);
			}

			currentAnimSet->keyFrames.push_back(newKeyFrame);
		}
	}

	inFile.close();
	return true;
}