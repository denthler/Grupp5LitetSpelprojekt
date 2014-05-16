//
//NonFallingEnemy.h
//
#ifndef _NONFALLINGENEMY_H_
#define _NONFALLINGENEMY_H_

#include "Enemy.h"

class NonFallingEnemy : public Enemy
{

public:
	//NonFallingEnemy();
	NonFallingEnemy(ID3D11Device * device, D3DXMATRIX position, ID3D11ShaderResourceView* tM, ID3D11ShaderResourceView* nM, std::vector<AnimationStack> aS, ID3D11Buffer* vB, int vC) :
		Enemy(device, position, tM, nM, aS, vB, vC){}
	~NonFallingEnemy();

	bool Update(float, std::vector<BoundingBox>&);
	bool FlipGravity(std::vector<BoundingBox>& bb, D3DXVECTOR3 wAxis);
	bool FlipGravityS(std::vector<BoundingBox>& bb, D3DXVECTOR3 wAxis);

};
#endif