#include<Windows.h>
#include"resource.h"

// å���� ��ȯ���� BOOL�� �Ǿ�������, DialogBox���� ������ �߻���Ű�Ƿ� INT_PTR�� ����
INT_PTR CALLBACK MainDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
HWND hDlgMain;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
    , LPTSTR lpszCmdParam, int nCmdShow) // ��Ƽ����Ʈ�� LPTSTR, �����ڵ�� LPWSTR
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