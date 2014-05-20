#include "EnemyManager.h"
#include "FallingEnemy.h"
#include "NonFallingEnemy.h"

EnemyManager::EnemyManager(std::vector<D3DXMATRIX> p, std::vector<int> type, ID3D11ShaderResourceView* tM, ID3D11ShaderResourceView* nM, std::vector<AnimationStack> aS, ID3D11Buffer* vB, int vC, ModelClass::BoundingBox box)
{
	/*
	maxEnemy = 1;
	enemySpawnPoint = p;
	vCount = vC;
	vBuffer = vB;
	bBox = box;
	textureMap = tM;
	normalMap = nM;
	enemyType = type;
	animationStack = aS;
	*/
}
EnemyManager::EnemyManager(std::vector<Mesh> m)
{
	enemies.clear();
	vCount.clear();
	vBuffer.clear();
	bBox.clear();
	textureMap.clear();
	normalMap.clear();
	animationStack.clear();

	enemySpawnPoint = m[0].transforms;
	for (int i = 0; i < m.size(); i++)
	{
		vCount.push_back(m[i].vCount);
		vBuffer.push_back(m[i].m_vertexBuffer);
		bBox.push_back(m[i].bBox[0]);
		textureMap.push_back(m[i].textureMap);
		normalMap.push_back(m[i].normalMap);
		animationStack.push_back(m[i].animationSets);
		
	}
	maxEnemy = 1;
	enemyType.push_back(0);
	enemyType.push_back(1);
	enemyType.push_back(0);
	enemyType.push_back(1);
	enemyType.push_back(0);
	enemyType.push_back(1);
	enemyType.push_back(0);
	enemyType.push_back(1);
	enemyType.push_back(0);
	enemyType.push_back(1);
}

EnemyManager::EnemyManager(const EnemyManager& other)
{
}

EnemyManager::~EnemyManager()
{

}

void EnemyManager::SpawnEnemy(ID3D11Device* device, int t)
{
	if (enemyType[t] == 0)
	{
		FallingEnemy* fEnemy = new FallingEnemy(device, enemySpawnPoint[t], textureMap[0], normalMap[0], animationStack[0], vBuffer[0], vCount[0]);
		fEnemy->bBox = bBox[0];
		fEnemy->bBoxOriginal = bBox[0];
		enemies.push_back(fEnemy);
	}
	else
	{
		NonFallingEnemy* fEnemy = new NonFallingEnemy(device, enemySpawnPoint[t], textureMap[1], normalMap[1], animationStack[1], vBuffer[1], vCount[1]);
		fEnemy->bBox = bBox[0];
		fEnemy->bBoxOriginal = bBox[0];
		enemies.push_back(fEnemy);
	}
}

void EnemyManager::Shutdown()
{	
	for (int i = 0; i < vBuffer.size(); i++)
	{
		if (vBuffer[i])
		{
			vBuffer[i]->Release();
			vBuffer[i] = 0;
		}
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
	for (int c = 0; c < enemySpawnPoint.size(); c++)
	{

		D3DXVECTOR3 enemyPoint = D3DXVECTOR3(enemySpawnPoint[c]._41, enemySpawnPoint[c]._42, enemySpawnPoint[c]._43);
		float length;
		length = D3DXVec3Length(&(playerPosition->GetPosition() - enemyPoint));
		if ((length < 50.0f) && (enemies.size() < maxEnemy))
			SpawnEnemy(device, c);
		for (int i = 0; i < enemies.size(); i++)
		{
			length = D3DXVec3Length(&(playerPosition->GetPosition() - enemies[i]->GetPosition()));
			if (length > 40.0f)
			{
				enemies.erase(enemies.begin() + i);
			}
			else if (length < 2.5f)
			{
				playerPosition->Kill();
			}

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
			ID3D11Buffer* temp = enemies[i]->GetVertBuffer();
			deviceContext->IASetVertexBuffers(0, 1, &temp, &stride, &offset);

			bool result = render->UpdateRender(deviceContext, enemies[i]->GetWorldMatrix(), viewMatrix, enemies[i]->GetTextureMap(), enemies[i]->GetNormalMap(), enemies[i]->GetMaterial(), enemies[i]->GetCurrentFrame());
			render->Draw(deviceContext, enemies[i]->GetVertexCount(), 1);

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
			ID3D11Buffer* temp = enemies[i]->GetVertBuffer();
			deviceContext->IASetVertexBuffers(0, 1, &temp, &stride, &offset);

			bool result = render->UpdateRenderShadow(deviceContext, enemies[i]->GetWorldMatrix(), enemies[i]->GetCurrentFrame());
			render->Draw(deviceContext, enemies[i]->GetVertexCount(), 3);

		}
	}
}
