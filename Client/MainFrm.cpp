// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "JDArchive.h"
#include "MainFrm.h"
#include "ServerCfgDlg.h"
#include "ClientListView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const int TIMER_RANGE = 200;
static const COLORREF COLOR_TRANSPARENT = RGB(192, 192, 192);

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CNewFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CNewFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_PROGRESS, OnUpdateStatusBarPanels)
	ON_UPDATE_COMMAND_UI(ID_LOGIN, OnUpdateLogin)
	ON_COMMAND(ID_LOGOUT, OnLogout)
	ON_UPDATE_COMMAND_UI(ID_LOGOUT, OnUpdateLogout)
	ON_COMMAND(ID_SERVER_CONFIG, OnServerConfig)
	ON_COMMAND(ID_CHANGEPASSWORD, OnChangepassword)
	ON_UPDATE_COMMAND_UI(ID_CHANGEPASSWORD, OnUpdateChangepassword)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_COMMAND(ID_LOGIN, OnPromptLogin)
	ON_COMMAND(ID_ADMIN_USERS, OnAdminUsers)
	ON_COMMAND(ID_ADMIN_ROLES, OnAdminRoles)
	ON_UPDATE_COMMAND_UI(ID_CAT_ADD, OnUpdateCatAdd)
	ON_COMMAND(ID_CAT_ADD, OnCatAdd)
	ON_UPDATE_COMMAND_UI(ID_CAT_PRIV, OnUpdateCatPriv)
	ON_COMMAND(ID_CAT_PRIV, OnCatPriv)
	ON_UPDATE_COMMAND_UI(ID_CAT_ADDFILE, OnUpdateCatAddfile)
	ON_COMMAND(ID_CAT_ADDFILE, OnCatAddfile)
	ON_UPDATE_COMMAND_UI(ID_FILE_ADD_VER, OnUpdateFileAddVer)
	ON_COMMAND(ID_FILE_ADD_VER, OnFileAddVer)
	ON_COMMAND(ID_VER_DOWNLOAD, OnVerDownload)
	ON_UPDATE_COMMAND_UI(ID_VER_DOWNLOAD, OnUpdateVerDownload)
	ON_UPDATE_COMMAND_UI(ID_ADMIN_ROLES, OnUpdateAdmin)
	ON_UPDATE_COMMAND_UI(ID_CAT_CHANGE, OnUpdateCatChange)
	ON_COMMAND(ID_CAT_CHANGE, OnCatChange)
	ON_UPDATE_COMMAND_UI(ID_CAT_DELETE, OnUpdateCatDelete)
	ON_COMMAND(ID_CAT_DELETE, OnCatDelete)
	ON_COMMAND(ID_FILE_DELETE, OnFileDelete)
	ON_UPDATE_COMMAND_UI(ID_FILE_DELETE, OnUpdateFileDelete)
	ON_COMMAND(ID_VER_DELETE, OnVerDelete)
	ON_UPDATE_COMMAND_UI(ID_VER_DELETE, OnUpdateVerDelete)
	ON_COMMAND(ID_VER_CHECK, OnVerCheck)
	ON_UPDATE_COMMAND_UI(ID_VER_CHECK, OnUpdateVerCheck)
	ON_COMMAND(ID_VER_UNCHECK, OnVerUncheck)
	ON_UPDATE_COMMAND_UI(ID_VER_UNCHECK, OnUpdateVerUncheck)
	ON_COMMAND(ID_APP_HELP, OnAppHelp)
	ON_COMMAND(ID_UPLOADPATCH, OnUploadpatch)
	ON_UPDATE_COMMAND_UI(ID_UPLOADPATCH, OnUpdateUploadpatch)
	ON_COMMAND(ID_ADMIN_CAT_INIT, OnAdminCatInit)
	ON_UPDATE_COMMAND_UI(ID_ADMIN_CAT_INIT, OnUpdateAdminCatInit)
	ON_COMMAND(ID_ADMIN_RECOVERY, OnAdminRecovery)
	ON_UPDATE_COMMAND_UI(ID_ADMIN_RECOVERY, OnUpdateAdminRecovery)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_FILE_MANAGEVER, OnFileManagever)
	ON_UPDATE_COMMAND_UI(ID_FILE_MANAGEVER, OnUpdateFileManagever)
	ON_COMMAND(ID_REFRESH, OnRefresh)
	ON_UPDATE_COMMAND_UI(ID_REFRESH, OnUpdateRefresh)
	ON_COMMAND(ID_ADMIN_DELTREE, OnAdminDeltree)
	ON_UPDATE_COMMAND_UI(ID_ADMIN_DELTREE, OnUpdateAdminDeltree)
	ON_WM_SETFOCUS()
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_PRIVILEGE, OnUpdateStatusBarPanels)	
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_USER, OnUpdateStatusBarPanels)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_ROLE, OnUpdateStatusBarPanels)
	ON_UPDATE_COMMAND_UI(ID_ADMIN_USERS, OnUpdateAdmin)
	ON_COMMAND(ID_ADMIN_DOWNLOADTREE, OnAdminDownloadtree)
	ON_UPDATE_COMMAND_UI(ID_ADMIN_DOWNLOADTREE, OnUpdateAdminDownloadtree)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SERVER_RESPONSE, OnServerResponse)
	ON_MESSAGE(WM_DISCONNECT, OnDisconnect)
	ON_MESSAGE(WM_SERVER_ERROR, OnServerError)
	ON_MESSAGE(WM_SERVER_LIST_ERROR, OnServerListError)
	ON_MESSAGE(WM_TASK_START, OnTaskStart)
	ON_MESSAGE(WM_CONNECT, OnConnect)
	ON_MESSAGE(WM_PATCH_INSTALL, OnPatchInstall)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_PROGRESS,
	ID_INDICATOR_USER,
	ID_INDICATOR_ROLE,
	ID_INDICATOR_PRIVILEGE,
};

enum {
	STATUS_PANEL_DEFAULT = 0,
	STATUS_PANEL_PROGRESS,
	STATUS_PANEL_USER,
	STATUS_PANEL_ROLE,
	STATUS_PANEL_PRIVILEGE,
};

static WORD TOOLIDS[] = {
	IDB_TOOLBAR_HC,
	16, 15,
	ID_LOGIN,
    ID_LOGOUT,
    ID_REFRESH,
    ID_CAT_ADD,
    ID_CAT_ADDFILE,
    ID_FILE_ADD_VER,
    ID_VER_DOWNLOAD,
    ID_VER_CHECK,
    ID_VER_UNCHECK,
    ID_APP_HELP,
    ID_CAT_DELETE,
    ID_FILE_DELETE,
    ID_VER_DELETE,
    ID_CAT_PRIV,
    ID_ADMIN_USERS,
    ID_ADMIN_ROLES,
    ID_UPLOADPATCH,
    ID_ADMIN_CAT_INIT,
    ID_ADMIN_DELTREE,
    ID_ADMIN_RECOVERY,
    ID_EDIT_CUT,
    ID_EDIT_PASTE,
    ID_FILE_MANAGEVER,
    ID_SERVER_CONFIG,
	NULL
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CNewFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;	
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT|TBSTYLE_TRANSPARENT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndMenuBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP|CBRS_SIZE_DYNAMIC|CBRS_TOOLTIPS|CBRS_GRIPPER))
	{
		TRACE0("Failed to create menu bar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndMenuBar.SetMenu(&m_DefaultNewMenu);
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndMenuBar);
	DockControlBar(&m_wndToolBar);
	m_wndMenuBar.SetWindowText(_T("资料库菜单栏"));
	m_wndToolBar.SetWindowText(_T("资料库快捷工具栏"));
	
	m_DefaultNewMenu.LoadToolBar(TOOLIDS, COLOR_TRANSPARENT);
	m_popup.LoadMenu(IDR_POPUP);
	m_popup.LoadToolBar(TOOLIDS, COLOR_TRANSPARENT);
	m_wndToolBar.LoadHiColor(MAKEINTRESOURCE(IDB_TOOLBAR_HC_MAIN), COLOR_TRANSPARENT);	

	m_wndStatusBar.SetIcon(STATUS_PANEL_DEFAULT, IDI_INFO);
	m_wndStatusBar.SetPaneWidth(STATUS_PANEL_PROGRESS, 160);
	m_wndStatusBar.SetIcon(STATUS_PANEL_USER, IDI_NOUSER);
	m_wndStatusBar.SetPaneText(STATUS_PANEL_USER, _T("未登录"));

	CString sTitle;
	sTitle.Format(_T("%s %s"), APP_TITLE, APP_VER);
	SetWindowText(sTitle);

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);	

	m_wndProgress.Create(this);
	m_wndProgress.Hide();
	_SESSION.SetProgWnd(&m_wndProgress);
	return 0;
}

void CMainFrame::OnDestroy() 
{	
	CRect rect;
	m_pCatView->GetClientRect(&rect);
	AfxSetLeftPanelWidth(rect.Width());
	AfxSaveFrame();
	KillTimer(ID_TMR_CHECK_CONN);
	CNewFrameWnd::OnDestroy();	
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{	
	WNDCLASS wndcls = { 0 };
    wndcls.lpfnWndProc = ::DefWindowProc;
	wndcls.hInstance = AfxGetInstanceHandle();
	wndcls.lpszClassName = APP_CLASS_NAME;
	if(!AfxRegisterClass(&wndcls))
	{
        TRACE("Class Registration Failed\n");
        return FALSE; 
	}

	if(!CNewFrameWnd::PreCreateWindow(cs))
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = wndcls.lpszClassName;
	//cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
		return FALSE;

	int nTreeWidth = AfxGetLeftPanelWidth();
	nTreeWidth = max(100, nTreeWidth);

	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CClientTreeView), CSize(nTreeWidth, 0), pContext) || !m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CClientListView), CSize(0, 0), pContext))
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}

	m_pCatView = (CClientTreeView*)(m_wndSplitter.GetPane(0, 0));
	m_pCatView->SetMenu(&m_popup);
	m_pFileView = (CClientListView*)(m_wndSplitter.GetPane(0, 1));
	m_pFileView->SetMenu(&m_popup);
	m_pCatView ->SetFileView(m_pFileView);
	ASSERT(m_pCatView);

	return CNewFrameWnd::OnCreateClient(lpcs, pContext);
}

void CMainFrame::OnUpdateStatusBarPanels(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE);	
}

void CMainFrame::OnLogout() 
{
	// TODO: Add your command handler code here	
	_SESSION.Abandon();
}

void CMainFrame::OnServerConfig() 
{
	// TODO: Add your command handler code here	
	CServerCfgDlg dlg(_SESSION.GetServerAddr());
	if (dlg.DoModal() == IDOK)
		_SESSION.SetServerAddr(dlg.m_sAddress);
}

int CMainFrame::ShowMessage(CString sText, UINT nFlag)
{
	sText.Replace(_T("$JD$"), APP_TITLE);
	return MessageBox(sText, APP_TITLE, nFlag);
}

void CMainFrame::OnLogin()
{
	m_wndStatusBar.SetIcon(STATUS_PANEL_USER, IDI_USER);
	m_wndStatusBar.SetPaneText(STATUS_PANEL_USER, _SESSION.GetUserName());
	m_wndStatusBar.SetPaneText(STATUS_PANEL_ROLE, _SESSION.GetRoleName());
	m_wndStatusBar.SetPaneText(STATUS_PANEL_PRIVILEGE, _SESSION.GetUserPrivilegeName());
	if (_SESSION.GetUserPrivilege() >= JDPRIV_ADMIN)
		m_wndStatusBar.SetIcon(STATUS_PANEL_PRIVILEGE, IDI_ADMIN);
	ShowMessage(_T("你已成功登录到$JD$。"), MB_ICONINFORMATION);
	m_pCatView->OnLogin();
	SetTimer(ID_TMR_CHECK_CONN, 5000, NULL);
}

LRESULT CMainFrame::OnDisconnect(WPARAM wParam, LPARAM lParam)
{
	EndProgress();
	KillTimer(ID_TMR_CHECK_CONN);
	m_wndStatusBar.SetIcon(STATUS_PANEL_USER, IDI_NOUSER);
	m_wndStatusBar.SetPaneText(STATUS_PANEL_USER, _T("未登录"));
	m_wndStatusBar.ClearPane(STATUS_PANEL_ROLE);
	m_wndStatusBar.ClearPane(STATUS_PANEL_PRIVILEGE);
	m_pCatView->OnLogout();
	m_pFileView->OnLogout();
	
	if (wParam)
		ShowMessage(_T("你与$JD$的连接已经断开。"));
	
	return (LRESULT)0;
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == ID_TMR_CHECK_CONN)
		_SESSION.SendHeartbeat();		

	CNewFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::SetProgressText(LPCTSTR lpszText)
{
	m_wndStatusBar.SetPaneText(STATUS_PANEL_PROGRESS, lpszText);
	m_wndStatusBar.SetIcon(STATUS_PANEL_PROGRESS, IDI_BUSY);
}

void CMainFrame::EndProgress()
{
	m_wndStatusBar.ClearPane(STATUS_PANEL_PROGRESS);
}

LRESULT CMainFrame::OnConnect(WPARAM wParam, LPARAM lParam)
{
	if (wParam)
	{
		SetProgressText(_T("正在连接服务器"));
	}
	else
	{
		EndProgress();
		ShowMessage(_T("无法连接到$JD$服务器，请正确设置服务器参数或联系系统管理员。"));
		OnServerConfig();
	}
	return 0;
}

LRESULT CMainFrame::OnServerListError(WPARAM wParam, LPARAM lParam)
{
	m_pCatView->OnListError(lParam);
	return 0;
}

void CMainFrame::OnAppHelp() 
{
	// TODO: Add your command handler code here
	AfxGetApp()->WinHelp(0);
}

void CMainFrame::OnRefresh() 
{
	m_pCatView->Refresh();
}

void CMainFrame::OnUpdateRefresh(CCmdUI* pCmdUI) 
{	
	pCmdUI->Enable(m_pCatView->CanRefresh() != NULL);
}

LRESULT CMainFrame::OnPatchInstall(WPARAM wParam, LPARAM lParam)
{
	::ShellExecute(NULL, _T("open"), (LPCTSTR)lParam, NULL, NULL, SW_SHOW);
	PostMessage(WM_CLOSE);
	return 0;
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_wndMenuBar.TranslateFrameMessage(pMsg)) 
	    return TRUE; 
	return CNewFrameWnd::PreTranslateMessage(pMsg);
}

void CMainFrame::OnSetFocus(CWnd* pOldWnd) 
{
	CNewFrameWnd::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
	m_pCatView->SetFocus();
}
