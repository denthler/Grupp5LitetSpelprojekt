#include "Application.h"


Application::Application()
{
	swapChain = 0;
	D3DDevice = 0;
	D3DDeviceContext = 0;
	renderTargetView = 0;
	depthStencilBuffer = 0;
	depthStencilState = 0;
	depthStencilView = 0;
	rasterState = 0;
	hwnd = 0;
	swapChain = 0;
	m4xMsaaQuality = 0;
	//Initialized = false;

	Initialized = false;
	mMinimized = false;
	mMaximized = false;
	mResizing = true;
	mClientWidth = 0;
	mClientHeight = 0;

	m_World = 0;

	startT = 0;
	freq = 0;
	ticksPerM = 0.0f;
	frameTime = 0.0f;
}

Application::~Application()
{
	for (auto it = screens.begin(); it != screens.end(); ++it)
	{
		delete *it;
	}
	CleanUp();
}

bool Application::InitializeTimer()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&startT);
	bool result = QueryPerformanceFrequency((LARGE_INTEGER*)&freq);

	if (!result)
	{
		if (freq != 1000)
		{
			freq = 1000;
			isHighPerformanceSupported = false;
			return false;
		}
	}
	else
	{
		isHighPerformanceSupported = true;
	}
	ticksPerM = (float)(freq / 1000);
	return true;
}

float Application::GetElapsedTime()
{
	INT64 current = 0.;
	float difference;
	if (isHighPerformanceSupported)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&current);
	}
	else
	{
		current = startT;
	}

	difference = (float)(current - startT);
	frameTime = difference / ticksPerM;
	
	startT = current;
	return frameTime;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
	}
	return appPointer->WndProc(hwnd, msg, wparam, lparam);
}

LRESULT CALLBACK Application::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg)
	{
	
	case WM_ACTIVATE:
		{
			if( LOWORD(wparam) == WA_INACTIVE )
			{
				mAppPaused = true;
			}
			else
			{
				mAppPaused = false;
			}
			return 0;
		}

		case WM_SIZE:
		{
		mClientWidth  = LOWORD(lparam);
		mClientHeight = HIWORD(lparam);
		if( Initialized )
		{
			if( wparam == SIZE_MINIMIZED )
			{
				mAppPaused = true;
				mMinimized = true;
				mMaximized = false;
			}
			else if( wparam == SIZE_MAXIMIZED )
			{
				mAppPaused = false;
				mMinimized = false;
				mMaximized = true;
				UpdateStates(mClientWidth, mClientHeight);
			}
			else if( wparam == SIZE_RESTORED )
			{
				
				// Restoring from minimized state?
				if( mMinimized )
				{
					mAppPaused = false;
					mMinimized = false;
					UpdateStates(mClientWidth, mClientHeight);
				}

				// Restoring from maximized state?
				else if( mMaximized )
				{
					mAppPaused = false;
					mMaximized = false;
					UpdateStates(mClientWidth, mClientHeight);
				}
				else if( mResizing )
				{
					// Do not do anything
				}
				else 
				{
					UpdateStates(mClientWidth, mClientHeight);
				}
			}
		}
		return 0;
		}
	case WM_ENTERSIZEMOVE:
		{
		mAppPaused = true;
		mResizing  = true;
		return 0;
		}

	case WM_EXITSIZEMOVE:
		{
		mAppPaused = false;
		mResizing  = false;
		UpdateStates(mClientWidth, mClientHeight);
		return 0;
		}

	case WM_GETMINMAXINFO:
		{
		((MINMAXINFO*)lparam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lparam)->ptMinTrackSize.y = 200; 
		return 0;
		}	
	}
		
	return DefWindowProc(hwnd, msg, wparam, lparam);		
}

bool Application::Initialize(HINSTANCE hInstance, int screenWidth, int screenHeight)
{
	appPointer = this;

	WNDCLASSEX wndClass;
	memset(&wndClass, 0, sizeof(WNDCLASSEX));
	wndClass.hInstance = hInstance;
	wndClass.lpszClassName = L"D3DClass";
	wndClass.lpfnWndProc = (WNDPROC)WindowProc;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.cbSize = sizeof(WNDCLASSEX);


	RegisterClassEx(&wndClass);

	 hwnd = CreateWindow(L"D3DClass", L"SteamPunk", WS_OVERLAPPEDWINDOW,
		0, 0, screenWidth, screenHeight, NULL, NULL, hInstance, 0);

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	Initialized = true;
	return true;
}

void Application::CleanUp()
{
	if(swapChain)
	{
		swapChain->SetFullscreenState(false, NULL);
	}

	if(rasterState)
	{
		rasterState->Release();
		rasterState = 0;
	}

	if(depthStencilView)
	{
		depthStencilView->Release();
		depthStencilView = 0;
	}

	if(depthStencilState)
	{
		depthStencilState->Release();
		depthStencilState = 0;
	}

	if(depthStencilBuffer)
	{
		depthStencilBuffer->Release();
		depthStencilBuffer = 0;
	}

	if(renderTargetView)
	{
		renderTargetView->Release();
		renderTargetView = 0;
	}

	if(D3DDeviceContext)
	{
		D3DDeviceContext->Release();
		D3DDeviceContext = 0;
	}

	if(D3DDevice)
	{
		D3DDevice->Release();
		D3DDevice = 0;
	}

	if(swapChain)
	{
		swapChain->Release();
		swapChain = 0;
	}

	if(m_World)
	{
		m_World->CleanUp();
		delete m_World;
		m_World = 0;
	}
}

void Application::RunWorld()
{
	/*
	m_World->Update();
	Begin(1.0f, 0.0f, 0.0f, 1.0f);
	m_World->Draw(D3DDeviceContext);
	End(true);
	*/
}
#pragma region D3D11
bool Application::InitializeD3D(int screenWidth, int screenHeight, bool vSync, bool Msaa)
{
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* aOut;
	unsigned int numModes, i, stringLength;
	unsigned int numerator = 60, denominator = 1;
	DXGI_MODE_DESC* dML;
	DXGI_ADAPTER_DESC aDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC sCD;
	D3D_FEATURE_LEVEL featureLevel;

	result = D3D11CreateDevice(
			0,                 
			D3D_DRIVER_TYPE_HARDWARE,
			0,                 
			NULL, 
			0, 0,              
			D3D11_SDK_VERSION,
			&D3DDevice,
			&featureLevel,
			&D3DDeviceContext);

	if( featureLevel != D3D_FEATURE_LEVEL_11_0 )
	{
		return false;
	}

	IDXGIDevice* dxgiDevice = 0;
	(D3DDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));
	if(FAILED(result))
	{
		return false;
	}
	
	(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&adapter));

	(adapter->GetParent(__uuidof(IDXGIFactory), (void**)&factory));
	result = adapter->EnumOutputs(0, &aOut);
	if(FAILED(result))
	{
		return false;
	}
	result = aOut->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, 
		&numModes, NULL);
	if(FAILED(result))
	{
		return false;
	}
	dML = new DXGI_MODE_DESC[numModes];

	result = aOut->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, 
		&numModes, dML);
	if(FAILED(result))
	{
		return false;
	}

	for(i = 0; i < numModes; i++)
	{

		if(dML[i].Width == (unsigned int)screenWidth)
		{
			if(dML[i].Height == (unsigned int)screenHeight)
			{

				numerator = dML[i].RefreshRate.Numerator;
				denominator = dML[i].RefreshRate.Denominator;
			}
		}

	}

	result = adapter->GetDesc(&aDesc);
	if(FAILED(result))
	{
		return false;
	}

	//int videoCMemory = (int)(aDesc.DedicatedVideoMemory / 1024 / 1024);
	char m_videoCDesc;
	//wcstombs_s(&stringLength, m_videoCDesc, 128, aDesc.Description, 128);
	
	(D3DDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality));
	//assert( m4xMsaaQuality > 0 );

	ZeroMemory(&sCD, sizeof(sCD));
	sCD.BufferCount = 1;
	sCD.BufferDesc.Width = screenWidth;
	sCD.BufferDesc.Height = screenHeight;
	sCD.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	if(vSync)
	{
		sCD.BufferDesc.RefreshRate.Numerator = numerator;
		sCD.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		sCD.BufferDesc.RefreshRate.Numerator = 0;
		sCD.BufferDesc.RefreshRate.Denominator = 1;
	}

	if( Msaa )
	{
		sCD.SampleDesc.Count   = 4;
		sCD.SampleDesc.Quality = m4xMsaaQuality-1;
	}
	else
	{
		sCD.SampleDesc.Count   = 1;
		sCD.SampleDesc.Quality = 0;
	}

	sCD.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	sCD.OutputWindow = hwnd;

	sCD.Windowed = true;

	sCD.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sCD.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	sCD.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	sCD.Flags = 0;

	factory->CreateSwapChain(D3DDevice, &sCD, &swapChain);

	factory->MakeWindowAssociation(hwnd ,DXGI_MWA_NO_WINDOW_CHANGES);

	delete[] dML;  
	dML = 0;

	aOut->Release();
	aOut = 0;

	adapter->Release();
	adapter = 0;

	factory->Release();
	factory = 0;

	D3D11_RASTERIZER_DESC rasterDesc;

	rasterDesc.AntialiasedLineEnable = false;
	//rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	result = D3DDevice->CreateRasterizerState(&rasterDesc, &rasterState);
	if(FAILED(result))
	{
		return false;
	}

	D3DDeviceContext->RSSetState(rasterState);

	if(!UpdateStates(screenWidth, screenHeight))
		return false;

	/*
	m_World = new WorldClass();
	if(!m_World)
		return false;

	m_World->Initialize(GetDevice(), GetWindow(), GetProj());
	*/
	//
	if (!InitializeTimer())
		return false;


	
	return true;
}

bool Application::UpdateStates(int screenWidth, int screenHeight)
{

	HRESULT result;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	float fieldOfView, screenAspect;

	if(renderTargetView)
	{
		renderTargetView->Release();
		renderTargetView = 0;
	}

	if(depthStencilView)
	{
		depthStencilView->Release();
		depthStencilView = 0;
	}

	if(depthStencilBuffer)
	{
		depthStencilBuffer->Release();
		depthStencilBuffer = 0;
	}

	(swapChain->ResizeBuffers(1, screenWidth, screenHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if(FAILED(result))
	{
		return false;
	}

	result = D3DDevice->CreateRenderTargetView(backBufferPtr, NULL, &renderTargetView);
	if(FAILED(result))
	{
		return false;
	}

	backBufferPtr->Release();
	backBufferPtr = 0;

	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	bool MSAA4X = true;
	if (MSAA4X)
	{
		depthBufferDesc.SampleDesc.Count   = 4;
		depthBufferDesc.SampleDesc.Quality = m4xMsaaQuality-1;
	}

	else
	{
		depthBufferDesc.SampleDesc.Count   = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
	}
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	result = D3DDevice->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuffer);
	if(FAILED(result))
	{
		return false;
	}

	result = D3DDevice->CreateDepthStencilView(depthStencilBuffer, 0,
		&depthStencilView);
	if(FAILED(result))
	{
		return false;
	}

	viewport.Width = (float)screenWidth;
	viewport.Height = (float)screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	fieldOfView = (float)D3DX_PI / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;

	D3DXMatrixPerspectiveFovLH(&projectionMatrix, fieldOfView, screenAspect, 0.1f, 10000.0f);

	// Shadow Map
	viewportShadow.TopLeftX = 0.0f;
	viewportShadow.TopLeftY = 0.0f;
	viewportShadow.Width = (float)(1024);
	viewportShadow.Height = (float)(1024);
	viewportShadow.MinDepth = 0.0f;
	viewportShadow.MaxDepth = 1.0f;

	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = 1024;
	texDesc.Height = 1024;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	ID3D11Texture2D* depthMap = 0;
	D3DDevice->CreateTexture2D(&texDesc, 0, &depthMap);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsv;
	dsv.Flags = 0;
	dsv.Format = DXGI_FORMAT_D32_FLOAT;
	dsv.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsv.Texture2D.MipSlice = 0;
	D3DDevice->CreateDepthStencilView(depthMap, &dsv, &depthStencilViewShadow);

	D3D11_SHADER_RESOURCE_VIEW_DESC srv;
	srv.Format = DXGI_FORMAT_R32_FLOAT;
	srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv.Texture2D.MipLevels = 1;
	srv.Texture2D.MostDetailedMip = 0;
	D3DDevice->CreateShaderResourceView(depthMap, &srv, &shaderResourceViewShadow);

	return true;
}

void Application::Begin(float red, float green, float blue, float alpha)
{
	float color[4];

	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	D3DDeviceContext->RSSetViewports(1, &viewport);
	D3DDeviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	D3DDeviceContext->ClearRenderTargetView(renderTargetView, color);
	D3DDeviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);	
}

void Application::BeginShadow()
{
	D3DDeviceContext->RSSetViewports(1, &viewportShadow);
	D3DDeviceContext->OMSetRenderTargets(0, 0, depthStencilViewShadow);

	D3DDeviceContext->ClearDepthStencilView(depthStencilViewShadow, D3D11_CLEAR_DEPTH, 1.0f, 0);	
}

void Application::End(bool vSync)
{
	
	if(vSync)
	{
		swapChain->Present(1, 0);
	}
	else
	{
		swapChain->Present(0, 0);
	}
}

void Application::AddScreen(Screen * screen)
{
	screens.push_back(screen);
}

void Application::RemoveScreen(Screen * screen)
{
	delete screen;
	screens.remove(screen);
}

#pragma endregion