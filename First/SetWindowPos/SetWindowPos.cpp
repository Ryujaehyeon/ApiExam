#include <Windows.h>
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("SetWindowPos");

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
	static HWND hBtn;
	RECT brt;
	const TCHAR* Mes = TEXT("클릭한 지점으로 버튼을 옮깁니다.");

	switch (iMessage) {
	case WM_CREATE:
		hWndMain = hWnd;
		hBtn = CreateWindow(TEXT("button"), TEXT("버튼"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			20, 80, 100, 25, hWnd, (HMENU)0, g_hInst, NULL);
		return 0;
	case WM_LBUTTONDOWN:
		//GetWindowRect(hBtn, &brt);
		//MoveWindow(hBtn, LOWORD(lParam), HIWORD(lParam),
		//	brt.right - brt.left, brt.bottom - brt.top, TRUE);
		SetWindowPos(hBtn, NULL, LOWORD(lParam), HIWORD(lParam), 0, 0,
			SWP_NOSIZE | SWP_NOZORDER);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 10, 10, Mes, lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}