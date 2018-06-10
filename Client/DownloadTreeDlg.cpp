// DownloadTreeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "JDArchive.h"
#include "DownloadTreeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDownloadTreeDlg dialog


CDownloadTreeDlg::CDownloadTreeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDownloadTreeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDownloadTreeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDownloadTreeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDownloadTreeDlg)
	DDX_Control(pDX, IDC_BUTTON1, m_wndPath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDownloadTreeDlg, CDialog)
	//{{AFX_MSG_MAP(CDownloadTreeDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDownloadTreeDlg message handlers

BOOL CDownloadTreeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndPath.SetButtonStyle(BC_BTN_ICON | BC_ICO_FOLDER | BC_CTL_FOLDERSONLY);
	m_wndPath.SetDialogTitle(_T("目录树下载"));
	m_wndPath.SetDialogBanner(_T("请选择要保存的路径:"));
	m_wndPath.SetTooltipText(_T("浏览"));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDownloadTreeDlg::OnOK() 
{
	// TODO: Add extra validation here
	m_sPathName = m_wndPath.GetPathName();
	if (!m_sPathName.IsEmpty())
		CDialog::OnOK();
}
