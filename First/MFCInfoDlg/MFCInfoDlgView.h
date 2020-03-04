
// MFCInfoDlgView.h: CMFCInfoDlgView 클래스의 인터페이스
//

#pragma once


class CMFCInfoDlgView : public CView
{
protected: // serialization에서만 만들어집니다.
	CMFCInfoDlgView() noexcept;
	DECLARE_DYNCREATE(CMFCInfoDlgView)

// 특성입니다.
public:
	CMFCInfoDlgDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CMFCInfoDlgView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // MFCInfoDlgView.cpp의 디버그 버전
inline CMFCInfoDlgDoc* CMFCInfoDlgView::GetDocument() const
   { return reinterpret_cast<CMFCInfoDlgDoc*>(m_pDocument); }
#endif

