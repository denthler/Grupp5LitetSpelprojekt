#include "Application.h"
#include "OBJ.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevious, LPSTR lpCmdLine, int cmd)
{
	Object object;
	object.LoadFromObjFile("C:/Users/User/Documents/GitHub/Grupp5LitetSpelprojekt/", "scalecube.obj");
	Application application;

	application.Initialize(hInstance);

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

		}
	}
	return 0;
}

