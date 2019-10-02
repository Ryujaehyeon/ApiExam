#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("TextOut");

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

	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (iMessage)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	//case WM_LBUTTONDOWN:
	//	hdc = GetDC(hWnd);
	//	TextOut(hdc, 100, 100, TEXT("Beautiful Korea"), 15);
	//	ReleaseDC(hWnd, hdc);
	//	return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps); 
		//SetTextAlign(hdc, TA_CENTER);
		//SetTextAlign(hdc, TA_UPDATECP);
		//SetTextAlign(hdc, TA_LEFT);
		//SetTextAlign(hdc, TA_RIGHT);
		//TextOut(hdc, 200, 60, TEXT("Beautiful Korea"), 15);
		//TextOut(hdc, 200, 80, TEXT("is My"), 5);
		//TextOut(hdc, 200, 100, TEXT("Lovely Home Country"), 19);

		TCHAR str[128];
		int Score = 85;
		wsprintf(str, TEXT("현재 점수는 %d점 입니다."), Score);
		TextOut(hdc, 10, 10, str, lstrlen(str));

		EndPaint(hWnd, &ps);
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}