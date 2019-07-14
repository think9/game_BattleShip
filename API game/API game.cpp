﻿// API game.cpp: 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include "API game.h"
#include <math.h>

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_APIGAME, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_APIGAME));

    MSG msg;

    // 기본 메시지 루프입니다.
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_APIGAME);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_POPUP,
      0, 0, 800, 600, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, SW_MAXIMIZE);	//nCmdShow
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

//포물선 계산을 위한 상수 값 정의
#define pi 3.1416
#define gravity 9.8
#define ballsize 20

void DrawBitmap(HDC hdc, int x, int y, int ex, int ey, HBITMAP hBit);

//Radian 값 -> 각도 변환
double radian(double degree)
{
	return degree*pi / 180;
}

//미사일 발사 함수
int throwBall(HDC hdc, int x, int y, int degree, int time, RECT map, bool &nowDraw, RECT player, int &health, bool &state)
{
	state = FALSE;
	static int ballx = x;
	static int bally = y;
	double dx, dy;
	
	//최초 공의 위치 설정
	if (time < 2)
	{
		ballx = x;
		bally = y;
	}
	
	//시간 가속도 설정 및 위치비교를 통한 진행방향 설정
	if (x < player.left)
		dx = 4 * cos(radian(degree))*time;
	else
		dx = 4 * cos(radian(degree))*time*-1;
	
	//시간 가속도를 통한 진행방향 설정
	dy = (80 * (sin(radian(degree))*time) - ((gravity*time*time) / 2)) / 50;
	
	//MoveToEx(hdc, ballx, bally, NULL);
	//LineTo(hdc, ballx + dx, bally - dy);
	DrawBitmap(hdc, ballx, bally, ballsize, ballsize, LoadBitmap(hInst, MAKEINTRESOURCE(IDB_Bomb)));
	ballx = ballx + dx;
	bally = bally - dy;

	//맵 밖으로 나갔을 시 종료
	if (ballx > map.right)
	{
		nowDraw = FALSE;
		state = TRUE;
	}
	else if (bally > map.bottom)
	{
		nowDraw = FALSE;
		state = TRUE;
	}
	else if (ballx < map.left)
	{
		nowDraw = FALSE;
		state = TRUE;
	}
	else if (bally < map.top)
	{
		nowDraw = FALSE;
		state = TRUE;
	}

	//다른 객체와의 충돌
	if (player.left <= ballx + ballsize / 2 && ballx + ballsize / 2 <= player.right
		&& player.top <= bally + ballsize / 2 && bally + ballsize / 2 <= player.bottom)
	{
		health--;
		nowDraw = FALSE;
		state = TRUE;
	}
	return health;
}

//배(Ship) 구조체 정의
struct Tank
{
	RECT player;
	POINT center;
	int degree = 45;
	bool turn;
	int health;
};

#define size 50
#define step 10

void DrawBitmap(HDC hdc, int x, int y, int ex, int ey, HBITMAP hBit)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	int bx, by;
	BITMAP bit;

	MemDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(MemDC, hBit);

	GetObject(hBit, sizeof(BITMAP), &bit);
	bx = bit.bmWidth;
	by = bit.bmHeight;

	StretchBlt(hdc, x, y, ex, ey, MemDC, 0, 0, bx, by, SRCCOPY);

	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static Tank t1, t2;
	static RECT map;
	static int time = 0;
	static int move =11;	//한 턴에 10칸 이동 가능
	static bool nowDraw = FALSE;
	static TCHAR str[30], str2[30], str3[20];
	static bool gameState;
	switch (message)
	{
	//최초 실행 시 설정
	case WM_CREATE:
		GetClientRect(hWnd, &map);
		SetTimer(hWnd, 1, 100, NULL);	//미사일 발사를 위한 타이머
		t1.center.x = map.right / 5 * 2;
		t1.center.y = map.bottom / 3 * 2;
		t1.player.left = t1.center.x - size;
		t1.player.top = t1.center.y - size;
		t1.player.right = t1.center.x + size;
		t1.player.bottom = t1.center.y + size;
		t2.center.x = map.right - 100;
		t2.center.y = map.bottom / 3 * 2;
		t2.player.left = t2.center.x - size;
		t2.player.top = t2.center.y - size;
		t2.player.right = t2.center.x + size;
		t2.player.bottom = t2.center.y + size;

		t1.health = 3;
		t2.health = 3;
		t1.turn = TRUE;
		t2.turn = FALSE;
		wsprintf(str, TEXT("P1 Health : %d"), t1.health);
		wsprintf(str2, TEXT("P2 Health : %d"), t2.health);
		gameState = TRUE;
		break;
	//윈도우 창 크기 설정
	case WM_SIZE:
		GetClientRect(hWnd, &map);
		break;
	//타이머 정의
	case WM_TIMER:
			time++;
			InvalidateRect(hWnd, NULL, FALSE);
			break;
	//버튼 이벤트
	case WM_KEYDOWN:
		if(gameState)
		{
			switch (wParam)
			{
			//왼쪽 버튼
			case VK_LEFT:
				move--;
				if (move > 0)
				{
					InvalidateRect(hWnd, NULL, TRUE);
					if (t1.turn)
					{
						t1.center.x -= step;
						if (t1.center.x <= map.left)
							t1.center.x = map.left + size;
						t1.player.left = t1.center.x - size;
						t1.player.right = t1.center.x + size;
						InvalidateRect(hWnd, &t1.player, TRUE);
					}
					if (t2.turn)
					{
						t2.center.x -= step;
						if (t2.center.x <= map.left)
							t2.center.x = map.left + size;
						t2.player.left = t2.center.x - size;
						t2.player.right = t2.center.x + size;
						InvalidateRect(hWnd, &t2.player, TRUE);
					}
				}
				break;
			//오른쪽 버튼
			case VK_RIGHT:
				move--;
				if (move > 0)
				{
					InvalidateRect(hWnd, NULL, TRUE);
					if (t1.turn)
					{
						t1.center.x += step;
						if (t1.center.x >= map.right)
							t1.center.x = map.right - size;
						t1.player.left = t1.center.x - size;
						t1.player.right = t1.center.x + size;
						InvalidateRect(hWnd, &t1.player, TRUE);
					}
					if (t2.turn)
					{
						t2.center.x += step;
						if (t2.center.x >= map.right)
							t2.center.x = map.right - size;
						t2.player.left = t2.center.x - size;
						t2.player.right = t2.center.x + size;
						InvalidateRect(hWnd, &t2.player, TRUE);
					}
				}
				break;
			//위쪽 버튼
			case VK_UP:
				move--;
				if (move > 0)
				{
					InvalidateRect(hWnd, NULL, TRUE);
					if (t1.turn)
					{
						t1.center.y -= step;
						if (t1.center.y <= map.top)
							t1.center.y = map.top + size;
						t1.player.top = t1.center.y - size;
						t1.player.bottom = t1.center.y + size;
						InvalidateRect(hWnd, &t1.player, TRUE);
					}
					if (t2.turn)
					{
						t2.center.y -= step;
						if (t2.center.y <= map.top)
							t2.center.y = map.top + size;
						t2.player.top = t2.center.y - size;
						t2.player.bottom = t2.center.y + size;
						InvalidateRect(hWnd, &t2.player, TRUE);
					}
				}
				break;
			//아래쪽 버튼
			case VK_DOWN:
				move--;
				if (move > 0)
				{
					InvalidateRect(hWnd, NULL, TRUE);
					if (t1.turn)
					{
						t1.center.y += step;
						if (t1.center.y >= map.bottom)
							t1.center.y = map.bottom - size;
						t1.player.top = t1.center.y - size;
						t1.player.bottom = t1.center.y + size;
						InvalidateRect(hWnd, &t1.player, TRUE);
					}
					if (t2.turn)
					{
						t2.center.y += step;
						if (t2.center.y >= map.bottom)
							t2.center.y = map.bottom - size;
						t2.player.top = t2.center.y - size;
						t2.player.bottom = t2.center.y + size;
						InvalidateRect(hWnd, &t2.player, TRUE);
					}
				}
				break;
			//스페이스바 버튼 - 미사일 발사 및 턴 변경
			case VK_SPACE:
				//nowDraw = !nowDraw;
				if (nowDraw == FALSE)
					nowDraw = TRUE;
				time = 0;
				t1.turn = !t1.turn;
				t2.turn = !t2.turn;
				break;
			//End 버튼 - 미사일 각도 조절 (+)
			case VK_END:
				if (t1.turn)
				{
					t1.degree += 15;
					if (t1.degree > 80)
						t1.degree = 80;
				}
				if (t2.turn)
				{
					t2.degree += 15;
					if (t2.degree > 80)
						t2.degree = 80;
				}
				break;
			//Home 버튼 - 미사일 각도 조절 (-)
			case VK_HOME:
				if (t1.turn)
				{
					t1.degree -= 15;
					if (t1.degree < 30)
						t1.degree = 30;
				}
				if (t2.turn)
				{
					t2.degree -= 15;
					if (t2.degree < 30)
						t2.degree = 30;
				}
				break;
			}
		}
	//메뉴 버튼 이벤트
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);	// 메뉴 선택을 구문 분석합니다.
		switch (wmId)
		{
		//메뉴 - 새 게임 클릭 이벤트
		case ID_NEWGAME:
			gameState = TRUE;
			t1.center.x = map.right / 5 * 2;
			t1.center.y = map.bottom / 3 * 2;
			t1.player.left = t1.center.x - size;
			t1.player.top = t1.center.y - size;
			t1.player.right = t1.center.x + size;
			t1.player.bottom = t1.center.y + size;
			t2.center.x = map.right / 5 * 4;
			t2.center.y = map.bottom / 3 * 2;
			t2.player.left = t2.center.x - size;
			t2.player.top = t2.center.y - size;
			t2.player.right = t2.center.x + size;
			t2.player.bottom = t2.center.y + size;

			t1.health = 3;
			t2.health = 3;
			t1.turn = TRUE;
			t2.turn = FALSE;
			wsprintf(str, TEXT("P1 Health : %d"), t1.health);
			wsprintf(str2, TEXT("P2 Health : %d"), t2.health);
			break;
		//메뉴 - 게임 정보 이벤트
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		//메뉴 - 종료 이벤트
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	
	//윈도우창 변화
	case WM_PAINT:
	{
		//리소스 및 배 그리기
		hdc = BeginPaint(hWnd, &ps);
		DrawBitmap(hdc, 0, 0, map.right, map.bottom, LoadBitmap(hInst, MAKEINTRESOURCE(IDB_Background)));
		DrawBitmap(hdc, t1.player.left, t1.player.top, size * 2, size * 2, LoadBitmap(hInst, MAKEINTRESOURCE(IDB_Ship1)));
		DrawBitmap(hdc, t2.player.left, t2.player.top, size * 2, size * 2, LoadBitmap(hInst, MAKEINTRESOURCE(IDB_Ship2)));
		
		//미사일 그리기
		if (nowDraw)
		{
			if (!t1.turn)	//t1의 미사일
				t2.health = throwBall(hdc, t1.player.right, t1.player.top, t1.degree, time, map, nowDraw, t2.player, t2.health, gameState);
			if (!t2.turn)	//t2의 미사일
				t1.health = throwBall(hdc, t2.player.left, t2.player.top, t2.degree, time, map, nowDraw, t1.player, t1.health, gameState);
			//미사일이 날아가는 동안 최대 이동횟수로 설정하여 배를 움직일 수 없도록 함
			move = 11;
		}
		
		//좌측 상단 상태 표시
		wsprintf(str, TEXT("P1 Health : %d, P1 Degree : %d"), t1.health, t1.degree);
		wsprintf(str2, TEXT("P2 Health : %d, P2 Degree : %d"), t2.health, t2.degree);
		TextOut(hdc, 0, 0, str, lstrlen(str));
		TextOut(hdc, 0, 20, str2, lstrlen(str2));
		
		//게임 종료 여부 확인
		if (t1.health == 0)
		{
			wsprintf(str3, TEXT("Player2 Win!"));
			TextOut(hdc, 100, 100, str3, lstrlen(str3));
			gameState = FALSE;
		}
		else if (t2.health == 0)
		{
			wsprintf(str3, TEXT("Player1 Win!"));
			TextOut(hdc, 100, 100, str3, lstrlen(str3));
			gameState = FALSE;
		}
		EndPaint(hWnd, &ps);
	}
	break;
	//윈도우 
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
