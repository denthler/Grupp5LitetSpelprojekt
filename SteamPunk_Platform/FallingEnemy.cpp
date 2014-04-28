#include "FallingEnemy.h"

FallingEnemy::~FallingEnemy()
{

}

void FallingEnemy::FlipGravity()
{

}

void FallingEnemy::FlipGravityS()
{

}

bool FallingEnemy::Update(float gameTime, std::vector<BoundingBox>& bb)
{

	Enemy::Update(gameTime, bb);
	return true;
}