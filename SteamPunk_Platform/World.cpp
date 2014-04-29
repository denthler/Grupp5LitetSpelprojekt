#include "World.h"

WorldClass::WorldClass()
{
	
	camera = 0;
	model = 0;
	model2 = 0;
	model3 = 0;
	renderClass = 0;
	pointLight = 0;
	player = 0;
	input = 0;
	enemy = 0;
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

	model = new ModelClass();

	if(!model)
	{
		return false;
	}
	
	result = model->Initialize(DContext, L"platform.obj");
	if(!result)
	{
		return false;
	}

	model3 = new ModelClass();

	if (!model3)
	{
		return false;
	}
	/*
	result = model3->Initialize(DContext, L"platform3.obj");
	if (!result)
	{
		return false;
	}
	*/
	model2 = new ModelClass();

	if(!model2)
	{
		return false;
	}
	
	result = model2->Initialize(DContext, L"platform2.obj");
	if(!result)
	{
		return false;
	}

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
	
	result = player->Initialize(DContext, L"player.obj", D3DXVECTOR3 (0.0f, 10.0f, 0.0f));
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
	
	enemy = new NonFallingEnemy(DContext, L"player.obj", D3DXVECTOR3(0.0f, 10.0f, 0.0f));

	return true;
}

void WorldClass::HandleInput()
{
	input->Update();
	if(input->CheckKeyPress(DIK_SPACE))
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
	if(input->CheckKeyPress(DIK_W))
	{
		if(camera->IsNotFlipping())
		{
			camera->Flip();
			player->FlipGravity();
			enemy->FlipGravity();
		}
	}
	if(input->CheckKeyPress(DIK_S))
	{
		if(camera->IsNotFlipping())
		{
			camera->FlipS();
			player->FlipGravityS();
			enemy->FlipGravityS();
		}
	}
}

void WorldClass::Run(ID3D11DeviceContext* DContext, DWORD time)
{

	Update(time);

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
	if(model)
	{
		model->Shutdown();
		delete model;
		model = 0;
	}
	if(model2)
	{
		model2->Shutdown();
		delete model2;
		model2 = 0;
	}
	if (model3)
	{
		model3->Shutdown();
		delete model3;
		model3 = 0;
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
}

bool WorldClass::Update(float time)
{
	HandleInput();
	camera->Update(player->GetPosition());
	renderClass->UpdateFrustum(camera->GetView(), projection);
	std::vector<ModelClass::BoundingBox> tempBB;
	//tempBB.push_back(model->bBox);
	//tempBB.push_back(model2->bBox);
	//tempBB.push_back(model3->bBox);
	//player->Update(0.0f, tempBB); 
	pManager.Update(player->GetPosition(), tempBB);
	enemy->Update(time, tempBB);
	//
	//static float red = 0.0f;
	//static float redCount = 0.01f;
	//red += redCount;
	//if ((red > 1.0f) || (red < 0.0f))
	//	redCount *= -1.0f ;
	//
	player->Update(time, tempBB);
	pointLight->SetPosition(player->GetPosition().x, player->GetPosition().y, -50.0f);
	//pointLight->SetDiffuseColor(red, 0.5f, 0.5f, 1.0f);

	return true;
}

void WorldClass::Draw(ID3D11DeviceContext* DContext)
{
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;
	
	bool result;

	viewMatrix = camera->GetView();
	model->GetWorldMatrix(worldMatrix);
	
	ID3D11ShaderResourceView* tempTex;

	if (model->HasTexture(0))
	{
		tempTex = model->GetSubsetTexture(0);
	}
	else
	{
		tempTex = 0;
	}
	pManager.Draw(DContext, renderClass, viewMatrix, tempTex, pointLight, model->GetMaterial(0));

	/*
	for(int j = 0; j < model->GetSubsetCount(); j++)
	{
		if(model->IndexCount(j) > 0)
		{
			model->Apply(DContext, j);
			if(model->HasTexture(j))
			{
				tempTex = model->GetSubsetTexture(j);
			}
			else
			{
				tempTex = 0;
			}

			result = renderClass->UpdateRender(DContext, worldMatrix, viewMatrix, tempTex, pointLight, model->GetMaterial(j));
			if(!result)
			{
				return;
			}
			renderClass->Draw(DContext, model->IndexCount(j));
		}
	}

	for(int j = 0; j < model2->GetSubsetCount(); j++)
	{
		if(model2->IndexCount(j) > 0)
		{
			model2->Apply(DContext, j);
			if(model2->HasTexture(j))
			{
				tempTex = model2->GetSubsetTexture(j);
			}
			else
			{
				tempTex = 0;
			}

			result = renderClass->UpdateRender(DContext, worldMatrix, viewMatrix, tempTex, pointLight, model2->GetMaterial(j));
			if(!result)
			{
				return;
			}
			renderClass->Draw(DContext, model2->IndexCount(j));
		}
	}

	for (int j = 0; j < model3->GetSubsetCount(); j++)
	{
		if (model3->IndexCount(j) > 0)
		{
			model3->Apply(DContext, j);
			if (model3->HasTexture(j))
			{
				tempTex = model3->GetSubsetTexture(j);
			}
			else
			{
				tempTex = 0;
			}

			result = renderClass->UpdateRender(DContext, worldMatrix, viewMatrix, tempTex, pointLight, model3->GetMaterial(j));
			if (!result)
			{
				return;
			}
			renderClass->Draw(DContext, model3->IndexCount(j));
		}
	}
	*/
	for(int j = 0; j < player->GetSubsetCount(); j++)
	{
		if(player->IndexCount(j) > 0)
		{
			player->Apply(DContext, j);
			if(player->HasTexture(j))
			{
				tempTex = player->GetSubsetTexture(j);
			}
			else
			{
				tempTex = 0;
			}

			result = renderClass->UpdateRender(DContext, player->GetWorldMatrix(), viewMatrix, tempTex, pointLight, player->GetMaterial(j));
			if(!result)
			{
				return;
			}
			renderClass->Draw(DContext, player->IndexCount(j));
		}
	}

	for (int j = 0; j < enemy->GetSubsetCount(); j++)
	{
		if (enemy->IndexCount(j) > 0)
		{
			enemy->Apply(DContext, j);
			if (enemy->HasTexture(j))
			{
				tempTex = enemy->GetSubsetTexture(j);
			}
			else
			{
				tempTex = 0;
			}

			result = renderClass->UpdateRender(DContext, enemy->GetWorldMatrix(), viewMatrix, tempTex, pointLight, player->GetMaterial(j));
			if (!result)
			{
				return;
			}
			renderClass->Draw(DContext, enemy->IndexCount(j));
		}
	}
		
}