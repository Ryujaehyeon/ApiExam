#include <Windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("RopMode2");

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
    WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
    WndClass.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&WndClass);

    hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, (HMENU)NULL, hInstance, NULL);
    ShowWindow(hWnd, nCmdShow);

    HACCEL hAccel;
    hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));
    while (GetMessage(&Message, NULL, 0, 0)) {
        if (!TranslateAccelerator(hWnd, hAccel, &Message))
        {
            TranslateMessage(&Message);
            DispatchMessage(&Message);
        }
    }
    return (int)Message.wParam;
}

enum class Shape
{
    eCURVE = 0,
    eLINE,
    eELLIPSE,
    eRECT,
    eNULL_Shape
};
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    HBRUSH MyBrush, OldBrush;
    static int x, y, oldx, oldy;
    static BOOL bNowDraw = FALSE;
    static Shape eMenuNumber = Shape::eNULL_Shape;

    switch (iMessage) {
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_CURVE:
            eMenuNumber = Shape::eCURVE;
            break;
        case ID_LINE:
            eMenuNumber = Shape::eLINE;
            break;
        case ID_ELLIPSE:
            eMenuNumber = Shape::eELLIPSE;
            break;
        case ID_RECT:
            eMenuNumber = Shape::eRECT;
            break;
        }
        return 0;
    case WM_LBUTTONDOWN:
        x = LOWORD(lParam);
        y = HIWORD(lParam);
        oldx = x;
        oldy = y;
        bNowDraw = TRUE;
        return 0;
    case WM_MOUSEMOVE:
        if (!bNowDraw)
            return 0;

        switch (eMenuNumber)
        {
        case Shape::eCURVE:
            hdc = GetDC(hWnd);
            MoveToEx(hdc, x, y, NULL);
            x = LOWORD(lParam);
            y = HIWORD(lParam);
            LineTo(hdc, x, y);
            ReleaseDC(hWnd, hdc);
            break;
        case Shape::eLINE:
            hdc = GetDC(hWnd);
            SetROP2(hdc, R2_NOT);			                // 그리기 반전 모드로 변경
            MoveToEx(hdc, x, y, NULL);	                    // 현재 위치 좌표 선정
            LineTo(hdc, oldx, oldy);		                // 현재 좌표 + 이전 좌표를 가지고 선 그리기, R2_NOT로 인해 두번 그려질때 선이 지워짐
            x = LOWORD(lParam);
            y = HIWORD(lParam);
            MoveToEx(hdc, x, y, NULL);                      // 현재 위치 좌표 선정
            LineTo(hdc, oldx, oldy);                        // 현재 좌표 + 이전 좌표를 가지고 선 그리기
            ReleaseDC(hWnd, hdc);
            break;
        case Shape::eELLIPSE:
            hdc = GetDC(hWnd);
            SetROP2(hdc, R2_NOT);			                // 그리기 반전 모드로 변경
            SelectObject(hdc, GetStockObject(NULL_BRUSH));  // NULL_BRUSH로 도형 안을 비운다.
            Ellipse(hdc, oldx, oldy, x, y);
            x = LOWORD(lParam);
            y = HIWORD(lParam);
            Ellipse(hdc, oldx, oldy, x, y);
            ReleaseDC(hWnd, hdc);
            break;
        case Shape::eRECT:
            hdc = GetDC(hWnd);
            SetROP2(hdc, R2_NOT);			                // 그리기 반전 모드로 변경
            SelectObject(hdc, GetStockObject(NULL_BRUSH));  // NULL_BRUSH로 도형 안을 비운다.
            Rectangle(hdc, oldx, oldy, x, y);
            x = LOWORD(lParam);
            y = HIWORD(lParam);
            Rectangle(hdc, oldx, oldy, x, y);
            ReleaseDC(hWnd, hdc);
            break;
        }
        return 0;
    case WM_LBUTTONUP:
        bNowDraw = FALSE;
        switch (eMenuNumber) {
        case Shape::eCURVE:
            break;
        case Shape::eLINE:
            hdc = GetDC(hWnd);
            MoveToEx(hdc, x, y, NULL);
            LineTo(hdc, oldx, oldy);
            ReleaseDC(hWnd, hdc);
            break;
        case Shape::eELLIPSE:
            hdc = GetDC(hWnd);
            SelectObject(hdc, GetStockObject(NULL_BRUSH));  // NULL_BRUSH로 도형 안을 비운다.
            Ellipse(hdc, oldx, oldy, x, y);
            ReleaseDC(hWnd, hdc);
            break;
        case Shape::eRECT:
            hdc = GetDC(hWnd);
            SelectObject(hdc, GetStockObject(NULL_BRUSH));  // NULL_BRUSH로 도형 안을 비운다.
            Rectangle(hdc, oldx, oldy, x, y);
            ReleaseDC(hWnd, hdc);
            break;
        }
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}