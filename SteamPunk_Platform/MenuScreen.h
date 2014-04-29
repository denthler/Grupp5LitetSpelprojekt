#pragma once

#include "Screen.h"
#include <D3DX11async.h>
#include <D3D11.h>
#include <D3DX10math.h>
#include <WinDef.h>

class MenuScreen : public Screen
{
	ID3D11Device * device;
	ID3D11DeviceContext * deviceContext;
public:
	MenuScreen(ID3D11Device* DContext, ID3D11DeviceContext * deviceContext, HWND hwnd, D3DXMATRIX proj, HINSTANCE hInstance);
	~MenuScreen();

	void Update();
	void Draw();
};