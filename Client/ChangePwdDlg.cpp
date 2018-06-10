// ChangePwdDlg.cpp : implementation file
//

#include "stdafx.h"
#include "JDArchive.h"
#include "ChangePwdDlg.h"
#include "MD5.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChangePwdDlg dialog


CChangePwdDlg::CChangePwdDlg(CWnd* pParent /*=NULL*/)
	: CNewDialog(CChangePwdDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChangePwdDlg)
	m_sOldPassword = _T("");
	m_sNewPassword = _T("");
	m_sNewPassword2 = _T("");
	//}}AFX_DATA_INIT
}


void CChangePwdDlg::DoDataExchange(CDataExchange* pDX)
{
	CNewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChangePwdDlg)
	DDX_Control(pDX, IDC_NEWPWD2, m_wndNewPassword2);
	DDX_Control(pDX, IDC_NEWPWD, m_wndNewPassword);
	DDX_Control(pDX, IDC_OLDPWD, m_wndOldPassword);
	DDX_Text(pDX, IDC_OLDPWD, m_sOldPassword);
	DDX_Text(pDX, IDC_NEWPWD, m_sNewPassword);
	DDX_Text(pDX, IDC_NEWPWD2, m_sNewPassword2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChangePwdDlg, CNewDialog)
	//{{AFX_MSG_MAP(CChangePwdDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



CString CChangePwdDlg::GetOldPassword() const
{
	CMD5 md5;
	return (LPCTSTR)md5.CalcText(m_sOldPassword);
}

CString CChangePwdDlg::GetNewPassword() const
{	
	CMD5 md5;
	return (LPCTSTR)md5.CalcText(m_sNewPassword);
}

void CChangePwdDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	if (m_sNewPassword != m_sNewPassword2)
	{
		MessageBox(_T("新密码与重复新密码必须相同。"), _T("修改密码"), MB_ICONEXCLAMATION);
		m_wndNewPassword2.SetFocus();
		m_wndNewPassword2.Highlight();
		return;
	}
	EndDialog(IDOK);
}
