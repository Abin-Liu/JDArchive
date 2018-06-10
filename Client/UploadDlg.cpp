// UploadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "JDArchive.h"
#include "UploadDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUploadDlg dialog


CUploadDlg::CUploadDlg(LPCTSTR lpszFileName, int nVersion, LPCTSTR lpszFilePath, CWnd* pParent /*=NULL*/)
	: CNewDialog(CUploadDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUploadDlg)
	m_sVerDesc = _T("");
	m_sFileName = lpszFileName;
	m_sVersion = _T("");
	//}}AFX_DATA_INIT
	m_sFilePath = lpszFilePath;
	m_sVersion = AfxParseVersion(nVersion);
}

void CUploadDlg::DoDataExchange(CDataExchange* pDX)
{
	CNewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUploadDlg)
	DDX_Control(pDX, IDC_VERDESC, m_wndVerDesc);
	DDX_Control(pDX, IDC_FILEPATH, m_wndFilePath);
	DDX_Text(pDX, IDC_VERDESC, m_sVerDesc);
	DDV_MaxChars(pDX, m_sVerDesc, 79);
	DDX_Text(pDX, IDC_NAME, m_sFileName);
	DDX_Text(pDX, IDC_VERSION, m_sVersion);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUploadDlg, CNewDialog)
	//{{AFX_MSG_MAP(CUploadDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUploadDlg message handlers

BOOL CUploadDlg::OnInitDialog() 
{
	CNewDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndFilePath.SetPathName(m_sFilePath);
	if (m_sFilePath.IsEmpty())
		m_wndFilePath.SetFilter(m_sFileName + _T("|") + m_sFileName + _T("||"));
	else
		m_wndFilePath.EnableWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUploadDlg::OnOK() 
{
	// TODO: Add extra validation here
	m_sFilePath = m_wndFilePath.GetPathName();
	if (m_sFilePath.IsEmpty())
	{
		MessageBox(_T("请选择要上传的文件版本"), _T("版本上传"), MB_ICONEXCLAMATION);
		m_wndFilePath.SetFocus();
		return;
	}

	if (m_wndFilePath.GetFileName().CompareNoCase(m_sFileName))
	{
		CString sMsg;
		sMsg.Format(_T("上传的版本文件名必须为\"%s\""), (LPCTSTR)m_sFileName);
		MessageBox(sMsg, _T("无效文件名"), MB_ICONEXCLAMATION);
		m_wndFilePath.SetFocus();
		return;
	}

	if (_SESSION.GetUserPrivilege() < JDPRIV_SUPERVISOR && AfxIsExecutable(m_sFileName))
	{
		MessageBox(_T("部门主管以上权限的用户才允许上传可执行文件"), _T("文件类型错误"), MB_ICONEXCLAMATION);
		return;
	}

	if (!UpdateData(TRUE))
		return;	

	m_sVerDesc.TrimLeft();
	m_sVerDesc.TrimRight();
	EndDialog(IDOK);
}

void CUploadDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	_SESSION.TaskCancelRequestUploadVer();
	CNewDialog::OnCancel();
}
