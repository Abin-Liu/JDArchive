// RecoverDlg.cpp : implementation file
//

#include "stdafx.h"
#include "JDArchive.h"
#include "RecoverDlg.h"
#include "DatabaseCache.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRecoverDlg dialog


CRecoverDlg::CRecoverDlg(CWnd* pParent /*=NULL*/)
	: CNewDialog(CRecoverDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRecoverDlg)
	m_sFileName = _T("");
	m_nRoleIndex = -1;
	//}}AFX_DATA_INIT
	m_nRoleID = 0;
}


void CRecoverDlg::DoDataExchange(CDataExchange* pDX)
{
	CNewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecoverDlg)
	DDX_Control(pDX, IDC_FILENAME, m_wndFileName);
	DDX_Control(pDX, IDC_ROLES, m_wndRoles);
	DDX_Text(pDX, IDC_FILENAME, m_sFileName);
	DDX_CBIndex(pDX, IDC_ROLES, m_nRoleIndex);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRecoverDlg, CNewDialog)
	//{{AFX_MSG_MAP(CRecoverDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecoverDlg message handlers

BOOL CRecoverDlg::OnInitDialog() 
{
	CNewDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	_CACHE.EnumRoles(EnumRoleProc, &m_wndRoles);	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CRecoverDlg::EnumRoleProc(int nRoleID, LPCTSTR lpszRoleName, LPVOID lParam)
{
	CComboBox* pCombo = (CComboBox*)lParam;
	const int IDX = pCombo->AddString(lpszRoleName);
	pCombo->SetItemData(IDX, nRoleID);
	return TRUE;
}

void CRecoverDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	if (m_nRoleIndex < 0)
	{
		MessageBox(_T("请选择申请恢复数据的部门"), _T("数据恢复"), MB_ICONEXCLAMATION);
		m_wndRoles.SetFocus();
		return;
	}

	m_nRoleID = m_wndRoles.GetItemData(m_nRoleIndex);
	m_sFileName.TrimLeft();
	m_sFileName.TrimRight();
	EndDialog(IDOK);
}
