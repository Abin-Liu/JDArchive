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
	m_wndPath.SetDialogTitle(_T("�ϴ�Ŀ¼"));
	m_wndPath.SetDialogBanner(_T("��ѡ��Ҫ�ϴ���Ŀ¼:"));
	m_wndPath.SetTooltipText(_T("���"));
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
		MessageBox(_T("�������ϴ������̷�����ѡ��һ��Ŀ¼��"), _T("��Ч��Ŀ¼"), MB_ICONEXCLAMATION);
}
