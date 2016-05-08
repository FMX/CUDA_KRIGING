// ContourGeneratorView.cpp : implementation of the CContourGeneratorView class
//

#include "stdafx.h"
#include "ContourGenerator.h"

#include "ContourGeneratorDoc.h"
#include "ContourGeneratorView.h"
#include "Contour.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CContourGeneratorView

IMPLEMENT_DYNCREATE(CContourGeneratorView, CView)

BEGIN_MESSAGE_MAP(CContourGeneratorView, CView)
	//{{AFX_MSG_MAP(CContourGeneratorView)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_SET_CONTOUR_LEVEL, OnSetContourLevel)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CContourGeneratorView construction/destruction

CContourGeneratorView::CContourGeneratorView()
{
	// TODO: add construction code here

}

CContourGeneratorView::~CContourGeneratorView()
{
}

BOOL CContourGeneratorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CContourGeneratorView drawing

void CContourGeneratorView::OnDraw(CDC* pDC)
{
	CContourGeneratorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

	CRect rect;

	GetClientRect(&rect);



	rect.DeflateRect(5,5);

	if( pDoc->m_pContour != NULL && pDoc->m_pContour->IsHavingContours() )
	{
		pDoc->m_pContour->Draw(pDC,rect);
	}

//	rect.left = 100;
//	rect.right = 300;
//	rect.top = 100;
//	rect.bottom = 130;
//	m_ColorTable.DrawColorSpectrum(pDC,rect);

	
	
}

/////////////////////////////////////////////////////////////////////////////
// CContourGeneratorView printing

BOOL CContourGeneratorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CContourGeneratorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CContourGeneratorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CContourGeneratorView diagnostics

#ifdef _DEBUG
void CContourGeneratorView::AssertValid() const
{
	CView::AssertValid();
}

void CContourGeneratorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CContourGeneratorDoc* CContourGeneratorView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CContourGeneratorDoc)));
	return (CContourGeneratorDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CContourGeneratorView message handlers

void CContourGeneratorView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{

	OnSetContourLevel();

	CView::OnLButtonDblClk(nFlags, point);
}


//DEL void CContourGeneratorView::OnLoadcolortable() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL /*
//DEL 	CClrFileDialog Dlg(
//DEL 		TRUE, // TRUE for FileOpen, FALSE for FileSaveAs
//DEL 		"clr",
//DEL 		"*.clr",	//LPCTSTR lpszFileName = NULL,
//DEL 		OFN_HIDEREADONLY  | OFN_OVERWRITEPROMPT | OFN_ENABLETEMPLATE,
//DEL 		"Data Files (*.clr) | *.clr ||",
//DEL 		NULL);
//DEL 	if(Dlg.DoModal()!=IDOK)
//DEL 		return;
//DEL 
//DEL 	m_ColorTable.LoadColorTable(Dlg.GetPathName());
//DEL 
//DEL 	Invalidate();*/
//DEL 
//DEL 
//DEL }


void CContourGeneratorView::OnSetContourLevel() 
{
	// TODO: Add your command handler code here
	CContourGeneratorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	if(pDoc->m_pContour==NULL)
	{
		AfxMessageBox("没有数据");
		return;
	}

	if(!pDoc->m_pContour->IsHavingContours())
		return;
	
	pDoc->m_pContour->EditContourLevels();

	this->Invalidate();
}
