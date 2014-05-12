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
	EnemyManager(D3DXMATRIX, ID3D11ShaderResourceView* tM, ID3D11ShaderResourceView* nM, std::vector<AnimationStack> aS, ID3D11Buffer* vB, int vC, ModelClass::BoundingBox);
	EnemyManager(const EnemyManager&);
	~EnemyManager();

	void Update(std::vector<ModelClass::BoundingBox>&, float, Player* playerPosition, ID3D11Device* device);
	void Draw(ID3D11DeviceContext*, Render*, D3DXMATRIX);
	void Shutdown();
	void FlipGravityW();

	void FlipGravityS();
private:
	void SpawnEnemy(ID3D11Device* device);
	int maxEnemy;

	D3DXMATRIX enemySpawnPoint;
	std::vector<Enemy*> enemies;
	ID3D11Buffer* vBuffer;
	int vCount;
	ModelClass::BoundingBox bBox;

	ID3D11ShaderResourceView* textureMap;
	ID3D11ShaderResourceView* normalMap;
	std::vector<AnimationStack> animationStack;
	ModelClass::Material mat;
};

#endif