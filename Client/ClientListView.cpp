// ClientListView.cpp : implementation file
//

#include "stdafx.h"
#include "JDArchive.h"
#include "ClientListView.h"
#include "DatabaseCache.h"
#include "PopupMenu.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClientListView

IMPLEMENT_DYNCREATE(CClientListView, CListView)

CClientListView::CClientListView()
{
	m_il.Create(1, 20, ILC_COLOR24, 1, 1);
	m_pMenu = NULL;
	m_nFileID = 0;
	m_nSelectedVerID = 0;
	m_nSelection = -1;
}

CClientListView::~CClientListView()
{
	m_il.DeleteImageList();
}

BEGIN_MESSAGE_MAP(CClientListView, CListView)
	//{{AFX_MSG_MAP(CClientListView)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemchanged)
	ON_WM_CONTEXTMENU()
	ON_WM_KEYUP()
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientListView message handlers


int CClientListView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CListCtrl& lc = GetListCtrl();
	ASSERT(lc.GetSafeHwnd());

	lc.ModifyStyle(LVS_EDITLABELS | LVS_ICON | LVS_SMALLICON | LVS_LIST | LVS_NOSCROLL | LVS_NOCOLUMNHEADER, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | LVS_NOSORTHEADER);
	lc.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	lc.SetImageList(&m_il, LVSIL_SMALL);

	lc.InsertColumn(0, _T("版本号"), LVCFMT_LEFT, 120);
	lc.InsertColumn(1, _T("文件大小"), LVCFMT_RIGHT, 80);
	lc.InsertColumn(2, _T("归档人"), LVCFMT_LEFT, 80);	
	lc.InsertColumn(3, _T("归档日期"), LVCFMT_LEFT, 100);
	lc.InsertColumn(4, _T("审核人"), LVCFMT_LEFT, 80);
	lc.InsertColumn(5, _T("审核日期"), LVCFMT_LEFT, 100);
	lc.InsertColumn(6, _T("版本说明"), LVCFMT_LEFT, 280);

	OnLogout();
	return 0;
}

int CClientListView::GetFileID() const
{
	return m_nFileID;
}

void CClientListView::SetFileID(int nFileID)
{
	if (nFileID == m_nFileID)
		return;

	m_nFileID = nFileID;
	CListCtrl& lc = GetListCtrl();
	lc.DeleteAllItems();
	m_nSelectedVerID = 0;
	m_nSelection = -1;
	if (nFileID < 1)
		return;

	if (!_CACHE.EnumVersions(nFileID, EnumVersionProc, &lc))
		_SESSION.TaskListVersion(nFileID); // Request data from the server
}

void CClientListView::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_nSelection = -1;
	if (pNMListView->uNewState & LVIS_SELECTED)
		m_nSelection = pNMListView->iItem;

	if (m_nSelection == -1)
		m_nSelectedVerID = 0;
	else
		m_nSelectedVerID = (int)GetListCtrl().GetItemData(m_nSelection);
	*pResult = 0;	
}

int CClientListView::GetSelectedVerID() const
{
	return m_nSelectedVerID;
}

void CClientListView::OnLogout()
{
	GetListCtrl().DeleteAllItems();
}

BOOL CClientListView::EnumVersionProc(int nVersionID, LPCVERDATA pVersionData, LPVOID lParam)
{
	CListCtrl* pList = (CListCtrl*)lParam;	
	const int IDX = pList->InsertItem(0, AfxParseVersion(pVersionData->nVersion));
	pList->SetItemText(IDX, 1, pVersionData->sSize);		
	CString sName;
	if (_CACHE.GetUser(pVersionData->nCreatorID, sName))
		pList->SetItemText(IDX, 2, sName);
	pList->SetItemText(IDX, 3, pVersionData->dtCreate.Format(VAR_DATEVALUEONLY));
	if (_CACHE.GetUser(pVersionData->nCheckerID, sName))
	{
		pList->SetItemText(IDX, 4, sName);
		pList->SetItemText(IDX, 5, pVersionData->dtCheck.Format(VAR_DATEVALUEONLY));
	}

	pList->SetItemText(IDX, 6, pVersionData->sDesc);
	pList->SetItemData(IDX, nVersionID);
	return TRUE;
}

void CClientListView::AddVersion(CLBPacket *pPacket)
{
	int nVerID = _CACHE.AddVersion(pPacket);
	LPCVERDATA p = _CACHE.GetVersion(nVerID);
	if (p == NULL || p->nFileID != m_nFileID)
		return;

	CListCtrl& lc = GetListCtrl();
	EnumVersionProc(nVerID, p, &lc);	
}

void CClientListView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	if (m_nSelectedVerID > 0)
		PopupMenu(m_pMenu->GetSubMenu(4), AfxGetMainWnd());	
}

void CClientListView::DeleteVer(int nVerID)
{
	int nFileID = _CACHE.DeleteVersion(nVerID);
	CListCtrl& lc = GetListCtrl();
	if (nFileID && nFileID == m_nFileID)
	{
		for (int i = 0; i < lc.GetItemCount(); i++)
		{
			if ((int)lc.GetItemData(i) == nVerID)
			{
				lc.DeleteItem(i);
				return;
			}
		}
	}
}

int CClientListView::GetSelectedVerCheckState() const
{
	if (m_nSelection == -1)
		return -1;

	return GetListCtrl().GetItemText(m_nSelection, 4).IsEmpty() ? 0 : 1;
}

void CClientListView::CheckVersion(CLBPacket* pPacket)
{
	int nVerID = pPacket->ReadInt();
	LPVERDATA p = _CACHE.GetVersion(nVerID);
	if (p == NULL)
		return;

	p->nCheckerID = pPacket->ReadInt();
	pPacket->ReadData(&p->dtCheck, sizeof(COleDateTime));
	CString sCheckerName;
	_CACHE.GetUser(p->nCheckerID, sCheckerName);

	CListCtrl& lc = GetListCtrl();
	for (int i = 0; i < lc.GetItemCount(); i++)
	{
		if ((int)lc.GetItemData(i) == nVerID)
		{
			lc.SetItemText(i, 4, sCheckerName);
			lc.SetItemText(i, 5, p->dtCheck.Format(VAR_DATEVALUEONLY));
			return;
		}
	}
}

void CClientListView::UncheckVersion(CLBPacket* pPacket)
{
	int nVerID = pPacket->ReadInt();
	LPVERDATA p = _CACHE.GetVersion(nVerID);
	if (p == NULL)
		return;

	p->nCheckerID = 0;
	p->dtCheck.SetStatus(COleDateTime::invalid);

	CListCtrl& lc = GetListCtrl();
	for (int i = 0; i < lc.GetItemCount(); i++)
	{
		if ((int)lc.GetItemData(i) == nVerID)
		{
			lc.SetItemText(i, 4, _T(""));
			lc.SetItemText(i, 5, _T(""));
			return;
		}
	}
}

void CClientListView::SetMenu(CMenu *pMenu)
{
	m_pMenu = pMenu;
}

void CClientListView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == VK_ESCAPE)
	{
		if (m_nSelection != -1)
			GetListCtrl().SetItemState(m_nSelection, 0, LVIS_FOCUSED | LVIS_SELECTED);
	}	
	else
	{
		CListView::OnKeyUp(nChar, nRepCnt, nFlags);
	}	
}

void CClientListView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if (m_nSelectedVerID)
		_SESSION.TaskDownloadVersion(m_nSelectedVerID);
	*pResult = 0;
}
