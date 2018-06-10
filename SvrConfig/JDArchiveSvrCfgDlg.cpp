// JDArchiveSvrCfgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "JDArchiveSvrCfg.h"
#include "JDArchiveSvrCfgDlg.h"
#include "Ini.h"
#include "BusyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJDArchiveSvrCfgDlg dialog

CJDArchiveSvrCfgDlg::CJDArchiveSvrCfgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CJDArchiveSvrCfgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CJDArchiveSvrCfgDlg)
	m_bAutoDel = FALSE;
	m_sDBName = _T("");
	m_nDelDays = 0;
	m_sHost = _T("");
	m_nHour = 0;
	m_nInterval = 0;
	m_nMaxConn = 0;
	m_sPassword = _T("");
	m_sUserName = _T("");
	m_bDebug = FALSE;
	m_bBackupNow = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// Load data from ServerConfig.ini
	CIni ini(AfxGetAppEx()->GetAppDir() + _T("\\ServerConfig.ini"));
	m_nMaxConn = ini.GetInt(_T("General"), _T("Max Connection"), 256);
	m_sRootPath = ini.GetString(_T("General"), _T("Root Folder"), _T("D:\\"));
	m_bDebug = ini.GetInt(_T("General"), _T("Debug"), FALSE);
	m_sHost = ini.GetString(_T("Database"), _T("Host"), _T("localhost"));
	m_sDBName = ini.GetString(_T("Database"), _T("Name"));
	m_sUserName = ini.GetString(_T("Database"), _T("User"), _T("sa"));
	m_sPassword = ini.GetString(_T("Database"), _T("Password"));
	m_sBackupPath = ini.GetString(_T("Backup"), _T("Folder"), _T("E:\\"));
	m_nInterval = ini.GetInt(_T("Backup"), _T("Interval"), 1);
	m_nHour = ini.GetInt(_T("Backup"), _T("Hour"), 2);
	m_bAutoDel = ini.GetInt(_T("Backup"), _T("Auto Delete"), TRUE);
	m_nDelDays = ini.GetInt(_T("Backup"), _T("Auto Delete Days"), 30);
	m_bBackupNow = ini.GetBool(_T("Run Once"), _T("Backup"), FALSE);
}

void CJDArchiveSvrCfgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJDArchiveSvrCfgDlg)
	DDX_Control(pDX, IDC_AUTODEL, m_wndAutoDel);
	DDX_Control(pDX, IDC_USERNAME, m_wndUserName);
	DDX_Control(pDX, IDC_ROOTPATH, m_wndRootPath);
	DDX_Control(pDX, IDC_PASSWORD, m_wndPassword);
	DDX_Control(pDX, IDC_MAXCONN, m_wndMaxConn);
	DDX_Control(pDX, IDC_INTERVAL, m_wndInterval);
	DDX_Control(pDX, IDC_HOUR, m_wndHour);
	DDX_Control(pDX, IDC_HOST, m_wndHost);
	DDX_Control(pDX, IDC_DELDAYS, m_wndDelDays);
	DDX_Control(pDX, IDC_DBNAME, m_wndDBName);
	DDX_Control(pDX, IDC_BACKUPPATH, m_wndBackupPath);
	DDX_Check(pDX, IDC_AUTODEL, m_bAutoDel);
	DDX_Text(pDX, IDC_DBNAME, m_sDBName);
	DDX_Text(pDX, IDC_DELDAYS, m_nDelDays);
	DDV_MinMaxInt(pDX, m_nDelDays, 1, 90);
	DDX_Text(pDX, IDC_HOST, m_sHost);
	DDX_Text(pDX, IDC_HOUR, m_nHour);
	DDV_MinMaxInt(pDX, m_nHour, 0, 23);
	DDX_Text(pDX, IDC_INTERVAL, m_nInterval);
	DDV_MinMaxInt(pDX, m_nInterval, 1, 30);
	DDX_Text(pDX, IDC_MAXCONN, m_nMaxConn);
	DDV_MinMaxInt(pDX, m_nMaxConn, 1, 65535);
	DDX_Text(pDX, IDC_PASSWORD, m_sPassword);
	DDX_Text(pDX, IDC_USERNAME, m_sUserName);
	DDX_Check(pDX, IDC_DEBUG, m_bDebug);
	DDX_Check(pDX, IDC_BACKUPNOW, m_bBackupNow);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CJDArchiveSvrCfgDlg, CDialog)
	//{{AFX_MSG_MAP(CJDArchiveSvrCfgDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_AUTODEL, OnAutodel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJDArchiveSvrCfgDlg message handlers

BOOL CJDArchiveSvrCfgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_wndRootPath.SetButtonStyle(BC_BTN_ICON | BC_ICO_ARROWFOLDER | BC_CTL_FOLDERSONLY);
	m_wndRootPath.SetDialogTitle(_T("文件根目录"));
	m_wndRootPath.SetDialogBanner(_T("请选择资料库文件存储主目录："));
	m_wndRootPath.SetTooltipText(_T("浏览"));
	m_wndRootPath.SetPathName(m_sRootPath);	

	m_wndBackupPath.SetButtonStyle(BC_BTN_ICON | BC_ICO_ARROWFOLDER | BC_CTL_FOLDERSONLY);
	m_wndBackupPath.SetDialogTitle(_T("备份目录"));
	m_wndBackupPath.SetDialogBanner(_T("请选择资料库备份存储目录："));
	m_wndBackupPath.SetTooltipText(_T("浏览"));
	m_wndBackupPath.SetPathName(m_sBackupPath);	

	m_wndDelDays.EnableWindow(m_bAutoDel);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CJDArchiveSvrCfgDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CJDArchiveSvrCfgDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CJDArchiveSvrCfgDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!UpdateData(TRUE))
		return;

	m_sRootPath = m_wndRootPath.GetPathName();
	if (!TestFolder(m_sRootPath))
	{
		MessageBox(_T("无效的目录名。"), _T("文件根目录"), MB_ICONEXCLAMATION);
		m_wndRootPath.SetFocus();
		return;
	}

	m_sBackupPath = m_wndBackupPath.GetPathName();	
	if (!TestFolder(m_sBackupPath))
	{
		MessageBox(_T("无效的目录名。"), _T("备份根目录"), MB_ICONEXCLAMATION);
		m_wndBackupPath.SetFocus();
		return;
	}

	if (m_sBackupPath.CompareNoCase(m_sRootPath) == 0)
	{
		MessageBox(_T("文件根目录与备份根目录不得相同。"), _T("目录冲突"), MB_ICONEXCLAMATION);
		m_wndBackupPath.SetFocus();
		return;
	}

	if (!ValidateInput(m_sHost, m_wndHost, _T("主机地址")))
		return;

	if (!ValidateInput(m_sDBName, m_wndDBName, _T("数据库名称")))
		return;

	m_sUserName.TrimLeft();
	m_sUserName.TrimRight();

	// Save to ServerConfig.ini
	CIni ini(AfxGetAppEx()->GetAppDir() + _T("\\ServerConfig.ini"));
	ini.WriteInt(_T("General"), _T("Max Connection"), m_nMaxConn);
	ini.WriteString(_T("General"), _T("Root Folder"), m_sRootPath);
	ini.WriteInt(_T("General"), _T("Debug"), m_bDebug);
	ini.WriteString(_T("Database"), _T("Host"), m_sHost);
	ini.WriteString(_T("Database"), _T("Name"), m_sDBName);
	ini.WriteString(_T("Database"), _T("User"), m_sUserName);
	ini.WriteString(_T("Database"), _T("Password"), m_sPassword);
	ini.WriteString(_T("Backup"), _T("Folder"), m_sBackupPath);
	ini.WriteInt(_T("Backup"), _T("Interval"), m_nInterval);
	ini.WriteInt(_T("Backup"), _T("Hour"), m_nHour);
	ini.WriteInt(_T("Backup"), _T("Auto Delete"), m_bAutoDel);
	ini.WriteInt(_T("Backup"), _T("Auto Delete Days"), m_nDelDays);
	ini.WriteBool(_T("Run Once"), _T("Backup"), m_bBackupNow);
	
	// Restart service
	if (AfxSilent())
	{
		MessageBox(_T("设置已保存。"), _T("设置完成"), MB_ICONINFORMATION);
		EndDialog(IDOK);
	}
	else if (MessageBox(_T("设置已保存，要使这些改动立即生效，你必须重启JDArchiveSvr服务。\n\n是否立即重启服务？"), _T("重启服务"), MB_ICONQUESTION | MB_OKCANCEL) == IDOK)
	{
		CBusyDlg dlg;
		dlg.DoModal();
		MessageBox(_T("设置已生效。"), _T("设置完成"), MB_ICONINFORMATION);
	}
}

BOOL CJDArchiveSvrCfgDlg::TestFolder(LPCTSTR lpszFolder)
{
	DWORD dwAttr = ::GetFileAttributes(lpszFolder);
	if (dwAttr == -1)
		return FALSE;
	return (dwAttr & FILE_ATTRIBUTE_DIRECTORY);
}

BOOL CJDArchiveSvrCfgDlg::ValidateInput(CString &sValue, CEditEx &edit, LPCTSTR lpszName)
{
	sValue.TrimLeft();
	sValue.TrimRight();
	if (!sValue.IsEmpty())
		return TRUE;

	CString sErr;
	sErr.Format(_T("%s不能为空。"), lpszName);
	MessageBox(sErr, _T("无效的输入"), MB_ICONEXCLAMATION);
	return FALSE;
}

void CJDArchiveSvrCfgDlg::OnAutodel() 
{
	// TODO: Add your control notification handler code here
	m_wndDelDays.EnableWindow(m_wndAutoDel.GetCheck());
}
