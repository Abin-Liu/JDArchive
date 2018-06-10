; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CProgressDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "JDArchive.h"
LastPage=0

ClassCount=34
Class1=CJDArchiveApp
Class3=CMainFrame
Class4=CAboutDlg

ResourceCount=23
Resource1=IDD_RECOVERLISTDLG
Class2=CChildView
Resource2=IDD_FILEDLG
Class5=CLoginDlg
Resource3=IDD_LOGINDLG
Class6=CServerCfgDlg
Resource4=IDD_CATINITDLG
Class7=CChangePwdDlg
Class8=CFileDlg
Class9=CClientRebar
Class10=CClientTreeView
Class11=CClientListView
Class12=CTooltipTreeView
Class13=CCustomToolbar
Class14=CRecoverListDlg
Class15=CCatInitDlg
Class16=CProgressStatusBar
Resource5=IDD_BUSYDLG
Class17=CUserListDlg
Resource6=IDD_SERVERCFGDLG
Resource7=IDD_VERMANAGEDLG
Class18=CCatDlg
Resource8=IDR_POPUP
Resource9=IDD_CHANGEPWDDLG
Class19=CCatPrivDlg
Resource10=IDD_PATCHDLG
Class20=CRoleListDlg
Class21=CUserDlg
Resource11=IDD_UPDATEDLG
Class22=CAddRoleDlg
Class23=CLBListCtrl
Resource12=IDD_ABOUTBOX
Class24=CUploadDlg
Resource13=IDD_ROLEDLG
Resource14=IDD_CATPRIVDLG
Class25=CProgressDlg
Resource15=IDR_MAINFRAME
Class26=CPatchUploadDlg
Resource16=IDD_UPLOADDLG
Class27=CPatchConfirmDlg
Class28=CLockDlg
Resource17=IDD_ADDROLE
Resource18=IDD_USERLISTDLG
Resource19=IDD_USERDLG
Class29=CBusyDlg
Resource20=IDD_CATDLG
Class30=CVerManageDlg
Resource21=IDD_PROGDLG
Class31=CRecoverDlg
Class32=CLBStatusBar
Class33=CwelcomeDlg
Resource22=IDD_RECOVERDLG
Class34=CDownloadTreeDlg
Resource23=IDD_DOWNLOADTREEDLG

[CLS:CJDArchiveApp]
Type=0
HeaderFile=JDArchive.h
ImplementationFile=JDArchive.cpp
Filter=N
BaseClass=CWinApp
VirtualFilter=AC

[CLS:CChildView]
Type=0
HeaderFile=ChildView.h
ImplementationFile=ChildView.cpp
Filter=N

[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=W
BaseClass=CNewFrameWnd
VirtualFilter=fWC
LastObject=CMainFrame




[CLS:CAboutDlg]
Type=0
HeaderFile=JDArchive.cpp
ImplementationFile=JDArchive.cpp
Filter=D
LastObject=CAboutDlg
BaseClass=CDialog
VirtualFilter=dWC

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_TITLE,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_COMPANY,static,1342308352

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_SERVER_CONFIG
Command2=ID_VIEW_TOOLBAR
Command3=ID_VIEW_STATUS_BAR
Command4=ID_APP_EXIT
Command5=ID_LOGIN
Command6=ID_LOGOUT
Command7=ID_CHANGEPASSWORD
Command8=ID_CAT_ADD
Command9=ID_EDIT_PASTE
Command10=ID_CAT_ADDFILE
Command11=ID_CAT_ADD
Command12=ID_EDIT_CUT
Command13=ID_EDIT_PASTE
Command14=ID_CAT_CHANGE
Command15=ID_CAT_DELETE
Command16=ID_CAT_PRIV
Command17=ID_FILE_ADD_VER
Command18=ID_EDIT_CUT
Command19=ID_FILE_DELETE
Command20=ID_FILE_MANAGEVER
Command21=ID_VER_DOWNLOAD
Command22=ID_VER_CHECK
Command23=ID_VER_UNCHECK
Command24=ID_VER_DELETE
Command25=ID_ADMIN_CAT_INIT
Command26=ID_ADMIN_DOWNLOADTREE
Command27=ID_ADMIN_USERS
Command28=ID_ADMIN_ROLES
Command29=ID_ADMIN_RECOVERY
Command30=ID_ADMIN_DELTREE
Command31=ID_UPLOADPATCH
Command32=ID_APP_HELP
Command33=ID_APP_ABOUT
CommandCount=33

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_LOGIN
Command2=ID_CAT_ADDFILE
Command3=ID_VER_CHECK
Command4=ID_VER_UNCHECK
Command5=ID_EDIT_PASTE
Command6=ID_VER_DOWNLOAD
Command7=ID_APP_HELP
Command8=ID_REFRESH
Command9=ID_NEXT_PANE
Command10=ID_PREV_PANE
Command11=ID_EDIT_CUT
CommandCount=11

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_LOGIN
Command2=ID_LOGOUT
Command3=ID_REFRESH
Command4=ID_CAT_ADD
Command5=ID_CAT_ADDFILE
Command6=ID_FILE_ADD_VER
Command7=ID_VER_DOWNLOAD
Command8=ID_VER_CHECK
Command9=ID_VER_UNCHECK
Command10=ID_APP_HELP
CommandCount=10

[DLG:IDD_LOGINDLG]
Type=1
Class=CLoginDlg
ControlCount=7
Control1=IDC_STATIC,static,1342308352
Control2=IDC_USERNAME,edit,1350631552
Control3=IDC_STATIC,static,1342308352
Control4=IDC_PASSWORD,edit,1350631584
Control5=IDOK,button,1342242817
Control6=IDCANCEL,button,1342242816
Control7=IDC_STATIC,static,1342177283

[CLS:CLoginDlg]
Type=0
HeaderFile=LoginDlg.h
ImplementationFile=LoginDlg.cpp
BaseClass=CDialog
Filter=W
VirtualFilter=dWC
LastObject=IDOK

[DLG:IDD_SERVERCFGDLG]
Type=1
Class=CServerCfgDlg
ControlCount=4
Control1=IDC_STATIC,static,1342308352
Control2=IDC_ADDRESS,edit,1350631552
Control3=IDOK,button,1342242817
Control4=IDCANCEL,button,1342242816

[CLS:CServerCfgDlg]
Type=0
HeaderFile=ServerCfgDlg.h
ImplementationFile=ServerCfgDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_PORT

[DLG:IDD_CHANGEPWDDLG]
Type=1
Class=CChangePwdDlg
ControlCount=8
Control1=IDC_STATIC,static,1342308352
Control2=IDC_OLDPWD,edit,1350631584
Control3=IDC_STATIC,static,1342308352
Control4=IDC_NEWPWD,edit,1350631584
Control5=IDC_STATIC,static,1342308352
Control6=IDC_NEWPWD2,edit,1350631584
Control7=IDOK,button,1342242817
Control8=IDCANCEL,button,1342242816

[CLS:CChangePwdDlg]
Type=0
HeaderFile=ChangePwdDlg.h
ImplementationFile=ChangePwdDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CChangePwdDlg
VirtualFilter=dWC

[CLS:CClientRebar]
Type=0
HeaderFile=ClientRebar.h
ImplementationFile=ClientRebar.cpp
BaseClass=CDialogBar
Filter=D
LastObject=CClientRebar
VirtualFilter=dWC

[CLS:CClientTreeView]
Type=0
HeaderFile=ClientTreeView.h
ImplementationFile=ClientTreeView.cpp
BaseClass=CTooltipTreeView
Filter=W
VirtualFilter=VWC

[CLS:CClientListView]
Type=0
HeaderFile=ClientListView.h
ImplementationFile=ClientListView.cpp
BaseClass=CListView
Filter=C
VirtualFilter=VWC

[CLS:CTooltipTreeView]
Type=0
HeaderFile=TooltipTreeView.h
ImplementationFile=TooltipTreeView.cpp
BaseClass=CTreeView
Filter=C
VirtualFilter=VWC

[CLS:CCustomToolbar]
Type=0
HeaderFile=CustomToolbar.h
ImplementationFile=CustomToolbar.cpp
BaseClass=CToolBar
Filter=W

[CLS:CBusyDlg]
Type=0
HeaderFile=BusyDlg.h
ImplementationFile=BusyDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC

[CLS:CProgressStatusBar]
Type=0
HeaderFile=ProgressStatusBar.h
ImplementationFile=ProgressStatusBar.cpp
BaseClass=CStatusBar
Filter=W

[DLG:IDD_USERLISTDLG]
Type=1
Class=CUserListDlg
ControlCount=5
Control1=IDC_LIST1,SysListView32,1350664221
Control2=IDC_ADD,button,1342242816
Control3=IDC_EDIT,button,1476460544
Control4=IDCANCEL,button,1342242816
Control5=IDC_RESETPWD,button,1476460544

[CLS:CUserListDlg]
Type=0
HeaderFile=UserListDlg.h
ImplementationFile=UserListDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_LIST1

[DLG:IDD_USERDLG]
Type=1
Class=CUserDlg
ControlCount=8
Control1=IDC_STATIC,static,1342308352
Control2=IDC_NAME,edit,1350631552
Control3=IDC_STATIC,static,1342308352
Control4=IDC_ROLES,combobox,1344340227
Control5=IDC_STATIC,static,1342308352
Control6=IDC_PRIVILEGES,combobox,1344339971
Control7=IDOK,button,1342242817
Control8=IDCANCEL,button,1342242816

[DLG:IDD_CATDLG]
Type=1
Class=CCatDlg
ControlCount=8
Control1=IDC_STATIC,static,1342308352
Control2=IDC_PARENTCAT,edit,1484851328
Control3=IDC_STATIC,static,1342308352
Control4=IDC_NAME,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_DESC,edit,1352728580
Control7=IDOK,button,1342242817
Control8=IDCANCEL,button,1342242816

[CLS:CCatDlg]
Type=0
HeaderFile=CatDlg.h
ImplementationFile=CatDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_PRIV

[MNU:IDR_POPUP]
Type=1
Class=CMainFrame
Command1=ID_REFRESH
Command2=ID_REFRESH
Command3=ID_CAT_ADD
Command4=ID_EDIT_PASTE
Command5=ID_REFRESH
Command6=ID_CAT_ADDFILE
Command7=ID_CAT_ADD
Command8=ID_EDIT_CUT
Command9=ID_EDIT_PASTE
Command10=ID_CAT_CHANGE
Command11=ID_CAT_DELETE
Command12=ID_CAT_PRIV
Command13=ID_REFRESH
Command14=ID_FILE_ADD_VER
Command15=ID_EDIT_CUT
Command16=ID_FILE_DELETE
Command17=ID_FILE_MANAGEVER
Command18=ID_REFRESH
Command19=ID_VER_DOWNLOAD
Command20=ID_VER_CHECK
Command21=ID_VER_UNCHECK
Command22=ID_VER_DELETE
CommandCount=22

[DLG:IDD_CATPRIVDLG]
Type=1
Class=CCatPrivDlg
ControlCount=5
Control1=IDC_STATIC,static,1342308352
Control2=IDC_LIST1,SysListView32,1350664213
Control3=IDOK,button,1342242817
Control4=IDCANCEL,button,1342242816
Control5=IDC_APPLYSUBCATS,button,1342242819

[CLS:CCatPrivDlg]
Type=0
HeaderFile=CatPrivDlg.h
ImplementationFile=CatPrivDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_APPLYSUBCATS

[DLG:IDD_ROLEDLG]
Type=1
Class=CRoleListDlg
ControlCount=3
Control1=IDC_LIST1,SysListView32,1350664221
Control2=IDC_ADD,button,1342242816
Control3=IDCANCEL,button,1342242816

[CLS:CRoleListDlg]
Type=0
HeaderFile=RoleListDlg.h
ImplementationFile=RoleListDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CRoleListDlg
VirtualFilter=dWC

[CLS:CUserDlg]
Type=0
HeaderFile=UserDlg.h
ImplementationFile=UserDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CUserDlg
VirtualFilter=dWC

[DLG:IDD_ADDROLE]
Type=1
Class=CAddRoleDlg
ControlCount=4
Control1=IDC_STATIC,static,1342308352
Control2=IDC_NAME,edit,1350631552
Control3=IDOK,button,1342242817
Control4=IDCANCEL,button,1342242816

[CLS:CAddRoleDlg]
Type=0
HeaderFile=AddRoleDlg.h
ImplementationFile=AddRoleDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC

[CLS:CLBListCtrl]
Type=0
HeaderFile=LBListCtrl.h
ImplementationFile=LBListCtrl.cpp
BaseClass=CListCtrl
Filter=W
VirtualFilter=FWC

[DLG:IDD_UPLOADDLG]
Type=1
Class=CUploadDlg
ControlCount=10
Control1=IDC_STATIC,static,1342308352
Control2=IDC_NAME,edit,1484849280
Control3=IDC_STATIC,static,1342308352
Control4=IDC_FILEPATH,button,1342242816
Control5=IDC_STATIC,static,1342308352
Control6=IDC_VERSION,edit,1484849280
Control7=IDC_STATIC,static,1342308352
Control8=IDC_VERDESC,edit,1350631428
Control9=IDOK,button,1342242817
Control10=IDCANCEL,button,1342242816

[CLS:CUploadDlg]
Type=0
HeaderFile=UploadDlg.h
ImplementationFile=UploadDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_VERDESC

[DLG:IDD_FILEDLG]
Type=1
Class=CFileDlg
ControlCount=7
Control1=IDC_STATIC,static,1342308352
Control2=IDC_FILENAME,edit,1484849280
Control3=IDC_BROWSE,button,1342242816
Control4=IDC_STATIC,static,1342308352
Control5=IDC_VERDESC,edit,1352728580
Control6=IDOK,button,1342242817
Control7=IDCANCEL,button,1342242816

[CLS:CFileDlg]
Type=0
HeaderFile=FileDlg.h
ImplementationFile=FileDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC

[DLG:IDD_PROGDLG]
Type=1
Class=CProgressDlg
ControlCount=4
Control1=IDC_PROGRESS1,msctls_progress32,1350565889
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342177283
Control4=IDC_TEXT,edit,1342179332

[CLS:CProgressDlg]
Type=0
HeaderFile=ProgressDlg.h
ImplementationFile=ProgressDlg.cpp
BaseClass=CNewDialog
Filter=D
VirtualFilter=dWC
LastObject=CProgressDlg

[DLG:IDD_PATCHDLG]
Type=1
Class=CPatchUploadDlg
ControlCount=6
Control1=IDC_STATIC,static,1342308352
Control2=IDC_PATH,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_VERSION,edit,1484857472
Control5=IDOK,button,1342242817
Control6=IDCANCEL,button,1342242816

[CLS:CPatchUploadDlg]
Type=0
HeaderFile=PatchUploadDlg.h
ImplementationFile=PatchUploadDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CPatchUploadDlg

[DLG:IDD_UPDATEDLG]
Type=1
Class=CPatchConfirmDlg
ControlCount=14
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342177283
Control8=IDC_STATIC,static,1342177283
Control9=IDC_VER,static,1342308352
Control10=IDC_FILENAME,static,1342308352
Control11=IDC_AUTHOR,static,1342308352
Control12=IDC_DATE,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_FILELEN,static,1342308352

[CLS:CPatchConfirmDlg]
Type=0
HeaderFile=PatchConfirmDlg.h
ImplementationFile=PatchConfirmDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CPatchConfirmDlg

[CLS:CLockDlg]
Type=0
HeaderFile=LockDlg.h
ImplementationFile=LockDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC

[DLG:IDD_CATINITDLG]
Type=1
Class=CCatInitDlg
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_PATH,button,1342242816

[CLS:CCatInitDlg]
Type=0
HeaderFile=CatInitDlg.h
ImplementationFile=CatInitDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CCatInitDlg

[DLG:IDD_BUSYDLG]
Type=1
Class=CBusyDlg
ControlCount=4
Control1=IDCANCEL,button,1342242816
Control2=IDC_STATIC,static,1342177283
Control3=IDC_STATIC,static,1342308352
Control4=IDC_TEXT,static,1342308352

[DLG:IDD_VERMANAGEDLG]
Type=1
Class=CVerManageDlg
ControlCount=18
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_FILENAME,edit,1484849280
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_CURMAJOR,edit,1484849281
Control8=IDC_CURMINOR,edit,1484849281
Control9=IDC_STATIC,button,1342177287
Control10=IDC_STATIC,button,1342177287
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_MAJOR1,edit,1350639745
Control14=IDC_MINOR,edit,1350639745
Control15=IDC_STATIC,static,1342308352
Control16=IDC_CURBUILD,edit,1484849281
Control17=IDC_STATIC,static,1342308352
Control18=IDC_BUILD,edit,1350639745

[CLS:CVerManageDlg]
Type=0
HeaderFile=VerManageDlg.h
ImplementationFile=VerManageDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CVerManageDlg
VirtualFilter=dWC

[DLG:IDD_RECOVERDLG]
Type=1
Class=CRecoverDlg
ControlCount=6
Control1=IDC_STATIC,static,1342308352
Control2=IDC_ROLES,combobox,1344340227
Control3=IDC_STATIC,static,1342308352
Control4=IDC_FILENAME,edit,1350631552
Control5=IDOK,button,1342242817
Control6=IDCANCEL,button,1342242816

[CLS:CRecoverDlg]
Type=0
HeaderFile=RecoverDlg.h
ImplementationFile=RecoverDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CRecoverDlg
VirtualFilter=dWC

[DLG:IDD_RECOVERLISTDLG]
Type=1
Class=CRecoverListDlg
ControlCount=3
Control1=IDOK,button,1476460545
Control2=IDCANCEL,button,1342242816
Control3=IDC_LIST1,SysListView32,1350664197

[CLS:CRecoverListDlg]
Type=0
HeaderFile=RecoverListDlg.h
ImplementationFile=RecoverListDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_LIST1

[CLS:CLBStatusBar]
Type=0
HeaderFile=LBStatusBar.h
ImplementationFile=LBStatusBar.cpp
BaseClass=CStatusBar
Filter=W

[CLS:CwelcomeDlg]
Type=0
HeaderFile=welcomeDlg.h
ImplementationFile=welcomeDlg.cpp
BaseClass=CDialog
Filter=W
LastObject=IDC_WEL_LOGIN
VirtualFilter=dWC

[DLG:IDD_DOWNLOADTREEDLG]
Type=1
Class=CDownloadTreeDlg
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_BUTTON1,button,1342242816

[CLS:CDownloadTreeDlg]
Type=0
HeaderFile=DownloadTreeDlg.h
ImplementationFile=DownloadTreeDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CDownloadTreeDlg
VirtualFilter=dWC

