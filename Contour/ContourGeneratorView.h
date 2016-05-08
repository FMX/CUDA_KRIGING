// ContourGeneratorView.h : interface of the CContourGeneratorView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONTOURGENERATORVIEW_H__6FDAA57E_C7F7_40C4_8123_53F570188117__INCLUDED_)
#define AFX_CONTOURGENERATORVIEW_H__6FDAA57E_C7F7_40C4_8123_53F570188117__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "ColorLookUpTable.h"

class CContourGeneratorView : public CView
{
protected: // create from serialization only
	CContourGeneratorView();
	DECLARE_DYNCREATE(CContourGeneratorView)

// Attributes
public:
	CContourGeneratorDoc* GetDocument();

// Operations
public:
//	CColorLookUpTable m_ColorTable;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CContourGeneratorView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CContourGeneratorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CContourGeneratorView)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSetContourLevel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ContourGeneratorView.cpp
inline CContourGeneratorDoc* CContourGeneratorView::GetDocument()
   { return (CContourGeneratorDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTOURGENERATORVIEW_H__6FDAA57E_C7F7_40C4_8123_53F570188117__INCLUDED_)
