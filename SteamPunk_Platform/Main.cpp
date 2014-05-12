#include "Application.h"
//#include "World.h"
#include "MenuScreen.h"
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevious, LPSTR lpCmdLine, int cmd)
{	
	Application application;
	WorldClass world;
	application.Initialize(hInstance, 800, 640);
	application.InitializeD3D(800, 640, true, true);
	world.Initialize(application.GetDevice(), application.GetWindow(), application.GetProj(), hInstance);
	MenuScreen menu(application.GetDevice(), application.GetDeviceContext(), application.GetWindow(), application.GetProj(), hInstance);


	MSG msg;
	memset(&msg, 0, sizeof(MSG));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, application.GetWindow(), 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{			
			world.Update(application.GetElapsedTime(), application.GetDevice());

			application.BeginShadow();
			world.DrawShadow(application.GetDeviceContext());
			world.renderClass->setShadowMap(application.GetShadowMap());

			application.Begin(1.0f, 0.0f, 0.0f, 1.0f);
			world.Draw(application.GetDeviceContext());
			menu.Draw();
			application.End(true);
		}
	}
	application.CleanUp();
	world.CleanUp();
	return 0;
}



