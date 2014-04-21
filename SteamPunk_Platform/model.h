////////////////////////////////////////////////////////////////////////////////
// Filename: model.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODEL_H_
#define _MODEL_H_

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <xnamath.h>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "texture.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class ModelClass
{

private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};

	struct FaceType
	{
		int vIndex1, vIndex2, vIndex3;
		int tIndex1, tIndex2, tIndex3;
		int nIndex1, nIndex2, nIndex3;
	};
	
public:

	struct BoundingBox
	{
		D3DXVECTOR3 min;
		D3DXVECTOR3 max;
	};
	struct Material
	{
		D3DXVECTOR4 difColor;
		D3DXVECTOR4 ambColor;
		int texArrayIndex;
		bool hasTexture;
		std::wstring name;
	};
	struct SubsetType
	{
		TextureClass* m_Texture;

		ID3D11Buffer* vertBuff;
		ID3D11Buffer* indexBuff;

		std::vector<unsigned long> indices;
		std::vector<VertexType> vertices;
		Material mat;
	};
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	virtual bool Initialize(ID3D11Device*, WCHAR*);
	virtual void Shutdown();
	virtual void Apply(ID3D11DeviceContext*, int);

	void GetWorldMatrix(D3DXMATRIX&);
	int GetIndexCount();

	Material* GetMaterial(int i)
	{
		return &subset[i].mat;
	};
	int GetSubsetCount()
	{
		return subset.size();
	}
	bool HasTexture(int i)
	{
		return subset[i].mat.hasTexture;
	}
	ID3D11ShaderResourceView* GetSubsetTexture(int i)
	{
		return subset[i].m_Texture->GetTexture();
	}
	int IndexCount(int i)
	{
		return subset[i].indices.size();
	}

	ID3D11ShaderResourceView* GetTexture();

	bool LoadDataStructures(std::wstring, std::vector<std::wstring>&);

	BoundingBox bBox;

private:
	bool InitializeBuffers(ID3D11Device*, int);
	void ShutdownBuffers();

	bool LoadTexture(ID3D11Device*, const WCHAR*, int i);
	void ReleaseTexture();


	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	D3DXMATRIX m_worldMatrix;

	//TextureClass* m_Texture;
	std::vector<TextureClass*> m_Texture;
	std::vector<VertexType> vertices;
	std::vector<Material> material;
	std::vector<SubsetType> subset;
	std::vector<int> subsetIndexStart;
	std::vector<int> subsetMaterialArray;
	std::vector<unsigned long> indices;
	int subsetCount;
};

#endif