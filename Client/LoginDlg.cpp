// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "JDArchive.h"
#include "LoginDlg.h"
#include "MD5.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg dialog


CLoginDlg::CLoginDlg(CWnd* pParent)
	: CNewDialog(CLoginDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoginDlg)
	m_sUserName = _T("");
	m_sPassword = _T("");
	//}}AFX_DATA_INIT	
	m_bIsConfirm = _SESSION.IsLoggedin();
	if (m_bIsConfirm)
		m_sUserName = _SESSION.GetUserName();
	else
		m_sUserName = AfxGetUserName();
}


void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CNewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoginDlg)
	DDX_Control(pDX, IDC_USERNAME, m_wndUserName);
	DDX_Control(pDX, IDC_PASSWORD, m_wndPassword);
	DDX_Text(pDX, IDC_USERNAME, m_sUserName);
	DDV_MaxChars(pDX, m_sUserName, 20);
	DDX_Text(pDX, IDC_PASSWORD, m_sPassword);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoginDlg, CNewDialog)
	//{{AFX_MSG_MAP(CLoginDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg message handlers

BOOL CLoginDlg::OnInitDialog() 
{
	CNewDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (m_bIsConfirm)
	{
		m_wndUserName.EnableWindow(FALSE);
		SetWindowText(_T("管理身份验证"));
	}
	
	if (!m_sUserName.IsEmpty())
	{
		m_wndPassword.SetFocus();
		return FALSE;
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CString CLoginDlg::GetUserName() const
{
	return m_sUserName;
}

CString CLoginDlg::GetPassword() const
{
	CMD5 md5;
	return (LPCTSTR)md5.CalcText(m_sPassword);
}

void CLoginDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!UpdateData(TRUE))
		return;

	if (m_bIsConfirm)
	{
		if (!_SESSION.IsLoggedin())
		{
			MessageBox(_T("资料库连接已断开。"), _T("管理员身份验证"), MB_ICONEXCLAMATION);
			EndDialog(IDCANCEL);
		}

		if (_SESSION.GetPassword() != GetPassword())
		{
			MessageBox(_T("管理员身份验证失败：密码错误。"), _T("管理员身份验证"), MB_ICONEXCLAMATION);
			m_wndPassword.SetFocus();
			m_wndPassword.Highlight();
			return;
		}

		EndDialog(IDOK);
	}
	else
	{
		m_sUserName.TrimLeft();
		m_sUserName.TrimRight();
		if (m_sUserName.IsEmpty())
		{
			m_wndUserName.SetFocus();
			m_wndUserName.Highlight();
		}
		else
		{
			AfxSetUserName(m_sUserName);
			EndDialog(IDOK);
		}
	}	
}
