// ServerCfgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "JDArchive.h"
#include "ServerCfgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerCfgDlg dialog


CServerCfgDlg::CServerCfgDlg(LPCTSTR lpszAddress, CWnd* pParent /*=NULL*/)
	: CNewDialog(CServerCfgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CServerCfgDlg)
	m_sAddress = _T("");
	//}}AFX_DATA_INIT
	m_sAddress = lpszAddress;
}


void CServerCfgDlg::DoDataExchange(CDataExchange* pDX)
{
	CNewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerCfgDlg)
	DDX_Control(pDX, IDC_ADDRESS, m_wndAddress);
	DDX_Text(pDX, IDC_ADDRESS, m_sAddress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CServerCfgDlg, CNewDialog)
	//{{AFX_MSG_MAP(CServerCfgDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerCfgDlg message handlers

void CServerCfgDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!UpdateData(TRUE))
		return;

	m_sAddress.TrimLeft();
	m_sAddress.TrimRight();
	if (m_sAddress.IsEmpty())
	{
		m_wndAddress.SetFocus();
		m_wndAddress.Highlight();
		return;
	}	

	EndDialog(IDOK);
}
