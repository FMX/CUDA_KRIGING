// ContourGeneratorDoc.h : interface of the CContourGeneratorDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONTOURGENERATORDOC_H__776AA081_6954_4146_96B0_3929300F0771__INCLUDED_)
#define AFX_CONTOURGENERATORDOC_H__776AA081_6954_4146_96B0_3929300F0771__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GeoData.h"

class CContour;

class CContourGeneratorDoc : public CDocument
{
protected: // create from serialization only
	CContourGeneratorDoc();
	DECLARE_DYNCREATE(CContourGeneratorDoc)

// Attributes
public:

	CGeoData m_GeoData;
    CContour* m_pContour;
	float m_value;
	BOOL m_bDrawGrid;

	typedef CArray<CGeoPoint,CGeoPoint&> CCurve;
	typedef CTypedPtrList<CPtrList, CCurve*> CCurveList;

	CCurveList* 	m_pCureList;
	


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CContourGeneratorDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CContourGeneratorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CContourGeneratorDoc)
	afx_msg void OnLoadcolortable();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTOURGENERATORDOC_H__776AA081_6954_4146_96B0_3929300F0771__INCLUDED_)
