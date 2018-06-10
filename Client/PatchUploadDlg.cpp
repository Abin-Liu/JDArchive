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
	m_wndPath.SetFilter(_T("�����ļ�(*.exe)|*.exe|Windows��װ�ļ�(*.msi)|*.msi||"));
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPatchUploadDlg::OnOK() 
{
	// TODO: Add extra validation here
	m_sFilePath = m_wndPath.GetPathName();
	if (m_sFilePath.IsEmpty())
	{
		MessageBox(_T("��ѡ��Ҫ�ϴ��İ汾�ļ�"), _T("�汾�ϴ�"), MB_ICONEXCLAMATION);
		m_wndPath.SetFocus();
		return;
	}

	m_sFileName = AfxGetFileName(m_sFilePath);
	EndDialog(IDOK);
}
