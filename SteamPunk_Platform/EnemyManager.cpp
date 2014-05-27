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
	spawnPoints.clear();

	for (int i = 0; i < m.size(); i++)
	{
		for (int j = 0; j < m[i].transforms.size(); j++)
		{
			EnemySpawnPoint temp;
			temp.location = m[i].transforms[j];
			temp.maxEnemies = 1;
			temp.enemyCount = 0;
			temp.model = i;


			int type1 = m[i].type.find("type1");
			int type2 = m[i].type.find("type2");

			if (type1 > 0)
				temp.type = 0;
			if (type2 > 0)
				temp.type = 1;

			spawnPoints.push_back(temp);
		}

		vCount.push_back(m[i].vCount);
		vBuffer.push_back(m[i].m_vertexBuffer);
		bBox.push_back(m[i].bBox[0]);
		textureMap.push_back(m[i].textureMap);
		normalMap.push_back(m[i].normalMap);
		animationStack.push_back(m[i].animationSets);
	}
}

EnemyManager::EnemyManager(const EnemyManager& other)
{
}

EnemyManager::~EnemyManager()
{

}

void EnemyManager::SpawnEnemy(ID3D11Device* device, int t)
{
	spawnPoints[t].currentEnemy = enemies.size();
	if (spawnPoints[t].type == 0)
	{
		FallingEnemy* fEnemy = new FallingEnemy(device, spawnPoints[t].location, textureMap[spawnPoints[t].model], normalMap[spawnPoints[t].model], animationStack[spawnPoints[t].model], vBuffer[spawnPoints[t].model], vCount[spawnPoints[t].model]);
		fEnemy->bBox = bBox[spawnPoints[t].model];
		fEnemy->bBoxOriginal = bBox[spawnPoints[t].model];
		enemies.push_back(fEnemy);
	}
	else
	{
		NonFallingEnemy* fEnemy = new NonFallingEnemy(device, spawnPoints[t].location, textureMap[spawnPoints[t].model], normalMap[spawnPoints[t].model], animationStack[spawnPoints[t].model], vBuffer[spawnPoints[t].model], vCount[spawnPoints[t].model]);
		fEnemy->bBox = bBox[spawnPoints[t].model];
		fEnemy->bBoxOriginal = bBox[spawnPoints[t].model];
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
	for (int c = 0; c < spawnPoints.size(); c++)
	{

		D3DXVECTOR3 enemyPoint = D3DXVECTOR3(spawnPoints[c].location._41, spawnPoints[c].location._42, spawnPoints[c].location._43);
		float length;
		length = D3DXVec3Length(&(playerPosition->GetPosition() - enemyPoint));

		if ((length < 50.0f) && (spawnPoints[c].enemyCount < spawnPoints[c].maxEnemies))
		{
			spawnPoints[c].enemyCount++;
			SpawnEnemy(device, c);
		}

		if (spawnPoints[c].enemyCount > 0)
		{
			length = D3DXVec3Length(&(playerPosition->GetPosition() - enemies[spawnPoints[c].currentEnemy]->GetPosition()));
			if (length > 50.0f)
			{
				enemies.erase(enemies.begin() + spawnPoints[c].currentEnemy);
				spawnPoints[c].enemyCount--;

				for (int i = 0; i < spawnPoints.size(); i++)
				{
					if (spawnPoints[c].currentEnemy < spawnPoints[i].currentEnemy)
						spawnPoints[i].currentEnemy--;
				}
			}
			else if (length < 2.5f)
			{
				playerPosition->deathAni = true;
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
