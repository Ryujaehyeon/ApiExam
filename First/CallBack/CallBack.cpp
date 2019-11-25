#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("CallBack");

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

// TIMERPROC callback function로 검색해서
// https://docs.microsoft.com/ko-kr/windows/win32/api/winuser/nc-winuser-timerproc 을 보면
// 3번째 인자가 UINT_PTR 인것을 알 수 있으며 64비트 호환 프로그래밍을 위해 _PTR 접미사가 붙어있는 타입이 존재하며,
// 원래는 x86 환경 UINT idEvent였으나 현재 실행 환경인 x64로 실행하려면 UINT_PTR로 변경해야한다.
void CALLBACK TimerProc(HWND hWnd, UINT iMsg, UINT_PTR idEvent, DWORD dwTime)
{
	HDC hdc;
	int i;
	hdc = GetDC(hWnd);
	for (i = 0; i < 1000; i++)
	{
		SetPixel(hdc, rand() % 500, rand() % 400,
			RGB(rand() % 256, rand() % 256, rand() % 256));
	}
	ReleaseDC(hWnd, hdc);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;

	switch (iMessage){
	case WM_CREATE:
		SetTimer(hWnd, 1, 100, TimerProc);
		return 0;
	case WM_LBUTTONDOWN:
		hdc = GetDC(hWnd);
		Ellipse(hdc, LOWORD(lParam) - 10, HIWORD(lParam) - 10,
			LOWORD(lParam) + 10, HIWORD(lParam) + 10);
		ReleaseDC(hWnd, hdc);
		return 0;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}
