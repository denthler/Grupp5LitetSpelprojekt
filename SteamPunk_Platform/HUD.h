#pragma once
#include "PlatformManager.h"
#include "Button.h"
#include "Screen.h"
#include <D3DX11async.h>
#include <D3D11.h>
#include <D3DX10math.h>
#include <WinDef.h>

class HUD : public Screen
{
	Button startButton;
	ID3D11Device * device;
	ID3D11DeviceContext * deviceContext;
	ID3D11Buffer* triangleVertBuffer;
	ID3D11Buffer* quadVertBuffer;
	ID3D11VertexShader * VS;
	ID3D11PixelShader * PS;
	ID3D11VertexShader * textureVS;
	ID3D11PixelShader * texturePS;
	ID3D11ShaderResourceView * cog_fullTexture, * cog_emptyTexture;
	ID3D11SamplerState * samplerState;
	ID3D11InputLayout* vertLayout, * quadLayout;
	ID3D11BlendState * blendState;
	PlatformManager * platformManager;
public:
	HUD(ID3D11Device* device, ID3D11DeviceContext * deviceContext, HWND hwnd, D3DXMATRIX proj, HINSTANCE hInstance, PlatformManager * platformManager);
	~HUD();

	void Update();
	void Draw();
};