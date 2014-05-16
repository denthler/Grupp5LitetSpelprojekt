//
//Enemy.h
//
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "model.h"
#include <string>

class Enemy : public ModelClass
{	
	//D3DXVECTOR3 position;

	Enemy(); // Hide the default constructor, it's not needed.
	D3DXVECTOR3 prevPos;// = position;
public:
	Enemy(ID3D11Device * device, D3DXMATRIX position, ID3D11ShaderResourceView* tM, ID3D11ShaderResourceView* nM, std::vector<AnimationStack> aS, ID3D11Buffer* vB, int vC);
	~Enemy();

	virtual bool Update(float, std::vector<BoundingBox>&);
	//D3DXMATRIX GetWorldMatrix()
	//{
		//D3DXMATRIX worldMatrix;
		//D3DXMatrixTranslation(&worldMatrix, position.x, position.y, position.z);
		//return m_worldMatrix;
	//};

	//D3DXVECTOR3 worldUp;
	virtual bool FlipGravity(std::vector<BoundingBox>& bb, D3DXVECTOR3);// { D3DXVec3Cross(&worldAxis, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), &worldAxis); }
	virtual bool FlipGravityS(std::vector<BoundingBox>& bb, D3DXVECTOR3);// { D3DXVec3Cross(&worldAxis, &worldAxis, &D3DXVECTOR3(0.0f, 0.0f, 1.0f)); }
};

#endif