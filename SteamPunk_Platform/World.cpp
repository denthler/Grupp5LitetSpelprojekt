#include "World.h"

WorldClass::WorldClass()
{
	
	camera = 0;
	renderClass = 0;
	player = 0;
	input = 0;
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
	camera->Initialize(0.0f, 10.0f, -10.0f, D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	player = new Player();

	if(!player)
	{
		return false;
	}
	
	D3DXVECTOR3 playerPos(rManager.player.transforms[0]._41, rManager.player.transforms[0]._42 + 10.0f, rManager.player.transforms[0]._43);

	//D3DXVECTOR3 playerPos(-4.0f, 8.0f, 0.0f);

	result = player->Initialize(DContext, playerPos, 
		rManager.player.textureMap, rManager.player.normalMap, rManager.player.animationSets, rManager.player.m_vertexBuffer, rManager.player.vCount);
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

	eManager = new EnemyManager(rManager.enemys[0].transforms[0],
		rManager.enemys[0].textureMap, rManager.enemys[0].normalMap, rManager.enemys[0].animationSets,
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

	//Draw(DContext);
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
	if(input)
	{
		input->Shutdown();
		delete input;
		input = 0; 
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
	renderClass->setLightPosition(player->GetPosition());

	//pointLight->SetDiffuseColor(red, 0.5f, 0.5f, 1.0f);

	return true;
}

void WorldClass::Draw(ID3D11DeviceContext* DContext)
{
	D3DXMATRIX viewMatrix;

	bool result;

	viewMatrix = camera->GetView();

	Player::Material material = player->GetMaterial();
	ID3D11ShaderResourceView* tempTex;
	ID3D11ShaderResourceView* tempNor;

	tempNor = 0;
	tempTex = 0;
	pManager.Draw(DContext, renderClass, viewMatrix, tempTex, tempNor, material);

	player->Apply(DContext);
	result = renderClass->UpdateRender(DContext, player->GetWorldMatrix(), viewMatrix, player->GetTextureMap(), player->GetNormalMap(), player->GetMaterial(), player->GetCurrentFrame());
	renderClass->Draw(DContext, rManager.player.vCount, 1);

	eManager->Draw(DContext, renderClass, viewMatrix);
}

void WorldClass::DrawShadow(ID3D11DeviceContext* DContext)
{
	bool result;

	pManager.DrawShadow(DContext, renderClass);

	player->Apply(DContext);
	result = renderClass->UpdateRenderShadow(DContext, player->GetWorldMatrix(), player->GetCurrentFrame());
	renderClass->Draw(DContext, rManager.player.vCount, 3);

	//eManager->Draw(DContext, renderClass, viewMatrix, pointLight);
}
