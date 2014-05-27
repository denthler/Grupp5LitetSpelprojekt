#include "World.h"

WorldClass::WorldClass()
{
	camera = 0;
	renderClass = 0;
	player = 0;
	input = 0;
	eManager = 0;
	hud = 0;
	exit = false;
	loading = false;
}

WorldClass::WorldClass(const WorldClass& other)
{
}

WorldClass::~WorldClass()
{

}

bool WorldClass::Initialize(ID3D11Device* device, ID3D11DeviceContext * deviceContext, HWND hwnd, D3DXMATRIX proj, HINSTANCE hInstance)
{
	this->device = device;
	bBoxRender.Init(device, deviceContext);
	bool result;

	currentLevel = 0;

	projection = proj;
	//orthoGraph = ortho;

	renderClass = new Render();
	if(!renderClass)
	{
		return false;
	}

	result = renderClass->Initialize(device, hwnd, L"shader.fx", proj);
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

	context = deviceContext;
	hwn = hwnd;
	pro = proj;
	hInst = hInstance;

	menu = new Menu(device);

	stringstream ss;
	ss << "Level";
	ss << currentLevel;

	NewLevel(device, ss.str());

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(VertexTypeT)* 6;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	device->CreateBuffer(&bufferDesc, NULL, &loadingBuffer);
	
	temptexture = new TextureClass();

	temptexture->Initialize(device, "Resources/Textures/loadingscreen.png");	

	winLoading = new TextureClass();

	winLoading->Initialize(device, "Resources/Textures/winscreen.png");

	sound = new SoundClass();
	if (!sound)
	{
		return false;
	}
	result = sound->Initialize(hwnd);
	if (!result)
	{
		return false;
	}
	sound->AddSound(L"Resources/Sound/comicbounce.wav");
	sound->AddSound(L"Resources/Sound/Footsteps_on_Cement-Tim_Fryer-870410055.wav");
	sound->AddSound(L"Resources/Sound/Industrial Revolution.wav");
	sound->AddSound(L"Resources/Sound/Professor_Blood_-_Professor_Blood_-_invasion.wav");
	sound->AddSound(L"Resources/Sound/Spiky_-_Steampunk_Engineering.wav");
	//sound->AddSound(L"Resources/Sound/walk.wav");

	return true;
}

void WorldClass::HandleInput(std::vector<ModelClass::BoundingBox>& tempBB)
{
	if (input->CheckSingleKeyPress(DIK_SPACE))
	{
		player->SetJump();
		if (!sound->IsPlaying(0) && player->IsOnGround())
			sound->PlayWaveFile(false, 0);
	}
	if(input->CheckKeyPress(DIK_A))
	{
		player->SetLeft();
		if (!sound->IsPlaying(1) && player->IsOnGround() && player->IsMoving())
			sound->PlayWaveFile(false, 1);
	}
	if(input->CheckKeyPress(DIK_D))
	{
		player->SetRight();
		if (!sound->IsPlaying(1) && player->IsOnGround() && player->IsMoving())
			sound->PlayWaveFile(false, 1);
	}
	if(input->CheckSingleKeyPress(DIK_W))
	{
		if(camera->IsNotFlipping())
		{
			if (player->CanFlip())
			{
				if (player->FlipGravity(tempBB))
					camera->Flip();
				eManager->FlipGravityW(tempBB, player->GetAxis());
			}
			
		}
	}
	if (input->CheckSingleKeyPress(DIK_S))
	{
		if(camera->IsNotFlipping())
		{
			if (player->CanFlip())
			{
				if(player->FlipGravityS(tempBB))
					camera->FlipS();
				eManager->FlipGravityS(tempBB, player->GetAxis());

			}

			
		}
	}
}

void WorldClass::HandleMenuInput(ID3D11Device* device)
{
	input->Update();
	if(input->CheckKeyPress(DIK_P))
	{
		menu->Pause();
	}

	if (menu->pause)
	{
		if (input->CheckSingleKeyPress(DIK_W))
		{
			menu->RotateCogUp();
		}
		if (input->CheckSingleKeyPress(DIK_S))
		{
			menu->RotateCogDown();
		}
		if (input->CheckSingleKeyPress(DIK_SPACE))
		{
			if (menu->currentOption == 0)
			{
				if (menu->controlScreen)
					menu->controlScreen = false;
				else
					menu->controlScreen = true;
			}
			else if (menu->currentOption == 1)
			{
				menu->pause = false;
				menu->notMoving = true;
				currentLevel = 1;
				NewLevel(device, "Level1");
			}
			else if (menu->currentOption == 2)
			{
				exit = true;
			}
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
	if (sound)
	{
		sound->Shutdown();
		delete sound;
		sound = 0;
	}
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

	if (hud)
	{
		delete hud;
		hud = 0;
	}

	if (temptexture)
	{
		temptexture->Shutdown();
		delete temptexture;
		temptexture = 0;
	}
	if (winLoading)
	{
		winLoading->Shutdown();
		delete winLoading;
		winLoading = 0;
	}
	if (loadingBuffer)
	{
		loadingBuffer->Release();
		loadingBuffer = 0;
	}
}

void WorldClass::NewLevel(ID3D11Device* device, std::string level)
{
	level += ".SPL";

	rManager.LoadLevel(level, device);
	pManager.CreateLevel(rManager.meshes);

	D3DXVECTOR3 playerPos(rManager.player.transforms[0]._41, rManager.player.transforms[0]._42 + 10.0f, rManager.player.transforms[0]._43);
	player->Initialize(device, playerPos, rManager.player.textureMap, rManager.player.normalMap, rManager.player.animationSets, rManager.player.m_vertexBuffer, rManager.player.vCount);
	player->bBox = rManager.player.bBox[0];
	player->bBoxOriginal = rManager.player.bBox[0];
	player->SetWorldMatrix(rManager.player.transforms[0]);
	player->SetWorldAxis();
	camera->Reset();

	if (currentLevel != 0)
	{
		//eManager = new EnemyManager(rManager.enemys[0].transforms, tempType, rManager.enemys[0].textureMap, rManager.enemys[0].normalMap,
		eManager = new EnemyManager(rManager.enemys);
		//rManager.enemys[0].animationSets, rManager.enemys[0].m_vertexBuffer, rManager.enemys[0].vCount, rManager.enemys[0].bBox[0]);
	}		

	hud = new HUD(device, context, hwn, pro, hInst, &pManager);
}

bool WorldClass::Update(float time, ID3D11Device* DContext)
{
	menu->Update(player->GetWorldMatrix(), player->Rotated, player->worldAxis);
	HandleMenuInput(DContext);
	std::vector<ModelClass::BoundingBox> tempBB;
	if (currentLevel == 0)
	{
		menu->pause = true;



		pManager.Update(player->GetPosition(), tempBB, time, true);

		camera->Update(player->GetPosition());
		renderClass->UpdateFrustum(camera->GetView(), projection);
		renderClass->setLightPosition(player->GetPosition());
	}

	if(!menu->pause && currentLevel != 0)
	{
		std::vector<ModelClass::BoundingBox> tempBB;
		if (!sound->IsPlaying(currentLevel + 1))
		{
			sound->Stop(currentLevel);
			sound->PlayWaveFile(true, currentLevel + 1);
		}
	
		pManager.Update(player->GetPosition(), tempBB, time, player->IsOnGround());
	
		if(!player->deathAni)
			HandleInput(tempBB);

		eManager->Update(tempBB, time, player, DContext);
		if (player->IsDead())
		{
			camera->Reset();
			player->Revive();
			
			stringstream ss;
			ss << "Level";
			ss << currentLevel;
			loading = false;
			NewLevel(DContext, ss.str());
		}
		player->Update(time, tempBB);
		
		camera->Update(player->GetPosition());
		renderClass->UpdateFrustum(camera->GetView(), projection);
		renderClass->setLightPosition(player->GetPosition());

		//if (pManager.endLevel)
		if (loading && pManager.endLevel)
		{
			currentLevel++;
			stringstream ss;
			ss << "Level";
			ss << currentLevel;
			loading = false;
			NewLevel(DContext, ss.str());
		}		
		if (loading && pManager.endGame)
		{
			currentLevel = 0;
			stringstream ss;
			ss << "Level";
			ss << currentLevel;
			loading = false;
			NewLevel(DContext, ss.str());
		}

		if (pManager.endLevel || pManager.endGame)
			loading = true;

		//pointLight->SetDiffuseColor(red, 0.5f, 0.5f, 1.0f);
		hud->Update();
		
	
	}
	bBoxRender.Update(tempBB);
	return true;
}

void WorldClass::DrawLoadingScreen(ID3D11DeviceContext* DContext, float width, float height)
{
	D3DXMATRIX viewMatrix;

	bool result;

	D3DXMatrixIdentity(&viewMatrix);

	VertexTypeT* vertices = new VertexTypeT[6];
	if (!vertices)
	{
		return;
	}

	vertices[0].position = D3DXVECTOR3((-800.0f / 2.0f), (-640.0f / 2.0f), 5.0f);  
	vertices[0].texture = D3DXVECTOR2(0.0f, 1.0f);
	vertices[0].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertices[0].tangent = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

	vertices[1].position = D3DXVECTOR3((800.0f / 2.0f), (640.0f / 2.0f), 5.0f);  
	vertices[1].texture = D3DXVECTOR2(1.0f, 0.0f);
	vertices[1].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertices[1].tangent = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

	vertices[2].position = D3DXVECTOR3((-800.0f / 2.0f), (640.0f / 2.0f), 5.0f);  
	vertices[2].texture = D3DXVECTOR2(0.0f, 0.0f);
	vertices[2].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertices[2].tangent = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

	vertices[3].position = D3DXVECTOR3((-800.0f / 2.0f), (-640.0f / 2.0f), 5.0f);  
	vertices[3].texture = D3DXVECTOR2(0.0f, 1.0f);
	vertices[3].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertices[3].tangent = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

	vertices[4].position = D3DXVECTOR3((800.0f / 2.0f), (-640.0f / 2.0f), 5.0f);  
	vertices[4].texture = D3DXVECTOR2(1.0f, 1.0f);
	vertices[4].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertices[4].tangent = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

	vertices[5].position = D3DXVECTOR3((800.0f / 2.0f), (640.0f / 2.0f), 5.0f);  
	vertices[5].texture = D3DXVECTOR2(1.0f, 0.0f);
	vertices[5].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertices[5].tangent = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	result = DContext->Map(loadingBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return;
	}

	VertexTypeT* verticesPtr = (VertexTypeT*)mappedResource.pData;
	
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexTypeT)* 6));
	
	DContext->Unmap(loadingBuffer, 0);

	delete[] vertices;
	vertices = 0;

	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexTypeT);
	offset = 0;

	DContext->IASetVertexBuffers(0, 1, &loadingBuffer, &stride, &offset);
	DContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3DXMATRIX orthoMatrix;
	D3DXMatrixOrthoLH(&orthoMatrix, (float)width, (float)height, 0.1f, 10000.0f);
	renderClass->SetProjectionMatrix(orthoMatrix);

	ID3D11ShaderResourceView* tempTex;

	tempTex = temptexture->GetTexture();

	if (menu->controlScreen)
		tempTex = menu->GetControlScreenTexture();

	if (pManager.endGame)
		tempTex = winLoading->GetTexture();

	result = renderClass->UpdateRender(DContext, viewMatrix, viewMatrix, tempTex, NULL, player->GetMaterial(), player->GetCurrentFrame());
	renderClass->Draw(DContext, 6, 0);
}

void WorldClass::Draw(ID3D11DeviceContext* DContext)
{
	D3DXMATRIX viewMatrix;

	bool result;

	viewMatrix = camera->GetView();
	renderClass->SetProjectionMatrix(projection);

	Player::Material material = player->GetMaterial();
	ID3D11ShaderResourceView* tempTex;
	ID3D11ShaderResourceView* tempNor;

	tempNor = 0;
	tempTex = 0;
	pManager.Draw(DContext, renderClass, viewMatrix, tempTex, tempNor, material);

	if (currentLevel != 0)
	{
		player->Apply(DContext);
		result = renderClass->UpdateRender(DContext, player->GetWorldMatrix(), viewMatrix, player->GetTextureMap(), player->GetNormalMap(), player->GetMaterial(), player->GetCurrentFrame());
		renderClass->Draw(DContext, rManager.player.vCount, 1);

		eManager->Draw(DContext, renderClass, viewMatrix);
	}

	menu->Draw(DContext, renderClass, viewMatrix, tempTex, tempNor, material);

	hud->Draw();

	vector<D3DXMATRIX> worldMatrices(pManager.GetWorldMatrices());
	worldMatrices.push_back(player->GetWorldMatrix());
	bBoxRender.Draw(worldMatrices, camera->GetView(), projection);
}

void WorldClass::DrawShadow(ID3D11DeviceContext* DContext)
{
	bool result;
	renderClass->SetProjectionMatrix(projection);

	pManager.DrawShadow(DContext, renderClass);

	player->Apply(DContext);
	result = renderClass->UpdateRenderShadow(DContext, player->GetWorldMatrix(), player->GetCurrentFrame());
	renderClass->Draw(DContext, rManager.player.vCount, 3);

	if (currentLevel != 0)
	{
		eManager->DrawShadow(DContext, renderClass);
	}	
}
