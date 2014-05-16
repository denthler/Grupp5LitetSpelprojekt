#ifndef _WORLD_H_
#define _WORLD_H_

//////////////
// INCLUDES //
//////////////
#include "FallingEnemy.h"
#include "NonFallingEnemy.h"
#include "Camera.h"
#include "model.h"
#include "Render.h"
#include "Player.h"
#include "input.h"
#include "ResourceManager.h"
#include "PlatformManager.h"
#include "EnemyManager.h"

class WorldClass
{
public:
	WorldClass();
	WorldClass(const WorldClass&);
	~WorldClass();

	bool Initialize(ID3D11Device*, HWND, D3DXMATRIX, HINSTANCE);
	void Run(ID3D11DeviceContext*, DWORD);
	void CleanUp();
	bool Update(float, ID3D11Device*);
	void Draw(ID3D11DeviceContext*);
	void DrawShadow(ID3D11DeviceContext*);

	Render* renderClass;
private:
	void HandleInput(std::vector<ModelClass::BoundingBox>& tempBB);
	Camera* camera;
	Player* player;
	InputClass* input;
	ResourceManager rManager;
	PlatformManager pManager;
	D3DXMATRIX projection;
	EnemyManager* eManager;

};

#endif