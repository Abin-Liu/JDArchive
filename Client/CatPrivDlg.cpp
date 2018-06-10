// CatPrivDlg.cpp : implementation file
//

#include "stdafx.h"
#include "JDArchive.h"
#include "CatPrivDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCatPrivDlg dialog


CCatPrivDlg::CCatPrivDlg(int nRoleID, LPCTSTR lpszPrivileges, CWnd* pParent /*=NULL*/)
	: CNewDialog(CCatPrivDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCatPrivDlg)
	m_bApplySubCats = AfxGetApp()->GetProfileInt(_T(""), _T("Apply Sub Category"), TRUE);
	//}}AFX_DATA_INIT
	m_nRoleID = nRoleID;
	m_sPrivs = lpszPrivileges;
}

void CCatPrivDlg::DoDataExchange(CDataExchange* pDX)
{
	CNewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCatPrivDlg)
	DDX_Control(pDX, IDC_LIST1, m_wndList);
	DDX_Check(pDX, IDC_APPLYSUBCATS, m_bApplySubCats);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCatPrivDlg, CNewDialog)
	//{{AFX_MSG_MAP(CCatPrivDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCatPrivDlg message handlers

BOOL CCatPrivDlg::OnInitDialog() 
{
	CNewDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndList.SetExtendedStyle(LVS_EX_CHECKBOXES);
	m_wndList.InsertColumn(0, _T("ÓÃ»§×é"), LVCFMT_LEFT, 280);
	_CACHE.EnumRoles(EnumRoleProc, this);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCatPrivDlg::OnOK() 
{
	// TODO: Add extra validation here
	m_sPrivs.Empty();
	CString sSeg;
	for (int i = 0; i < m_wndList.GetItemCount(); i++)
	{
		if (m_wndList.GetCheck(i))
		{
			sSeg.Format(_T("|%d"), m_wndList.GetItemData(i));
			m_sPrivs += sSeg;
		}
	}

	if (!m_sPrivs.IsEmpty())
		m_sPrivs += _T('|');
	
	AfxGetApp()->WriteProfileInt(_T(""), _T("Apply Sub Category"), m_bApplySubCats);

	CNewDialog::OnOK();
}

CString CCatPrivDlg::GetPrivileges() const
{
	return m_sPrivs;
}

BOOL CCatPrivDlg::EnumRoleProc(int nRoleID, LPCTSTR lpszRoleName, LPVOID lParam)
{
	CCatPrivDlg* self = (CCatPrivDlg*)lParam;
	if (self->m_nRoleID == nRoleID)
		return TRUE;

	const int IDX = self->m_wndList.InsertItem(INT_MAX, lpszRoleName);
	self->m_wndList.SetItemData(IDX, nRoleID);
	CString sPriv;
	sPriv.Format(_T("|%d|"), nRoleID);
	if (self->m_sPrivs.Find(sPriv) != -1)
		self->m_wndList.SetCheck(IDX, TRUE);	
	return TRUE;
}
