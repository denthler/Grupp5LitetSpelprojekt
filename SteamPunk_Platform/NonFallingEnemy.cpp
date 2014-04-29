#include "NonFallingEnemy.h"

NonFallingEnemy::~NonFallingEnemy()
{

}

void NonFallingEnemy::FlipGravity()
{

}

void NonFallingEnemy::FlipGravityS()
{

}

bool NonFallingEnemy::Update(float gameTime, std::vector<BoundingBox>& bb)
{

	Enemy::Update(gameTime, bb);
	return true;
}