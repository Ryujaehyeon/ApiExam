// DInfoDlg.cpp: 구현 파일
//

#include "pch.h"
#include "MFCInfoDlg.h"
#include "CInfoDlg.h"
#include "afxdialogex.h"


// DInfoDlg 대화 상자

IMPLEMENT_DYNAMIC(CInfoDlg, CDialogEx)

CInfoDlg::CInfoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_x(0)
	, m_y(0)
	, m_str(_T(""))
{

}

CInfoDlg::~CInfoDlg()
{
}

void CInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_X, m_x);
	DDX_Text(pDX, IDC_Y, m_y);
	DDX_Text(pDX, IDC_STR, m_str);
}


BEGIN_MESSAGE_MAP(CInfoDlg, CDialogEx)
END_MESSAGE_MAP()


// DInfoDlg 메시지 처리기
