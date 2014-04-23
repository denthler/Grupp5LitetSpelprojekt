#include "Enemy.h"
#include "AABB.h"

Enemy::Enemy(ID3D11Device * device, WCHAR * filename, D3DXVECTOR3 position)
{
	worldUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	ModelClass::Initialize(device, filename);
	this->position = position;
}

Enemy::~Enemy()
{

}

void Enemy::Update(DWORD gameTime, std::vector<BoundingBox> & bb)
{
	// Gravity
	BoundingBox box;
	float gravity = -0.1f;
	D3DXVECTOR3 dy = worldUp * gravity;
	box = this->bBox;
	box.Translate(dy);
	if (WontCollideWithOtherObjects(box, bb))
	{	
		position += dy;
		this->bBox.Translate(dy);
	}
	
	// Movement to the right
	float moveSpeed = -0.1f;
	D3DXVECTOR3 right;
	D3DXVec3Cross(&right, &worldUp, &D3DXVECTOR3(0.0f, 0.0f, 1.0f));	
	D3DXVECTOR3 dx = right * moveSpeed;
	box = this->bBox;
	box.Translate(dx);
	//if (WontCollideWithOtherObjects(box, bb))
	//{
		position += dx;
		this->bBox.Translate(dx);
	//}
	/*
	BoundingBox a;
	a.min = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	a.max = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	BoundingBox b;
	b.min = D3DXVECTOR3(5.5f, 5.5f, 5.5f);
	b.max = D3DXVECTOR3(6.5f, 6.5f, 6.5f);
	bool result = a.Intersection(b);
	*/

}

bool Enemy::WontCollideWithOtherObjects(BoundingBox box, std::vector<BoundingBox> & bb)
{
	for (auto i = bb.begin(); i != bb.end(); ++i)
	{
		if (box.Intersection(*i))
		{
			return false;
		}
	}
	return true;
}