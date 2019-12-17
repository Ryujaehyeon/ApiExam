#include <Windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK ReadingDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
HINSTANCE g_hInst;
HWND hDlgMain;
LPCTSTR lpszClass = TEXT("ReadingCounter");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
    g_hInst = hInstance;
    DialogBox(g_hInst, MAKEINTRESOURCE(IDD_READING), HWND_DESKTOP, ReadingDlgProc);
    return 0;
}

int PageStart, PageEnd, NowPage;
int PageSec, NowSec;
enum class tag_Status{WAIT, RUN, PAUSE} Status;

INT_PTR CALLBACK ReadingDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam) 
{
    TCHAR Mes[256];

    switch (iMessage) {
    case WM_INITDIALOG:
        hDlgMain = hDlg;
        Status = tag_Status::WAIT;
        return TRUE;
    case WM_TIMER:
        NowSec--;
        if (NowSec == 0) {
            if (IsDlgButtonChecked(hDlg, IDC_CHKBEEP) == BST_CHECKED) {
                MessageBeep(0);
            }
            NowPage++;
            NowSec = PageSec;
            if (NowPage > PageEnd) {
                SetDlgItemText(hDlg, IDC_STTIME, TEXT("�� �о����ϴ�."));
                KillTimer(hDlg, 1);
                Status = tag_Status::WAIT;
                return TRUE;
            }
        }
        wsprintf(Mes, TEXT("%d ������ �д� �� : %d �� ����"), NowPage, NowSec);
        SetDlgItemText(hDlg, IDC_STTIME, Mes);
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDC_BTNSTART:
            PageStart = GetDlgItemInt(hDlg, IDC_EDSTART, NULL, FALSE);
            PageEnd = GetDlgItemInt(hDlg, IDC_EDEND, NULL, FALSE);
            if (PageStart == 0 || PageEnd == 0 || PageStart >= PageEnd) {
                MessageBox(hDlg, TEXT("������ ��ȣ�� �ٽ� �Է��Ͻÿ�."),
                    TEXT("�˸�"), MB_OK);
                return TRUE;
            }
            NowPage = PageStart;
            PageSec = GetDlgItemInt(hDlg, IDC_EDTIME, NULL, FALSE);
            if (PageSec <= 0) {
                MessageBox(hDlg, TEXT("�ð��� �ٽ� �Է��Ͻÿ�."), TEXT("�˸�"), MB_OK);
                return TRUE;
            }
            NowSec = PageSec;
            SetTimer(hDlg, 1, 1000, NULL);
            SendMessage(hDlg, WM_TIMER, 1, 0);
            Status = tag_Status::RUN;
            return TRUE;
        case IDC_BTNPAUSE:
            if (Status == tag_Status::PAUSE) {
                SetTimer(hDlg, 1, 1000, NULL);
                Status = tag_Status::RUN;
            }
            else if (Status == tag_Status::RUN) {
                KillTimer(hDlg, 1);
                Status = tag_Status::PAUSE;
            }
            return TRUE;
        case IDC_CANCEL:
            KillTimer(hDlg, 1);
            EndDialog(hDlg, IDCANCEL);
            return TRUE;
        }
        return FALSE;
    }
    return FALSE;
}