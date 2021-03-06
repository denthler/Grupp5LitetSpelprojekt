#include "Enemy.h"


Enemy::Enemy(ID3D11Device * device, D3DXMATRIX p, ID3D11ShaderResourceView* tM, ID3D11ShaderResourceView* nM, std::vector<AnimationStack> aS, ID3D11Buffer* vB, int vC)
{
	//worldUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	
	ModelClass::Initialize(device);

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

	D3DXVec3TransformNormal(&worldAxis, &worldAxis, &p);
	if(worldAxis.x < 0.5 && worldAxis.x > -0.5)
		worldAxis.x = 0.0f;
	if(worldAxis.y < 0.5 && worldAxis.y > -0.5)
		worldAxis.y = 0.0f;
	if(worldAxis.z < 0.5 && worldAxis.z > -0.5)
		worldAxis.z = 0.0f;

	D3DXVec3Normalize(&worldAxis, &worldAxis);

	//ModelClass::position = position;
	m_worldMatrix = p;
	position = D3DXVECTOR3(p._41, p._42, p._43);
	moveScale = -0.03f;
	prevPos = position;
}

Enemy::~Enemy()
{

}

D3DXMATRIX Enemy::GetWorldMatrix()
{
	ModelClass::GetWorldMatrix();
	//Enemy only
	if (OnGround)
	{
		D3DXVECTOR3 temp;
		D3DXVec3Cross(&temp, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), &worldAxis);
		float value = -1.5f;
		if (moveScale < 0)
			value *= -1.0f;
		bBox.max += temp * value;
		bBox.min += temp * value;
	}
	//

	return m_worldMatrix;
}

bool Enemy::FlipGravity(std::vector<BoundingBox>& bb, D3DXVECTOR3 wAxis)
{
	//Fix This
	position += worldAxis * 1.5f;
	prevPos = position;
	worldAxis = wAxis;

	/*
	worldAxis = wAxis;
	D3DXVECTOR3 right;
	if (moveScale > 0)
		D3DXVec3Cross(&right, &worldAxis, &D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	else
		D3DXVec3Cross(&right, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), &worldAxis);
	position += right * 2.0f;
	prevPos = position;
	GetWorldMatrix();
	
	D3DXVECTOR3 tempMax = bBox.max;
	D3DXVECTOR3 tempMin = bBox.min;
	tempMax = position + tempMax;
	tempMin = position + tempMin;
	for (int j = 0; j < bb.size(); j++)
	{
		if (tempMax.x > bb[j].min.x &&
			tempMin.x < bb[j].max.x &&
			tempMax.y > bb[j].min.y &&
			tempMin.y < bb[j].max.y &&
			tempMax.z > bb[j].min.z &&
			tempMin.z < bb[j].max.z)
		{
			position -= right * 4.0f;
			prevPos = position;
		}
	}
	*/
	return true;
}

bool Enemy::FlipGravityS(std::vector<BoundingBox>& bb, D3DXVECTOR3 wAxis)
{

	position += worldAxis * 1.5f;
	prevPos = position;
	worldAxis = wAxis;
	//Fix this
	/*
	worldAxis = wAxis;
	D3DXVECTOR3 right;
	if (moveScale > 0)
		D3DXVec3Cross(&right, &worldAxis, &D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	else
		D3DXVec3Cross(&right, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), &worldAxis);
	position -= right * 2.0f;
	prevPos = position;
	GetWorldMatrix();
	
	D3DXVECTOR3 tempMax = bBox.max;
	D3DXVECTOR3 tempMin = bBox.min;
	tempMax = position + tempMax;
	tempMin = position + tempMin;
	for (int j = 0; j < bb.size(); j++)
	{
		if (tempMax.x > bb[j].min.x &&
			tempMin.x < bb[j].max.x &&
			tempMax.y > bb[j].min.y &&
			tempMin.y < bb[j].max.y &&
			tempMax.z > bb[j].min.z &&
			tempMin.z < bb[j].max.z)
		{
			position += right * 4.0f;
			prevPos = position;
		}
	}
	*/
	return true;
}

bool Enemy::Update(float gameTime, std::vector<BoundingBox>& bb)
{
	gameTime *= (1.0f / gameTime);
	static bool test = false;
	bool test2;
	D3DXVECTOR3 right;
	
	D3DXVec3Cross(&right, &worldAxis, &D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	if (OnGround)
		velocity = right * moveScale;

	test = OnGround;
	test2 = ModelClass::Update(gameTime, bb);

	if (!(OnGround) && (test))
	{
		position = prevPos;
		moveScale *= -1.0f;
		velocity = right * (moveScale);
		//velocity += (worldAxis * 0.0028f);
	}
	else if (test2)
	{
		moveScale *= -1.0f;
		//velocity += right * (moveScale);
		return true;
	}
	else
		prevPos = position;
	if (moveScale > 0)// && OnGround)
	{
		Rotated = false;
	}
	else
	{
		Rotated = true;
	}

	if (!OnGround)
	{
		for (int i = 0; i < animationStack.size(); i++)
		{
			if (animationStack[i].name == "Fall")
				currentAnimStack = i;
		}
	}		
	else
	{
		for (int i = 0; i < animationStack.size(); i++)
		{
			if (animationStack[i].name == "Run")
				currentAnimStack = i;
		}
	}

	currentFrame = animationStack[currentAnimStack].keyFrames[(int)animationTime].boneTransforms;
	animationTime += gameTime;

	if (animationTime > animationStack[currentAnimStack].keyFrames.size() - 1)
		animationTime = 0;

	return false;
}