#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("SimplePaint");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpszCmdParam, int nCmdShow)
{
    HWND hWnd;
    MSG Message;
    WNDCLASS WndClass;
    g_hInst = hInstance;

    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    WndClass.hInstance = hInstance;
    WndClass.lpfnWndProc = WndProc;
    WndClass.lpszClassName = lpszClass;
    WndClass.lpszMenuName = NULL;
    WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    RegisterClass(&WndClass);

    hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, (HMENU)NULL, hInstance, NULL);

    ShowWindow(hWnd, nCmdShow);

    while (GetMessage(&Message, NULL, 0, 0))
    {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }

    return (int)Message.wParam;
}

enum class eRadio
{
    ID_R1 = 101,
    ID_R2,
    ID_R3,
    ID_R4,
};

HWND r1, r2, r3, r4, c1;
int Graph = 0;
COLORREF Color = RGB(0, 0, 0);
const UINT uDefWidth = 2;
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    HPEN MyPen, OldPen;
    static int x, y;
    static BOOL bNowDraw = FALSE;
    static int iWidth = uDefWidth;

    switch (iMessage)
    {
    case WM_CREATE:
        // 생성 및 초기화
        // 그룹상자 및 라디오 버튼
        CreateWindow(TEXT("button"), TEXT("Graph"), WS_CHILD | WS_VISIBLE |
            BS_GROUPBOX, 5, 5, 120, 140, hWnd, (HMENU)0, g_hInst, NULL);
        CreateWindow(TEXT("button"), TEXT("Reset"), WS_CHILD | WS_VISIBLE |
            BS_PUSHBUTTON, 5, 150, 50, 30, hWnd, (HMENU)1, g_hInst, NULL);
        r1 = CreateWindow(TEXT("button"), TEXT("Red"), WS_CHILD | WS_VISIBLE |
            BS_AUTORADIOBUTTON | WS_GROUP,
            10, 20, 100, 30, hWnd, (HMENU)eRadio::ID_R1, g_hInst, NULL);
        r2 = CreateWindow(TEXT("button"), TEXT("Blue"), WS_CHILD | WS_VISIBLE |
            BS_AUTORADIOBUTTON,
            10, 50, 100, 30, hWnd, (HMENU)eRadio::ID_R2, g_hInst, NULL);
        r3 = CreateWindow(TEXT("button"), TEXT("Yellow"), WS_CHILD | WS_VISIBLE |
            BS_AUTORADIOBUTTON,
            10, 80, 100, 30, hWnd, (HMENU)eRadio::ID_R3, g_hInst, NULL);
        r4 = CreateWindow(TEXT("button"), TEXT("Black"), WS_CHILD | WS_VISIBLE |
            BS_AUTORADIOBUTTON,
            10, 110, 100, 30, hWnd, (HMENU)eRadio::ID_R4, g_hInst, NULL);
        // 라디오 버튼 초기 선택
        CheckRadioButton(hWnd, (UINT)eRadio::ID_R1, (UINT)eRadio::ID_R4, (UINT)eRadio::ID_R4);

        // 그룹 상자 및 체크 박스
        CreateWindow(TEXT("button"), TEXT("Width"), WS_CHILD | WS_VISIBLE |
            BS_GROUPBOX, 130, 5, 120, 140, hWnd, (HMENU)2, g_hInst, NULL);

        c1 = CreateWindow(TEXT("button"), TEXT("Bold"), WS_CHILD | WS_VISIBLE |
            BS_AUTOCHECKBOX, 140, 20, 100, 30, hWnd, (HMENU)3, g_hInst, NULL);
        
        break;
    case WM_COMMAND: // 처리
        switch (LOWORD(wParam)) {
        case 1:
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case 3:
            if ((BOOL)(SendMessage(c1, BM_GETCHECK, 0, 0) == BST_CHECKED)) iWidth = 5;
            else iWidth = uDefWidth;
            break;
        case (UINT)eRadio::ID_R1:
            Color = RGB(255, 0, 0);
            break;
        case (UINT)eRadio::ID_R2:
            Color = RGB(0, 0, 255);
            break;
        case (UINT)eRadio::ID_R3:
            Color = RGB(255, 255, 0);
            break;
        case (UINT)eRadio::ID_R4:
            Color = RGB(0, 0, 0);
            break;
        }
        return 0;
    case WM_LBUTTONDOWN:
        x = LOWORD(lParam);
        y = HIWORD(lParam);
        bNowDraw = TRUE;
        return 0;
    case WM_MOUSEMOVE: // 그리기
        if (bNowDraw)
        {
            hdc = GetDC(hWnd);
            MyPen = CreatePen(PS_SOLID, iWidth, Color);
            OldPen = (HPEN)SelectObject(hdc, MyPen);

            MoveToEx(hdc, x, y, NULL);
            x = LOWORD(lParam);
            y = HIWORD(lParam);
            LineTo(hdc, x, y);

            SelectObject(hdc, OldPen);
            DeleteObject(MyPen);
            ReleaseDC(hWnd, hdc);
        }
        return 0;
    case WM_LBUTTONUP:
        bNowDraw = FALSE;
        return 0;
    case WM_LBUTTONDBLCLK:
        // 화면 지우기
        InvalidateRect(hWnd, NULL, TRUE);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}