; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CContourGeneratorDoc
LastTemplate=CFileDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ContourGenerator.h"
LastPage=0

ClassCount=7
Class1=CContourGeneratorApp
Class2=CContourGeneratorDoc
Class3=CContourGeneratorView
Class4=CMainFrame

ResourceCount=4
Resource1=IDD_EDIT_CONTOUR_LEVELS
Class5=CAboutDlg
Resource2=IDR_MAINFRAME
Class6=CClrFileDialog
Resource3=IDD_ABOUTBOX
Class7=CEditContourLevelDlg
Resource4=IDD_FILECOLORTMP_DIALOG

[CLS:CContourGeneratorApp]
Type=0
HeaderFile=ContourGenerator.h
ImplementationFile=ContourGenerator.cpp
Filter=N

[CLS:CContourGeneratorDoc]
Type=0
HeaderFile=ContourGeneratorDoc.h
ImplementationFile=ContourGeneratorDoc.cpp
Filter=N
BaseClass=CDocument
VirtualFilter=DC
LastObject=CContourGeneratorDoc

[CLS:CContourGeneratorView]
Type=0
HeaderFile=ContourGeneratorView.h
ImplementationFile=ContourGeneratorView.cpp
Filter=C
BaseClass=CView
VirtualFilter=VWC
LastObject=ID_SET_COLORTABLE


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=ID_LOADCOLORTABLE




[CLS:CAboutDlg]
Type=0
HeaderFile=ContourGenerator.cpp
ImplementationFile=ContourGenerator.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_PRINT
Command6=ID_FILE_PRINT_PREVIEW
Command7=ID_FILE_PRINT_SETUP
Command8=ID_FILE_MRU_FILE1
Command9=ID_APP_EXIT
Command10=ID_EDIT_UNDO
Command11=ID_EDIT_CUT
Command12=ID_EDIT_COPY
Command13=ID_EDIT_PASTE
Command14=ID_VIEW_TOOLBAR
Command15=ID_VIEW_STATUS_BAR
Command16=ID_APP_ABOUT
CommandCount=16

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_SET_CONTOUR_LEVEL
Command5=ID_LOADCOLORTABLE
Command6=ID_APP_ABOUT
CommandCount=6

[DLG:IDD_EDIT_CONTOUR_LEVELS]
Type=1
Class=CEditContourLevelDlg
ControlCount=22
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_CONTOUR_LEVEL_LIST,listbox,1352728833
Control4=IDC_ADD_LEVEL,button,1342242816
Control5=IDC_DELETE_LEVEL,button,1342242816
Control6=IDC_STATIC,button,1342177287
Control7=IDC_NEW_LEVEL,edit,1350631552
Control8=IDC_ZMIN,static,1342312448
Control9=IDC_ZMAX,static,1342312448
Control10=IDC_STATIC,button,1342177287
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,button,1342177287
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,static,1342308352
Control17=IDC_LEVEL_MIN,edit,1350631552
Control18=IDC_LEVEL_INTERVAL,edit,1350631552
Control19=IDC_LEVEL_MAX,edit,1350631552
Control20=IDC_GENERATE_LEVEL,button,1342242816
Control21=IDC_STATIC,button,1342177287
Control22=IDC_MOVE_ALL,button,1342242816

[CLS:CEditContourLevelDlg]
Type=0
HeaderFile=EditContourLevelDlg.h
ImplementationFile=EditContourLevelDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CEditContourLevelDlg

[DLG:IDD_FILECOLORTMP_DIALOG]
Type=1
Class=?
ControlCount=1
Control1=IDC_STATIC_PREVIEW,static,1342312448

[CLS:CClrFileDialog]
Type=0
HeaderFile=ClrFileDialog.h
ImplementationFile=ClrFileDialog.cpp
BaseClass=CFileDialog
Filter=D
VirtualFilter=dWC

