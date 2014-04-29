#ifndef _WORLD_H_
#define _WORLD_H_

//////////////
// INCLUDES //
//////////////
#include "FallingEnemy.h"
#include "Camera.h"
#include "model.h"
#include "Render.h"
#include "PointLight.h"
#include "Player.h"
#include "input.h"
#include "ResourceManager.h"
#include "PlatformManager.h"

class WorldClass
{
public:
	WorldClass();
	WorldClass(const WorldClass&);
	~WorldClass();

	bool Initialize(ID3D11Device*, HWND, D3DXMATRIX, HINSTANCE);
	void Run(ID3D11DeviceContext*, DWORD);
	void CleanUp();
	bool Update(float);
	void Draw(ID3D11DeviceContext*);

private:
	void HandleInput();
	Camera* camera;
	FallingEnemy * enemy;
	Render* renderClass;
	PointLightClass* pointLight;
	Player* player;
	InputClass* input;
	ResourceManager rManager;
	PlatformManager pManager;
};

#endif