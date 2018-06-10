// BusyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "JDArchiveSvrCfg.h"
#include "BusyDlg.h"
#include <Winsvc.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBusyDlg dialog


CBusyDlg::CBusyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBusyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBusyDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CBusyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBusyDlg)
	DDX_Control(pDX, IDC_TEXT, m_wndText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBusyDlg, CDialog)
	//{{AFX_MSG_MAP(CBusyDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBusyDlg message handlers

BOOL CBusyDlg::ProcService(CStatic& wnd)
{
	SC_HANDLE hSC = ::OpenSCManager(NULL, NULL, GENERIC_EXECUTE);
	if (hSC == NULL)
		return FALSE;

	SC_HANDLE hSvc = ::OpenService(hSC, _T("JDArchiveSvr"), SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);
	if (hSvc == NULL)
	{
		::CloseServiceHandle(hSC);
		return FALSE;
	}

	SERVICE_STATUS status;
	if(!::QueryServiceStatus(hSvc, &status))
	{
		::CloseServiceHandle(hSvc);
		::CloseServiceHandle(hSC);
		return FALSE;
	}

	if (status.dwCurrentState == SERVICE_RUNNING)
	{
		wnd.SetWindowText(_T("正在停止服务……"));
		if(::ControlService(hSvc, SERVICE_CONTROL_STOP, &status))
		{
			while(::QueryServiceStatus(hSvc, &status))
			{
				::Sleep(status.dwWaitHint);
				if (status.dwCurrentState == SERVICE_STOPPED)
					break;				
			}
		}
	}

	if (status.dwCurrentState == SERVICE_STOPPED)
	{
		wnd.SetWindowText(_T("正在启动服务……"));
		if (::StartService(hSvc, NULL, NULL))
		{
			while(::QueryServiceStatus(hSvc, &status))
			{
				::Sleep(status.dwWaitHint);
				if (status.dwCurrentState == SERVICE_RUNNING)
					break;				
			}
		}
	}

	::CloseServiceHandle(hSvc);
	::CloseServiceHandle(hSC);
	return TRUE;
}

BOOL CBusyDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();	
	// TODO: Add extra initialization here
	DWORD dwThreadID;
	::CreateThread(NULL, 0, ThreadProc, this, 0, &dwThreadID);	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

DWORD WINAPI CBusyDlg::ThreadProc(LPVOID lpParameter)
{
	CBusyDlg* pWnd = (CBusyDlg*)lpParameter;
	ProcService(pWnd->m_wndText);
	pWnd->PostMessage(WM_CLOSE);
	return 0;
}
