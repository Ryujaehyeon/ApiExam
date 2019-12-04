#include <Windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("Menu");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1));
	WndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	
	HACCEL hAccel;
	hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));
	while (GetMessage(&Message, NULL, 0, 0)) {
		if (!TranslateAccelerator(hWnd, hAccel, &Message))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	}
	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR str[256];

	switch (iMessage){
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_FILE_MENU1:
			MessageBox(hWnd, TEXT("첫 번째 메뉴를 선택했습니다."), TEXT("Menu Demo"), MB_OK);
			break;
		case ID_FILE_MENU2:
			MessageBox(hWnd, TEXT("두 번째 메뉴를 선택했습니다."), TEXT("Menu Demo"), MB_OK);
			break;
		case ID_FILE_OPEN:
			MessageBox(hWnd, TEXT("열기 메뉴를 선택했습니다."), TEXT("Menu Demo"), MB_OK);
			break;
		case ID_FILE_SAVE:
			MessageBox(hWnd, TEXT("저장 메뉴를 선택했습니다."), TEXT("Menu Demo"), MB_OK);
			break;
		case ID_FILE_NEW:
			MessageBox(hWnd, TEXT("새 페이지 메뉴를 선택했습니다."), TEXT("Menu Demo"), MB_OK);
			break;
		case ID_EDIT_CUT:
			MessageBox(hWnd, TEXT("잘라내기를 선택했습니다."), TEXT("Menu Demo"), MB_OK);
			break;
		case ID_EDIT_COPY:
			MessageBox(hWnd, TEXT("복사를 선택했습니다."), TEXT("Menu Demo"), MB_OK);
			break;
		case ID_EDIT_PASTER:
			MessageBox(hWnd, TEXT("붙여넣기를 선택했습니다."), TEXT("Menu Demo"), MB_OK);
			break;
		case ID_EDIT_DELETE:
			MessageBox(hWnd, TEXT("삭제를 선택했습니다."), TEXT("Menu Demo"), MB_OK);
			break;
		case ID_EDIT_SELECTALL:
			MessageBox(hWnd, TEXT("전체 선택을 선택했습니다."), TEXT("Menu Demo"), MB_OK);
			break;
		case ID_FILE_PRINT:
			MessageBox(hWnd, TEXT("출력 메뉴를 선택했습니다."), TEXT("Menu Demo"), MB_OK);
			break;
		case ID_FILE_EXIT:
			DestroyWindow(hWnd);
			break;
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		LoadString(g_hInst, IDS_STRING_NEW, str, 256);
		TextOut(hdc, 10, 10, str, lstrlen(str));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}
