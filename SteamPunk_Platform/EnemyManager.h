//
//EnemyManager.h
//
#ifndef _EnemyManager_H_
#define _EnemyManager_H_

#include "Enemy.h"
#include <vector>
#include "Render.h"
//Temp
#include "Player.h"
//

class EnemyManager
{

public:
	EnemyManager(D3DXMATRIX, ID3D11Buffer*, int, ModelClass::BoundingBox);
	EnemyManager(const EnemyManager&);
	~EnemyManager();

	void Update(std::vector<ModelClass::BoundingBox>&, float, Player* playerPosition, ID3D11Device* device);
	void Draw(ID3D11DeviceContext*, Render*, D3DXMATRIX, ID3D11ShaderResourceView*, PointLightClass*, ModelClass::Material);
	void Shutdown();
private:
	void SpawnEnemy(ID3D11Device* device);
	int maxEnemy;

	D3DXMATRIX enemySpawnPoint;
	std::vector<Enemy*> enemies;
	ID3D11Buffer* vBuffer;
	int vCount;
	ModelClass::BoundingBox bBox;
};

#endif