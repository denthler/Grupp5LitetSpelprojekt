#include "FallingEnemy.h"

FallingEnemy::~FallingEnemy()
{

}

bool FallingEnemy::FlipGravity(std::vector<BoundingBox>& bb, D3DXVECTOR3 wAxis)
{
	return Enemy::FlipGravity(bb, wAxis);
}

bool FallingEnemy::FlipGravityS(std::vector<BoundingBox>& bb, D3DXVECTOR3 wAxis)
{
	return Enemy::FlipGravityS(bb, wAxis);
}

bool FallingEnemy::Update(float gameTime, std::vector<BoundingBox>& bb)
{
	Enemy::Update(gameTime, bb);
	return true;
}