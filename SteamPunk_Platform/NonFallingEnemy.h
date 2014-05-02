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
	NonFallingEnemy(ID3D11Device * device, WCHAR * filename, D3DXMATRIX position) :
		Enemy(device, position){}
	~NonFallingEnemy();

	bool Update(float, std::vector<BoundingBox>&);
	void FlipGravity();
	void FlipGravityS();

};
#endif