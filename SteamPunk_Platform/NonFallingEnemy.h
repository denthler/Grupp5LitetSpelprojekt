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
	NonFallingEnemy(ID3D11Device * device, D3DXVECTOR3 position, ID3D11ShaderResourceView* tM, ID3D11ShaderResourceView* nM, std::vector<AnimationStack> aS, ID3D11Buffer* vB, int vC) :
		Enemy(device, position, tM, nM, aS, vB, vC){}
	~NonFallingEnemy();

	bool Update(float, std::vector<BoundingBox>&);
	void FlipGravity();
	void FlipGravityS();

};
#endif