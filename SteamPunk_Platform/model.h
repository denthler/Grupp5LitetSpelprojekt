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

struct VertexTypeT
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 texture;
	D3DXVECTOR3 tangent;
};

class ModelClass
{
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

	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	virtual bool Initialize(ID3D11Device*);
	virtual void Shutdown();
	virtual void Apply(ID3D11DeviceContext*, ID3D11Buffer*);

	void GetWorldMatrix(D3DXMATRIX&);
	int GetIndexCount();

	Material GetMaterial()
	{
		return mat;
	};

	ID3D11ShaderResourceView* GetTexture();

	virtual bool Update(float, std::vector<ModelClass::BoundingBox>&);
	virtual void FlipGravity(){ D3DXVec3Cross(&worldAxis, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), &worldAxis); };
	virtual void FlipGravityS(){ D3DXVec3Cross(&worldAxis, &worldAxis, &D3DXVECTOR3(0.0f, 0.0f, 1.0f)); };
	BoundingBox bBox;
protected:
	D3DXVECTOR3 position;
	D3DXVECTOR3 velocity;
	float gravity;
	D3DXVECTOR3 worldAxis;
	float moveScale;
	bool OnGround;

private:
	//bool LoadTexture(ID3D11Device*, const WCHAR*, int i);

	bool VerticalCollisionTest(D3DXVECTOR3&, std::vector<ModelClass::BoundingBox>&);
	bool HorizontalCollisionTest(D3DXVECTOR3&, std::vector<ModelClass::BoundingBox>&, float);

	D3DXMATRIX m_worldMatrix;
	Material mat;
};

#endif