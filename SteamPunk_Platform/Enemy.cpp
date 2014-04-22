#include "Enemy.h"

Enemy::Enemy(ID3D11Device * device, WCHAR * filename, D3DXVECTOR3 position)
{
	worldUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	ModelClass::Initialize(device, filename);
	this->position = position;
}

Enemy::~Enemy()
{

}

void Enemy::Update()
{
	/*
	float gravity = -0.1f;
	position += worldUp * gravity;
	*/
	
	// An enemy that always moves to the right, unaffected by gravity
	float moveSpeed = 0.1f;
	D3DXVECTOR3 right;
	D3DXVec3Cross(&right, &worldUp, &D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	position += right * moveSpeed;
}