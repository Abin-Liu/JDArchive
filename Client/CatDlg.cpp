// CatDlg.cpp : implementation file
//

#include "stdafx.h"
#include "JDArchive.h"
#include "CatDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCatDlg dialog


CCatDlg::CCatDlg(int nRoleID, int nParentCatID, LPCTSTR lpszParentName, int nCatID, LPCTSTR lpszName, LPCTSTR lpszDesc, CWnd* pParent /*=NULL*/)
	: CNewDialog(CCatDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCatDlg)
	m_sName = _T("");
	m_sParentName = _T("");
	m_sDesc = _T("");
	//}}AFX_DATA_INIT
	m_nCatID = nCatID;
	m_nRoleID = nRoleID;
	m_nParentCatID = nParentCatID;
	m_sParentName = lpszParentName;
	m_sName = lpszName;
	m_sDesc = lpszDesc;
}

void CCatDlg::DoDataExchange(CDataExchange* pDX)
{
	CNewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCatDlg)
	DDX_Control(pDX, IDC_DESC, m_wndDesc);
	DDX_Control(pDX, IDC_NAME, m_wndName);
	DDX_Text(pDX, IDC_NAME, m_sName);
	DDV_MaxChars(pDX, m_sName, 64);
	DDX_Text(pDX, IDC_PARENTCAT, m_sParentName);
	DDX_Text(pDX, IDC_DESC, m_sDesc);
	DDV_MaxChars(pDX, m_sDesc, 79);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCatDlg, CNewDialog)
	//{{AFX_MSG_MAP(CCatDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCatDlg message handlers

void CCatDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!UpdateData(TRUE))
		return;

	m_sName.TrimLeft();
	m_sName.TrimRight();
	if (m_sName.IsEmpty())
	{
		m_wndName.SetFocus();
		m_wndName.Highlight();
		return;
	}

	m_sDesc.TrimLeft();
	m_sDesc.TrimRight();
	EndDialog(IDOK);
}