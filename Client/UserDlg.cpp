// UserDlg.cpp : implementation file
//

#include "stdafx.h"
#include "JDArchive.h"
#include "UserDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserDlg dialog


CUserDlg::CUserDlg(int nUserID, CWnd* pParent /*=NULL*/)
	: CNewDialog(CUserDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserDlg)
	m_sName = _T("");
	m_nRoleIndex = -1;
	m_nPrivileges = 1;
	//}}AFX_DATA_INIT
	m_nUserID = nUserID;
	if (nUserID > 0)
		_CACHE.GetUser(nUserID, m_sName, &m_nRoleID, &m_nPrivileges);
	else
		m_nRoleID = -1;

	int kk = 0;
}


void CUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CNewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserDlg)
	DDX_Control(pDX, IDC_NAME, m_wndName);
	DDX_Control(pDX, IDC_PRIVILEGES, m_wndPrivileges);
	DDX_Control(pDX, IDC_ROLES, m_wndRoles);
	DDX_Text(pDX, IDC_NAME, m_sName);
	DDV_MaxChars(pDX, m_sName, 20);
	DDX_CBIndex(pDX, IDC_ROLES, m_nRoleIndex);
	DDX_CBIndex(pDX, IDC_PRIVILEGES, m_nPrivileges);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserDlg, CNewDialog)
	//{{AFX_MSG_MAP(CUserDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserDlg message handlers

BOOL CUserDlg::OnInitDialog() 
{
	CNewDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndName.EnableWindow(m_nUserID == 0);
	_CACHE.EnumRoles(EnumRoleProc, this);
	int i = 0;

	for (i = 0; i < m_wndRoles.GetCount(); i++)
	{
		if ((int)m_wndRoles.GetItemData(i) == m_nRoleID)
		{
			m_wndRoles.SetCurSel(i);
			break;
		}
	}
	
	for (i = 0; i < _PRIVILEGES_COUNT - 1 && i < _SESSION.GetUserPrivilege(); i++)
		m_wndPrivileges.AddString(_PRIVILEGES[i]);
	m_wndPrivileges.SetCurSel(m_nPrivileges);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CUserDlg::EnumRoleProc(int nRoleID, LPCTSTR lpszRoleName, LPVOID lParam)
{
	CUserDlg* self = (CUserDlg*)lParam;
	const int IDX = self->m_wndRoles.AddString(lpszRoleName);
	self->m_wndRoles.SetItemData(IDX, nRoleID);
	if (self->m_nRoleID == nRoleID)
		self->m_nRoleIndex = IDX;
	return TRUE;
}

void CUserDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	if (m_nRoleIndex < 0)
	{
		MessageBox(_T("请选择用户组。"), _T("用户信息"), MB_ICONEXCLAMATION);
		m_wndRoles.SetFocus();
		return;
	}

	m_nRoleID = m_wndRoles.GetItemData(m_nRoleIndex);
	EndDialog(IDOK);
}
