#include "NonFallingEnemy.h"

NonFallingEnemy::~NonFallingEnemy()
{

}

bool NonFallingEnemy::FlipGravity(std::vector<BoundingBox>& bb, D3DXVECTOR3 wAxis)
{
	return true;
}

bool NonFallingEnemy::FlipGravityS(std::vector<BoundingBox>& bb, D3DXVECTOR3 wAxis)
{
	return true;
}

bool NonFallingEnemy::Update(float gameTime, std::vector<BoundingBox>& bb)
{

	Enemy::Update(gameTime, bb);
	return true;
}