#include "Enemy.h"

Enemy::Enemy(ID3D11Device * device, WCHAR * filename, D3DXVECTOR3 position)
{
	//worldUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	ModelClass::Initialize(device, filename);
	ModelClass::position = position;
	moveScale = -0.1f;
}

Enemy::~Enemy()
{

}

bool Enemy::Update(std::vector<BoundingBox>& bb)
{
	static bool test = false;
	bool test2;
	D3DXVECTOR3 right;
	D3DXVec3Cross(&right, &worldAxis, &D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	if (OnGround)
		velocity = right * moveScale;
	
	test = OnGround;
	test2 = ModelClass::Update(bb);

	if (!(OnGround) && (test))
	{
		moveScale *= -1.0f;
		velocity = right * (moveScale);
		velocity += (worldAxis * 0.003f);
		
	}
	else if (test2)
	{
		moveScale *= -1.0f;
		//velocity += right * (moveScale);
		return true;
	}

	return false;
}