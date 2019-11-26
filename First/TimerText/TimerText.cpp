#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("TimerText");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
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
	static TCHAR str[128];
	static int y = 100;
	static int iNum = 0;

	switch (iMessage){
	case WM_CREATE:
		lstrcpy(str, "0");
		InvalidateRect(hWnd, NULL, TRUE);
		SetTimer(hWnd, 0, 2000, NULL); // 2초 갱신 타이머
		SetTimer(hWnd, 1, 10, NULL); // 좌표 변경용 타이머

		return 0;

	case WM_TIMER:
		switch (wParam)
		{
		case 0:
			if (iNum < 1)
				iNum++;
			else
				iNum = 0;
			y = 100;
			break;

		case 1:
			if (y > 10)
				y -= 1;
			break;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		wsprintf(str, "%d", iNum);
		TextOut(hdc, 10, y, str, lstrlen(str));
		EndPaint(hWnd, &ps);
		return 0;

	case WM_DESTROY:
		KillTimer(hWnd, 0);
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}
