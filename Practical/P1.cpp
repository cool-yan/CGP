// Ask the compiler to include minimal header files for our program.
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>
#include <cmath>
#include <string>
#include <dinput.h>
#include "FrameTimer.h"

using namespace std;

//--------------------------------------------------------------------

//    Window handle
enum Militia {
    MILITIA_DOWN,
    MILITIA_LEFT,
    MILITIA_RIGHT,
    MILITIA_UP
};

//---------------------------------------------------------------------------
FrameTimer timer = FrameTimer();
int framePerSecond = 60;
float delta_time = 1.0 / 60.0;
D3DXVECTOR3 gravity(0, 9.8, 0);

//---------------------------------------------------------------------------
HWND g_hWnd = NULL;
WNDCLASS wndClass;
MSG msg;
//	Define Direct3D 9.
IDirect3D9* direct3D9;
//	Define how the screen presents.
D3DPRESENT_PARAMETERS d3dPP;
IDirect3DDevice9* d3dDevice;
LPDIRECTINPUT8 dInput;
LPDIRECTINPUTDEVICE8  dInputKeyboardDevice;
LPDIRECTINPUTDEVICE8  dInputMouseDevice;

BYTE  diKeys[256];
DIMOUSESTATE mouseState;
D3DXVECTOR2 mousePos(200, 200);

int starMovingSpeed = 3, rotationSpeed = 3;
int starRotation = 0;
D3DXVECTOR2 starCenter = D3DXVECTOR2(200,200);

bool windowed = true;
LPDIRECT3DTEXTURE9 mouseTexture = NULL;
LPDIRECT3DTEXTURE9 texture = NULL;
LPDIRECT3DTEXTURE9 bgtexture = NULL;
LPDIRECT3DTEXTURE9 bg1texture = NULL;
LPDIRECT3DTEXTURE9 bg2texture = NULL;
LPDIRECT3DTEXTURE9 bg3texture = NULL;
LPD3DXSPRITE sprite = NULL;
D3DXVECTOR3 spritePosition(0, 0, 0);
D3DXVECTOR3 mousePosition(0, 0, 0);
RECT mouseRect;

RECT numberRect;
LPDIRECT3DTEXTURE9 numTexture = NULL;
D3DXVECTOR3 numPosition(0, 0, 0);

RECT boxRect;
LPDIRECT3DTEXTURE9 boxTexture = NULL;
LPDIRECT3DTEXTURE9 transparentBoxTexture = NULL;
LPDIRECT3DTEXTURE9 boxToDraw = NULL;
bool mouseOverBox = false;
D3DXVECTOR3 boxPosition(0, 0, 0);
bool boxMoving = false;


RECT explosionRect;
LPDIRECT3DTEXTURE9 explosionTexture = NULL;
D3DXVECTOR3 explosionPosition(0, 0, 0);
int currentFrame = 0;

//-------------------------------------------------------------------

RECT militiaRect;
LPDIRECT3DTEXTURE9 militiaTexture = NULL;
D3DXVECTOR3 militiaPosition(0, 0, 0);
float currentMilitiaFrame = 0;
float militiaDeltaFrame;
int militiaMovingSpeed = 3;
int militiaFPS = 20;
float militiaMass = 10;
D3DXVECTOR3 militiaVelocity(0, 0, 0);
D3DXVECTOR3 militiaAcceleration(0, 0, 0);
bool militiaOnGround = true;


D3DXVECTOR3 addForce(float force, float mass) {
    return D3DXVECTOR3(0,- force / mass,0);
}

D3DXVECTOR3 acclerate(D3DXVECTOR3 v_now, D3DXVECTOR3 acclerate) {
    return v_now += acclerate * delta_time;
}
D3DXVECTOR3 move(D3DXVECTOR3 velocity) {
    return velocity * delta_time;
}


//-------------------------------------------------

int r = 0, g = 0, b = 0;
int currentValue = 0;
int deltaValue = -1;
int speed = 5;


//--------------------------------------------------------------------

LPD3DXFONT font = NULL;
RECT textRect;

D3DXVECTOR2 getVerticOfACircle(int radius, int degree, int centerX, int centerY);

LPD3DXLINE line = NULL;
D3DXVECTOR2 lineVertices[] = { D3DXVECTOR2(200, 200), D3DXVECTOR2(200, 400) };
D3DXVECTOR2 line1Vertices[] = { getVerticOfACircle(100, 30, 200, 200), getVerticOfACircle(150, 30, 200,200) };
D3DXVECTOR2 line2Vertices[] = { getVerticOfACircle(100, 60, 200, 200), getVerticOfACircle(150, 60, 200,200) };
D3DXVECTOR2 line3Vertices[] = { getVerticOfACircle(100, 90, 200, 200), getVerticOfACircle(150, 90, 200,200) };
D3DXVECTOR2 line4Vertices[] = { getVerticOfACircle(100, 120, 200, 200), getVerticOfACircle(150, 120, 200,200) };
D3DXVECTOR2 line5Vertices[] = { getVerticOfACircle(100, 150, 200, 200), getVerticOfACircle(150, 150, 200,200) };
D3DXVECTOR2 line6Vertices[] = { getVerticOfACircle(100, 180, 200, 200), getVerticOfACircle(150, 180, 200,200) };
D3DXVECTOR2 line7Vertices[] = { getVerticOfACircle(100, 210, 200, 200), getVerticOfACircle(150, 210, 200,200) };
D3DXVECTOR2 line8Vertices[] = { getVerticOfACircle(100, 240, 200, 200), getVerticOfACircle(150, 240, 200,200) };
D3DXVECTOR2 line9Vertices[] = { getVerticOfACircle(100, 270, 200, 200), getVerticOfACircle(150, 270, 200,200) };
D3DXVECTOR2 line10Vertices[] = { getVerticOfACircle(100, 300, 200, 200), getVerticOfACircle(150, 300, 200,200) };
D3DXVECTOR2 line11Vertices[] = { getVerticOfACircle(100, 330, 200, 200), getVerticOfACircle(150, 330, 200,200) };
D3DXVECTOR2 line12Vertices[] = { getVerticOfACircle(100, 0, 200, 200), getVerticOfACircle(150, 0, 200,200)};
D3DXVECTOR2 lineHrVertices[] = { D3DXVECTOR2(200, 200), D3DXVECTOR2(200, 400) };
D3DXVECTOR2 lineMinVertices[] = { D3DXVECTOR2(200, 200), D3DXVECTOR2(200, 400) };
D3DXVECTOR2 startVertices[5];
int currentAngle = 0;


//--------------------------------------------------------------------


void GetStartPoint(D3DXVECTOR2 startVertices[], D3DXVECTOR2 center) {
    for (int i = 0; i <= 6; i++) {
        startVertices[i] = getVerticOfACircle(100, 144 * i + starRotation, starCenter.x, starCenter.y);
    }
}

float D2R(float degree) {
    return 3.1415926 * degree / 180;
}

D3DXVECTOR2 getVerticOfACircle(int radius, int degree, int centerX, int centerY) {
    int y = radius * sin(D2R(degree)) + centerY;
    int x = radius * cos(D2R(degree)) + centerX;

    return D3DXVECTOR2(x, y);
}

RECT getNumberRect(int num, int columnCount, int rowCount, int width, int height) {
    RECT numberRect;

    numberRect.left = (num % columnCount) * (width / columnCount);
    numberRect.top = (num / rowCount) * (height / rowCount);

    numberRect.right = numberRect.left + (width / columnCount);
    numberRect.bottom = numberRect.top + (height / rowCount);

    return numberRect;
}

string GetMousePos() {
    return "Mouse postion- x: "+to_string(mousePos.x) + " y: "+to_string(mousePos.y);
}

void ToggleFullScreen() {


}
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
        case VK_SPACE: {
            currentFrame = currentFrame!=24 ? currentFrame+1:0;
			explosionRect = getNumberRect(currentFrame, 5, 5, 500, 500);
            break;
        }

		case VK_UP:
			spritePosition.y -= 10;
			break;
		case VK_DOWN:
			spritePosition.y += 10;
			break;
		case VK_LEFT:
			spritePosition.x -= 10;
			break;
		case VK_RIGHT:
			spritePosition.x += 10;
			break;

        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
			numberRect = getNumberRect(wParam - '0', 4, 4, 128,128);
            break;
        case 'R':
            r = 1;
            g = 0;
            b = 0;
            break;
        case 'G':
            r = 0;
            g = 1;
            b = 0;
            break;
        case 'B':
            r = 0;
            g = 0;
            b = 1;
            break;

        case 'F':
            ToggleFullScreen();
            break;
            
        default:
            break;
        }
        break;

	case WM_MOUSEMOVE:
        mousePosition.x = (short)LOWORD(lParam);
        mousePosition.y = (short)HIWORD(lParam);
		break;

	case WM_LBUTTONDOWN:
		if (mouseOverBox) {
			boxMoving = true;
		}
        dInputMouseDevice->Acquire();
        dInputKeyboardDevice->Acquire();
		break;
	case WM_LBUTTONUP:
		boxMoving = false;
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

bool InitDirectX() {  

    direct3D9 = Direct3DCreate9(D3D_SDK_VERSION);
    ZeroMemory(&d3dPP, sizeof(d3dPP));
    //	Refer to Direct3D 9 documentation for the meaning of the members.
    d3dPP.Windowed = true;
    d3dPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dPP.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dPP.BackBufferCount = 1;
    d3dPP.BackBufferWidth = 800;
    d3dPP.BackBufferHeight = 600;
    d3dPP.hDeviceWindow = g_hWnd;

    //	Create a Direct3D 9 device.
    HRESULT hr = direct3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dPP, &d3dDevice);

    //	To Do: Cout out the message to indicate the failure.
    return (SUCCEEDED(hr));
 }

void PingPongEffect() {
    if (currentValue <= 0 || currentValue >= 255) {
        deltaValue *= -1;
    }
    currentValue = currentValue + deltaValue * speed;
}

void Update() {
    currentFrame = ++currentFrame % 24;
    explosionRect = getNumberRect(currentFrame, 5, 5, 500, 500);

    if (diKeys[DIK_UP] & 0x80)
    {
        starCenter.y -= starMovingSpeed;
    }
    if (diKeys[DIK_DOWN] & 0x80) {
        starCenter.y += starMovingSpeed;
    }
    if (diKeys[DIK_LEFT] & 0x80) {
        starCenter.x -= starMovingSpeed;
    }
    if (diKeys[DIK_RIGHT] & 0x80) {
        starCenter.x += starMovingSpeed;
    }
    if (diKeys[DIK_R] & 0x80) {
        starRotation += rotationSpeed;
    }
    currentAngle += 15;
}

void moveBox() {
	if (boxMoving) {
		boxPosition.x = mousePos.x - (boxRect.right - boxRect.left) / 2;
		boxPosition.y = mousePos.y - (boxRect.bottom - boxRect.top) / 2;
	}
}   

void InitText() {
    HRESULT hr = D3DXCreateFont(d3dDevice, 25, 0, 0, 1, false,
        DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, "Arial", &font);

    textRect.left = 100;
    textRect.top = 100;
    textRect.right = 500;
    textRect.bottom = 125;

    hr = D3DXCreateLine(d3dDevice, &line);

}


void SpriteRender() {

    //	Specify the "	" rectangle.
    RECT spriteRect;
    spriteRect.left = 60;
    spriteRect.top = 200;
    spriteRect.right = 135;
    spriteRect.bottom = 260;

    // Specify alpha blend will ensure that the sprite will render the background with alpha.
    sprite->Begin(D3DXSPRITE_ALPHABLEND);
    //sprite->Draw(numTexture, &numberRect, NULL, &numPosition, D3DCOLOR_XRGB(255, 255, 255));
	moveBox();

    if (mousePos.x >= boxPosition.x && mousePos.x <= boxPosition.x + (boxRect.right - boxRect.left) &&
        mousePos.y >= boxPosition.y && mousePos.y <= boxPosition.y + (boxRect.bottom - boxRect.top)) {
		mouseOverBox = true;
        sprite->Draw(transparentBoxTexture, &boxRect, NULL, &boxPosition, D3DCOLOR_XRGB(255, 255, 255));
    }
	else {
		mouseOverBox = false;
        sprite->Draw(boxTexture, &boxRect, NULL, &boxPosition, D3DCOLOR_XRGB(255, 255, 255));
    }
    
    sprite->Draw(explosionTexture, &explosionRect, NULL, &explosionPosition, D3DCOLOR_XRGB(255, 255, 255));
    
    // Sprite rendering. Study the documentation.
    //sprite->Draw(texture, NULL, NULL, NULL, D3DCOLOR_XRGB(255, 255, 255));
    //sprite->Draw(texture, &spriteRect, NULL, NULL, D3DCOLOR_XRGB(255, 255, 255));
    string mousePosStr = GetMousePos();
    font->DrawText(sprite, (mousePosStr.c_str()), mousePosStr.length(), &textRect, 0, D3DCOLOR_XRGB(255, 255, 255));

    //sprite->Draw(texture, &spriteRect, NULL, &spritePosition, D3DCOLOR_XRGB(255, 255, 255));
    D3DXVECTOR3 currentMPos(mousePos.x, mousePos.y, 0);
    sprite->Draw(mouseTexture, NULL, NULL, &currentMPos, D3DCOLOR_XRGB(255, 255, 255));
    // End sprite drawing
    sprite->End();
}


void Render() {
	//	Clear the back buffer.
	d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(128,128,128), 1.0f, 0);
	//	Begin the scene
	d3dDevice->BeginScene();
    SpriteRender();

    line->Begin();

    lineVertices[0] = getVerticOfACircle(0, currentAngle / 60, 200, 200);
    lineVertices[1] = getVerticOfACircle(125, currentAngle / 60, 200, 200);


    line->Draw(lineVertices, 2, D3DCOLOR_XRGB(255, 0, 0));
    line->Draw(line1Vertices, 2, D3DCOLOR_XRGB(0, 0, 0));
    line->Draw(line2Vertices, 2, D3DCOLOR_XRGB(0, 0, 0));
    line->Draw(line3Vertices, 2, D3DCOLOR_XRGB(0, 0, 0));
    line->Draw(line4Vertices, 2, D3DCOLOR_XRGB(0, 0, 0));
    line->Draw(line5Vertices, 2, D3DCOLOR_XRGB(0, 0, 0));
    line->Draw(line6Vertices, 2, D3DCOLOR_XRGB(0, 0, 0));
    line->Draw(line7Vertices, 2, D3DCOLOR_XRGB(0, 0, 0));
    line->Draw(line8Vertices, 2, D3DCOLOR_XRGB(0, 0, 0));
    line->Draw(line9Vertices, 2, D3DCOLOR_XRGB(0, 0, 0));
    line->Draw(line10Vertices, 2, D3DCOLOR_XRGB(0, 0, 0));
    line->Draw(line11Vertices, 2, D3DCOLOR_XRGB(0, 0, 0));
    line->Draw(line12Vertices, 2, D3DCOLOR_XRGB(0, 0, 0));
    GetStartPoint(startVertices, D3DXVECTOR2(200, 200));
    line->Draw(startVertices, 6, D3DCOLOR_XRGB(255,0,0));

    D3DXVECTOR2 lineVertices2[] = { getVerticOfACircle(0, currentAngle, 200, 200) ,getVerticOfACircle(150, currentAngle, 200,200)};
    
    line->Draw(lineVertices2, 2, D3DCOLOR_XRGB(0, 255, 0));
    line->End();

	d3dDevice->EndScene();
	//	Present the back buffer to screen
	d3dDevice->Present(NULL, NULL, NULL, NULL);
}

void InitDInput() {
    HRESULT hr = DirectInput8Create(GetModuleHandle(NULL), 0x0800, IID_IDirectInput8, (void**)&dInput, NULL);
    hr = dInput->CreateDevice(GUID_SysKeyboard, &dInputKeyboardDevice, NULL);
    dInputKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
    dInputKeyboardDevice->SetCooperativeLevel(g_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

    hr = dInput->CreateDevice(GUID_SysMouse, &dInputMouseDevice, NULL);
    dInputMouseDevice->SetDataFormat(&c_dfDIMouse);
    dInputMouseDevice->SetCooperativeLevel(g_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

}

void UpdateMousePos() {
    mousePos.x += mouseState.lX;
    mousePos.y += mouseState.lY;
}

RECT GetMilitiaRect(int num) {
    return getNumberRect(num, 4,4, 128, 256);
}

void GetInput() {
    dInputMouseDevice->GetDeviceState(sizeof(mouseState), &mouseState);
    dInputKeyboardDevice->GetDeviceState(256, diKeys);
    UpdateMousePos();
}
void InitMilitia() {
    militiaDeltaFrame = (float)militiaFPS / (float)framePerSecond;
    D3DXCreateTextureFromFile(d3dDevice, "assets/militia.png", &militiaTexture);
    militiaRect = GetMilitiaRect(0);
    militiaPosition = D3DXVECTOR3(200,200,0);
}

void CleanupMilitia() {
    militiaTexture->Release();
    militiaTexture = NULL;
}

void MilitiaPhysis() {
    if (militiaPosition.y > 300) {
        militiaOnGround = true;
        militiaPosition.y = 300;
        militiaVelocity = D3DXVECTOR3(0, 0, 0);
    }
    else {
        militiaVelocity =  acclerate(militiaVelocity, gravity);
    }
      militiaPosition += move(militiaVelocity);

}

void MilitiaUpdate() {
    for (int i = 0; i < timer.FramesToUpdate(); i++)
    {
        militiaMovingSpeed = diKeys[DIK_LSHIFT] & 0x80 ? 10 : 3;
        if (diKeys[DIK_UP] & 0x80)
        {
            currentMilitiaFrame += militiaDeltaFrame;
            militiaRect = GetMilitiaRect(MILITIA_UP * 4 + ((int)currentMilitiaFrame % 4));
            militiaPosition.y -= militiaMovingSpeed;
        }
        else if (diKeys[DIK_DOWN] & 0x80) {
            currentMilitiaFrame += militiaDeltaFrame;
            militiaRect = GetMilitiaRect(MILITIA_DOWN * 4 + ((int)currentMilitiaFrame % 4));
            militiaPosition.y += militiaMovingSpeed;
        }
        else if (diKeys[DIK_LEFT] & 0x80) {
            currentMilitiaFrame += militiaDeltaFrame;
            militiaRect = GetMilitiaRect(MILITIA_LEFT * 4 + ((int)currentMilitiaFrame % 4));
            militiaPosition.x -= militiaMovingSpeed;
        }
        else if (diKeys[DIK_RIGHT] & 0x80) {
            currentMilitiaFrame += militiaDeltaFrame;
            militiaRect = GetMilitiaRect(MILITIA_RIGHT * 4 + ((int)currentMilitiaFrame % 4));
            militiaPosition.x += militiaMovingSpeed;
        }
        else if (diKeys[DIK_SPACE] & 0x80 && militiaOnGround) {
            militiaOnGround = false;
            militiaAcceleration += addForce(100, militiaMass);
            militiaVelocity = acclerate(militiaVelocity, militiaAcceleration);
            cout << "something";
        }
        else if(currentMilitiaFrame!=0){
            currentMilitiaFrame = 0;
            militiaRect = GetMilitiaRect(0);
        }
    }
}
void MilitiaRender() {
    d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(128, 128, 128), 1.0f, 0);
    //	Begin the scene
    d3dDevice->BeginScene();
    sprite->Begin(D3DXSPRITE_ALPHABLEND);

    sprite->Draw(militiaTexture, &militiaRect, NULL, &militiaPosition, D3DCOLOR_XRGB(255, 255, 255));

    sprite->End();
    d3dDevice->EndScene();
    d3dDevice->Present(NULL, NULL, NULL, NULL);
}

void CleanupDInput() {
    dInputKeyboardDevice->Unacquire();
    dInputKeyboardDevice->Release();
    dInputKeyboardDevice = NULL;

    dInputMouseDevice->Unacquire();
    dInputMouseDevice->Release();
    dInputMouseDevice = NULL;

    //	Release DirectInput.
    dInput->Release();
    dInput = NULL;
}

void CleanupDirectX() {
	direct3D9->Release();
	direct3D9 = NULL;
    d3dDevice->Release();
    d3dDevice = NULL;
}

void InitSprite() {

    HRESULT hr = D3DXCreateSprite(d3dDevice, &sprite);
    
    hr = D3DXCreateTextureFromFile(d3dDevice, "assets/bg1.png", &texture);

    hr = D3DXCreateTextureFromFile(d3dDevice, "assets/pointer.png", &mouseTexture);
    mousePosition = D3DXVECTOR3(0, 0, 0);
    mouseRect.left = mouseRect.top = 0;
    mouseRect.right = mouseRect.bottom = 24;

    hr = D3DXCreateTextureFromFile(d3dDevice, "assets/numbers.bmp", &numTexture);
    numPosition = D3DXVECTOR3(0, 0, 0);
    numberRect.left = numberRect.top = 0;
    numberRect.right = numberRect.bottom = 32;

    hr = D3DXCreateTextureFromFile(d3dDevice, "assets/04.bmp", &boxTexture);

    hr = D3DXCreateTextureFromFileEx(d3dDevice, "assets/04.bmp", D3DX_DEFAULT, D3DX_DEFAULT,
        D3DX_DEFAULT, NULL, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED,
        D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_XRGB(0, 128, 64),
        NULL, NULL, &transparentBoxTexture);
    boxPosition = D3DXVECTOR3(200, 300, 0);
    boxRect = getNumberRect(3, 2, 2, 64, 64);

    hr = D3DXCreateTextureFromFile(d3dDevice, "assets/explosion-tran.png", &explosionTexture);
    explosionPosition = D3DXVECTOR3(0, 0, 0);
    explosionRect = getNumberRect(0, 5, 5, 500, 500);


    hr = D3DXCreateTextureFromFile(d3dDevice, "assets/bg1.png", &bg1texture);
    hr = D3DXCreateTextureFromFile(d3dDevice, "assets/bg2.png", &bg2texture);
    hr = D3DXCreateTextureFromFile(d3dDevice, "assets/bg3.png", &bg3texture);
    bgtexture = bg1texture;


    //hr = D3DXCreateTextureFromFileEx(d3dDevice, "01.bmp", D3DX_DEFAULT, D3DX_DEFAULT, 
    //									D3DX_DEFAULT, NULL, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, 
    //									D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_XRGB(255, 255, 255), 
    //									NULL, NULL, &texture);
}
int main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{

	ShowCursor(true);
    timer.Init(framePerSecond);

    CreateMyWindow();
	if (!InitDirectX())
		return 0;

    InitDInput();
    dInputKeyboardDevice->Acquire();
   
    InitSprite();
    InitText();
    InitMilitia();
    while (WindowIsRunning()) {
        GetInput();

        MilitiaPhysis();
        MilitiaUpdate();
        MilitiaRender();



		//Update();
        //Render();
        //	Begin to draw the lines.

    }

    sprite->Release();
    sprite = NULL;

    texture->Release();
    texture = NULL;
	mouseTexture->Release();
	mouseTexture = NULL;
    
	bg1texture->Release();
	bg2texture->Release();
	bg3texture->Release();
	bg1texture = NULL;
	bg2texture = NULL;
	bg3texture = NULL;
    line->Release();

    CleanupMilitia();
    CleanupDInput();
    CleanupDirectX();
	CleanupWindow();
    return 0;
}