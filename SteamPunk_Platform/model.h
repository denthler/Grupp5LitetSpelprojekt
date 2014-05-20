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

struct KeyFramesAni
{
	std::vector<D3DMATRIX> boneTransforms;
};

struct AnimationStack
{
	std::string name;
	std::vector<KeyFramesAni> keyFrames;
};

struct VertexTypeT
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 texture;
	D3DXVECTOR3 tangent;
};

struct VertexTypeAni
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 texture;
	D3DXVECTOR3 tangent;
	float Weight[3];
	BYTE BoneIndex[3];
};

class ModelClass
{
public:
	struct BoundingBox
	{
		D3DXVECTOR3 min;
		D3DXVECTOR3 max;
		std::string type;
	};

	struct Material
	{
		D3DXVECTOR4 difColor;
		D3DXVECTOR4 ambColor;
		int texArrayIndex;
		bool hasTexture;
		bool hasNormal;
		std::wstring name;
	};

	ModelClass();
	ModelClass(D3DXMATRIX, BoundingBox, std::vector<AnimationStack>, ID3D11Buffer*, int, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*);
	ModelClass(const ModelClass&);
	~ModelClass();

	virtual bool Initialize(ID3D11Device*);
	virtual void Shutdown();
	virtual void Apply(ID3D11DeviceContext*);

	virtual D3DXMATRIX GetWorldMatrix();
	int GetIndexCount();

	D3DXMATRIX GetWorldMatrixMechanic()
	{
		return m_worldMatrix;
	}
	Material GetMaterial()
	{
		return mat;
	};
	ID3D11Buffer* GetVertBuffer()
	{
		return m_vertexBuffer;
	};

	std::vector<D3DMATRIX> GetCurrentFrame(){ return currentFrame; };
	ID3D11ShaderResourceView* GetTextureMap (){ return textureMap; };
	ID3D11ShaderResourceView* GetNormalMap(){ return normalMap; };
	int GetVertexCount(){ return vCount; };

	BoundingBox GetBoundingBox()
	{
		return bBox;
	}

	D3DXVECTOR3 GetPosition()
	{
		//D3DXVECTOR3 p = D3DXVECTOR3(m_worldMatrix._41, m_worldMatrix._42, m_worldMatrix._43);
		return position;
	}

	virtual bool Update(float, std::vector<ModelClass::BoundingBox>&);
	void UpdateMechanic(float);
	virtual bool FlipGravity(std::vector<BoundingBox>& bb);// { D3DXVec3Cross(&worldAxis, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), &worldAxis); };
	virtual bool FlipGravityS(std::vector<BoundingBox>& bb);// { D3DXVec3Cross(&worldAxis, &worldAxis, &D3DXVECTOR3(0.0f, 0.0f, 1.0f)); };
	BoundingBox bBox;
	BoundingBox bBoxOriginal;

	bool Rotated;
	D3DXVECTOR3 worldAxis;
protected:
	D3DXVECTOR3 position;
	D3DXVECTOR3 velocity;
	bool FallDamage;
	float gravity;
	float moveScale;
	bool OnGround;
	bool dead;

	ID3D11ShaderResourceView* textureMap;
	ID3D11ShaderResourceView* normalMap;
	ID3D11Buffer* m_vertexBuffer;
	int vCount;
	std::vector<AnimationStack> animationStack;
	std::vector<D3DMATRIX> currentFrame;
	float animationTime;
	int currentAnimStack;
	Material mat;

	D3DXMATRIX m_worldMatrix;
private:
	//bool LoadTexture(ID3D11Device*, const WCHAR*, int i);

	bool VerticalCollisionTest(D3DXVECTOR3&, std::vector<ModelClass::BoundingBox>&);
	bool HorizontalCollisionTest(D3DXVECTOR3&, std::vector<ModelClass::BoundingBox>&, float);
};

#endif