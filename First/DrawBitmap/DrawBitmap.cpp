#include <Windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("DrawBitmap");

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
void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit) {
    HDC MemDC;
    HBITMAP oldBitmap;
    int bx, by;
    BITMAP bit;

    MemDC = CreateCompatibleDC(hdc);
    oldBitmap = (HBITMAP)SelectObject(MemDC, hBit);

    GetObject(hBit, sizeof(BITMAP), &bit);
    bx = bit.bmWidth;
    by = bit.bmHeight;

    BitBlt(hdc, x, y, bx, by, MemDC, 0, 0, SRCCOPY);

    SelectObject(MemDC, oldBitmap);
    DeleteDC(MemDC);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    HDC hdc, MemDC;
    PAINTSTRUCT ps;
    static HBITMAP MyBitmap;

    switch (iMessage) {
    case WM_CREATE:
        MyBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
        return 0;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        DrawBitmap(hdc, 10, 10, MyBitmap);
        EndPaint(hWnd, &ps);
        return 0;
    case WM_DESTROY:
        DeleteObject(MyBitmap);
        PostQuitMessage(0);
        return 0;
    }
    return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}