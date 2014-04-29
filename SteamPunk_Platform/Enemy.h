//
//Enemy.h
//
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "model.h"

class Enemy : public ModelClass
{	
	//D3DXVECTOR3 position;

	Enemy(); // Hide the default constructor, it's not needed.
public:
	Enemy(ID3D11Device * device, D3DXVECTOR3 position);	
	~Enemy();

	virtual bool Update(float, std::vector<BoundingBox>&);
	D3DXMATRIX GetWorldMatrix()
	{
		D3DXMATRIX worldMatrix;
		D3DXMatrixTranslation(&worldMatrix, position.x, position.y, position.z);
		return worldMatrix;
	};

	//D3DXVECTOR3 worldUp;
	//void FlipWorldRight(){ D3DXVec3Cross(&worldAxis, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), &worldAxis); }
	//void FlipWorldLeft(){ D3DXVec3Cross(&worldAxis, &worldAxis, &D3DXVECTOR3(0.0f, 0.0f, 1.0f)); }
};

#endif