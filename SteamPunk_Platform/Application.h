//
//Application.h
//
#ifndef APPLICATION_H_
#define APPLICATION_H_

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "d3dx11d.lib")
#pragma comment(lib, "Effects11d.lib")

#include<Windows.h>
#define WIN32_LEAN_AND_MEAN
//
//includes
//
#include <list>
#include <d3d11.h>
#include <d3d10effect.h>
#include <d3d11shader.h>
#include <d3dx11effect.h>
#include "D3DX11async.h"
#include "Camera.h"
#include <D3DX10math.h>
#include "World.h"

#include "Screen.h"

class Application
{
public:
	Application();
	~Application();

	LRESULT CALLBACK Application::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	bool Initialized;
	bool mAppPaused;
	bool mMinimized;
	bool mMaximized;
	bool mResizing;
	int mClientWidth;
	int mClientHeight;

	HWND GetWindow()
	{
		return hwnd;
	}
	ID3D11ShaderResourceView* GetShadowMap(){ return shaderResourceViewShadow; };

	float GetElapsedTime();
	bool Initialize(HINSTANCE hInstance, int, int);
	bool InitializeD3D(int screenWidth, int screenHeight, bool vSync, bool Msaa);
	bool UpdateStates(int screenWidth, int screenHeight);
	void CleanUp();
	bool InitializeTimer();
	void Begin(float red, float green, float blue, float alpha);
	void BeginShadow();
	void End(bool);
	D3DXMATRIX GetProj()
	{
		return projectionMatrix;
	}
	ID3D11Device* GetDevice()
	{
		return D3DDevice;
	}
	ID3D11DeviceContext* GetDeviceContext()
	{
		return D3DDeviceContext;
	}
	void RunWorld();
	//void UpdateRender();
	void AddScreen(Screen * screen);
	void RemoveScreen(Screen * screen);

private:

	//D3D11
	ID3D11Device * D3DDevice;
	ID3D11DeviceContext * D3DDeviceContext;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11RasterizerState* rasterState;
	D3DXMATRIX projectionMatrix;
	IDXGISwapChain* swapChain;
	UINT m4xMsaaQuality;
	D3D11_VIEWPORT viewport;
	//
	HWND hwnd;
	WorldClass* m_World;

	//ShadowMap
	ID3D11DepthStencilView* depthStencilViewShadow;
	ID3D11ShaderResourceView* shaderResourceViewShadow;
	D3D11_VIEWPORT viewportShadow;

	//bool Initialized;	
	INT64 startT;
	INT64 freq;
	bool isHighPerformanceSupported;
	float ticksPerM;
	float frameTime;
	list<Screen *> screens;
};//application

static Application* appPointer = 0;
#endif

