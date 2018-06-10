// UserListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "JDArchive.h"
#include "UserListDlg.h"
#include "DatabaseCache.h"
#include "UserDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserListDlg dialog


CUserListDlg::CUserListDlg(CWnd* pParent /*=NULL*/)
	: CNewDialog(CUserListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserListDlg)
	//}}AFX_DATA_INIT	
}


void CUserListDlg::DoDataExchange(CDataExchange* pDX)
{
	CNewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserListDlg)
	DDX_Control(pDX, IDC_RESETPWD, m_wndResetPwd);
	DDX_Control(pDX, IDC_EDIT, m_wndEdit);
	DDX_Control(pDX, IDC_LIST1, m_wndList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserListDlg, CNewDialog)
	//{{AFX_MSG_MAP(CUserListDlg)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	ON_BN_CLICKED(IDC_RESETPWD, OnResetpwd)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnItemchangedList1)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_DATA_NOTIFY, OnDataNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserListDlg message handlers

BOOL CUserListDlg::OnInitDialog() 
{
	CNewDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndList.InsertColumn(0, _T("用户名"), LVCFMT_LEFT, 100);
	m_wndList.InsertColumn(1, _T("用户组"), LVCFMT_LEFT, 120);
	m_wndList.InsertColumn(2, _T("权限"), LVCFMT_LEFT, 120);

	_SESSION.RegisterUserNotify(m_hWnd);
	_CACHE.EnumUsers(EnumUserProc, &m_wndList);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUserListDlg::OnAdd() 
{
	// TODO: Add your control notification handler code here
	CUserDlg dlg;
	if (dlg.DoModal() == IDOK)
		_SESSION.TaskAddUser(dlg.m_nRoleID, dlg.m_nPrivileges, dlg.m_sName);
}

void CUserListDlg::OnEdit() 
{
	// TODO: Add your control notification handler code here
	int nUserID = (int)m_wndList.GetSelectionData();
	if (nUserID < 1)
		return;

	CUserDlg dlg(nUserID);
	if (dlg.DoModal() == IDOK)
		_SESSION.TaskChangeUser(nUserID, dlg.m_nRoleID, dlg.m_nPrivileges);
}

void CUserListDlg::OnResetpwd() 
{
	// TODO: Add your control notification handler code here
	if (MessageBox(_T("重置密码将使此用户的密码被清空，确定要继续吗？"), _T("重置用户密码"), MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2) != IDYES)
		return;

	int nUserID = (int)m_wndList.GetSelectionData();
	if (nUserID > 0)
		_SESSION.TaskResetPassword(nUserID);
}

BOOL CUserListDlg::EnumUserProc(int nUserID, LPCUSERDATA pUserData, LPVOID lParam)
{
	CListCtrl* pList = (CListCtrl*)lParam;
	const int IDX = pList->InsertItem(INT_MAX, pUserData->sName);
	CString str;
	if (_CACHE.GetRole(pUserData->nRoleID, str))	
		pList->SetItemText(IDX, 1, str);

	pList->SetItemText(IDX, 2, _SESSION.GetUserPrivilegeName(pUserData->nPrivilege));
	pList->SetItemData(IDX, nUserID);
	return TRUE;
}

LRESULT CUserListDlg::OnDataNotify(WPARAM bIsResetPwd, LPARAM nUserID)
{
	CString sName, sRoleName, sPrivilegeName, sMsg;
	int nRoleID, nPrivilege;
	if (!_CACHE.GetUser(nUserID, sName, &nRoleID, &nPrivilege))
		return 0;
	
	if (bIsResetPwd)
	{
		sMsg.Format(_T("用户\"%s\"的密码已被清空，请立即通知他(她)设定新密码，否则将严重影响系统安全！"), (LPCTSTR)sName);
	}
	else
	{
		_CACHE.GetRole(nRoleID, sRoleName);
		sPrivilegeName = _SESSION.GetUserPrivilegeName(nPrivilege);

		int nIdx = m_wndList.FindItem(nUserID);
		BOOL bIsNew = nIdx == -1;
		if (bIsNew)
			nIdx = m_wndList.InsertItem(INT_MAX, sName);

		m_wndList.SetItemText(nIdx, 1, sRoleName);
		m_wndList.SetItemText(nIdx, 2, sPrivilegeName);
		m_wndList.SetItemData(nIdx, nUserID);		
		
		if (bIsNew)
			sMsg.Format(_T("用户\"%s\"已添加，密码暂为空，请立即通知他(她)设定新密码，否则将严重影响系统安全！"), (LPCTSTR)sName);
		else
			sMsg.Format(_T("用户\"%s\"数据已修改成功。"), (LPCTSTR)sName);		
	}

	MessageBox(sMsg, _T("用户信息"), MB_ICONINFORMATION);	
	return 0;
}

void CUserListDlg::OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	BOOL bEnable = FALSE;
	int nUserID = m_wndList.GetSelectionData();
	CString sName;
	int nPriv;
	if (_CACHE.GetUser(nUserID, sName, NULL, &nPriv))
		bEnable = nPriv < _SESSION.GetUserPrivilege();	

	m_wndEdit.EnableWindow(bEnable);
	m_wndResetPwd.EnableWindow(bEnable);

	*pResult = 0;
}
