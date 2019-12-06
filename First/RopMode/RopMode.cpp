#include <Windows.h>
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("RopMode");

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
	static int sx, sy, oldx, oldy;
	static BOOL bNowDraw = FALSE;
	HDC hdc;

	switch (iMessage) {
	case WM_LBUTTONDOWN:					// 마우스 왼쪽 버튼 누른 상태일 때, 
		sx = LOWORD(lParam);				// 좌표 X 저장
		sy = HIWORD(lParam);				// 좌표 Y 저장
		oldx = sx;
		oldy = sy;
		bNowDraw = TRUE;					// 누른 상태를 지속적으로 갱신해준다.
		return 0;

	case WM_MOUSEMOVE:						// 마우스 무브 중
		if (bNowDraw) {
			hdc = GetDC(hWnd);
			SetROP2(hdc, R2_NOT);			// 그리기 반전 모드로 변경
			MoveToEx(hdc, sx, sy, NULL);	// 현재 위치 좌표 선정
			LineTo(hdc, oldx, oldy);		// 현재 좌표 + 이전 좌표를 가지고 선 그리기, R2_NOT로 인해 두번 그려질때 선이 지워짐
			oldx = LOWORD(lParam);
			oldy = HIWORD(lParam);
			MoveToEx(hdc, sx, sy, NULL);    // 현재 위치 좌표 선정
			LineTo(hdc, oldx, oldy);        // 현재 좌표 + 이전 좌표를 가지고 선 그리기
			ReleaseDC(hWnd, hdc);       
		}
		return 0;
	case WM_LBUTTONUP:
		bNowDraw = FALSE;
		hdc = GetDC(hWnd);
		MoveToEx(hdc, sx, sy, NULL);
		LineTo(hdc, oldx, oldy);
		ReleaseDC(hWnd, hdc);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}