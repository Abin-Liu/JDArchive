// RecoverListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "JDArchive.h"
#include "RecoverListDlg.h"
#include "..\\LBNetwork.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRecoverListDlg dialog


CRecoverListDlg::CRecoverListDlg(CWnd* pParent /*=NULL*/)
	: CNewDialog(CRecoverListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRecoverListDlg)
	//}}AFX_DATA_INIT
	m_nVerID = 0;
}


void CRecoverListDlg::DoDataExchange(CDataExchange* pDX)
{
	CNewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecoverListDlg)
	DDX_Control(pDX, IDC_LIST1, m_wndList);
	DDX_Control(pDX, IDOK, m_wndOK);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRecoverListDlg, CNewDialog)
	//{{AFX_MSG_MAP(CRecoverListDlg)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnItemchangedList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecoverListDlg message handlers

BOOL CRecoverListDlg::OnInitDialog() 
{
	CNewDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndList.InitializeStyle();
	m_wndList.InsertColumn(0, _T("原文件名"), LVCFMT_LEFT, 150);
	m_wndList.InsertColumn(1, _T("版本"), LVCFMT_LEFT, 80);
	m_wndList.InsertColumn(2, _T("文件大小"), LVCFMT_RIGHT, 80);
	m_wndList.InsertColumn(3, _T("归档人"), LVCFMT_LEFT, 65);
	m_wndList.InsertColumn(4, _T("归档日期"), LVCFMT_LEFT, 80);
	m_wndList.InsertColumn(5, _T("删除人"), LVCFMT_LEFT, 65);
	m_wndList.InsertColumn(6, _T("删除日期"), LVCFMT_LEFT, 80);
	m_wndList.InsertColumn(7, _T("版本说明"), LVCFMT_LEFT, 100);
	
	CString str;
	for (int i = 0; i < m_aData.GetSize(); i++)
	{
		RECOVERDATA& data = m_aData.ElementAt(i);
		int IDX = m_wndList.InsertItem(INT_MAX, data.szFileName);
		m_wndList.SetItemData(IDX, data.nVerID);
		m_wndList.SetItemText(IDX, 1, AfxParseVersion(data.nVersion));
		m_wndList.SetItemText(IDX, 2, AfxSizeToStr(data.dwSize));
		_CACHE.GetUser(data.nCreatorID, str);
		m_wndList.SetItemText(IDX, 3, str);
		m_wndList.SetItemText(IDX, 4, data.createDate.Format(VAR_DATEVALUEONLY));
		_CACHE.GetUser(data.nDeleterID, str);
		m_wndList.SetItemText(IDX, 5, str);
		m_wndList.SetItemText(IDX, 6, data.deleteDate.Format(VAR_DATEVALUEONLY));
		m_wndList.SetItemText(IDX, 7, data.szDesc);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRecoverListDlg::OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_wndOK.EnableWindow(m_wndList.GetSelectedCount() > 0);
	*pResult = 0;
}

void CRecoverListDlg::OnOK() 
{
	// TODO: Add extra validation here
	m_nVerID = m_wndList.GetSelectionData();
	if (m_nVerID > 0)
		EndDialog(IDOK);			
}
