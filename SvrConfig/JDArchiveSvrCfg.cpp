// JDArchiveSvrCfg.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "JDArchiveSvrCfg.h"
#include "JDArchiveSvrCfgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJDArchiveSvrCfgApp

BEGIN_MESSAGE_MAP(CJDArchiveSvrCfgApp, CWinApp)
	//{{AFX_MSG_MAP(CJDArchiveSvrCfgApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJDArchiveSvrCfgApp construction

CJDArchiveSvrCfgApp::CJDArchiveSvrCfgApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_bSilent = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CJDArchiveSvrCfgApp object

CJDArchiveSvrCfgApp theApp;

BOOL AfxSilent()
{
	return theApp.m_bSilent;
}

/////////////////////////////////////////////////////////////////////////////
// CJDArchiveSvrCfgApp initialization

BOOL CJDArchiveSvrCfgApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
	SetInstanceUniqueID(_T("{732568FB-4BDE-47C9-9BA6-61E95BEA22A6}"));
	if (!IsInstanceUnique())
		return FALSE;

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	m_bSilent = (_tcsicmp(m_lpCmdLine, _T("-silent")) == 0);

	WNDCLASS wc = { 0 };
	::GetClassInfo(AfxGetInstanceHandle(), _T("#32770"), &wc);   
	wc.lpszClassName = _T("JDArchiveSvrCfg");
	AfxRegisterClass(&wc);

	CJDArchiveSvrCfgDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
