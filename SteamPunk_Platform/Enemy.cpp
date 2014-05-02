#include "Enemy.h"

Enemy::Enemy(ID3D11Device * device, D3DXVECTOR3 position, ID3D11ShaderResourceView* tM, ID3D11ShaderResourceView* nM, std::vector<AnimationStack> aS, ID3D11Buffer* vB, int vC)
{
	//worldUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	ModelClass::Initialize(device);
	ModelClass::position = position;

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
	moveScale = -0.05f;
}

Enemy::~Enemy()
{

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

	animationTime++;
	currentFrame = animationStack[0].keyFrames[animationTime].boneTransforms;

	if (animationTime > 24)
		animationTime = 0;

	return false;
}