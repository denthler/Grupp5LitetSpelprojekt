//
//Application.h
//

//
//includes
//
#include <d3d11.h>

#ifndef APPLICATION_H_
#define APPLICATION_H_

class Application
{
public:
	Application();
	~Application();

	HWND GetWindow()
	{
		return hwnd;
	}
	bool Initialize(HINSTANCE);
private:

	ID3D11Device * D3DDevice;
	ID3D11DeviceContext * D3DDeviceContext;
	HWND hwnd;

};//application
#endif

