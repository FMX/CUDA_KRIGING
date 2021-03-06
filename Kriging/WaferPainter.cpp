// WaferPainter.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "WaferPainter.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "WaferPainterDoc.h"
#include "WaferPainterView.h"
#include "ContourView.h"
#include "OptionDlg.h"
#include "3DContourView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWaferPainterApp

BEGIN_MESSAGE_MAP(CWaferPainterApp, CWinApp)
	//{{AFX_MSG_MAP(CWaferPainterApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_INTEROPTION, OnInteroption)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWaferPainterApp construction

CWaferPainterApp::CWaferPainterApp() : m_nDiameter(0), m_nInterpolater(1), m_processor(0), m_nocuda(true)
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

CWaferPainterApp::~CWaferPainterApp()
{
	delete m_pContourTemplate;
	delete m_p3DContourTemplate;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CWaferPainterApp object

CWaferPainterApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CWaferPainterApp initialization

BOOL CWaferPainterApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_WAFERPTYPE,
		RUNTIME_CLASS(CWaferPainterDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CWaferPainterView));
	AddDocTemplate(pDocTemplate);

	m_pContourTemplate = new CMultiDocTemplate(
		IDR_CONTOURTYPE,
		RUNTIME_CLASS(CContourDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CContourView));

	m_p3DContourTemplate = new CMultiDocTemplate(
		IDR_3DCONTOURTYPE,
		RUNTIME_CLASS(C3DContourDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(C3DContourView));


	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.

	pMainFrame->ShowWindow(m_nCmdShow);


	if (cudaIns.isCUDACapable())
		m_nocuda = false;
	else
		m_nocuda = true;
	pMainFrame->ShowCudaStatus();

	pMainFrame->UpdateWindow();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CWaferPainterApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CWaferPainterApp message handlers


void CWaferPainterApp::OnInteroption()
{
	// TODO: Add your command handler code here
	COptionDlg dlg;
	dlg.m_nDiameter = m_nDiameter;
	dlg.m_nInterpolater = m_nInterpolater;
	dlg.m_processor = m_processor;

	if (dlg.DoModal() == IDOK) {
		m_nDiameter = dlg.m_nDiameter;
		m_nInterpolater = dlg.m_nInterpolater;
		m_processor = dlg.m_processor;
		char ms[25];
		sprintf(ms, "processor %d", m_processor);
		AfxMessageBox(ms);
	}
}
