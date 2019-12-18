#include <Windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK PaintDlgProc(HWND , UINT , WPARAM , LPARAM );
int GetCheckedRadioButton(HWND hDlg, int iFirst, int iLast);

HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("SimplePaint2");

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

enum class tag_ColorRef
{
    eRed,
    eBlue,
    eYellow,
    eBlack,
} SelectColor;

COLORREF arColorRef[] = { RGB(255, 0, 0), RGB(0, 0, 255), RGB(255, 255, 0), RGB(0, 0, 0) };

HWND r1, r2, r3, r4, c1;
int Graph = 0;
const UINT uDefWidth = 2;
HPEN MyPen, OldPen;
static int x, y;
static BOOL bNowDraw = FALSE;
static int iWidth = uDefWidth;
BOOL bBold = FALSE;
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    switch (iMessage)
    {
    case WM_CREATE:
        hWndMain = hWnd;
        SelectColor = tag_ColorRef::eBlack;
        return 0;
    case WM_RBUTTONDOWN:
        DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOGPAINT),
            hWnd, PaintDlgProc);
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
            MyPen = CreatePen(PS_SOLID, iWidth, arColorRef[(UINT)SelectColor]);
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
    case WM_KEYDOWN:
        switch (wParam) {
        case VK_SPACE:
        case VK_BACK:
            InvalidateRect(hWnd, NULL, TRUE);
        }
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

INT_PTR CALLBACK PaintDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    switch (iMessage) {
    case WM_INITDIALOG:
        CheckDlgButton(hDlg, IDC_CHECK_BOLD, bBold ? BST_CHECKED : BST_UNCHECKED);
        CheckRadioButton(hDlg, IDC_RADIO_RED, IDC_RADIO_BLACK, IDC_RADIO_RED + (UINT)SelectColor);
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDOK:
            SelectColor = tag_ColorRef(GetCheckedRadioButton(hDlg, IDC_RADIO_RED,
                IDC_RADIO_BLACK) - IDC_RADIO_RED);

            bBold = (IsDlgButtonChecked(hDlg, IDC_CHECK_BOLD) == BST_CHECKED);
            if (bBold) 
                iWidth = 5;
            else 
                iWidth = uDefWidth;
            EndDialog(hDlg, IDOK);
            return TRUE;
        case IDCANCEL:
            EndDialog(hDlg, IDCANCEL);
            return TRUE;
        }
        return FALSE;
    }
    return FALSE;
}

int GetCheckedRadioButton(HWND hDlg, int iFirst, int iLast)
{
    int id;
    for (id = iFirst; id <= iLast; id++) {
        if (IsDlgButtonChecked(hDlg, id) == BST_CHECKED)
            return id;
    }
    return -1;
}