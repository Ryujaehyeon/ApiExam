#include<Windows.h>
#include"resource.h"

// 책에는 반환형이 BOOL로 되어있으나, DialogBox에서 에러를 발생시키므로 INT_PTR로 변경
INT_PTR CALLBACK MainDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
HWND hDlgMain;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
    , LPTSTR lpszCmdParam, int nCmdShow) // 멀티바이트는 LPTSTR, 유니코드는 LPWSTR
{
    DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), HWND_DESKTOP, MainDlgProc);
    return 0;
}

INT_PTR CALLBACK MainDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    switch (iMessage) {
    case WM_INITDIALOG:
        hDlgMain = hDlg;
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDOK:
        case IDCANCEL:
            EndDialog(hDlg, IDOK);
            return TRUE;
        }
        return FALSE;
    }
    return FALSE;
}