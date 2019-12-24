#include <Windows.h>
#include <mmsystem.h>
#include "resource.h"

#define MAXSTAGE 3
#define BW 32
#define BH 32
#define MAXUNDO 1000

#ifdef _DEBUG
#pragma comment(lib,"winmm.lib")//디버그 모드인 경우
#else
#pragma comment(lib,"winmm.lib")//릴리즈 모드인 경우
#endif



struct tag_MoveInfo
{
    char dx : 3;
    char dy : 3;
    char bWithPack : 2;
};
tag_MoveInfo MoveInfo[MAXUNDO];
int UndoIdx;
BOOL bSound = TRUE;
void DrawScreen(HDC hdc);
BOOL TestEnd();
void Move(int dir);
void InitStage();
void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit);
void Undo();
void Redo();
void ErasePack(int x, int y);

enum class tag_ObjectID
{
    eWall,
    ePack,
    ePoint,
    eTile,
    eManLeft,
    eManRight,
    eManUp,
    eManDown,
    eMax
}ObjID;

char ns[18][21];
int nStage;
int nx, ny;
int nMove;
int ManBit;
HBITMAP hBit[(UINT)(tag_ObjectID::eMax)];


char arStage[MAXSTAGE][18][21] = {

     {
     "####################",
     "####################",
     "####################",
     "#####   ############",
     "#####O  ############",
     "#####  O############",
     "###  O O ###########",
     "### # ## ###########",
     "#   # ## #####  ..##",
     "# O  O   @      ..##",
     "##### ### # ##  ..##",
     "#####     ##########",
     "####################",
     "####################",
     "####################",
     "####################",
     "####################",
     "####################"
     },
     {
     "####################",
     "####################",
     "####################",
     "####################",
     "####..  #     ######",
     "####..  # O  O  ####",
     "####..  #O####  ####",
     "####..    @ ##  ####",
     "####..  # #  O #####",
     "######### ##O O ####",
     "###### O  O O O ####",
     "######    #     ####",
     "####################",
     "####################",
     "####################",
     "####################",
     "####################",
     "####################"
     },
     {
     "####################",
     "####################",
     "####################",
     "####################",
     "##########     @####",
     "########## O#O #####",
     "########## O  O#####",
     "###########O O #####",
     "########## O # #####",
     "##....  ## O  O  ###",
     "###...    O  O   ###",
     "##....  ############",
     "####################",
     "####################",
     "####################",
     "####################",
     "####################",
     "####################"
     },
};

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("Sokoban2");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
    HWND hWnd;
    MSG Message;
    WNDCLASS WndClass;
    g_hInst = hInstance;

    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    WndClass.hInstance = hInstance;
    WndClass.lpfnWndProc = WndProc;
    WndClass.lpszClassName = lpszClass;
    WndClass.lpszMenuName = NULL;
    WndClass.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&WndClass);

    hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, (HMENU)NULL, hInstance, NULL);
    ShowWindow(hWnd, nCmdShow);

    while (GetMessage(&Message, NULL, 0, 0)) {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }
    return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    RECT crt;
    int i;
    TCHAR Message[256];

    switch (iMessage) {
    case WM_CREATE:
        hWndMain = hWnd;
        SetRect(&crt, 0, 0, 900, BH * 18);
        AdjustWindowRect(&crt, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE);
        SetWindowPos(hWnd, NULL, 0, 0, crt.right - crt.left, crt.bottom - crt.top, SWP_NOMOVE | SWP_NOZORDER);
        for (i = 0; i < (UINT)tag_ObjectID::eMax; i++) {
            hBit[i] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_WALL + i));
        }
        nStage = 0;
        InitStage();
        return 0;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        DrawScreen(hdc);
        EndPaint(hWnd, &ps);
        return 0;
    case WM_KEYDOWN:
        switch (wParam) {
        case VK_LEFT:
        case VK_RIGHT:
        case VK_UP:
        case VK_DOWN:
            Move(wParam);
            if (TestEnd()) {
                wsprintf(Message, TEXT("%d 스테이지를 풀었습니다.")
                    TEXT("다음 스테이지로 이동합니다."), nStage + 1);
                MessageBox(hWnd, Message, TEXT("알림"), MB_OK);
                if (nStage < MAXSTAGE - 1) {
                    nStage++;
                }
                InitStage();
            }
            break;
        case 'Q':
            DestroyWindow(hWnd);
            break;
        case 'R':
            InitStage();
            break;
        case 'N':
            if (nStage < MAXSTAGE - 1) {
                nStage++;
                InitStage();
            }
            break;
        case 'P':
            if (nStage > 0) {
                nStage--;
                InitStage();
            }
            break;
        case 'S':
            bSound = !bSound;
            break;
        case 'Z':
            Undo();
            break;
        case 'X':
            Redo();
            break;
        }
        return 0;
    case WM_DESTROY:
        for (i = 0; i < (UINT)tag_ObjectID::eMax; i++) {
            DeleteObject(hBit[i]);
        }
        PostQuitMessage(0);
        return 0;
    }
    return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}

void Undo() 
{
    if (UndoIdx != 0) {
        UndoIdx--;
        if (MoveInfo[UndoIdx].bWithPack) {
            ErasePack(nx + MoveInfo[UndoIdx].dx, ny + MoveInfo[UndoIdx].dy);
            ns[ny][nx] = 'O';
        }
        nx -= MoveInfo[UndoIdx].dx;
        ny -= MoveInfo[UndoIdx].dy;
        InvalidateRect(hWndMain, NULL, FALSE);
    }
}
void Redo() 
{
    if (MoveInfo[UndoIdx].dx != -2) {
        nx += MoveInfo[UndoIdx].dx;
        ny += MoveInfo[UndoIdx].dy;
        if (MoveInfo[UndoIdx].bWithPack) {
            ErasePack(nx, ny);
            ns[ny + MoveInfo[UndoIdx].dy][nx + MoveInfo[UndoIdx].dx] = 'O';
        }
        InvalidateRect(hWndMain, NULL, FALSE);
        UndoIdx++;
    }
}
void DrawScreen(HDC hdc)
{
    int x, y;
    int iBit;
    TCHAR Message[256];
    for (y = 0; y < 18; y++)
    {
        for (x = 0; x < 20; x++)
        {
            switch (ns[y][x]) {
            case '#':
                iBit = 0;
                break;
            case 'O':
                iBit = 1;
                break;
            case '.':
                iBit = 2;
                break;
            case ' ':
                iBit = 3;
                break;
            case '@':
                iBit = 4;
                break;
            }
            DrawBitmap(hdc, x * BW, y * BH, hBit[iBit]);
        }
    }
    DrawBitmap(hdc, nx * BW, ny * BH, hBit[ManBit]);
    wsprintf(Message, TEXT("SOKOBAN"));
    TextOut(hdc, 700, 10, Message, lstrlen(Message));
    wsprintf(Message, TEXT("Q:종료, R:다시 시작"));
    TextOut(hdc, 700, 30, Message, lstrlen(Message));
    wsprintf(Message, TEXT("N:다음, P:이전"));
    TextOut(hdc, 700, 50, Message, lstrlen(Message));
    wsprintf(Message, TEXT("Z:되돌리기, X:재실행"));
    TextOut(hdc, 700, 70, Message, lstrlen(Message));
    wsprintf(Message, TEXT("S:소리 ON/OFF"));
    TextOut(hdc, 700, 90, Message, lstrlen(Message));
    wsprintf(Message, TEXT("스테이지 : %d"), nStage + 1);
    TextOut(hdc, 700, 110, Message, lstrlen(Message));
    wsprintf(Message, TEXT("이동 회수 : %d"), nMove);
    TextOut(hdc, 700, 130, Message, lstrlen(Message));
};

BOOL TestEnd() 
{
    int x, y;

    for (y = 0; y < 18; y++) {
        for (x = 0; x < 20; x++) {
            if (arStage[nStage][y][x] == '.' && ns[y][x] != 'O') {
                return FALSE;
            }
        }
    }
    return TRUE;
};
void Move(int dir) 
{
    int dx = 0, dy = 0;
    BOOL bWithPack = FALSE;
    int i;

    switch (dir) {
    case VK_LEFT:
        ManBit = (UINT)tag_ObjectID::eManLeft;
        dx = -1;
        break;
    case VK_RIGHT:
        ManBit = (UINT)tag_ObjectID::eManRight;
        dx = 1;
        break;
    case VK_UP:
        ManBit = (UINT)tag_ObjectID::eManUp;
        dy = -1;
        break;
    case VK_DOWN:
        ManBit = (UINT)tag_ObjectID::eManDown;
        dy = 1;
        break;
    }

    if (ns[ny + dy][nx + dx] != '#') { // 벽 감지
        if (ns[ny + dy][nx + dx] == 'O') { // 돌 감지
            if (ns[ny + dy * 2][nx + dx * 2] == ' ' || ns[ny + dy * 2][nx + dx * 2] == '.') { // 타일인지, 돌 놓는 곳 인지
                ErasePack(nx + dx, ny + dy);
                ns[ny + dy * 2][nx + dx * 2] = 'O';
                bWithPack = TRUE;
            }
            else {
                return;
            }
        }
        nx += dx;
        ny += dy;
        nMove++;

        MoveInfo[UndoIdx].dx = dx;
        MoveInfo[UndoIdx].dy = dy;
        MoveInfo[UndoIdx].bWithPack = bWithPack;
        UndoIdx++;
        MoveInfo[UndoIdx].dx = -2;

        if (UndoIdx == MAXUNDO - 1) {
            for (i = 100; i < UndoIdx; i++) {
                MoveInfo[i - 100] = MoveInfo[i]; // 100 ~ 1000번 째까지 번지의 정보를 0 ~ 900번 번지에 덮어쓴다.
            }
            for (i = MAXUNDO - 100; i < MAXUNDO; i++) {
                MoveInfo[i].dx = -2; // 900 ~ 1000 번째 번지 초기화
            }
            UndoIdx -= 100;
        }

        if (bSound) {
            PlaySound(MAKEINTRESOURCE(/*bWithPack ? */IDB_PACK/* : IDR_MOVE*/),
                g_hInst, SND_RESOURCE | SND_ASYNC);
        }
        InvalidateRect(hWndMain, NULL, FALSE);
    }
};
void InitStage() 
{
    int x, y;
    memcpy(ns, arStage[nStage], sizeof(ns));
    for (y = 0; y < 18; y++) 
    {
        for ( x = 0; x < 20; x++)
        {
            if (ns[y][x] == '@') {
                nx = x;
                ny = y;
                ns[y][x] = ' ';
            }
        }
    }
    nMove = 0;
    UndoIdx = 0;
    ManBit = 4;
    for (x = 0; x < MAXUNDO; x++) {
        MoveInfo[x].dx = -2;
    }

    InvalidateRect(hWndMain, NULL, TRUE);
};

void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit)
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

    BitBlt(hdc, x, y, bx, by, MemDC, 0, 0, SRCCOPY);

    SelectObject(MemDC, OldBitmap);
    DeleteDC(MemDC);
};

void ErasePack(int x, int y) 
{
    if (arStage[nStage][y][x] == '.') 
    {
        ns[y][x] = '.';
    }
    else
    {
        ns[y][x] = ' ';
    }
}