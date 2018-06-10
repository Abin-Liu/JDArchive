// PatchUploadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "JDArchive.h"
#include "PatchUploadDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPatchUploadDlg dialog


CPatchUploadDlg::CPatchUploadDlg(CWnd* pParent /*=NULL*/)
	: CNewDialog(CPatchUploadDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPatchUploadDlg)
	m_sVersion = _T("");
	//}}AFX_DATA_INIT
	m_sVersion.Format(_T("%d.%02d"), APP_VERSION / 100, APP_VERSION % 100);
}


void CPatchUploadDlg::DoDataExchange(CDataExchange* pDX)
{
	CNewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPatchUploadDlg)
	DDX_Control(pDX, IDC_PATH, m_wndPath);
	DDX_Text(pDX, IDC_VERSION, m_sVersion);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPatchUploadDlg, CNewDialog)
	//{{AFX_MSG_MAP(CPatchUploadDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPatchUploadDlg message handlers

BOOL CPatchUploadDlg::OnInitDialog() 
{
	CNewDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndPath.SetDefExt(_T("exe"));
	m_wndPath.SetFilter(_T("程序文件(*.exe)|*.exe|Windows安装文件(*.msi)|*.msi||"));
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPatchUploadDlg::OnOK() 
{
	// TODO: Add extra validation here
	m_sFilePath = m_wndPath.GetPathName();
	if (m_sFilePath.IsEmpty())
	{
		MessageBox(_T("请选择要上传的版本文件"), _T("版本上传"), MB_ICONEXCLAMATION);
		m_wndPath.SetFocus();
		return;
	}

	m_sFileName = AfxGetFileName(m_sFilePath);
	EndDialog(IDOK);
}
