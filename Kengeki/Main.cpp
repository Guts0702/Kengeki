#include "Header.h"

struct CUSTOMVERTEX
{
	FLOAT	x, y, z, rhw;
	DWORD	color;
	FLOAT	tu, tv;
};

enum TEXTURE
{
	BACKGROUND_TEX,
	BACKGROUND2_TEX,
	PLAYER_TEX,
	ZANGEKI_TEX,
	ZANGEKI2_TEX,
	KIRISAGE_TEX,
	KIRINUKE_TEX,
	TEXMAX
};

//Directx�֌W----------------------------
LPDIRECT3DTEXTURE9	  g_pTexture[TEXMAX];	//	�摜�̏������Ă����ׂ̃|�C���^�z��
IDirect3DDevice9*	  g_pD3Device;		//	Direct3D�̃f�o�C�X
D3DPRESENT_PARAMETERS g_D3dPresentParameters;		//	�p�����[�^
D3DDISPLAYMODE		  g_D3DdisplayMode;
IDirect3D9*			  g_pDirect3D;		//	Direct3D�̃C���^�[�t�F�C�X
LPDIRECTINPUTDEVICE8 pKeyDevice = NULL;
LPDIRECTINPUT8 pDinput = NULL;
//---------------------------------------

BACKGROUND_STATE g_backGround = { 0.f,0.f,1920.f,980.f };
BACKGROUND_STATE g_backGround2 = { g_backGround.Width,0.f,g_backGround.Width + 1560.f,980.f };
PLAYER_STATE g_Player = { 200.f, 250.f, 60.f };
PLAYER_STATE g_Zangeki = { g_Player.x, g_Player.y, 40.f };
PLAYER_STATE g_Zangeki2 = { g_Player.x, g_Player.y, 40.f };
PLAYER_STATE g_Kirisage = { g_Player.x + 30.f, g_Player.y + 30.f, 40.f };
PLAYER_STATE g_Kirinuke = { g_Player.x - 200, g_Player.y, 60.f };

bool jumpFlag = false;
bool skyFlag = false;
bool LeftFlag = false;
bool RightFlag = false;
bool ZskillFlag = false;
bool Zskill2Flag = false;
bool XskillFlag = false;
float jump;
int Zskillcount = 0;
int Xskillcount = 0;
int Scount = 0;

static HRESULT hr;

KEYSTATE Key[KEYMAX];

int PreKey[KEYMAX] = { 0 };

int KeyState[5] = { 0 };

void KeyCheck(int VK, KEYKIND st) {
	if ((GetKeyState(VK) & 0x8000) != 0) {
		if (PreKey[st] == 0) {
			Key[st] = PUSH;
		}
		else {
			Key[st] = ON;
		}
		PreKey[st] = 1;
	}
	else {
		if (PreKey[st] == 0) {
			Key[st] = OFF;
		}
		else {
			Key[st] = RELEASE;
		}
		PreKey[st] = 0;
	}
}
/**
*���䏈��
*/
HRESULT InitDinput(HWND hWnd) {

	if (FAILED(hr = DirectInput8Create(GetModuleHandle(NULL),
		DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&pDinput, NULL)))
	{
		return hr;
	}
	if (FAILED(hr = pDinput->CreateDevice(GUID_SysKeyboard,
		&pKeyDevice, NULL)))
	{
		return hr;
	}
	if (FAILED(hr = pKeyDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return hr;
	}
	if (FAILED(hr = pKeyDevice->SetCooperativeLevel(
		hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
	{
		return hr;
	}
	pKeyDevice->Acquire();
	return S_OK;
}
// �Q�[���V�[���̐���֐�
void GameScene()
{
	// �L�[�̃`�F�b�N�֐��Q
	KeyCheck(VK_LEFT, LEFT);
	KeyCheck(VK_RIGHT, RIGHT);
	KeyCheck(VK_UP, UP);
	KeyCheck(VK_DOWN, DOWN);
	KeyCheck(VK_SPACE, SPACE);
	KeyCheck('Z', Z);
	KeyCheck('X', X);
	KeyCheck('C', C);
	KeyCheck('V', V);
}
void Control()
{
	if (Key[LEFT] == ON)
	{
		RightFlag = false;
		if (LeftFlag == false)
		{
			g_Player.x -= MOVE_SPEED;
			if (Key[LEFT] == PUSH || Key[LEFT] == ON)
			{
				if (g_Player.x <= 100)
				{
					LeftFlag = true;
				}
			}
		}
	}
	if (Key[RIGHT] == ON)
	{
		LeftFlag = false;
		if (RightFlag == false)
		{
			g_Player.x += MOVE_SPEED;
			if (Key[RIGHT] == PUSH || Key[RIGHT] == ON)
			{
				if (g_Player.x >= 670)
				{
					RightFlag = true;
				}
			}
		}
	}
	g_pD3Device->Present(NULL, NULL, NULL, NULL);
	if (Scount % 2 == 0) {
		if (g_Player.y == 250.f) {
			if (Zskillcount >= 30) {
				if (Key[Z] == ON)
				{
					g_Zangeki.x = g_Player.x;
					ZskillFlag = true;
					Zskillcount = 0;
					Scount++;
				}
			}
		}
	}
	if (Scount % 2 == 1) {
		if (g_Player.y == 250.f) {
			if (Zskillcount >= 30) {
				if (Key[Z] == ON)
				{
					g_Zangeki2.x = g_Player.x;
					Zskill2Flag = true;
					Zskillcount = 0;
					Scount++;
				}
			}
		}
	}
	if (g_Player.y == 250.f) {
		if (Xskillcount >= 30) {
			if (Key[X] == PUSH)
			{
				g_Kirinuke.x = g_Player.x;
				XskillFlag = true;
				Xskillcount = 0;
			}
		}
	}
	if (Key[SPACE] == PUSH)
		if (skyFlag != true)
		{
			jumpFlag = true;
			skyFlag = true;
			jump = JUMP_POWER;
		}
	//�W�����v�������ɂ��̏����ɓ���
	if (jumpFlag == true)
	{
		//�����x����d�͂�����
		jump += GRAVITY;
		//���x���v���C���[�̍��W�ɓ����
		g_Player.y += jump;

		if (g_Player.y >= 250.f) {
			if (jumpFlag == true)
			{
				g_Player.y = 250.f;
				jumpFlag = false;
				skyFlag = false;
			}
		}
	}
}
void Move()
{
	if (g_Player.x >= 550)
	{
		g_backGround.x -= BGMoveSpeed;
		g_backGround.Width -= BGMoveSpeed;
		if (g_backGround.Width <= 0.f)
		{
			g_backGround.x = 0.f;
			g_backGround.Width = 1920.f;
		}
	}
}
/*
*�`�揈��
*/
void Render(void)
{
	//���_��������--------------------------------------
	CUSTOMVERTEX backGround[4]
	{
		{ g_backGround.x, g_backGround.y, 1.f,1.f, 0xFFFFFFFF, 0.f, 0.f },
		{ g_backGround.Width, g_backGround.y, 1.f,1.f, 0xFFFFFFFF, 1.f, 0.f },
		{ g_backGround.Width, g_backGround.Height, 1.f,1.f, 0xFFFFFFFF, 1.f, 1.f },
		{ g_backGround.x, g_backGround.Height, 1.f,1.f, 0xFFFFFFFF, 0.f, 1.f }
	};
	CUSTOMVERTEX backGround2[4]
	{
		{ g_backGround.Width, g_backGround.y, 1.f,1.f, 0xFFFFFFFF, 0.f, 0.f },
		{ g_backGround.Width + 1920.f, g_backGround.y, 1.f,1.f, 0xFFFFFFFF, 1.f, 0.f },
		{ g_backGround.Width + 1920.f, g_backGround.Height, 1.f,1.f, 0xFFFFFFFF, 1.f, 1.f },
		{ g_backGround.Width, g_backGround.Height, 1.f,1.f, 0xFFFFFFFF, 0.f, 1.f }
	};
	CUSTOMVERTEX player[4]
	{
		{ g_Player.x - g_Player.scale, g_Player.y - g_Player.scale, 1.f, 1.f, 0xFFFFFFFF, 0.f, 0.f },
		{ g_Player.x + g_Player.scale, g_Player.y - g_Player.scale, 1.f, 1.f, 0xFFFFFFFF, 1.f, 0.f },
		{ g_Player.x + g_Player.scale, g_Player.y + g_Player.scale * 1.5, 1.f, 1.f, 0xFFFFFFFF, 1.f, 1.f },
		{ g_Player.x - g_Player.scale, g_Player.y + g_Player.scale * 1.5, 1.f, 1.f, 0xFFFFFFFF, 0.f, 1.f }
	};
	CUSTOMVERTEX Zangeki[4]
	{
		{ g_Zangeki.x - g_Zangeki.scale, g_Zangeki.y - g_Zangeki.scale, 1.f, 1.f, 0xFFFFFFFF, 0.f, 0.f },
		{ g_Zangeki.x + g_Zangeki.scale, g_Zangeki.y - g_Zangeki.scale, 1.f, 1.f, 0xFFFFFFFF, 1.f, 0.f },
		{ g_Zangeki.x + g_Zangeki.scale, g_Zangeki.y + g_Zangeki.scale * 1.5, 1.f, 1.f, 0xFFFFFFFF, 1.f, 1.f },
		{ g_Zangeki.x - g_Zangeki.scale, g_Zangeki.y + g_Zangeki.scale * 1.5, 1.f, 1.f, 0xFFFFFFFF, 0.f, 1.f }
	};
	CUSTOMVERTEX Zangeki2[4]
	{
		{ g_Zangeki2.x - g_Zangeki2.scale, g_Zangeki2.y - g_Zangeki2.scale, 1.f, 1.f, 0xFFFFFFFF, 0.f, 0.f },
		{ g_Zangeki2.x + g_Zangeki2.scale, g_Zangeki2.y - g_Zangeki2.scale, 1.f, 1.f, 0xFFFFFFFF, 1.f, 0.f },
		{ g_Zangeki2.x + g_Zangeki2.scale, g_Zangeki2.y + g_Zangeki2.scale * 1.5, 1.f, 1.f, 0xFFFFFFFF, 1.f, 1.f },
		{ g_Zangeki2.x - g_Zangeki2.scale, g_Zangeki2.y + g_Zangeki2.scale * 1.5, 1.f, 1.f, 0xFFFFFFFF, 0.f, 1.f }
	};
	CUSTOMVERTEX Kirinuke[4]
	{
		{ g_Kirinuke.x - g_Kirinuke.scale, g_Kirinuke.y - g_Kirinuke.scale * 2.0, 1.f, 1.f, 0xFFFFFFFF, 0.f, 0.f },
		{ g_Kirinuke.x + g_Kirinuke.scale, g_Kirinuke.y - g_Kirinuke.scale * 2.0, 1.f, 1.f, 0xFFFFFFFF, 1.f, 0.f },
		{ g_Kirinuke.x + g_Kirinuke.scale, g_Kirinuke.y + g_Kirinuke.scale * 1.5, 1.f, 1.f, 0xFFFFFFFF, 1.f, 1.f },
		{ g_Kirinuke.x - g_Kirinuke.scale, g_Kirinuke.y + g_Kirinuke.scale * 1.5, 1.f, 1.f, 0xFFFFFFFF, 0.f, 1.f }
	};
	//-----------------------------------------------------

	//��ʂ̏���
	g_pD3Device->Clear(0, NULL,
		D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(0x00, 0x00, 0x00),
		1.0, 0);

	//�`��̊J�n
	g_pD3Device->BeginScene();

	g_pD3Device->SetTexture(0, g_pTexture[BACKGROUND_TEX]);
	g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, backGround, sizeof(CUSTOMVERTEX));

	g_pD3Device->SetTexture(0, g_pTexture[BACKGROUND2_TEX]);
	g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, backGround2, sizeof(CUSTOMVERTEX));

	CUSTOMVERTEX drawplayer[4];
	for (int i = 0; i < 4; i++) {
		drawplayer[i] = player[i];
		drawplayer[i].x += g_Player.x;
		drawplayer[i].y += g_Player.y;
	}

	CUSTOMVERTEX drawZangeki[4];
	for (int i = 0; i < 4; i++) {
		drawZangeki[i] = Zangeki[i];
		drawZangeki[i].x += g_Zangeki.x;
		drawZangeki[i].y += g_Zangeki.y;
	}

	CUSTOMVERTEX drawZangeki2[4];
	for (int i = 0; i < 4; i++) {
		drawZangeki2[i] = Zangeki2[i];
		drawZangeki2[i].x += g_Zangeki2.x;
		drawZangeki2[i].y += g_Zangeki2.y;
	}

	CUSTOMVERTEX drawKirinuke[4];
	for (int i = 0; i < 4; i++) {
		drawKirinuke[i] = Kirinuke[i];
		drawKirinuke[i].x += g_Kirinuke.x;
		drawKirinuke[i].y += g_Kirinuke.y;
	}
	if (XskillFlag == false) {
		g_pD3Device->SetTexture(0, g_pTexture[PLAYER_TEX]);
		g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, drawplayer, sizeof(CUSTOMVERTEX));
	}
	if (ZskillFlag == true) {
		g_pD3Device->SetTexture(0, g_pTexture[ZANGEKI_TEX]);
		g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, drawZangeki, sizeof(CUSTOMVERTEX));
	}
	if (Zskill2Flag == true) {
		g_pD3Device->SetTexture(0, g_pTexture[ZANGEKI2_TEX]);
		g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, drawZangeki2, sizeof(CUSTOMVERTEX));
	}
	if (XskillFlag == true) {
		g_pD3Device->SetTexture(0, g_pTexture[KIRINUKE_TEX]);
		g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, drawKirinuke, sizeof(CUSTOMVERTEX));
	}
	//�`��̏I��
	g_pD3Device->EndScene();
	//�\��
	g_pD3Device->Present(NULL, NULL, NULL, NULL);

	//���_�ɓ����f�[�^��ݒ�
	g_pD3Device->SetFVF(D3DFVF_CUSTOMVERTEX);
}

/**
*���b�Z�[�W����
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wp, lp);
}

VOID FreeDx()
{
	if (pKeyDevice)
	{
		pKeyDevice->Unacquire();
	}
	SAFE_RELEASE(pKeyDevice);
	SAFE_RELEASE(pDinput);
	SAFE_RELEASE(g_pD3Device);
	SAFE_RELEASE(g_pDirect3D);
}
/**
*���C�����[�`��
*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,//��Windows�A�v���P�[�V�����̃��C���֐��@previous��prev
	PSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd;
	WNDCLASS Wndclass;
	MSG msg;

	//Windows���̐ݒ�
	Wndclass.style = CS_HREDRAW | CS_VREDRAW;
	Wndclass.lpfnWndProc = WndProc;
	Wndclass.cbClsExtra = Wndclass.cbWndExtra = 0;
	Wndclass.hInstance = hInstance;
	Wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	Wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	Wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	Wndclass.lpszMenuName = NULL;
	Wndclass.lpszClassName = TITLE;	//�N���X��
									//Window�̓o�^
	RegisterClass(&Wndclass);
	//Window�̐���
	hWnd = CreateWindow(
		TITLE,								//�E�B���h�E�̃N���X��
		TITLE, 							//�E�B���h�E�̃^�C�g��
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,	//�E�B���h�E�X�^�C��
		CW_USEDEFAULT,						// �E�B���h�E�̉������̈ʒux
		CW_USEDEFAULT,						// �E�B���h�E�̏c�����̈ʒuy
		DISPLAY_WIDTH,							// Width�i���j
		DISPLAY_HIGHT,							// Height�i�����j
		NULL,
		NULL,
		hInstance,							// �A�v���P�[�V�����C���X�^���X�̃n���h��
		NULL
	);
	if (!hWnd) return 0;

	//DirectX �I�u�W�F�N�g�̐���
	g_pDirect3D = Direct3DCreate9(
		D3D_SDK_VERSION);

	//Display Mode �̐ݒ�
	g_pDirect3D->GetAdapterDisplayMode(
		D3DADAPTER_DEFAULT,
		&g_D3DdisplayMode);

	ZeroMemory(&g_D3dPresentParameters,
		sizeof(D3DPRESENT_PARAMETERS));
	g_D3dPresentParameters.BackBufferFormat = g_D3DdisplayMode.Format;
	g_D3dPresentParameters.BackBufferCount = 1;
	g_D3dPresentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	g_D3dPresentParameters.Windowed = TRUE;

	//�f�o�C�X�����
	g_pDirect3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&g_D3dPresentParameters, &g_pD3Device);

	//�`��ݒ�
	g_pD3Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	g_pD3Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);  //SRC�̐ݒ�
	g_pD3Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	g_pD3Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_pD3Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	g_pD3Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	D3DXCreateTextureFromFile(
		g_pD3Device,
		"back.png",
		&g_pTexture[BACKGROUND_TEX]);
	D3DXCreateTextureFromFile(
		g_pD3Device,
		"back2.png",
		&g_pTexture[BACKGROUND2_TEX]);
	D3DXCreateTextureFromFile(
		g_pD3Device,
		"slime.png",
		&g_pTexture[PLAYER_TEX]);
	D3DXCreateTextureFromFile(
		g_pD3Device,
		"result_24.png",
		&g_pTexture[ZANGEKI_TEX]);
	D3DXCreateTextureFromFile(
		g_pD3Device,
		"result_24 2.png",
		&g_pTexture[ZANGEKI2_TEX]);
	D3DXCreateTextureFromFile(
		g_pD3Device,
		"result_31.png",
		&g_pTexture[KIRINUKE_TEX]);

	DWORD SyncOld = timeGetTime();	//	�V�X�e�����Ԃ��擾
	DWORD SyncNow;//���u���́v��current�A�u���́v��next�A�u�O�́v��previous�A�u��́v��old�B

	InitDinput(hWnd);

	timeBeginPeriod(1);

	ZeroMemory(&msg, sizeof(msg));

	HRESULT hr = pKeyDevice->Acquire();

	while (msg.message != WM_QUIT)
	{
		Sleep(1);
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			SyncNow = timeGetTime();
			if (SyncNow - SyncOld >= 1000 / 60) //	1�b�Ԃ�60�񂱂̒��ɓ���͂�
			{
				Zskillcount++;
				Xskillcount++;
				Render();
				GameScene();
				Control();
				Move();
				if (ZskillFlag == true)
				{
					ZSkill();
				}
				if (Zskill2Flag == true)
				{
					ZSkill2();
				}
				if (XskillFlag == true)
				{
					XSkill();
					XskillFlag = false;
				}
				SyncOld = SyncNow;
			}
		}
	}
	timeEndPeriod(1);

	FreeDx();
	return (int)msg.wParam;
}