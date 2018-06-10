// ClientTreeView.cpp : implementation file
//

#include "stdafx.h"
#include "JDArchive.h"
#include "ClientTreeView.h"
#include "PopupMenu.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const LPCTSTR WAITING_LABEL = _T("请稍候……");
static const LPCTSTR WAITING_TOOLTIP = _T("正在从服务器请求数据");


/////////////////////////////////////////////////////////////////////////////
// CClientTreeView

IMPLEMENT_DYNCREATE(CClientTreeView, CTreeView)

CClientTreeView::CClientTreeView()
{
	m_pMenu = NULL;
	m_pFileView = NULL;
	m_hRoot = NULL;
	m_hSelection = NULL;
	m_nSelStatus = SEL_NONE;
	m_nSelID = 0;
	m_nCx = 0;
	m_nSelRoleID = 0;
	m_pCutData = NULL;
	m_pSelData = NULL;
	m_hCutItem = NULL;
	m_bLockCut = FALSE;
}

CClientTreeView::~CClientTreeView()
{
	m_il.DeleteImageList();
}


BEGIN_MESSAGE_MAP(CClientTreeView, CTooltipTreeView)
	//{{AFX_MSG_MAP(CClientTreeView)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, OnItemexpanded)
	ON_NOTIFY_REFLECT(TVN_DELETEITEM, OnDeleteitem)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientTreeView message handlers

int CClientTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CTreeCtrl& tc = GetTreeCtrl();
	ASSERT(tc.GetSafeHwnd());
	tc.ModifyStyle(TVS_EDITLABELS | TVS_SINGLEEXPAND, TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS | TVS_DISABLEDRAGDROP | TVS_NOTOOLTIPS);
	EnableToolTips(TRUE);

	m_il.Create();
	tc.SetImageList(&m_il, TVSIL_NORMAL);
	tc.SetItemHeight(20);
	OnLogout();
	return 0;
}

void CClientTreeView::OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	HTREEITEM hItem = pNMTreeView->itemNew.hItem;
	if (hItem != m_hRoot && GetItemFileID(hItem) < 1)
	{
		CTreeCtrl& tc = GetTreeCtrl();
		if (pNMTreeView->action == TVE_EXPAND)
		{
			tc.SetItemImage(hItem, TREE_ICON_FOLDER_OPEN, TREE_ICON_FOLDER_OPEN);
			LPTREEDATA p = GetTreeData(hItem);
			if (p && !p->bRequested)
			{
				p->bRequested = TRUE;
				if (!p->bIsFile && p->nID > 0)
				{
					// Request Data from server...
					_SESSION.TaskListCategory(p->nID);

					if (p->nParentID > 0)
						_SESSION.TaskListFile(p->nID);
				}
			}			
		}
		else
		{
			tc.SetItemImage(hItem, TREE_ICON_FOLDER_CLOSE, TREE_ICON_FOLDER_CLOSE);
		}
	}	
	*pResult = 0;
}

void CClientTreeView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	LPTREEDATA p = NULL;
	m_hSelection = pNMTreeView->itemNew.hItem;
	if (m_hSelection == NULL || m_hSelection == m_hRoot)
	{
		m_nSelStatus = SEL_NONE;
		m_nSelID = 0;
		m_nSelRoleID = 0;
	}	
	else
	{
		p = GetTreeData(m_hSelection);
		if (p == NULL || p->nID == 0) // likely an hourglass busy node
		{
			m_nSelStatus = SEL_NONE;
			m_nSelID = 0;
			m_nSelRoleID = 0;
			p = NULL;
		}
		else if (p->bIsFile)
		{
			m_nSelStatus = SEL_FILE;
			m_nSelID = p->nID;
			m_nSelRoleID = p->nRoleID;
			p->bRequested = TRUE;
		}
		else if (p->nParentID == 0)
		{
			m_nSelStatus = SEL_ROLE_CAT;
			m_nSelID = p->nID;
			m_nSelRoleID = p->nRoleID;
		}
		else
		{
			m_nSelStatus = SEL_CAT;
			m_nSelID = p->nID;
			m_nSelRoleID = p->nRoleID;
		}
	}
	
	m_pSelData = p;
	m_pFileView->SetFileID(m_nSelStatus == SEL_FILE ? m_nSelID : 0);
	*pResult = 0;
}

BOOL CClientTreeView::AddOrUpdateFile(CLBPacket* pPacket, BOOL bSort)
{
	int nFileID = pPacket->ReadInt();
	if (nFileID < 1)
		return FALSE;
	
	int nCatID = pPacket->ReadInt();
	CString sName = ReadPacketString(pPacket);
	CString sDesc = ReadPacketString(pPacket);
	int nVersion = pPacket->ReadInt();
	CString sExt = ReadPacketString(pPacket);

	HTREEITEM hItem = _CACHE.GetFile(nFileID);
	if (hItem)
	{
		// Update existing file
		LPTREEDATA p = GetTreeData(hItem);
		p->sTooltip = sDesc;
		p->nVersion = nVersion;
		return TRUE;
	}

	// Add new file
	if (sName.IsEmpty())
		return FALSE;

	hItem = _CACHE.GetCategory(nCatID);	
	LPTREEDATA p = GetTreeData(hItem);
	if (p == NULL)
		return FALSE;

	int nRoleID = p->nRoleID;
	RemoveBusyNode(hItem);

	HTREEITEM hAfter = bSort ? GetInsertPos(hItem, sName, sExt) : TVI_LAST;	
	CTreeCtrl& tc = GetTreeCtrl();
	int nIconID = m_il.GetFileIcon(sName);	
	hItem = tc.InsertItem(sName, nIconID, nIconID, hItem, hAfter);
	p = (LPTREEDATA)CreateData(nFileID, nCatID, nRoleID, TRUE, sName, sExt, sDesc);
	p->nVersion = nVersion;
	tc.SetItemData(hItem, (DWORD)p);
	_CACHE.AddFile(nFileID, nCatID, hItem);

	if (bSort)
	{
		tc.EnsureVisible(hItem);
		tc.SelectItem(hItem);
	}

	return TRUE;
}

void CClientTreeView::OnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	HTREEITEM hItem = pNMTreeView->itemOld.hItem;
	if (hItem == m_hCutItem)
		m_hCutItem = NULL;

	if (hItem == m_hSelection)
		m_hSelection = NULL;
	
	LPTREEDATA p = GetTreeData(hItem);
	if (p)
	{
		if (m_pCutData == p)
			m_pCutData = NULL;

		if (m_pSelData == p)
			m_pSelData = NULL;

		if (p->bIsFile)
			_CACHE.DeleteFile(p->nID);
		else
			_CACHE.DeleteCategory(p->nID);

		delete p;
	}
	*pResult = 0;
}

LPTREEDATA CClientTreeView::GetTreeData(HTREEITEM hItem) const
{
	return (hItem && hItem != TVI_ROOT) ? (LPTREEDATA)GetTreeCtrl().GetItemData(hItem) : NULL;
}

DWORD CClientTreeView::CreateData(int nID, int nParentID, int nRoleID, BOOL bIsFile, LPCTSTR lpszName, LPCTSTR lpszExt, LPCTSTR lpszTooltip, LPCTSTR lpszPrivilege)
{
	LPTREEDATA p = new TREEDATA;
	p->hBusyNode = NULL;
	p->nID = nID;
	p->nParentID = nParentID;
	p->nRoleID = nRoleID;
	p->bIsFile = bIsFile;
	p->sName = lpszName;
	p->sExt = lpszExt;
	p->sExt.MakeLower();
	p->bRequested = nID < 1;
	p->sTooltip = lpszTooltip;
	p->sPrivileges = lpszPrivilege;
	p->nVersion = 1000000;
	return (DWORD)p;
}

void CClientTreeView::SetFileView(CClientListView *pFileView)
{
	ASSERT(pFileView);
	m_pFileView = pFileView;
}

int CClientTreeView::GetItemFileID(HTREEITEM hItem) const
{
	LPTREEDATA p = GetTreeData(hItem);
	return (p && p->bIsFile) ? p->nID : 0;
}

void CClientTreeView::AddBusyNode(HTREEITEM hItem)
{	
	LPTREEDATA p = GetTreeData(hItem);
	if (!p)
	{
		ASSERT(FALSE);
		return;
	}	

	CTreeCtrl& tc = GetTreeCtrl();	
	HTREEITEM hBusy = tc.InsertItem(WAITING_LABEL, TREE_ICON_BUSY, TREE_ICON_BUSY, hItem);
	p->hBusyNode = hBusy;
	tc.SetItemData(hBusy, CreateData(0, 0, 0, TRUE, NULL, _T("_"), WAITING_TOOLTIP));	
}

void CClientTreeView::RemoveBusyNode(HTREEITEM hItem)
{
	CTreeCtrl& tc = GetTreeCtrl();
	LPTREEDATA p = GetTreeData(hItem);
	if (p && p->hBusyNode)
	{
		tc.DeleteItem(p->hBusyNode);
		p->hBusyNode = NULL;
	}	
}

LPCTSTR CClientTreeView::OnTooltipRequest(HTREEITEM hItem)
{	
	LPTREEDATA p = GetTreeData(hItem);
	return p ? p->sTooltip : _T("");
}

void CClientTreeView::OnLogin()
{
	m_bLockCut = FALSE;
	CTreeCtrl& tc = GetTreeCtrl();
	tc.SetItemImage(m_hRoot, TREE_ICON_SERVER_CONNECT, TREE_ICON_SERVER_CONNECT);
	tc.Expand(m_hRoot, TVE_COLLAPSE | TVE_COLLAPSERESET);
	AddBusyNode(m_hRoot);
	_SESSION.TaskListCategory(0);
	tc.Expand(m_hRoot, TVE_EXPAND);
}

void CClientTreeView::OnLogout()
{
	CTreeCtrl& tc = GetTreeCtrl();
	tc.DeleteAllItems();
	AddRoot();	
}

void CClientTreeView::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CPoint point(GetMessagePos()); 
	UINT nFlags = 0; 
	CTreeCtrl& tc = GetTreeCtrl();
    tc.ScreenToClient(&point); 

	HTREEITEM hItem = tc.HitTest(point, &nFlags); 
    if ((nFlags & TVHT_ONITEM) && (hItem != NULL))
    {
		tc.SetFocus(); 
		tc.SelectItem(hItem);
		int nSubmenu = -1;

		switch (m_nSelStatus)
		{
		case SEL_ROLE_CAT:
			nSubmenu = 1;
			break;

		case SEL_CAT:
			nSubmenu = 2;
			break;

		case SEL_FILE:
			nSubmenu = 3;
			break;

		default:
			if (hItem == m_hRoot)
				nSubmenu = 0;
			break;
		}

		if (nSubmenu >= 0)
		{
			CMenu* pMenu = m_pMenu->GetSubMenu(nSubmenu);
			PopupMenu(pMenu, AfxGetMainWnd());
		}
	} 

	*pResult = 0;	
}

int CClientTreeView::GetSelectionStatus() const
{
	return m_nSelStatus;
}

CString CClientTreeView::GetSelectionText() const
{
	if (m_hSelection == NULL)
		return _T("");
	return GetTreeCtrl().GetItemText(m_hSelection);
}

BOOL CClientTreeView::IsSelectionCat(BOOL bAllowRole) const
{
	if (bAllowRole)
		return m_nSelStatus == SEL_ROLE_CAT || m_nSelStatus == SEL_CAT;
	return m_nSelStatus == SEL_CAT;
}

BOOL CClientTreeView::IsSelectionFile() const
{
	return m_nSelStatus == SEL_FILE;
}

int CClientTreeView::GetSelectionID() const
{
	return m_nSelID;
}

BOOL CClientTreeView::DeleteFile(int nFileID)
{
	HTREEITEM hItem = _CACHE.GetFile(nFileID);
	if (hItem)
	{
		//_CACHE.DeleteFile(nFileID);
		GetTreeCtrl().DeleteItem(hItem);
	}
	return hItem != NULL;	
}

LPTREEDATA CClientTreeView::GetSelectionData() const
{
	return GetTreeData(m_hSelection);
}

BOOL CClientTreeView::AddCategory(CLBPacket *pPacket, BOOL bSort)
{
	int nCatID = pPacket->ReadInt();
	if (nCatID < 1)
		return FALSE;

	if (bSort)
		int kk = 0;

	int nParentCatID = pPacket->ReadInt();
	HTREEITEM hParentItem = NULL;
	if (nParentCatID > 0)
		hParentItem = _CACHE.GetCategory(nParentCatID);
	if (hParentItem == NULL)
		hParentItem = m_hRoot;
	RemoveBusyNode(hParentItem);

	int nRoleID = pPacket->ReadInt();
	CString sName = ReadPacketString(pPacket);
	CString	sDesc = ReadPacketString(pPacket);
	CString sPrivileges = ReadPacketString(pPacket);

	HTREEITEM hAfter = bSort ? GetInsertPos(hParentItem, sName) : TVI_LAST;
	CTreeCtrl& tc = GetTreeCtrl();	
	HTREEITEM hItem = tc.InsertItem(sName, TREE_ICON_FOLDER_CLOSE, TREE_ICON_FOLDER_CLOSE, hParentItem, hAfter);
	tc.SetItemData(hItem, CreateData(nCatID, nParentCatID, nRoleID, FALSE, sName, NULL, sDesc, sPrivileges));
	AddBusyNode(hItem);
	_CACHE.AddCategory(nCatID, nParentCatID, hItem);

	if (bSort)
	{
		tc.EnsureVisible(hItem);
		tc.SelectItem(hItem);
	}
	return TRUE;
}

BOOL CClientTreeView::UpdateCategory(CLBPacket *pPacket)
{
	HTREEITEM hItem = _CACHE.GetCategory(pPacket->ReadInt());
	LPTREEDATA p = GetTreeData(hItem);
	if (!p)
		return FALSE;

	pPacket->ReadInt();
	pPacket->ReadInt();
	GetTreeCtrl().SetItemText(hItem, ReadPacketString(pPacket));
	p->sTooltip = ReadPacketString(pPacket);
	p->sPrivileges = ReadPacketString(pPacket);
	return TRUE;
}

BOOL CClientTreeView::AddOrUpdateCategory(CLBPacket *pPacket, BOOL bSort)
{
	if (UpdateCategory(pPacket))
		return TRUE;

	pPacket->ResetCursor();
	pPacket->ReadDword();
	return AddCategory(pPacket, bSort);
}

BOOL CClientTreeView::DeleteCategory(int nCatID)
{
	HTREEITEM hItem = _CACHE.GetCategory(nCatID);
	if (hItem)
	{
		//_CACHE.DeleteCategory(nCatID);
		GetTreeCtrl().DeleteItem(hItem);
	}
	return hItem != NULL;
}

int CClientTreeView::GetSelectionRoleID() const
{
	return m_nSelRoleID;
}

CString CClientTreeView::GetTreeItemText(HTREEITEM hItem) const
{
	return GetTreeCtrl().GetItemText(hItem);
}

BOOL CClientTreeView::OnListError(int nCatID)
{
	HTREEITEM hItem = _CACHE.GetCategory(nCatID);
	if (hItem)
	{
		RemoveBusyNode(hItem);
		return TRUE;
	}
	return FALSE;
}

void CClientTreeView::AddRoot()
{
	CTreeCtrl& tc = GetTreeCtrl();
	m_hRoot = tc.InsertItem(APP_TITLE, TREE_ICON_SERVER_DISCONNECT, TREE_ICON_SERVER_DISCONNECT);
	LPTREEDATA pData = (LPTREEDATA)CreateData(0, 0, 0, FALSE, APP_TITLE, NULL, CString(APP_TITLE) + _T("服务器"));
	pData->bRequested = TRUE;
	tc.SetItemData(m_hRoot, (DWORD)pData);	
}

BOOL CClientTreeView::VerifySelectionCategoryRole(int nPrivilege, BOOL bAllowRoot) const
{
	return IsSelectionCat(bAllowRoot) && _SESSION.VerifyPrivilege(nPrivilege, GetSelectionRoleID());
}

BOOL CClientTreeView::VerifySelectionFileRole(int nPrivilege) const
{
	return IsSelectionFile() && _SESSION.VerifyPrivilege(nPrivilege, GetSelectionRoleID());
}

HTREEITEM CClientTreeView::GetSelectionParent() const
{
	CTreeCtrl& tc = GetTreeCtrl();
	HTREEITEM hItem = tc.GetSelectedItem();
	if (hItem)
	{
		hItem = tc.GetParentItem(hItem);
		if (hItem == TVI_ROOT)
			hItem = NULL;
	}
	return hItem;
}

BOOL CClientTreeView::SelectionHasChildren() const
{
	CTreeCtrl& tc = GetTreeCtrl();
	HTREEITEM hItem = tc.GetSelectedItem();
	return hItem ? tc.ItemHasChildren(hItem) : FALSE;	
}

void CClientTreeView::SetMenu(CMenu *pMenu)
{
	m_pMenu = pMenu;
}

BOOL CClientTreeView::CanCut() const
{
	return (!m_bLockCut && m_nSelStatus == SEL_CAT || m_nSelStatus == SEL_FILE) && _SESSION.VerifyPrivilege(JDPRIV_SUPERVISOR, m_nSelRoleID);
}

BOOL CClientTreeView::CanPaste() const
{
	if (!(m_hCutItem && m_hSelection && m_nSelRoleID && m_pCutData && m_pCutData != m_pSelData && m_pCutData->nRoleID == m_nSelRoleID && IsSelectionCat(TRUE)))
		return FALSE;

	if (m_pCutData->bIsFile && m_pSelData->nParentID == 0)
		return FALSE;	

	if (!_SESSION.VerifyPrivilege(JDPRIV_SUPERVISOR, m_nSelRoleID))
		return FALSE;

	CTreeCtrl& tc = GetTreeCtrl();
	return tc.GetParentItem(m_hCutItem) != m_hSelection && !IsParentOf(m_hCutItem, m_hSelection);
}

void CClientTreeView::RemoveCut()
{
	if (m_hCutItem)
	{
		CTreeCtrl& tc = GetTreeCtrl();
		tc.SetItem(m_hCutItem, TVIF_STATE, NULL, 0, 0, 0, TVIS_CUT | TVIS_BOLD, 0);
		m_hCutItem = NULL;
		m_pCutData = NULL;
	}	
	LockCut(FALSE);
}

void CClientTreeView::Cut()
{	
	if (!CanCut())
		return;

	CTreeCtrl& tc = GetTreeCtrl();
	HTREEITEM hItem = tc.GetSelectedItem();
	if (hItem == NULL || hItem == m_hCutItem)
		return;

	if (m_hCutItem)
		tc.SetItem(m_hCutItem, TVIF_STATE, NULL, 0, 0, 0, TVIS_CUT | TVIS_BOLD, 0);

	m_pCutData = m_pSelData;
	m_hCutItem = hItem;
	ASSERT(m_pCutData);
	tc.SetItem(hItem, TVIF_STATE, NULL, 0, 0, TVIS_CUT | TVIS_BOLD, TVIS_CUT | TVIS_BOLD, 0);
}

void CClientTreeView::Paste(CLBPacket* pPacket)
{	
	CTreeCtrl& tc = GetTreeCtrl();
	RemoveCut();
	
	int nID = pPacket->ReadInt();
	int nParentCatID = pPacket->ReadInt();
	HTREEITEM hParent = _CACHE.GetCategory(nParentCatID);
	if (hParent == NULL)
		return;

	HTREEITEM hItem = NULL;
	if (pPacket->GetPacketID() == JD_PACKET_PASTE_FILE)
	{
		hItem = _CACHE.GetFile(nID);
		if (hItem)
		{
			_CACHE.DeleteFile(nID);
			_SESSION.TaskRequestFile(nID);
		}		
	}
	else
	{
		hItem = _CACHE.GetCategory(nID);
		if (hItem)
		{
			_CACHE.DeleteCategory(nID);
			_SESSION.TaskRequestCategory(nID);
		}
	}

	if (hItem)
		tc.DeleteItem(hItem);	
}

BOOL CClientTreeView::IsParentOf(HTREEITEM h1, HTREEITEM h2) const
{
	CTreeCtrl& tc = GetTreeCtrl();
	HTREEITEM hItem = tc.GetParentItem(h2);
	while (hItem && hItem != m_hRoot && hItem != TVI_ROOT)
	{
		if (hItem == h1)
			return TRUE;

		hItem = tc.GetParentItem(hItem);
	}
	return FALSE;
}

LPTREEDATA CClientTreeView::GetCutData() const
{
	return m_pCutData;
}

CString CClientTreeView::GetCutText() const
{
	return GetTreeCtrl().GetItemText(m_hCutItem);
}

void CClientTreeView::LockCut(BOOL bLock)
{
	m_bLockCut = bLock;
}

void CClientTreeView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == VK_ESCAPE)
	{
		if (m_pCutData)
			RemoveCut();
		else
			GetTreeCtrl().SelectItem(NULL);
	}	
	else
	{
		CTooltipTreeView::OnKeyUp(nChar, nRepCnt, nFlags);
	}	
}

LPTREEDATA CClientTreeView::CanRefresh() const
{
	if (!_SESSION.IsLoggedin())
		return FALSE;
	
	LPTREEDATA pData = GetTreeData(m_hSelection);
	if (pData == NULL)
		return NULL;

	if (pData->bIsFile && pData->nID == 0)
		return NULL; // Busy node

	return pData;
}

void CClientTreeView::Refresh()
{
	LPTREEDATA pData = CanRefresh();
	if (pData == NULL)
		return;

	pData->bRequested = TRUE;
	if (pData->bIsFile)
	{
		// Refresh a file
		_CACHE.DeleteVersions(pData->nID);
		m_pFileView->SetFileID(0);
		m_pFileView->SetFileID(pData->nID);
	}
	else
	{
		// Refresh a branch		
		CTreeCtrl& tc = GetTreeCtrl();
		tc.Expand(m_hSelection, TVE_COLLAPSE | TVE_COLLAPSERESET);
		AddBusyNode(m_hSelection);	
		tc.Expand(m_hSelection, TVE_EXPAND);

		// Request Data from server...
		_SESSION.TaskListCategory(pData->nID);

		if (pData->nParentID > 0)
			_SESSION.TaskListFile(pData->nID);
		
	}
}

HTREEITEM CClientTreeView::GetInsertPos(HTREEITEM hParent, LPCTSTR lpszName, LPCTSTR lpszExt) const
{
	CTreeCtrl& tc = GetTreeCtrl();	
	HTREEITEM hItem = tc.GetChildItem(hParent);	
	LPCTREEDATA p = GetTreeData(hItem);
	HTREEITEM hAfter = TVI_FIRST;
	
	while (p)
	{
		if (p->nID > 0 && !(lpszExt && !p->bIsFile))
		{
			if (lpszExt == NULL && p->bIsFile)
				break;

			if (lpszExt)
			{
				// File comparison
				int nResult = p->sExt.CompareNoCase(lpszExt);
				if (nResult > 0)
					break;

				if (nResult == 0)
				{
					// ext equal, compare name
					nResult = p->sName.CompareNoCase(lpszName);
					if (nResult > 0)
						break;
				}
			}
			else
			{
				// Folder copmparison
				int nResult = p->sName.CompareNoCase(lpszName);
				if (nResult > 0)
					break;
			}			
		}		
		
		hAfter = hItem;
		hItem = tc.GetNextSiblingItem(hItem);
		p = GetTreeData(hItem);	
	}

	return hAfter;
}

BOOL CClientTreeView::ChangeCategoryPriv(CLBPacket *pPacket)
{
	int nCatID = pPacket->ReadInt();
	HTREEITEM hItem = _CACHE.GetCategory(nCatID);
	LPTREEDATA pData = GetTreeData(hItem);
	if (pData)
	{
		pData->sPrivileges = ReadPacketString(pPacket);
		return TRUE;
	}
	return FALSE;
}
