// ClrFileDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ContourGenerator.h"
#include "ColorLookUpTable.h"
#include "ClrFileDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClrFileDialog

IMPLEMENT_DYNAMIC(CClrFileDialog, CFileDialog)

CClrFileDialog::CClrFileDialog(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
		DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
		CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
		// if OFN_ENABLETEMPLATE is set, define the custom dialog template here
		m_ofn.lpTemplateName = MAKEINTRESOURCE(IDD_FILECOLORTMP_DIALOG);
}


BEGIN_MESSAGE_MAP(CClrFileDialog, CFileDialog)
	//{{AFX_MSG_MAP(CClrFileDialog)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CClrFileDialog::DrawColorSpectrum()
{
	CWnd *pWnd=GetDlgItem(IDC_STATIC_PREVIEW);
	CDC *pDC=pWnd->GetDC();
	CRect rect;
	pWnd->GetClientRect(&rect);
	
	m_ColorTable.DrawColorSpectrum(pDC,rect);
	
	pWnd->ReleaseDC(pDC);
	
}

void CClrFileDialog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	DrawColorSpectrum();	
}

BOOL CClrFileDialog::OnInitDialog()
{
	CFileDialog::OnInitDialog();
	
	m_ofn.hInstance = AfxGetInstanceHandle();
	
	return TRUE;
}

void CClrFileDialog::OnFileNameChange()
{
	m_ColorTable.LoadColorTable(GetPathName());
	DrawColorSpectrum();

	CFileDialog::OnFileNameChange();

}