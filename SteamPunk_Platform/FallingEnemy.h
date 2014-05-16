//
//FallingEnemy.h
//
#ifndef _FALLINGENEMY_H_
#define _FALLINGENEMY_H_

#include "Enemy.h"

class FallingEnemy : public Enemy
{

public:
	//FallingEnemy();
	FallingEnemy(ID3D11Device * device, D3DXMATRIX p, ID3D11ShaderResourceView* tM, ID3D11ShaderResourceView* nM, std::vector<AnimationStack> aS, ID3D11Buffer* vB, int vC) :
		Enemy(device, p, tM, nM, aS, vB, vC){}
	~FallingEnemy();

	bool Update(float, std::vector<BoundingBox>&);
	bool FlipGravity(std::vector<BoundingBox>& bb, D3DXVECTOR3 wAxis);
	bool FlipGravityS(std::vector<BoundingBox>& bb, D3DXVECTOR3 wAxis);

};
#endif