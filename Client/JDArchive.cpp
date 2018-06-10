// JDArchive.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "JDArchive.h"

#include "MainFrm.h"
#include "JDImgeList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJDArchiveApp

BEGIN_MESSAGE_MAP(CJDArchiveApp, CWinApp)
	//{{AFX_MSG_MAP(CJDArchiveApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJDArchiveApp construction

CJDArchiveApp::CJDArchiveApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_bInitOK = FALSE;
	m_sVersion.Format(_T("%d.%02d"), APP_VERSION / 100, APP_VERSION % 100);

	DWORD dwOK = ::GetTempPath(MAX_PATH, m_sTempFolder.GetBuffer(MAX_PATH));
	m_sTempFolder.ReleaseBuffer();
	if (dwOK)
	{
		if (m_sTempFolder.Right(1) != _T('\\'))
			m_sTempFolder += _T('\\');

		m_sTempFolder += _T("JDArchive");
	}
	else
	{
		m_sTempFolder = GetAppDir() + _T("\\temp");
	}
	
	::CreateDirectory(m_sTempFolder, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CJDArchiveApp object

CJDArchiveApp theApp;

LPCTSTR AfxGetVersionStr()
{
	return theApp.m_sVersion;
}

CDatabaseCache& AfxGetCache()
{
	return theApp.m_cache;
}

CJDClientSession& AfxGetSession()
{
	return theApp.m_session;
}

void _POST(UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	CWnd* pWnd = theApp.m_pMainWnd;
	if (pWnd && pWnd->GetSafeHwnd())
		pWnd->PostMessage(nMsg, wParam, lParam);
}

static const double KB = 1024.0;
static const double MB = KB * 1024.0;
static const double GB = MB * 1024.0;

CString AfxSizeToStr(DWORD dwSize)
{
	double fSize = (double)dwSize;
	CString sPres;
	if (fSize >= GB)
		sPres.Format(_T("%.2f GB"), fSize / GB);
	else if (fSize >= MB)
		sPres.Format(_T("%.2f MB"), fSize / MB);
	else if (fSize >= KB)
		sPres.Format(_T("%.2f KB"), fSize / KB);
	else
		sPres.Format(_T("%d 字节"), dwSize);
	return sPres;
}

CString GetFilter(const CString& sFile, CString& sExt)
{
	CString sFilter, sUpper;
	int nPos = sFile.ReverseFind(_T('.'));
	if (nPos != -1)
	{
		sExt = sFile.Mid(nPos + 1);
		sExt.MakeLower();
		sUpper = sExt;
		sUpper.MakeUpper();
		sFilter.Format(_T("%s文件(*.%s)|*.%s||"), (LPCTSTR)sUpper, (LPCTSTR)sExt, (LPCTSTR)sExt);
	}
	return sFilter;
}

CString AfxGetFileName(const CString& sFullPath)
{
	CString sFileName;
	int nPos = max(sFullPath.ReverseFind(_T('\\')), sFullPath.ReverseFind(_T('/')));
	if (nPos == -1)
		sFileName = sFullPath;
	else
		sFileName = sFullPath.Mid(nPos + 1);
	return sFileName;
}

CString AfxParseVersion(int nVersion)
{
	CString sVersion;
	sVersion.Format(_T("%d.%02d.%04d"), nVersion / 1000000, (nVersion / 10000) % 100, nVersion % 10000);
	return sVersion;
}

BOOL AfxIsExecutable(LPCTSTR lpszFileName)
{
	CString sExt = CJDImgeList::GetFileExt(lpszFileName);
	sExt.MakeLower();
	return (sExt == _T("dll") || sExt == _T("exe") || sExt == _T("com") || sExt == _T("bat") || sExt == _T("msi"));
}

int AfxGetLeftPanelWidth()
{
	return theApp.GetProfileInt(_T("Windows"), _T("Category Panel Width"), 260);
}

void AfxSetLeftPanelWidth(int nWidth)
{
	theApp.WriteProfileInt(_T("Windows"), _T("Category Panel Width"), nWidth);
}

CString AfxGetUserName()
{
	return theApp.GetProfileString(_T(""), _T("User Name"));
}

void AfxSetUserName(LPCTSTR lpszUserName)
{
	theApp.WriteProfileString(_T(""), _T("User Name"), lpszUserName);
}

CString _TEMP(LPCTSTR lpszAppend)
{
	if (lpszAppend && *lpszAppend)
	{
		CString sAppend(lpszAppend);
		sAppend.TrimLeft(_T('\\'));
		return theApp.m_sTempFolder + _T('\\') + sAppend;
	}
	return theApp.m_sTempFolder;
}

void AfxSaveFrame()
{
	theApp.m_pfl.Save();
}

/////////////////////////////////////////////////////////////////////////////
// CJDArchiveApp initialization
BOOL CJDArchiveApp::InitInstance()
{	
	SetInstanceUniqueID(_T("{D3773877-E9F7-4FAB-9C17-6097500007AD}"));
	if (!IsInstanceUnique())
		return FALSE;	
	
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
	if (!LBNetworkStartup(2, 2))
	{
		AfxMessageBox(_T("无法初始化网络通讯组件，程序终止。"), MB_ICONEXCLAMATION);
		return FALSE;
	}	

	if (!m_session.Create())
	{
		AfxMessageBox(_T("无法创建通讯对象，程序终止。"), MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (!m_session.Start())
	{
		AfxMessageBox(_T("无法启动通讯线程，程序终止。"), MB_ICONEXCLAMATION);
		return FALSE;
	}
	
	SetRegistryKey(_T("Abin"));
	m_bInitOK = TRUE;
	m_session.SetServerAddr(GetProfileString(_T(""), _T("Server Address"), DEFAULT_SERVER_IP));

	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object.	

	CNewMenu::SetMenuDrawMode(CNewMenu::STYLE_XP_2003);
	CNewMenu::SetXpBlending(FALSE);

	CMainFrame* pFrame = new CMainFrame;
	m_pMainWnd = pFrame;

	// create and load the frame with its resources

	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);

	// The one and only window has been initialized, so show and update it.
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();	

	m_pfl.SetWindow(pFrame);
	m_pfl.SetManageBarStates(TRUE);
	m_pfl.SetProfileHeading(_T("Windows"));
	m_pfl.Load();

#ifdef _DEBUG
	m_log.Open(_T("Client.log"));
#endif
	_LOG(_T("Application started"));

	return TRUE;
}

int CJDArchiveApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_bInitOK)
		WriteProfileString(_T(""), _T("Server Address"), m_session.GetServerAddr());
	
	m_session.Abandon();
	LBNetworkCleanup();

	_LOG(_T("Application stopped"));
	return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
#include "HyperLink.h"
class CAboutDlg : public CNewDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CHyperLink	m_wndCompany;
	CString	m_sTitle;
	CString	m_sCompany;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CNewDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_sTitle = _T("");
	m_sCompany = _T("");
	//}}AFX_DATA_INIT

	m_sTitle.Format(_T("%s %s"), APP_TITLE, APP_VER);
	m_sCompany = APP_COMPANY;
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CNewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_COMPANY, m_wndCompany);
	DDX_Text(pDX, IDC_TITLE, m_sTitle);
	DDX_Text(pDX, IDC_COMPANY, m_sCompany);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CNewDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog() 
{
	CNewDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndCompany.SetUnderline(-1);
	m_wndCompany.SetURL(APP_COMPANY_URL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// App command to run the dialog
void CJDArchiveApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CJDArchiveApp::WinHelp(DWORD dwData, UINT nCmd) 
{
	// TODO: Add your specialized code here and/or call the base class
	::ShellExecute(NULL, _T("open"), _T("hh.exe"), GetAppDir() + _T("\\JDArchive.chm"), GetAppDir(), SW_SHOW);
	//CWinApp::WinHelp(dwData, nCmd);
}
