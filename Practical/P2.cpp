// Ask the compiler to include minimal header files for our program.
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <d3d9.h>

//--------------------------------------------------------------------

//    Window handle
HWND g_hWnd = NULL;
WNDCLASS wndClass;
MSG msg;
//--------------------------------------------------------------------

//    Window Procedure, for event handling
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        //    The message is post when we destroy the window.
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_KEYDOWN:
        switch (wParam) {
        case 'Q':
        case VK_ESCAPE:
            PostQuitMessage(0);
            break;
        default:
            break;
        }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
//--------------------------------------------------------------------


void CreateMyWindow()
{
    ZeroMemory(&wndClass, sizeof(wndClass));

    wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hInstance = GetModuleHandle(NULL);
    wndClass.lpfnWndProc = WindowProcedure;
    wndClass.lpszClassName = "My Window";
    wndClass.style = CS_HREDRAW | CS_VREDRAW;

    RegisterClass(&wndClass);

    g_hWnd = CreateWindowEx(0, wndClass.lpszClassName, "First window app", WS_OVERLAPPEDWINDOW, 0, 100, 800, 600, NULL, NULL, GetModuleHandleA(NULL), NULL);
    ShowWindow(g_hWnd, 1);

    ZeroMemory(&msg, sizeof(msg));

}

bool WindowIsRunning()
{
    //   PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
    //   TranslateMessage(&msg);
    //   DispatchMessage(&msg);
       //return msg.message != WM_QUIT;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT)
            return false;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return true;
}

void CleanupWindow()
{
    UnregisterClass(wndClass.lpszClassName, GetModuleHandleA(NULL));
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//	To Do: Ensure window is created successfully then only create D3D9 device.
	CreateMyWindow();

	//	Define Direct3D 9.
	IDirect3D9* direct3D9 = Direct3DCreate9(D3D_SDK_VERSION);

	//	Define how the screen presents.
	D3DPRESENT_PARAMETERS d3dPP;
	ZeroMemory(&d3dPP, sizeof(d3dPP));

	//	Refer to Direct3D 9 documentation for the meaning of the members.
	d3dPP.Windowed = true;
	d3dPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dPP.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dPP.BackBufferCount = 1;
	d3dPP.BackBufferWidth = 400;
	d3dPP.BackBufferHeight = 300;
	d3dPP.hDeviceWindow = g_hWnd;

	IDirect3DDevice9* d3dDevice;
	//	Create a Direct3D 9 device.
	HRESULT hr = direct3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dPP, &d3dDevice);

	//	To Do: Cout out the message to indicate the failure.
	if (FAILED(hr))
		return 0;

	while (WindowIsRunning())
	{
		//	To Do:
		//	Update.

		//	Clear the back buffer.
		d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

		//	Begin the scene
		d3dDevice->BeginScene();

		//	To Do:
		//	Drawing.

		//	End the scene
		d3dDevice->EndScene();

		//	Present the back buffer to screen
		d3dDevice->Present(NULL, NULL, NULL, NULL);
	}

	//	Release the device when exiting.
	d3dDevice->Release();
	//	Reset pointer to NULL, a good practice.
	d3dDevice = NULL;

	CleanupWindow();

	return 0;
}
//--------------------------------------------------------------------