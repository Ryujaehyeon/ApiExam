
// MFCKeyView.cpp: CMFCKeyView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MFCKey.h"
#endif

#include "MFCKeyDoc.h"
#include "MFCKeyView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCKeyView

IMPLEMENT_DYNCREATE(CMFCKeyView, CView)

BEGIN_MESSAGE_MAP(CMFCKeyView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CHAR()
END_MESSAGE_MAP()

// CMFCKeyView 생성/소멸

CMFCKeyView::CMFCKeyView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMFCKeyView::~CMFCKeyView()
{
}

BOOL CMFCKeyView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMFCKeyView 그리기

void CMFCKeyView::OnDraw(CDC* pDC)
{
	CMFCKeyDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	pDC->TextOutW(100, 100, pDoc->str);
}


// CMFCKeyView 인쇄

BOOL CMFCKeyView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMFCKeyView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMFCKeyView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CMFCKeyView 진단

#ifdef _DEBUG
void CMFCKeyView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCKeyView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCKeyDoc* CMFCKeyView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCKeyDoc)));
	return (CMFCKeyDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCKeyView 메시지 처리기


void CMFCKeyView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CMFCKeyDoc* pDoc = GetDocument();
	pDoc->str += (TCHAR)nChar;
	Invalidate();

	CView::OnChar(nChar, nRepCnt, nFlags);
}
