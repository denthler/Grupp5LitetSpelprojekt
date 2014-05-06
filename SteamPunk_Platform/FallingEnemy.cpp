#include "FallingEnemy.h"

FallingEnemy::~FallingEnemy()
{

}

void FallingEnemy::FlipGravity()
{
	D3DXVec3Cross(&worldAxis, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), &worldAxis);
	D3DXVECTOR3 temp;
	temp = (bBox.max - bBox.min) / 2.0f;
	D3DXVECTOR3 temp2;
	D3DXVec3Cross(&temp2, &worldAxis, &D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	position.x += temp.x * temp2.x;
	position.y += temp.y * temp2.y;
	position.z += temp.z * temp2.z;
	
}

void FallingEnemy::FlipGravityS()
{
	D3DXVec3Cross(&worldAxis, &worldAxis, &D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	D3DXVECTOR3 temp;
	temp = (bBox.max - bBox.min) / 2.0f;
	D3DXVECTOR3 temp2;
	D3DXVec3Cross(&temp2, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), &worldAxis);
	position.x += temp.x * temp2.x;
	position.y += temp.y * temp2.y;
	position.z += temp.z * temp2.z;
}

bool FallingEnemy::Update(float gameTime, std::vector<BoundingBox>& bb)
{
	Enemy::Update(gameTime, bb);
	return true;
}