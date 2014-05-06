#include "EnemyManager.h"
#include "FallingEnemy.h"

EnemyManager::EnemyManager(D3DXMATRIX p, ID3D11Buffer* buf, int v, ModelClass::BoundingBox box)
{
	maxEnemy = 1;
	enemySpawnPoint = p;
	vCount = v;
	vBuffer = buf;
	bBox = box;
	
}

EnemyManager::EnemyManager(const EnemyManager& other)
{
}

EnemyManager::~EnemyManager()
{

}

void EnemyManager::SpawnEnemy(ID3D11Device* device)
{

	FallingEnemy* fEnemy = new FallingEnemy(device, enemySpawnPoint);
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

void EnemyManager::FlipGravityW()
{
	for (int i = 0; i < enemies.size(); i++)
	{
		enemies[i]->FlipGravity();
	}
}

void EnemyManager::FlipGravityS()
{
	for (int i = 0; i < enemies.size(); i++)
	{
		enemies[i]->FlipGravityS();
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

void EnemyManager::Draw(ID3D11DeviceContext* deviceContext, Render* render, D3DXMATRIX viewMatrix, ID3D11ShaderResourceView* texture, PointLightClass* lightStruct, ModelClass::Material mat)
{
	unsigned int stride;
	unsigned int offset;

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	stride = sizeof(VertexTypeT);
	offset = 0;
	for (int i = 0; i < enemies.size(); i++)
	{
		if (render->InsideFrustum(enemies[i]->GetBoundingBox().min, enemies[i]->GetBoundingBox().max))
		{
			deviceContext->IASetVertexBuffers(0, 1, &vBuffer, &stride, &offset);
			D3DXMATRIX world;
			world = enemies[i]->GetWorldMatrix();
			bool result = render->UpdateRender(deviceContext, world, viewMatrix, texture, lightStruct, mat);
			render->Draw(deviceContext, vCount);
		}
	}
}