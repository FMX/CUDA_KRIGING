// ContourGenerator.h : main header file for the CONTOURGENERATOR application
//

#if !defined(AFX_CONTOURGENERATOR_H__43E06829_B931_4834_95AB_BED09CFED87F__INCLUDED_)
#define AFX_CONTOURGENERATOR_H__43E06829_B931_4834_95AB_BED09CFED87F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CContourGeneratorApp:
// See ContourGenerator.cpp for the implementation of this class
//

class CContourGeneratorApp : public CWinApp
{
public:
	CContourGeneratorApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CContourGeneratorApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CContourGeneratorApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTOURGENERATOR_H__43E06829_B931_4834_95AB_BED09CFED87F__INCLUDED_)
