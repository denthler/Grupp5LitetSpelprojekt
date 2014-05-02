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
#include "Texture.h"

/*struct KeyFramesAni
{
	std::vector<D3DMATRIX> boneTransforms;
};

struct AnimationStack
{
	std::string name;
	std::vector<KeyFramesAni> keyFrames;
};*/

struct Mesh
{
	std::string type;
	ID3D11Buffer* m_vertexBuffer;
	std::vector<D3DXMATRIX> transforms;
	std::vector<ModelClass::BoundingBox> bBox;

	ID3D11ShaderResourceView* textureMap;
	ID3D11ShaderResourceView* normalMap;
	std::vector<AnimationStack> animationSets;
	int vCount;
};

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	bool						LoadLevel(std::string level, ID3D11Device* DContext);
private:
	void						CreateStaticMeshes(ID3D11Device* DContext, int index);
	void						CreateAnimatedMeshes(ID3D11Device* DContext, int index);

public:
	Mesh						player;
	std::vector<Mesh>			enemys;
	std::vector<Mesh>			meshes;
private:
	LevelImporter				lImporter;
	AssetImporter				aImporter;
	TextureClass				texture;
};

#endif