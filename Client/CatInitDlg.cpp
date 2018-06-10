// CatInitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "JDArchive.h"
#include "CatInitDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCatInitDlg dialog


CCatInitDlg::CCatInitDlg(CWnd* pParent /*=NULL*/)
	: CNewDialog(CCatInitDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCatInitDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCatInitDlg::DoDataExchange(CDataExchange* pDX)
{
	CNewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCatInitDlg)
	DDX_Control(pDX, IDC_PATH, m_wndPath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCatInitDlg, CNewDialog)
	//{{AFX_MSG_MAP(CCatInitDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCatInitDlg message handlers

BOOL CCatInitDlg::OnInitDialog() 
{
	CNewDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndPath.SetButtonStyle(BC_BTN_ICON | BC_ICO_FOLDER | BC_CTL_FOLDERSONLY);
	m_wndPath.SetDialogTitle(_T("上传目录"));
	m_wndPath.SetDialogBanner(_T("请选择要上传的目录:"));
	m_wndPath.SetTooltipText(_T("浏览"));
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCatInitDlg::OnOK() 
{
	// TODO: Add extra validation here
	m_sPathName = m_wndPath.GetPathName();
	if (m_sPathName.IsEmpty())
		return;

	if (m_sPathName.GetLength() >= 4)
		CNewDialog::OnOK();
	else
		MessageBox(_T("不允许上传整个盘符，请选择一个目录。"), _T("无效的目录"), MB_ICONEXCLAMATION);
}
