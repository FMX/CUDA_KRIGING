// EditContourLevelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ContourGenerator.h"
#include "ContourValues.h"
#include "EditContourLevelDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditContourLevelDlg dialog


CEditContourLevelDlg::CEditContourLevelDlg(const CContourValues& rhs,CWnd* pParent /*=NULL*/)
	: CDialog(CEditContourLevelDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditContourLevelDlg)
	m_fNewLevel = 0.0f;
	m_levelInterval = 0.0f;
	m_levelMax = 0.0f;
	m_levelMin = 0.0f;
	m_strZMax = _T("");
	m_strZMin = _T("");
	//}}AFX_DATA_INIT

	m_Levels = rhs;

}


void CEditContourLevelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditContourLevelDlg)
	DDX_Control(pDX, IDC_CONTOUR_LEVEL_LIST, m_levelListBox);
	DDX_Text(pDX, IDC_NEW_LEVEL, m_fNewLevel);
	DDX_Text(pDX, IDC_LEVEL_INTERVAL, m_levelInterval);
	DDX_Text(pDX, IDC_LEVEL_MAX, m_levelMax);
	DDX_Text(pDX, IDC_LEVEL_MIN, m_levelMin);
	DDX_Text(pDX, IDC_ZMAX, m_strZMax);
	DDX_Text(pDX, IDC_ZMIN, m_strZMin);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditContourLevelDlg, CDialog)
	//{{AFX_MSG_MAP(CEditContourLevelDlg)
	ON_BN_CLICKED(IDC_DELETE_LEVEL, OnDeleteLevel)
	ON_BN_CLICKED(IDC_ADD_LEVEL, OnAddLevel)
	ON_BN_CLICKED(IDC_GENERATE_LEVEL, OnGenerateLevel)
	ON_BN_CLICKED(IDC_MOVE_ALL, OnMoveAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditContourLevelDlg message handlers

/*
void CEditContourLevelDlg::AddValuesToLevelList(CList<float,float>& valList)
{
	m_Levels.RemoveAll();

	POSITION pos = valList.GetHeadPosition();
	while(pos!=NULL)
	{
		m_Levels.AddTail(valList.GetNext(pos));
	}
}*/


void CEditContourLevelDlg::SetZMinMax(float zMin, float zMax)
{
	CString str;
	
	str.Format("%f",zMin);
    m_strZMin = str;

	str.Format("%f",zMax);
	m_strZMax = str;

	m_zMin = zMin;
	m_zMax = zMax;

}

void CEditContourLevelDlg::AddLevelsToListBox()
{


	if(m_Levels.GetNumberOfValues()==0)
		return;
	
	
	m_levelListBox.ResetContent();//先清空
	
	CValueList& list = m_Levels.GetValueList();
	POSITION pos = list.GetHeadPosition();
	float val;
	CString str;
	while(pos!=NULL)
	{
		val = list.GetNext(pos);
		if( val == (int)val )
		{
			str.Format("%.0f",val);
		}
		else
		{
			str.Format("%.3f",val);
		}
		
		m_levelListBox.AddString(str);
	}
	

}

BOOL CEditContourLevelDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	AddLevelsToListBox();


	
	return TRUE; 
}

void CEditContourLevelDlg::OnDeleteLevel() 
{
	int index = m_levelListBox.GetCurSel();
	if( index == LB_ERR)
		return;



	CValueList& list = m_Levels.GetValueList();
	list.RemoveAt(list.FindIndex(index));

	m_levelListBox.DeleteString(index);

	int count = m_levelListBox.GetCount();
	if( index+1 <= count-1)
	{
		m_levelListBox.SetCurSel(index+1);
	}
	else
	{
		m_levelListBox.SetCurSel(count-1);
	}



	
}

void CEditContourLevelDlg::OnAddLevel() 
{
	UpdateData();

	
	if( m_zMin > m_fNewLevel || m_fNewLevel > m_zMax )
	{
		MessageBox("数据超出范围");
		return;
	}

	m_Levels.AddValue(m_fNewLevel);
	AddLevelsToListBox();

	//选中刚加入的那一个值
	CValueList& list = m_Levels.GetValueList();
	POSITION pos = list.GetHeadPosition();
	int index=-1/*LB_ERR*/;
	while(pos!=NULL)
	{
		index++;
		if( m_fNewLevel == list.GetNext(pos) )
		{
			
			break;
		}

	}
	
	ASSERT(index!=LB_ERR);
	m_levelListBox.SetCurSel(index);




	
}

void CEditContourLevelDlg::OnGenerateLevel() 
{
	UpdateData();

//	BOOL bValid = ( m_levelMin >= m_zMin ) && ( m_levelMin < m_zMax ) &&
		          //		          ( m_levelMax > m_zMin ) && ( m_levelMax <= m_zMax ) ;
		          

	if(/* !bValid ||*/ m_levelMax <= m_levelMin || m_levelInterval==0 )
	{
		MessageBox("数据输入不对");
		return;
	}

	float level;

	m_Levels.RemoveAllValues();
	for(level = m_levelMin; level <= m_levelMax; level+=m_levelInterval )
	{
		m_Levels.AddValue(level);
	}

	AddLevelsToListBox();

	
}

void CEditContourLevelDlg::OnMoveAll() 
{
	// TODO: Add your control notification handler code here

	CValueList& list = m_Levels.GetValueList();
	list.RemoveAll();
	
	m_levelListBox.ResetContent();
	
}
