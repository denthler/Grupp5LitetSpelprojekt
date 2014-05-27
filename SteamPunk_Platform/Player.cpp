#include "Player.h"

Player::Player()
{
	velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	dead = false;
	deathAni = false;
	left = false; right = false; up = false; down = false; jump = false;
	OnGround = false;
	moveScale = -0.2f;//-0.1f
	gravity = -0.01f;
	worldAxis = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	numberOfCogs = 0;
	canFlip = true;
}

Player::~Player()
{
	
}

void Player::Shutdown()
{
	ModelClass::Shutdown();
}

bool Player::FlipGravity(std::vector<BoundingBox>& bb)
{
	canFlip = false;
	return ModelClass::FlipGravity(bb);
}

bool Player::FlipGravityS(std::vector<BoundingBox>& bb)
{
	canFlip = false;
	return ModelClass::FlipGravityS(bb);
}

bool Player::Initialize(ID3D11Device* device, D3DXVECTOR3 startPos, ID3D11ShaderResourceView* tM, ID3D11ShaderResourceView* nM, std::vector<AnimationStack> aS, ID3D11Buffer* vB, int vC)
{
	ModelClass::Initialize(device);

	position = startPos;

	textureMap = tM;
	if (textureMap)
	{
		mat.hasTexture = true;
	}
	else
	{
		mat.hasTexture = false;
	}

	normalMap = nM;
	if (normalMap)
	{
		mat.hasNormal = true;
	}
	else
	{
		mat.hasNormal = false;
	}

	animationStack = aS;
	m_vertexBuffer = vB;
	vCount = vC;
	animationTime = 0;

	StartPos = startPos;

	return true;
}

void Player::Kill()
{
	worldAxis = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	GetWorldMatrix();
	position = StartPos;
	dead = true;
}

void Player::AddCog()
{
	numberOfCogs++;
}

D3DXMATRIX Player::GetWorldMatrix()
{
	//Move this code to superclass later!!!!!
	/*
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
	*/
	return ModelClass::GetWorldMatrix();
}

bool Player::Update(float gameTime, std::vector<BoundingBox>& bb)
{
	D3DXVECTOR3 temp;

	//prevVelocity = velocity;
	//gameTime *= 0.05f;
	gameTime *= (1.0f / gameTime);

	static bool tempBool = false;
	D3DXVec3Cross(&temp, &worldAxis, &D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	if (!dead)
	{
		velocity.x -= (abs(temp.x) * velocity.x);
		velocity.y -= (abs(temp.y) * velocity.y);
		velocity.z -= (abs(temp.z) * velocity.z);

		//D3DXVECTOR3 temp2;
		//temp2 = (bBox.max - bBox.min) / 2.0f;

		if (left)
		{
			D3DXVec3Cross(&temp, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), &worldAxis);
			velocity -= moveScale * temp;
			if (!Rotated)// && tempBool)
			{
				position.x += (temp.x) * (.9f);
				position.y += (temp.y) * (.9f);
				position.z += (temp.z) * (.9f);
			}
			left = false;
			Rotated = true;
			runAni = true;
		}
		if (right)
		{
			D3DXVec3Cross(&temp, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), &worldAxis);
			velocity += moveScale * temp;
			if (Rotated)// && tempBool)
			{
				position.x -= (temp.x) * (.9f);
				position.y -= (temp.y) * (.9f);
				position.z -= (temp.z) * (.9f);
			}
			right = false;
			Rotated = false;
			runAni = true;
		}
		if (jump)
		{
			if (OnGround)
			{				
				jumpAni = true;
				animationTime = 0;
				Jump();
			}
			jump = false;
		}
		if(D3DXVec3Length(&velocity) == 0.0f)
			runAni = false;

		if(!OnGround)
			inAir = true;
		else
			canFlip = true;

		if (FallDamage)
		{
			animationTime = 0;
			FallDamage = false;
			deathAni = true;
		}
		if(OnGround && !landAni && inAir)
		{
			landAni = true;
			animationTime = 0;
			inAir = false;			
		}
	}
	else
	{
		animationTime = 0;
		deathAni = true;
	}

	if (deathAni)
	{
		for (int i = 0; i < animationStack.size(); i++)
		{
			if (animationStack[i].name == "Die")
				currentAnimStack = i;
		}
	}
	else if (!OnGround && !jumpAni)
	{
		for (int i = 0; i < animationStack.size(); i++)
		{
			if (animationStack[i].name == "Fall")
				currentAnimStack = i;
		}
	}
	else if (jumpAni)
	{
		for (int i = 0; i < animationStack.size(); i++)
		{
			if (animationStack[i].name == "Jump")
				currentAnimStack = i;
		}
	}	
	else if (landAni)
	{
		for (int i = 0; i < animationStack.size(); i++)
		{
			if (animationStack[i].name == "Land")
				currentAnimStack = i;
		}
	}
	else if (runAni)
	{
		for (int i = 0; i < animationStack.size(); i++)
		{
			if (animationStack[i].name == "Run")
				currentAnimStack = i;
		}
	}	
	else if (OnGround)
	{
		for (int i = 0; i < animationStack.size(); i++)
		{
			if (animationStack[i].name == "Idle")
				currentAnimStack = i;
		}
	}

	if (animationTime > animationStack[currentAnimStack].keyFrames.size() - 1 && deathAni)
	{
		animationTime = 0;
		deathAni = false;
		Kill();
	}
	else if (animationTime > animationStack[currentAnimStack].keyFrames.size() - 1 && jumpAni)
	{
		jumpAni = false;
		animationTime = 0;
	}
	else if (animationTime > animationStack[currentAnimStack].keyFrames.size() - 1 && landAni)
	{		
		landAni = false;
		inAir = false;
		OnGround = true;
		animationTime = 0;
	}
	else if (animationTime > animationStack[currentAnimStack].keyFrames.size() - 1)
		animationTime = 0;

	currentFrame = animationStack[currentAnimStack].keyFrames[(int)animationTime].boneTransforms;
	animationTime += gameTime;	

	tempBool = (ModelClass::Update(gameTime, bb));
	return tempBool;
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