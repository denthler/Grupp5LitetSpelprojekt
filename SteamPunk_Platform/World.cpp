#include "World.h"

WorldClass::WorldClass()
{
	
	camera = 0;
	renderClass = 0;
	pointLight = 0;
	player = 0;
	input = 0;
	enemy = 0;
	eManager = 0;
}

WorldClass::WorldClass(const WorldClass& other)
{
}

WorldClass::~WorldClass()
{
}

bool WorldClass::Initialize(ID3D11Device* DContext, HWND hwnd, D3DXMATRIX proj, HINSTANCE hInstance)
{
	bool result;

	projection = proj;

	rManager.LoadLevel("Level1.SPL", DContext);
	pManager.CreateLevel(rManager.meshes);

	renderClass = new Render();
	if(!renderClass)
	{
		return false;
	}

	result = renderClass->Initialize(DContext, hwnd, L"shader.fx", proj);
	if(!result)
	{
		return false;
	}

	camera = new Camera();
	if(!camera)
	{
		return false;
	}
	camera->Initialize(0.0f, 10.0f, -50.0f, D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	pointLight = new PointLightClass();
	if(!pointLight)
	{
		return false;
	}
	pointLight->SetDiffuseColor(0.5f, 0.5f, 0.5f, 1.0f);
	pointLight->SetPosition(0.0f, 20.0f, 0.0f); 
	pointLight->SetAmbientColor(0.4f, 0.4f, 0.4f, 1.0f);
	pointLight->SetAttenuation(0.2f, 0.01f, 0.0f);
	pointLight->SetRange(500.0f);

	player = new Player();

	if(!player)
	{
		return false;
	}
	
	result = player->Initialize(DContext, D3DXVECTOR3(rManager.player.transforms[0]._41, rManager.player.transforms[0]._42, rManager.player.transforms[0]._43));
	player->bBox = rManager.player.bBox[0];
	player->bBoxOriginal = rManager.player.bBox[0];
	if(!result)
	{
		return false;
	}

	input = new InputClass();

	if(!input)
	{
		return false;
	}
	
	result = input->Initialize(hInstance, hwnd, 0.0f, 0.0f); 
	if(!result)
	{
		return false;
	}
	
	enemy = new FallingEnemy(DContext, rManager.enemys[0].transforms[0]);
	enemy->bBox = rManager.enemys[0].bBox[0];

	eManager = new EnemyManager(rManager.enemys[0].transforms[0],
		rManager.enemys[0].m_vertexBuffer, rManager.enemys[0].vCount, rManager.enemys[0].bBox[0]);


	return true;
}

void WorldClass::HandleInput()
{
	input->Update();
	if (input->CheckSingleKeyPress(DIK_SPACE))
	{
		player->SetJump();
	}
	if(input->CheckKeyPress(DIK_A))
	{
		player->SetLeft();
	}
	if(input->CheckKeyPress(DIK_D))
	{
		player->SetRight();
	}
	if(input->CheckSingleKeyPress(DIK_W))
	{
		if(camera->IsNotFlipping())
		{
			//if (player->IsOnGround())
			//{
				camera->Flip();
				player->FlipGravity();
				eManager->FlipGravityW();
			//}
		}
	}
	if (input->CheckSingleKeyPress(DIK_S))
	{
		if(camera->IsNotFlipping())
		{
			//if (player->IsOnGround())
			//{
				camera->FlipS();
				player->FlipGravityS();
				eManager->FlipGravityS();
			//}
		}
	}
}

void WorldClass::Run(ID3D11DeviceContext* DContext, DWORD time)
{

	//Update(time);

	Draw(DContext);
}

void WorldClass::CleanUp()
{
	if(renderClass)
	{
		renderClass->CleanShader();
		delete renderClass;
		renderClass = 0;
	}
	if(camera)
	{
		delete camera;
		camera = 0;
	}
	if(player)
	{
		player->Shutdown();
		delete player;
		player = 0;
	}
	if(pointLight)
	{
		delete pointLight;
		pointLight = 0; 
	}
	if(input)
	{
		input->Shutdown();
		delete input;
		input = 0; 
	}
	if (enemy)
	{
		delete enemy;
		enemy = 0;
	}
	if (eManager)
	{
		eManager->Shutdown();
		delete eManager;
		eManager = 0;
	}
}

bool WorldClass::Update(float time, ID3D11Device* DContext)
{
	HandleInput();
	
	std::vector<ModelClass::BoundingBox> tempBB;
	
	pManager.Update(player->GetPosition(), tempBB);
	
	eManager->Update(tempBB, time, player, DContext);

	player->Update(time, tempBB);
	camera->Update(player->GetPosition());
	renderClass->UpdateFrustum(camera->GetView(), projection);
	pointLight->SetPosition(player->GetPosition().x, player->GetPosition().y, -50.0f);
	//pointLight->SetDiffuseColor(red, 0.5f, 0.5f, 1.0f);

	return true;
}

void WorldClass::Draw(ID3D11DeviceContext* DContext)
{
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;

	bool result;

	viewMatrix = camera->GetView();

	ID3D11ShaderResourceView* tempTex;

	tempTex = 0;
	pManager.Draw(DContext, renderClass, viewMatrix, tempTex, pointLight, player->GetMaterial());

	player->Apply(DContext, rManager.player.m_vertexBuffer);
	result = renderClass->UpdateRender(DContext, player->GetWorldMatrix(), viewMatrix, tempTex, pointLight, player->GetMaterial());
	if (!result)
	{
		return;
	}
	renderClass->Draw(DContext, rManager.player.vCount);

	eManager->Draw(DContext, renderClass, viewMatrix, tempTex, pointLight, player->GetMaterial());
	/*
	enemy->Apply(DContext, rManager.enemys[0].m_vertexBuffer);
	result = renderClass->UpdateRender(DContext, enemy->GetWorldMatrix(), viewMatrix, tempTex, pointLight, player->GetMaterial());
	if (!result)
	{
		return;
	}
	renderClass->Draw(DContext, rManager.enemys[0].vCount);
	*/
}