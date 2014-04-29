#include "FallingEnemy.h"

FallingEnemy::~FallingEnemy()
{

}

void FallingEnemy::FlipGravity()
{
	D3DXVec3Cross(&worldAxis, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), &worldAxis);
}

void FallingEnemy::FlipGravityS()
{
	D3DXVec3Cross(&worldAxis, &worldAxis, &D3DXVECTOR3(0.0f, 0.0f, 1.0f));
}

bool FallingEnemy::Update(float gameTime, std::vector<BoundingBox>& bb)
{

	Enemy::Update(gameTime, bb);
	return true;
}