#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("ChildWnd");
BOOL bEillpse = TRUE;

//typedef struct tagWNDCLASS
//{
//	UINT		style;
//	WNDPROC		lpfnWndProc;
//	int			cbClsExtra;
//	int			cbWndExtra;
//	HINSTANCE	hInstance;
//	HICON		hIcon;
//	HCURSOR		hCursor;
//	HBRUSH		hbrBackground;
//	LPCSTR		lpszMenuName;
//	LPCSTR		lpszClassName;
//}WNDCLASS;

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

	WndClass.hCursor = LoadCursor(NULL, IDC_CROSS);
	WndClass.lpfnWndProc = ChildProc;
	WndClass.lpszClassName = TEXT("ChildCls");
	WndClass.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
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
	switch (iMessage) {
	case WM_CREATE:
		//���ϵ� ������ ����
		//CreateWindow(TEXT("ChildCls"), NULL, WS_CHILD | WS_VISIBLE,
		//	100, 100, 100, 100, hWnd, (HMENU)NULL, g_hInst, NULL);
		CreateWindow(TEXT("ChildCls"), TEXT("Popup"), WS_POPUP | WS_CAPTION | WS_SYSMENU|WS_VISIBLE,
			100, 100, 110, 125, hWnd, (HMENU)NULL, g_hInst, NULL);
		//hWndMain = hWnd;
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}

LRESULT CALLBACK ChildProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	switch (iMessage) {
	case WM_LBUTTONDOWN:
		bEillpse = !bEillpse;
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if (bEillpse) {
			Ellipse(hdc, 10, 10, 90, 90);
		}
		else {
			MoveToEx(hdc, 10, 10, NULL); LineTo(hdc, 90, 90);
			MoveToEx(hdc, 10, 90, NULL); LineTo(hdc, 90, 10);
		}
		EndPaint(hWnd, &ps);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
