// AddRoleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "JDArchive.h"
#include "AddRoleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddRoleDlg dialog


CAddRoleDlg::CAddRoleDlg(CWnd* pParent /*=NULL*/)
	: CNewDialog(CAddRoleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddRoleDlg)
	m_sRoleName = _T("");
	//}}AFX_DATA_INIT
}


void CAddRoleDlg::DoDataExchange(CDataExchange* pDX)
{
	CNewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddRoleDlg)
	DDX_Control(pDX, IDC_NAME, m_wndRoleName);
	DDX_Text(pDX, IDC_NAME, m_sRoleName);
	DDV_MaxChars(pDX, m_sRoleName, 20);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddRoleDlg, CNewDialog)
	//{{AFX_MSG_MAP(CAddRoleDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddRoleDlg message handlers

void CAddRoleDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (UpdateData(TRUE))
	{
		m_sRoleName.TrimLeft();
		m_sRoleName.TrimRight();
		if (!m_sRoleName.IsEmpty())
		{
			EndDialog(IDOK);
			return;
		}
	}
	
	m_wndRoleName.SetFocus();
	m_wndRoleName.Highlight();	
}
