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
#include "AudioManager.h"

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
float delta_time  = 1.0 / 60.0;
D3DXVECTOR3 gravity(0, 9.8, 0);
float time_factor = 10;
//----------------------------------------------------------------------------

AudioManager* audioManager = new AudioManager();


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

int windowWidth = 800, windowHeight = 600;
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
RECT backgroundRect;
LPDIRECT3DTEXTURE9 backgroundTexture = NULL;
D3DXVECTOR3 backgroundPosition(0, 0, 0);

RECT militiaRect;
LPDIRECT3DTEXTURE9 militiaTexture = NULL;
D3DXVECTOR3 militiaPosition(0, 0, 0);
float currentMilitiaFrame = 0;
float militiaDeltaFrame;
int militiaDefaultMovingSpeed = 10;
int militiaFPS = 20;
float militiaMass = 3;
D3DXVECTOR3 militiaVelocity(0, 0, 0);
D3DXVECTOR3 militiaAcceleration(0, 0, 0);
bool militiaOnGround = false;
float jumpingForce = 1000;

D3DXVECTOR3 addForce(float force, float mass) {
    return D3DXVECTOR3(0,- force / mass,0);
}

D3DXVECTOR3 acclerate(D3DXVECTOR3 v_now, D3DXVECTOR3 acclerate) {
    return v_now += acclerate * time_factor * delta_time;
}

D3DXVECTOR3 move(D3DXVECTOR3 velocity) {
    return velocity * time_factor * delta_time;
}


//-------------------------------------------------

int r = 0, g = 0, b = 0;
int currentValue = 0;
int deltaValue = -1;
int speed = 5;


//--------------------------------------------------------------------


RECT getNumberRect(int num, int columnCount, int rowCount, int width, int height) {
    RECT numberRect;

    numberRect.left = (num % columnCount) * (width / columnCount);
    numberRect.top = (num / rowCount) * (height / rowCount);

    numberRect.right = numberRect.left + (width / columnCount);
    numberRect.bottom = numberRect.top + (height / rowCount);

    return numberRect;
}

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
// SpaceShip
float deaccelerationInSpace = 10.0f;

D3DXVECTOR3 player1Direction(0, 0, 0);
D3DXVECTOR3 player1Velocity(0, 0, 0);
D3DXVECTOR3 player1Acceleration(0, 0, 0);
D3DXVECTOR3 player1EngineForce(0, 0, 0);
float player1Mass = 3;
float player1EnginePower = 2;
int player1RotationSpeed = 1;
int player1RotationFactor = 10;

D3DXMATRIX player1WorldMatrix;
LPDIRECT3DTEXTURE9 player1Texture = NULL;
int playerSpriteWidth = 64;
int playerSpriteHeight = 64;
D3DXVECTOR2 player1Center(playerSpriteWidth / 4, playerSpriteHeight / 4);
D3DXVECTOR2 player1ScaleCenter(playerSpriteWidth / 4, playerSpriteHeight / 4);
D3DXVECTOR2 player1Scale(1, 1);

RECT player1Rect;
D3DXVECTOR3 player1Position(0, 0, 0);
float currentplayer1Frame = 0;
int player1MaxFrame = 2;
int player1FPS = 20;

class Bullet {
public:
	D3DXVECTOR2 position;
	D3DXVECTOR2 velocity;
	D3DXVECTOR3 explosionPosition;
	float rotationAngle;
    int currentFrame;
	bool active;
	bool explode;
	void Explode() {
		audioManager->playSound("assets/Audio/explosionCrunch_003.ogg");
		explode = true;
		active = false;
		explosionPosition = D3DXVECTOR3(position.x-51, position.y-51, 0);
		currentFrame = 0;
	}
};

class SpaceShip {
private:
	D3DXVECTOR2 position;
	D3DXMATRIX worldMatrix;
	float rotationAngle;
	int rotationSpeed;
    int rotationFactor;
	int maxFrame;
	int fps;
	int bulletActiveCount = 0;
	float enginePower;
    LPDIRECT3DTEXTURE9 texture = NULL;

    LPDIRECT3DTEXTURE9 bulletTexture = NULL;
	RECT bulletRect;
    D3DXVECTOR2 bCenter;
    D3DXVECTOR2 bScaleCenter;
    D3DXVECTOR2 bScale;
	Bullet bullet[50];

    RECT rect;
    int spriteWidth = 64;
    int spriteHeight = 64;
    D3DXVECTOR2 center;
    D3DXVECTOR2 scaleCenter;
    D3DXVECTOR2 scale;
	float maxVelocity = 10.0f;

public:
	float mass;
    D3DXVECTOR2 velocity;
	SpaceShip(D3DXVECTOR2 pos, 
        D3DXVECTOR2 vel, 
        int rotSpeed, 
        int maxF, 
        int fpsVal, 
        float m, 
        float engPower, 
        LPDIRECT3DTEXTURE9 tex,
        LPDIRECT3DTEXTURE9 btex) {
		position = pos;
		velocity = vel;
		rotationSpeed = rotSpeed;
		maxFrame = maxF;
		fps = fpsVal;
		mass = m;
		enginePower = engPower;
		texture = tex;
		bulletTexture = btex;
		center = D3DXVECTOR2(spriteWidth / 4, spriteHeight / 4);
		scaleCenter = D3DXVECTOR2(spriteWidth / 4, spriteHeight / 4);
		scale = D3DXVECTOR2(1, 1);
        rect.left = 32;
        rect.right = 64;
        rect.top = 0;
        rect.bottom = 32;

        

		bScale = D3DXVECTOR2(1, 1);
		bScaleCenter = D3DXVECTOR2(16, 16);
		bCenter = D3DXVECTOR2(16, 16);
   
		bulletRect.left = 0;
		bulletRect.top = 0;
		bulletRect.bottom = 16;
		bulletRect.right = 16;


        HRESULT hr = D3DXCreateTextureFromFile(d3dDevice, "assets/explosion-tran.png", &explosionTexture);
        explosionRect = getNumberRect(0, 5, 5, 500, 500);
	}

    D3DXVECTOR2 GetCenter() {
        return position;
    }

	void Render(LPD3DXSPRITE sprite) {
		D3DXMatrixTransformation2D(&worldMatrix, &scaleCenter, 0, &scale, &center, rotationAngle, &position);
		sprite->SetTransform(&worldMatrix);
        sprite->Draw(texture, &rect, NULL, NULL, D3DCOLOR_XRGB(255, 255, 255));
		D3DXMatrixIdentity(&worldMatrix);
		sprite->SetTransform(&worldMatrix);

		for (Bullet& b : bullet) {
                if (b.active) {
                        D3DXMatrixTransformation2D(&worldMatrix, &bScaleCenter, 0, &bScale, &bCenter, b.rotationAngle, &(b.position));
                        sprite->SetTransform(&worldMatrix);
                        sprite->Draw(bulletTexture, &bulletRect, NULL, NULL, D3DCOLOR_XRGB(255, 255, 255));
                        D3DXMatrixIdentity(&worldMatrix);
                        sprite->SetTransform(&worldMatrix);
                    }
                if (b.explode) {
                    if (b.currentFrame == 23) {
                        b.explode = false;
                        b.currentFrame = 0;
                    }
                    explosionRect = getNumberRect(b.currentFrame++, 5, 5, 500, 500);
                    sprite->Draw(explosionTexture, &explosionRect, NULL, &b.explosionPosition, D3DCOLOR_XRGB(255, 255, 255));
                }
            }

	}

	void Shoot(D3DXVECTOR2 vec2targ) {
		bulletActiveCount++;
        if(bulletActiveCount != 50)
		    for (Bullet& b : bullet) {
			    if (!b.active && !b.explode) {
				    D3DXVec2Normalize(&vec2targ, &vec2targ);
				    b.position = position;
				    b.velocity = vec2targ * 10.0f;
				    b.rotationAngle = atan2(b.velocity.y, b.velocity.x) + D3DXToRadian(90);
				    b.active = true;
				    break;
			    }
		    }
	}


    void Update() {

        for(Bullet &b:bullet){
            if (b.active) {
                b.position += b.velocity;
                if (b.position.x < 0 || b.position.x > windowWidth || b.position.y < 0 || b.position.y > windowHeight) {
                    b.active = false;
					bulletActiveCount--;
                }
                D3DXVECTOR2 vec2targ = (D3DXVECTOR2(player1Position.x, player1Position.y) - b.position);
                if (D3DXVec2LengthSq(&vec2targ) < pow(16 + 8, 2)) {
                    b.active = false;
                    bulletActiveCount--;
                    player1Velocity.x += b.velocity.x / player1Mass / 3;
                    player1Velocity.y += b.velocity.y / player1Mass / 3;
                    b.Explode();
                }
            }
        }


		float movingSpeed = 0;
        D3DXVECTOR2 vec2targ = (D3DXVECTOR2(player1Position.x, player1Position.y) - position);
		if (D3DXVec2LengthSq(&vec2targ) > (40000)) {
			D3DXVec2Normalize(&velocity, &vec2targ);
			movingSpeed = 1.0f;
		    velocity *= enginePower * movingSpeed;
		}
        if(diKeys[DIK_SPACE] & 0x80) {
            Shoot(vec2targ);
        }

        if (D3DXVec2LengthSq(&velocity) > 0.01f) {
			rotationAngle = atan2(velocity.y, velocity.x) + D3DXToRadian(90);
			velocity -= velocity / deaccelerationInSpace / 4;
        }
        if(velocity.x > maxVelocity)
			velocity.x = maxVelocity;
		if (velocity.y > maxVelocity)
			velocity.y = maxVelocity;
		position += velocity;
        if (position.x <= 0 && velocity.x < 0
            ||
            position.x >= windowWidth - spriteWidth / 2 && velocity.x > 0) {
            velocity.x *=-1;
        }
        if (position.y <= 0 && velocity.y < 0
            ||
            position.y >= windowHeight - spriteHeight / 2 && velocity.y > 0) {
            velocity.y *=-1;
        }
    }
};

SpaceShip *player2 = NULL;

//----------------------------------------------

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
    
    g_hWnd = CreateWindowEx(0, wndClass.lpszClassName, "First window app", WS_OVERLAPPEDWINDOW, 0, 100, windowWidth, windowHeight, NULL, NULL, GetModuleHandleA(NULL), NULL);
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
    d3dPP.BackBufferWidth = windowWidth;
    d3dPP.BackBufferHeight = windowHeight;
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

void GetInput() {
    dInputMouseDevice->GetDeviceState(sizeof(mouseState), &mouseState);
    dInputKeyboardDevice->GetDeviceState(256, diKeys);
    UpdateMousePos();
}

RECT GetMilitiaRect(int num) {
    return getNumberRect(num, 4,4, 128, 256);
}
void InitMilitia() {
    HRESULT hr = D3DXCreateFont(d3dDevice, 25, 0, 0, 1, false,
        DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, "Arial", &font);

    textRect.left = 100;
    textRect.top = 100;
    textRect.right = 500;
    textRect.bottom = 125;

    militiaDeltaFrame = (float)militiaFPS / (float)framePerSecond;
    D3DXCreateTextureFromFile(d3dDevice, "assets/militia.png", &militiaTexture);
    militiaRect = GetMilitiaRect(0);
    militiaPosition = D3DXVECTOR3(200,200,0);

	D3DXCreateTextureFromFile(d3dDevice, "assets/spritesheet-tiles-double.png", &backgroundTexture);
	backgroundRect = getNumberRect(182, 18,18, 4095,4095);
	backgroundPosition = D3DXVECTOR3(200, 364, 0);

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
    else if(!militiaOnGround){
        militiaVelocity =  acclerate(militiaVelocity, gravity);
    }
    militiaPosition += move(militiaVelocity);

}
void MilitiaUpdate() {
    for (int i = 0; i < timer.FramesToUpdate(); i++)
    {
        float militiaMovingSpeed = diKeys[DIK_LSHIFT] & 0x80 ? militiaDefaultMovingSpeed*3 : militiaDefaultMovingSpeed;
		if (militiaOnGround)
			militiaVelocity = D3DXVECTOR3(0, 0, 0);
        if (diKeys[DIK_UP] & 0x80)
        {
            currentMilitiaFrame += militiaDeltaFrame;
            militiaRect = GetMilitiaRect(MILITIA_UP * 4 + ((int)currentMilitiaFrame % 4));
			militiaVelocity.y = -militiaMovingSpeed;
        }
        else if (diKeys[DIK_DOWN] & 0x80) {
            currentMilitiaFrame += militiaDeltaFrame;
            militiaRect = GetMilitiaRect(MILITIA_DOWN * 4 + ((int)currentMilitiaFrame % 4));
            militiaVelocity.y = militiaMovingSpeed;
        }
        else if (diKeys[DIK_LEFT] & 0x80) {
            currentMilitiaFrame += militiaDeltaFrame;
            militiaRect = GetMilitiaRect(MILITIA_LEFT * 4 + ((int)currentMilitiaFrame % 4));
			militiaVelocity.x = -militiaMovingSpeed;
        }
        else if (diKeys[DIK_RIGHT] & 0x80) {
            currentMilitiaFrame += militiaDeltaFrame;
            militiaRect = GetMilitiaRect(MILITIA_RIGHT * 4 + ((int)currentMilitiaFrame % 4));
            militiaVelocity.x = militiaMovingSpeed;
        }
        else if(currentMilitiaFrame!=0){
            currentMilitiaFrame = 0;
            militiaRect = GetMilitiaRect(0);
        }
        if (diKeys[DIK_SPACE] & 0x80 && militiaOnGround) {
            militiaOnGround = false;
            militiaAcceleration = addForce(jumpingForce, militiaMass);
            militiaVelocity = acclerate(militiaVelocity, militiaAcceleration);
        }
    }
}
void MilitiaRender() {
    d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(128, 128, 128), 1.0f, 0);
    //	Begin the scene
    d3dDevice->BeginScene();
    sprite->Begin(D3DXSPRITE_ALPHABLEND);

    sprite->Draw(militiaTexture, &militiaRect, NULL, &militiaPosition, D3DCOLOR_XRGB(255, 255, 255));
    backgroundPosition.x = -100;
	sprite->Draw(backgroundTexture, &backgroundRect, NULL, &backgroundPosition, D3DCOLOR_XRGB(255, 255, 255));
	backgroundPosition.x += 226;
	sprite->Draw(backgroundTexture, &backgroundRect, NULL, &backgroundPosition, D3DCOLOR_XRGB(255, 255, 255));
    backgroundPosition.x += 226;
	sprite->Draw(backgroundTexture, &backgroundRect, NULL, &backgroundPosition, D3DCOLOR_XRGB(255, 255, 255));
    backgroundPosition.x += 226;
	sprite->Draw(backgroundTexture, &backgroundRect, NULL, &backgroundPosition, D3DCOLOR_XRGB(255, 255, 255));

    string vStr = "Velocity y: "+to_string(militiaVelocity.y);
    font->DrawTextA(sprite, vStr.c_str(), vStr.length(), &textRect, 0, D3DCOLOR_XRGB(255, 255, 255));

    sprite->End();
    d3dDevice->EndScene();
    d3dDevice->Present(NULL, NULL, NULL, NULL);
}

D3DXVECTOR3 addForce(float force, float mass, D3DXVECTOR3 direction, D3DXVECTOR3 currentVelocity) {
	D3DXVECTOR3 acceleration = D3DXVECTOR3(0, 0, 0);
	acceleration.x = (force / mass) * direction.x;
	acceleration.y = (force / mass) * direction.y;
	return currentVelocity + acceleration;
}

bool collided = false;
D3DXVECTOR3 CalculateCollisionVelocity(D3DXVECTOR2 pos1,
    float radius1,
    D3DXVECTOR3 velocity1,
    SpaceShip& p2) {
    D3DXVECTOR2 distance = pos1 - p2.GetCenter();

    if (D3DXVec2LengthSq(&distance) <= pow(radius1 + radius1, 2) && !collided) {
        collided = true;
        D3DXVECTOR2 v1 = D3DXVECTOR2(velocity1.x, velocity1.y);
        D3DXVECTOR2 relativeVelocity = p2.velocity - v1;
        D3DXVECTOR2 n;
        D3DXVec2Normalize(&n, &distance);
        float dotProduct = D3DXVec2Dot(&relativeVelocity, &n);

		float j = -2 * dotProduct / (1 / player1Mass + 1 / p2.mass);

        v1 -= j/ player1Mass * n;
        p2.velocity += j/ p2.mass * n;
        velocity1.x = v1.x;
        velocity1.y = v1.y;
    }
    else if (D3DXVec2LengthSq(&distance) > pow(radius1 + radius1, 2) &&collided) {
		collided = false;
    }
    return velocity1;
}

void InitSpaceShip() {
	LPDIRECT3DTEXTURE9 bulletTexture = NULL;
	D3DXCreateTextureFromFile(d3dDevice, "assets/practical9.png", &player1Texture);
	D3DXCreateTextureFromFile(d3dDevice, "assets/bullet.png", &bulletTexture);
	player1Rect = getNumberRect(0, 2, 2, playerSpriteWidth, playerSpriteHeight);
	player1Position = D3DXVECTOR3(200, 200, 0);
	player2 = new SpaceShip(D3DXVECTOR2(400, 400), D3DXVECTOR2(0, 0), 1, 2, 20, player1Mass, 2, player1Texture, bulletTexture);
}

void CleanupSpaceShip() {
    player1Texture->Release();
    player1Texture = NULL;
}

void SpaceShipPhysics() {
    if (player1Position.x <= 0 && player1Velocity.x < 0) {
        player1Velocity.x *= -1;
		audioManager->sound2pan = -1.0f;
		audioManager->playSound2();
    }
    if (player1Position.x >= windowWidth- playerSpriteWidth/2 && player1Velocity.x > 0) {

        player1Velocity.x *= -1;
        audioManager->sound2pan = -1.0f;
        audioManager->playSound2();
    }
    if (player1Position.y <= 0 && player1Velocity.y < 0) {
        player1Velocity.y *= -1;
    }
    if (player1Position.y >= windowHeight- playerSpriteHeight/2 && player1Velocity.y > 0) {
        player1Velocity.y *= -1;
    }

	player1Velocity = addForce(player1EnginePower, player1Mass, player1Direction, player1Velocity);
	player1Position += player1Velocity;
}

void SpaceShipUpdate() {
    for(int i=0; i< timer.FramesToUpdate(); i++){
		float turboSpeed = diKeys[DIK_LSHIFT] & 0x80 ? player1EnginePower * 2 : player1EnginePower;
		if (D3DXVec3LengthSq(&player1Velocity) > 0.0001f) {
            currentplayer1Frame += (float)player1FPS / (float)framePerSecond;
            player1Rect = getNumberRect(((int)currentplayer1Frame % player1MaxFrame) * 2, 2, 2, playerSpriteWidth, playerSpriteHeight);
        }
        else if(currentplayer1Frame > 0) {
			currentplayer1Frame = 0;            
            player1Rect = getNumberRect(((int)currentplayer1Frame % player1MaxFrame) * 2, 2, 2, playerSpriteWidth, playerSpriteHeight);

        }
        if (diKeys[DIK_UP] & 0x80) {
			D3DXVECTOR3 movingVelocity = D3DXVECTOR3(
                cos(player1Position.z/player1RotationFactor - D3DX_PI/2) * player1EnginePower / player1Mass,
                sin(player1Position.z/player1RotationFactor - D3DX_PI/2) * player1EnginePower /player1Mass, 
                0);
			player1Velocity += movingVelocity;
        }

        else if(D3DXVec3LengthSq(&player1Velocity) > 0.0001f) {
            D3DXVECTOR3 movingVelocity = D3DXVECTOR3(
                player1Velocity.x / deaccelerationInSpace,
                player1Velocity.y / deaccelerationInSpace,
                0);
            player1Velocity -= movingVelocity;
        }

        if (diKeys[DIK_LEFT] & 0x80) {
			player1Position.z -= player1RotationSpeed;
        }
        if (diKeys[DIK_RIGHT] & 0x80) {
            player1Position.z += player1RotationSpeed;
        }
		//if (diKeys[DIK_SPACE] & 0x80) {
		//	player1Position = D3DXVECTOR3(windowWidth/2, windowHeight/2, 0);
		//	player1Velocity = D3DXVECTOR3(0, 0, 0);
		//}
        player1Velocity = CalculateCollisionVelocity(
            D3DXVECTOR2(player1Position.x, player1Position.y),
            playerSpriteWidth / 4, 
            player1Velocity,
            *player2);
		player2->Update();

    }
    
}

void SpaceShipRender() {
	d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(128, 128, 128), 1.0f, 0);
	//	Begin the scene
	d3dDevice->BeginScene();
	sprite->Begin(D3DXSPRITE_ALPHABLEND);

    string vStr = "Rotation: " + to_string(player1Position.z * 180 / D3DX_PI);
    font->DrawTextA(sprite, vStr.c_str(), vStr.length(), &textRect, 0, D3DCOLOR_XRGB(255, 255, 255));

	D3DXVECTOR2 player1pos = D3DXVECTOR2(player1Position.x, player1Position.y);

    D3DXMatrixTransformation2D(&player1WorldMatrix,
        &player1ScaleCenter,
        0,
        &player1Scale, 
        &player1Center,
        player1Position.z/ player1RotationFactor,
        &player1pos);
	
    sprite->SetTransform(&player1WorldMatrix);
	sprite->Draw(player1Texture, &player1Rect, NULL,NULL, D3DCOLOR_XRGB(255, 255, 255));
	D3DXMatrixIdentity(&player1WorldMatrix);
	sprite->SetTransform(&player1WorldMatrix);

	player2->Render(sprite);

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

    audioManager->initializeAudio();
    audioManager->loadSounds();

    InitSprite();
    InitText();
    //InitMilitia();
    InitSpaceShip();
    while (WindowIsRunning()) {
        GetInput();

        //MilitiaPhysis();
        //MilitiaUpdate();
        //MilitiaRender();
        SpaceShipPhysics();
        SpaceShipUpdate();
        SpaceShipRender();
		audioManager->updateSound();
        if (diKeys[DIK_SPACE] & 0x80) {
			audioManager->playSound1();
        }



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

    CleanupSpaceShip();
    //CleanupMilitia();
    CleanupDInput();
    CleanupDirectX();
	CleanupWindow();
    return 0;
}