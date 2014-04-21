#include "Player.h"


Player::Player()
{
	velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	dead = false;
	left = false; right = false; up = false; down = false; jump = false;
	OnGround = false;
	moveScale = 0.1f;
	gravity = -0.001f;
	worldAxis = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
}

Player::~Player()
{
	
}

void Player::Apply(ID3D11DeviceContext* device, int count)
{
	ModelClass::Apply(device, count);
}

void Player::Shutdown()
{
	ModelClass::Shutdown();
}

bool Player::Initialize(ID3D11Device* device, WCHAR* filename, D3DXVECTOR3 startPos)
{
	position = startPos;
	return ModelClass::Initialize(device, filename);
}

void Player::Update(float gameTime, std::vector<BoundingBox>& bb)
{
	if(!dead)
	{
		D3DXVECTOR3 temp;
		
		D3DXVec3Cross(&temp, &worldAxis, &D3DXVECTOR3(0.0f, 0.0f, 1.0f));
		//temp *= 2.0f;
		//D3DXVec3Normalize(&temp,&temp);

		velocity.x -= (abs(temp.x) * velocity.x);
		velocity.y -= (abs(temp.y) * velocity.y);
		velocity.z -= (abs(temp.z) * velocity.z);

		if(left)
		{	
			
			D3DXVec3Cross(&temp, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), &worldAxis);
			velocity += -moveScale * temp;
			left = false;
		}
		if(right)
		{
			
			D3DXVec3Cross(&temp, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), &worldAxis);
			velocity += moveScale * temp;
			right = false;
		}
		if(jump)
		{
 			if(OnGround)
			{
				Jump();
			}
			jump = false;
		}
	}
	velocity += gravity * worldAxis;

	
	D3DXVECTOR3 moveAmount = velocity; //* gameTime;
	VerticalCollisionTest(moveAmount, bb);
	if(((abs(worldAxis.x) * moveAmount.x) != 0.0f)||
		((abs(worldAxis.y) * moveAmount.y) != 0.0f)||
		((abs(worldAxis.z) * moveAmount.z) != 0.0f))
	{
		OnGround = false;
	}
	else
	{
		OnGround = true;
	}
	D3DXVECTOR3 newPosition = position + moveAmount;
	position = newPosition;
}

void Player::Jump()
{
	velocity.x += (0.08f * worldAxis.x);
	velocity.y += (0.08f * worldAxis.y);
	velocity.z += (0.08f * worldAxis.z);
}

void Player::VerticalCollisionTest(D3DXVECTOR3& amount, std::vector<BoundingBox>& bb)
{
	if ((amount.x * worldAxis.x) == 0.0f && (amount.y * worldAxis.y) == 0.0f &&
		(amount.z * worldAxis.z) == 0.0f)
		return;

	D3DXVECTOR3 offset = position + amount;
	D3DXVECTOR3 tempMax = bBox.max;
	D3DXVECTOR3 tempMin = bBox.min;
	tempMax = offset + tempMax;
	tempMin = offset + tempMin;
	for(int j = 0; j < bb.size(); j++)
	{
		if(tempMax.x > bb[j].min.x && 
			tempMin.x < bb[j].max.x &&
			tempMax.y > bb[j].min.y &&
			tempMin.y < bb[j].max.y &&
			tempMax.z > bb[j].min.z &&
			tempMin.z < bb[j].max.z)
		{
			if((abs(worldAxis.x) * amount.x) != 0.0f)
			amount.x = 0.0f;
			if((abs(worldAxis.y) * amount.y) != 0.0f)
			amount.y = 0.0f;
			if((abs(worldAxis.z) * amount.z) != 0.0f)
			amount.z = 0.0f;
		}
	}
}