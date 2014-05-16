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

	//ModelClass::position = position;
	m_worldMatrix = p;
	position = D3DXVECTOR3(p._41, p._42, p._43);
	moveScale = -0.03f;
	prevPos = position;
}

Enemy::~Enemy()
{

}

bool Enemy::FlipGravity(std::vector<BoundingBox>& bb, D3DXVECTOR3 wAxis)
{
	//D3DXVec3Cross(&worldAxis, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), &worldAxis);
	worldAxis = wAxis;
	D3DXVECTOR3 right;
	D3DXVec3Cross(&right, &worldAxis, &D3DXVECTOR3(0.0f, 0.0f, 1.0f));
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
	return true;
}

bool Enemy::FlipGravityS(std::vector<BoundingBox>& bb, D3DXVECTOR3 wAxis)
{

	//D3DXVECTOR3 tempWorldAxis;
	//tempWorldAxis = worldAxis;
	//D3DXVec3Cross(&worldAxis, &worldAxis, &D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	worldAxis = wAxis;
	D3DXVECTOR3 right;
	D3DXVec3Cross(&right, &worldAxis, &D3DXVECTOR3(0.0f, 0.0f, 1.0f));
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
	return true;
}

bool Enemy::Update(float gameTime, std::vector<BoundingBox>& bb)
{
	gameTime *= 0.05f;
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