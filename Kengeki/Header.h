#ifndef HEADER
#define HEADER

#include <windows.h>
#include <d3dx9.h>
#include <dinput.h>

#define TITLE 	TEXT("Step4")
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define DISPLAY_WIDTH 1920
#define DISPLAY_HIGHT 1080
#define MOVE_SPEED 10.f
#define JUMP_POWER -27.f
#define GRAVITY 2.0f
#define BGMoveSpeed 7.f

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")    
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "Dinput8.lib")

#define SAFE_RELEASE(p) {if(p){(p)->Release(); (p)=NULL;}}

struct PLAYER_STATE
{
	float x, y, scale;
};

struct BACKGROUND_STATE
{
	float x, y, Width, Height;
};

enum KEYSTATE
{
	PUSH,
	RELEASE,
	ON,
	OFF
};

enum KEYKIND
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	SPACE,
	Z,
	X,
	C,
	V,
	KEYMAX
};

extern PLAYER_STATE	g_Player;
extern PLAYER_STATE	g_Zangeki;
extern PLAYER_STATE	g_Zangeki2;
extern PLAYER_STATE	g_Kirinuke;

extern bool XskillFlag;

void ZSkill();
void ZSkill2();
void XSkill();

#endif
