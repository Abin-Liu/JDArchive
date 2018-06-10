// RoleListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "JDArchive.h"
#include "RoleListDlg.h"
#include "AddRoleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRoleListDlg dialog


CRoleListDlg::CRoleListDlg(CWnd* pParent /*=NULL*/)
	: CNewDialog(CRoleListDlg::IDD, pParent) 
{
	//{{AFX_DATA_INIT(CRoleListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CRoleListDlg::DoDataExchange(CDataExchange* pDX)
{
	CNewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRoleListDlg)
	DDX_Control(pDX, IDC_LIST1, m_wndList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRoleListDlg, CNewDialog)
	//{{AFX_MSG_MAP(CRoleListDlg)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_DATA_NOTIFY, OnDataNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRoleListDlg message handlers

BOOL CRoleListDlg::OnInitDialog() 
{
	CNewDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	_SESSION.RegisterRoleNotify(m_hWnd);
	m_wndList.InsertColumn(0, _T("数据库ID"), LVCFMT_LEFT, 80);
	m_wndList.InsertColumn(1, _T("组名称"), LVCFMT_LEFT, 180);
	_CACHE.EnumRoles(EnumRoleProc, &m_wndList);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CRoleListDlg::EnumRoleProc(int nRoleID, LPCTSTR lpszRoleName, LPVOID lParam)
{
	static CString sID;
	CListCtrl* pList = (CListCtrl*)lParam;
	sID.Format(_T("%03d"), nRoleID);
	const int IDX = pList->InsertItem(INT_MAX, sID);
	pList->SetItemText(IDX, 1, lpszRoleName);
	pList->SetItemData(IDX, nRoleID);
	return TRUE;
}

void CRoleListDlg::OnAdd() 
{
	// TODO: Add your control notification handler code here
	CAddRoleDlg dlg;
	if (dlg.DoModal() == IDOK)
		_SESSION.TaskAddRole(dlg.m_sRoleName);
}

LRESULT CRoleListDlg::OnDataNotify(WPARAM wParam, LPARAM lParam)
{
	CString sName;
	if (!_CACHE.GetRole(lParam, sName))
		return 0;

	int nIdx = m_wndList.FindItem(lParam);
	if (nIdx == -1)
	{
		CString sID;
		sID.Format(_T("%03d"), lParam);
		nIdx = m_wndList.InsertItem(INT_MAX, sID);
		m_wndList.SetItemText(nIdx, 1, sName);
		m_wndList.SetItemData(nIdx, lParam);
	}
	else
	{
		m_wndList.SetItemText(nIdx, 1, sName);
	}

	return 0;
}
