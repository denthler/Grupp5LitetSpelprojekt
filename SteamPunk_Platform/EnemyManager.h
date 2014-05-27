//
//EnemyManager.h
//
#ifndef _EnemyManager_H_
#define _EnemyManager_H_

#include "Enemy.h"
#include <vector>
#include "Render.h"
#include "ResourceManager.h"
//Temp
#include "Player.h"
//

struct EnemySpawnPoint
{
	int maxEnemies;
	int enemyCount;
	int type;

	int currentEnemy;
	int model;
	D3DMATRIX location;
};

class EnemyManager
{

public:
	EnemyManager(std::vector<D3DXMATRIX>, std::vector<int>, ID3D11ShaderResourceView* tM, ID3D11ShaderResourceView* nM, std::vector<AnimationStack> aS, ID3D11Buffer* vB, int vC, ModelClass::BoundingBox);
	EnemyManager(std::vector<Mesh> m);
	EnemyManager(const EnemyManager&);
	~EnemyManager();

	void Update(std::vector<ModelClass::BoundingBox>&, float, Player* playerPosition, ID3D11Device* device);
	void Draw(ID3D11DeviceContext*, Render*, D3DXMATRIX);
	void DrawShadow(ID3D11DeviceContext*, Render*);
	void Shutdown();
	void FlipGravityW(std::vector<ModelClass::BoundingBox>& bb, D3DXVECTOR3);

	void FlipGravityS(std::vector<ModelClass::BoundingBox>& bb, D3DXVECTOR3);
private:
	void SpawnEnemy(ID3D11Device* device, int);

	std::vector<Enemy*> enemies;
	std::vector<ID3D11Buffer*> vBuffer;
	std::vector<int> vCount;
	std::vector<ModelClass::BoundingBox> bBox;

	std::vector<ID3D11ShaderResourceView*> textureMap;
	std::vector<ID3D11ShaderResourceView*> normalMap;
	std::vector<std::vector<AnimationStack>> animationStack;
	ModelClass::Material mat;

	std::vector<EnemySpawnPoint> spawnPoints;
};

#endif