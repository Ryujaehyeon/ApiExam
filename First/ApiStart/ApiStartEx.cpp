//#include <Windows.h>
//#include "resource.h"
//
//LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//HINSTANCE g_hInst;
//HWND hWndMain;
//LPCTSTR lpszClass = TEXT("WndClsEx");
//
//int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
//    , LPSTR lpszCmdParam, int nCmdShow)
//{
//    HWND hWnd;
//    MSG Message;
//    WNDCLASSEX WndClassEx;
//    g_hInst = hInstance;
//
//    WndClassEx.cbSize = sizeof(WNDCLASSEX);
//    WndClassEx.cbClsExtra = 0;
//    WndClassEx.cbWndExtra = 0;
//    WndClassEx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
//    WndClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);
//    WndClassEx.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
//    WndClassEx.hIconSm = NULL;
//    WndClassEx.hInstance = hInstance;
//    WndClassEx.lpfnWndProc = WndProc;
//    WndClassEx.lpszClassName = lpszClass;
//    WndClassEx.lpszMenuName = NULL;
//    WndClassEx.style = CS_HREDRAW | CS_VREDRAW;
//    if (RegisterClassEx(&WndClassEx) == 0) {
//        MessageBox(NULL, TEXT("윈도우 클래스 등록 실패"), TEXT("에러"), MB_OK);
//        return -1;
//    }
//}
//
//LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
//{
//	HDC hdc;
//	PAINTSTRUCT ps;
//
//	switch (iMessage) {
//	case WM_CREATE:
//		hWndMain = hWnd;
//		return 0;
//	case WM_PAINT:
//		hdc = BeginPaint(hWnd, &ps);
//		EndPaint(hWnd, &ps);
//		return 0;
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		return 0;
//	}
//	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
//}