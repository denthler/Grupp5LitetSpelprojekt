#include "Application.h"

Application::Application()
{
}

Application::~Application()
{
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}
	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

bool Application::Initialize(HINSTANCE hInstance)
{

	WNDCLASSEX wndClass;
	memset(&wndClass, 0, sizeof(WNDCLASSEX));
	wndClass.hInstance = hInstance;
	wndClass.lpszClassName = L"D3DClass";
	wndClass.lpfnWndProc = (WNDPROC)WindowProc;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.cbSize = sizeof(WNDCLASSEX);


	RegisterClassEx(&wndClass);

	 hwnd = CreateWindow(L"D3DClass", L"SteamPunk", WS_OVERLAPPEDWINDOW,
		0, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, 0);

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	return true;
}

