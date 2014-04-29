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
	FallingEnemy(ID3D11Device * device, D3DXVECTOR3 position) :
		Enemy(device, position){}
	~FallingEnemy();

	bool Update(float, std::vector<BoundingBox>&);
	void FlipGravity();
	void FlipGravityS();

};
#endif