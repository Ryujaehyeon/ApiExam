#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("WndExtra");

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
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	WndClass.cbWndExtra = 4;
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
	int x, y;

	switch (iMessage) {
	case WM_CREATE:
		hWndMain = hWnd;
		for (x = 0; x < 300; x += 100){
			for (y = 0; y< 300; y += 100){
				CreateWindow(TEXT("ChildCls"), NULL, WS_CHILD|WS_VISIBLE,
					x, y, 100, 100, hWnd, (HMENU)NULL, g_hInst, NULL);
			}
		}
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
	BOOL Prop;

	switch (iMessage) {
	case WM_CREATE:
		//SetWindowLong(hWnd, 0, TRUE); //여분 메모리 코드
		SetProp(hWnd, TEXT("bEllipse"), (HANDLE)TRUE);
		return 0;
	case WM_LBUTTONDOWN:
		Prop=(BOOL)GetProp(hWnd, TEXT("bEllipse"));
		//SetWindowLong(hWnd, 0, !GetWindowLong(hWnd, 0));
		SetProp(hWnd, TEXT("bEllipse"), (HANDLE)!Prop);
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		Prop = (BOOL)GetProp(hWnd, TEXT("bEllipse"));
		if (Prop) {
			Ellipse(hdc, 10, 10, 90, 90);
		}
		else
		{
			MoveToEx(hdc, 10, 10, NULL); LineTo(hdc, 90, 90);
			MoveToEx(hdc, 10, 90, NULL); LineTo(hdc, 90, 10);
		}
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		RemoveProp(hWnd, TEXT("bEllipse"));
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}
                                                                                                                                                                                                                              