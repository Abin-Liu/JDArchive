// FileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "JDArchive.h"
#include "FileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileDlg dialog


CFileDlg::CFileDlg(LPCTSTR lpszFileName, LPCTSTR lpszDesc, CWnd* pParent /*=NULL*/)
	: CNewDialog(CFileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileDlg)
	m_sFileName = _T("");
	m_sDesc = _T("");
	//}}AFX_DATA_INIT
	m_sFileName = lpszFileName;
	m_sDesc = lpszDesc;
}


void CFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CNewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileDlg)
	DDX_Control(pDX, IDC_FILENAME, m_wndFileName);
	DDX_Control(pDX, IDC_VERDESC, m_wndDesc);
	DDX_Control(pDX, IDC_BROWSE, m_wndBrowse);
	DDX_Text(pDX, IDC_FILENAME, m_sFileName);
	DDX_Text(pDX, IDC_VERDESC, m_sDesc);
	DDV_MaxChars(pDX, m_sDesc, 79);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileDlg, CNewDialog)
	//{{AFX_MSG_MAP(CFileDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileDlg message handlers

BOOL CFileDlg::OnInitDialog() 
{
	CNewDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (m_sFileName.IsEmpty())
	{
		m_wndFileName.ShowWindow(SW_HIDE);
	}
	else
	{
		m_wndBrowse.ShowWindow(SW_HIDE);
		m_wndBrowse.SetDialogTitle(_T("创建文件"));
		m_wndBrowse.SetDialogTitle(_T("请选择将要创建的文件："));
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFileDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!UpdateData(TRUE))
		return;

	if (m_sFileName.IsEmpty())
	{
		m_sFilePath = m_wndBrowse.GetPathName();
		m_sFileName = m_wndBrowse.GetFileName();		
		if (m_sFileName.IsEmpty())
			return;
	}

	if (_SESSION.GetUserPrivilege() < JDPRIV_SUPERVISOR && AfxIsExecutable(m_sFileName))
	{
		MessageBox(_T("部门主管以上权限的用户才允许上传可执行文件"), _T("文件类型错误"), MB_ICONEXCLAMATION);
		return;
	}

	m_sDesc.TrimLeft();
	m_sDesc.TrimRight();
	EndDialog(IDOK);
}
