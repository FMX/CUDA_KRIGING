// ContourGeneratorDoc.cpp : implementation of the CContourGeneratorDoc class
//

#include "stdafx.h"
#include "ContourGenerator.h"

#include "ContourGeneratorDoc.h"
#include "Contour.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CContourGeneratorDoc

IMPLEMENT_DYNCREATE(CContourGeneratorDoc, CDocument)

BEGIN_MESSAGE_MAP(CContourGeneratorDoc, CDocument)
	//{{AFX_MSG_MAP(CContourGeneratorDoc)
	ON_COMMAND(ID_LOADCOLORTABLE, OnLoadcolortable)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CContourGeneratorDoc construction/destruction

CContourGeneratorDoc::CContourGeneratorDoc()
{
	// TODO: add one-time construction code here
	m_bDrawGrid = FALSE;

	m_pCureList = NULL;

    m_pContour = NULL;

}

CContourGeneratorDoc::~CContourGeneratorDoc()
{
	if( m_pContour!=NULL )
		delete m_pContour;
}

BOOL CContourGeneratorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CContourGeneratorDoc serialization

void CContourGeneratorDoc::Serialize(CArchive& ar)
{
	TRACE0("CContourGeneratorDoc::Serialize(CArchive& ar)\n");
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
		
	}
}

/////////////////////////////////////////////////////////////////////////////
// CContourGeneratorDoc diagnostics

#ifdef _DEBUG
void CContourGeneratorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CContourGeneratorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CContourGeneratorDoc commands

BOOL CContourGeneratorDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{

	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	if( m_GeoData.ReadData(lpszPathName) == FALSE )
		return FALSE;

	if( m_pContour!=NULL)
	{
		delete m_pContour;
		m_pContour = NULL; 
	}

	if(m_pContour==NULL)
	{
		m_pContour = new CContour;
		m_pContour->SetDataOwner(&m_GeoData);
	}

	m_pContour->GenerateDefaultContourValues();
	m_pContour->GenerateContours();
	
	
	return TRUE;
}


//DEL void CContourGeneratorDoc::OnRunTracing() 
//DEL {
//DEL 
//DEL 	if( m_GeoData.GetGridData()==NULL)
//DEL 	{
//DEL 		AfxMessageBox("没有数据");
//DEL 		return;
//DEL 	}
//DEL 
//DEL 
//DEL 
//DEL 
//DEL //	m_Contour.GenerateContourValues(18,20,105);//先指定所要追踪的值
//DEL     m_Contour.GenerateDefaultContourValues();
//DEL 	m_Contour.GenerateContours();
//DEL 
//DEL //	m_ctrTracer.SetInput(m_GeoData.GetGridData());
//DEL //	if(m_pCureList==NULL)
//DEL //		m_pCureList = new CCurveList;
//DEL //	m_ctrTracer.SetOutput(m_pCureList);
//DEL //	m_ctrTracer.SetGridDataInfo(m_GeoData.GetDataInfo());
//DEL //	if( !m_ctrTracer.ExecuteTracing(70) )
//DEL //	{
//DEL //		delete m_pCureList;
//DEL //		m_pCureList = NULL;
//DEL //	}
//DEL 
//DEL 
//DEL 
//DEL 	
//DEL 	UpdateAllViews(NULL);
//DEL 
//DEL }



void CContourGeneratorDoc::OnLoadcolortable() 
{

	if( m_pContour==NULL)
	{
		AfxMessageBox("没有数据");
		return;
	}

	m_pContour->ModifyColorTable();
	 
	 UpdateAllViews(NULL);
	
}
