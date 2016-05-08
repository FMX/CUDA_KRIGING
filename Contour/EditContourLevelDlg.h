#if !defined(AFX_EDITCONTOURLEVELDLG_H__596FD252_E68D_4473_ACF7_5D5D1AC4E676__INCLUDED_)
#define AFX_EDITCONTOURLEVELDLG_H__596FD252_E68D_4473_ACF7_5D5D1AC4E676__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditContourLevelDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditContourLevelDlg dialog

class CEditContourLevelDlg : public CDialog
{
// Construction
public:
	CEditContourLevelDlg(const CContourValues& rhs,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEditContourLevelDlg)
	enum { IDD = IDD_EDIT_CONTOUR_LEVELS };
	CListBox	m_levelListBox;
	float	m_fNewLevel;
	float	m_levelInterval;
	float	m_levelMax;
	float	m_levelMin;
	CString	m_strZMax;
	CString	m_strZMin;
	//}}AFX_DATA

//	CList<float,float> m_Levels;
//	CList<float,float>& GetLevelsList(){return m_Levels;}

	CContourValues	m_Levels;

	float m_zMin;
	float m_zMax;

public:
	void SetZMinMax(float zMin, float zMax);
	void AddLevelsToListBox();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditContourLevelDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditContourLevelDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDeleteLevel();
	afx_msg void OnAddLevel();
	afx_msg void OnGenerateLevel();
	afx_msg void OnMoveAll();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITCONTOURLEVELDLG_H__596FD252_E68D_4473_ACF7_5D5D1AC4E676__INCLUDED_)
