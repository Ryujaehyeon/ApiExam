#include <Windows.h>
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("MyListBox"); // 기본적으로 선언되어있는 컨트롤 이름을 사용하면 예상과는 다른 모습으로 되므로 My등 이름을 다르게 해주자. 

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

#define ID_LISTBOX 100
#define ID_LISTBOX2 101

const TCHAR *Items[] = { TEXT("Apple"), TEXT("Orange"), TEXT("Melon"),TEXT("Grape"), TEXT("Strawberry") };
HWND hList, hCopyList;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    int i, iArrSize;
    TCHAR str[128];
    iArrSize = (sizeof(Items) / sizeof(Items[0]));

    switch (iMessage) {
    case WM_CREATE:
        hList = CreateWindow(TEXT("listbox"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | 
            LBS_NOTIFY, 10, 10, 100, 200, hWnd, (HMENU)ID_LISTBOX, g_hInst, NULL);
        hCopyList = CreateWindow(TEXT("listbox"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
            LBS_NOTIFY, 120, 10, 100, 200, hWnd, (HMENU)ID_LISTBOX2, g_hInst, NULL);

        for (i = 0; i < iArrSize; i++) {
            SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)Items[i]);
        }
        return 0;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_LISTBOX:
            switch (HIWORD(wParam)) {
            case LBN_SELCHANGE:
                i = SendMessage(hList, LB_GETCURSEL, 0, 0);
                SendMessage(hList, LB_GETTEXT, i, (LPARAM)str);
                SetWindowText(hWnd, str);
                break;
            case LBN_DBLCLK:
                i = SendMessage(hList, LB_GETCURSEL, 0, 0);
                SendMessage(hList, LB_GETTEXT, i, (LPARAM)str);
                SendMessage(hCopyList, LB_ADDSTRING, 0, (LPARAM)str);
                break;
            }
            return 0;
        case ID_LISTBOX2:
            switch (HIWORD(wParam)) {
            case LBN_SELCHANGE:
                i = SendMessage(hCopyList, LB_GETCURSEL, 0, 0);
                SendMessage(hCopyList, LB_GETTEXT, i, (LPARAM)str);
                SetWindowText(hWnd, str);
                break;
            case LBN_DBLCLK:
                i = SendMessage(hList, LB_GETCURSEL, 0, 0);
                SendMessage(hList, LB_GETTEXT, i, (LPARAM)str);
                SendMessage(hCopyList, LB_DELETESTRING, 0, (LPARAM)str);
                break;
            }
            return 0;
        }
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}