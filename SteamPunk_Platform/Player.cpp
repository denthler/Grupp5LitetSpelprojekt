#include "Player.h"

Player::Player()
{
	velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	dead = false;
	left = false; right = false; up = false; down = false; jump = false;
	OnGround = false;
	moveScale = -0.2f;//-0.1f
	gravity = -0.01f;
	worldAxis = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
}

Player::~Player()
{
	
}

void Player::Shutdown()
{
	ModelClass::Shutdown();
}

bool Player::Initialize(ID3D11Device* device, D3DXVECTOR3 startPos)
{
	position = startPos;
	StartPos = startPos;
	return ModelClass::Initialize(device);
}

void Player::Kill()
{
	position = StartPos;
}

D3DXMATRIX Player::GetWorldMatrix()
{
	//Move this code to superclass later!!!!!
	D3DXMATRIX posMatrix;
	D3DXMatrixTranslation(&posMatrix, position.x, position.y, position.z);
	D3DXMATRIX rotMatrix, rotMatrix2;
	if (Rotated)
	{
		if (worldAxis.y > 0.0f)
		{
			D3DXMatrixRotationY(&rotMatrix, D3DX_PI);
		}
		else if (worldAxis.y < 0.0f)
		{
			D3DXMatrixRotationY(&rotMatrix, 0.0f);
			D3DXMatrixRotationX(&rotMatrix2, D3DX_PI);
			rotMatrix *= rotMatrix2;
		}
		else if (worldAxis.x < 0.0f)
		{
			D3DXMatrixRotationZ(&rotMatrix, D3DX_PI / 2);
			D3DXMatrixRotationX(&rotMatrix2, D3DX_PI);
			rotMatrix *= rotMatrix2;
		}
		else if (worldAxis.x > 0.0f)
		{
			D3DXMatrixRotationZ(&rotMatrix, (3 * D3DX_PI) / 2);
			D3DXMatrixRotationX(&rotMatrix2, D3DX_PI);
			rotMatrix *= rotMatrix2;
		}
	}
	else
	{
		if (worldAxis.y > 0.0f)
		{
			D3DXMatrixRotationY(&rotMatrix, 0.0f);
		}
		else if (worldAxis.y < 0.0f)
		{
			D3DXMatrixRotationY(&rotMatrix, D3DX_PI);
			D3DXMatrixRotationX(&rotMatrix2, D3DX_PI);
			rotMatrix *= rotMatrix2;
		}
		else if (worldAxis.x < 0.0f)
		{
			D3DXMatrixRotationZ(&rotMatrix, D3DX_PI / 2);
			D3DXMatrixRotationX(&rotMatrix2, 0.0f);
			rotMatrix *= rotMatrix2;
		}
		else if (worldAxis.x > 0.0f)
		{
			D3DXMatrixRotationZ(&rotMatrix, (3 * D3DX_PI) / 2);
			D3DXMatrixRotationX(&rotMatrix2, 0.0f);
			rotMatrix *= rotMatrix2;
		}
	}
	m_worldMatrix = rotMatrix;
	m_worldMatrix._41 = position.x;
	m_worldMatrix._42 = position.y;
	m_worldMatrix._43 = position.z;
	D3DXVECTOR4 tempVec;
	D3DXVec3Transform(&tempVec, &bBoxOriginal.max, &rotMatrix);
	bBox.max.x = tempVec.x;
	bBox.max.y = tempVec.y;
	bBox.max.z = tempVec.z;
	D3DXVec3Transform(&tempVec, &bBoxOriginal.min, &rotMatrix);
	bBox.min.x = tempVec.x;
	bBox.min.y = tempVec.y;
	bBox.min.z = tempVec.z;

	if ((bBox.min.x > bBox.max.x))
	{
		bBox.min.x = bBox.max.x;
		bBox.max.x = tempVec.x;
	}
	if ((bBox.min.y > bBox.max.y))
	{
		bBox.min.y = bBox.max.y;
		bBox.max.y = tempVec.y;
	}
	if ((bBox.min.z > bBox.max.z))
	{
		bBox.min.z = bBox.max.z;
		bBox.max.z = tempVec.z;
	}
	return m_worldMatrix;

}

bool Player::Update(float gameTime, std::vector<BoundingBox>& bb)
{
	D3DXVECTOR3 temp;
	gameTime *= 0.05f;
	D3DXVec3Cross(&temp, &worldAxis, &D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	if(!dead)
	{		
		velocity.x -= (abs(temp.x) * velocity.x);
		velocity.y -= (abs(temp.y) * velocity.y);
		velocity.z -= (abs(temp.z) * velocity.z);
		
		if(left)
		{	
			D3DXVec3Cross(&temp, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), &worldAxis);
			velocity -= moveScale * temp;
			left = false;
			Rotated = true;
		}
		if(right)
		{
			
			D3DXVec3Cross(&temp, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), &worldAxis);
			velocity += moveScale * temp;
			right = false;
			Rotated = false;;
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
	
	return (ModelClass::Update(gameTime, bb));
	
}

void Player::Jump()
{
	velocity += (0.4f * worldAxis);
	
}

/*
bool Player::HorizontalCollisionTest(D3DXVECTOR3& amount, std::vector<BoundingBox>& bb, float value)
{
	bool result = false;
	D3DXVECTOR3 temp;
	D3DXVec3Cross(&temp, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), &worldAxis);
	D3DXVECTOR3 offset;
	
	//offset.x = position.x + ((amount.x * (worldAxis.x)) * -1.01f);
	//offset.y = position.y + ((amount.y * (worldAxis.y)) * -1.01f);
	//offset.z = position.z + ((amount.z * (worldAxis.z)) * -1.01f);
	
	offset.x = position.x + ((amount.x * temp.x) * value);
	offset.y = position.y + ((amount.y * temp.y)* value);
	offset.z = position.z + ((amount.z * temp.z)* value);
	
	//D3DXVECTOR3 offset = position + amount;
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
			if((abs(temp.x) * amount.x) != 0.0f)
			{
			amount.x = 0.0f;
			result = true;
			}
			if((abs(temp.y) * amount.y) != 0.0f)
			{
			amount.y = 0.0f;
			result = true;
			}
			if((abs(temp.z) * amount.z) != 0.0f)
			{
			amount.z = 0.0f;
			result = true;
			}
		}
	}

	return result;
}
void Player::VerticalCollisionTest(D3DXVECTOR3& amount, std::vector<BoundingBox>& bb)
{
	D3DXVECTOR3 temp;
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
*/