#ifndef _WORLD_H_
#define _WORLD_H_

//////////////
// INCLUDES //
//////////////
#include "Enemy.h"
#include "Camera.h"
#include "model.h"
#include "Render.h"
#include "PointLight.h"
#include "Player.h"
#include "input.h"

class WorldClass
{
public:
	WorldClass();
	WorldClass(const WorldClass&);
	~WorldClass();

	bool Initialize(ID3D11Device*, HWND, D3DXMATRIX, HINSTANCE);
	void Run(ID3D11DeviceContext*);
	void CleanUp();
	bool Update();
	void Draw(ID3D11DeviceContext*);

private:
	void HandleInput();
	Camera* camera;
	ModelClass* model;
	ModelClass* model2;
	Enemy * enemy;
	Render* renderClass;
	PointLightClass* pointLight;
	Player* player;
	InputClass* input;
};

#endif