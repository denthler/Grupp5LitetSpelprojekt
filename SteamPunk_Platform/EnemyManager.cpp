#include "EnemyManager.h"
#include "FallingEnemy.h"

EnemyManager::EnemyManager(D3DXMATRIX p, ID3D11ShaderResourceView* tM, ID3D11ShaderResourceView* nM, std::vector<AnimationStack> aS, ID3D11Buffer* vB, int vC, ModelClass::BoundingBox box)
{
	maxEnemy = 1;
	enemySpawnPoint = p;
	vCount = vC;
	vBuffer = vB;
	bBox = box;
	textureMap = tM;
	normalMap = nM;
	animationStack = aS;
}

EnemyManager::EnemyManager(const EnemyManager& other)
{
}

EnemyManager::~EnemyManager()
{

}

void EnemyManager::SpawnEnemy(ID3D11Device* device)
{
	FallingEnemy* fEnemy = new FallingEnemy(device, enemySpawnPoint, textureMap, normalMap, animationStack, vBuffer, vCount);
	fEnemy->bBox = bBox;
	fEnemy->bBoxOriginal = bBox;
	enemies.push_back(fEnemy);
}

void EnemyManager::Shutdown()
{
	if (vBuffer)
	{
		vBuffer->Release();
		vBuffer = 0;
	}
	enemies.clear();
}


void EnemyManager::FlipGravityW(std::vector<ModelClass::BoundingBox>& bb, D3DXVECTOR3 wAxis)
{
	for (int i = 0; i < enemies.size(); i++)
	{
		enemies[i]->FlipGravity(bb, wAxis);
	}
}

void EnemyManager::FlipGravityS(std::vector<ModelClass::BoundingBox>& bb, D3DXVECTOR3 wAxis)
{
	for (int i = 0; i < enemies.size(); i++)
	{
		enemies[i]->FlipGravityS(bb, wAxis);
	}
}

void EnemyManager::Update(std::vector<ModelClass::BoundingBox>& bBoxes, float time, Player* playerPosition, ID3D11Device* device)
{
	if (enemies.size() >= maxEnemy)
	{
		//return;
	}
	D3DXVECTOR3 enemyPoint = D3DXVECTOR3(enemySpawnPoint._41, enemySpawnPoint._42, enemySpawnPoint._43);
	float length;
	length = D3DXVec3Length(&(playerPosition->GetPosition() - enemyPoint));
	if ((length < 40.0f) && (enemies.size() < maxEnemy))
		SpawnEnemy(device);
	for (int i = 0; i < enemies.size(); i++)
	{
		length = D3DXVec3Length(&(playerPosition->GetPosition() - enemies[i]->GetPosition()));
		if (length > 30.0f)
		{
			enemies.erase(enemies.begin() + i);
		}
		else if (length < 2.5f)
		{
			playerPosition->Kill();
		}
		
	}
	for (int i = 0; i < enemies.size(); i++)
	{
		enemies[i]->Update(time, bBoxes);
	}

}

void EnemyManager::Draw(ID3D11DeviceContext* deviceContext, Render* render, D3DXMATRIX viewMatrix)
{
	unsigned int stride;
	unsigned int offset;

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	stride = sizeof(VertexTypeAni);
	offset = 0;
	for (int i = 0; i < enemies.size(); i++)
	{

		//if (render->InsideFrustum(enemies[i]->GetBoundingBox().min , enemies[i]->GetBoundingBox().max))

		if (render->InsideFrustum(enemies[i]->GetBoundingBox().min + enemies[i]->GetPosition(), enemies[i]->GetBoundingBox().max + enemies[i]->GetPosition()))

		{
			deviceContext->IASetVertexBuffers(0, 1, &vBuffer, &stride, &offset);

			bool result = render->UpdateRender(deviceContext, enemies[i]->GetWorldMatrix(), viewMatrix, enemies[i]->GetTextureMap(), enemies[i]->GetNormalMap(), enemies[i]->GetMaterial(), enemies[i]->GetCurrentFrame());
			render->Draw(deviceContext, vCount, 1);

		}
	}
}

void EnemyManager::DrawShadow(ID3D11DeviceContext* deviceContext, Render* render)
{
	unsigned int stride;
	unsigned int offset;

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	stride = sizeof(VertexTypeAni);
	offset = 0;
	for (int i = 0; i < enemies.size(); i++)
	{
		if (render->InsideFrustum(enemies[i]->GetBoundingBox().min + enemies[i]->GetPosition(), enemies[i]->GetBoundingBox().max + enemies[i]->GetPosition()))
		{
			deviceContext->IASetVertexBuffers(0, 1, &vBuffer, &stride, &offset);

			bool result = render->UpdateRenderShadow(deviceContext, enemies[i]->GetWorldMatrix(), enemies[i]->GetCurrentFrame());
			render->Draw(deviceContext, vCount, 3);

		}
	}
}
