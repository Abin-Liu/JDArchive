#if !defined(AFX_CLIENTTREEVIEW_H__F6B3D46D_A5C6_4176_9046_EBDFE2C0521B__INCLUDED_)
#define AFX_CLIENTTREEVIEW_H__F6B3D46D_A5C6_4176_9046_EBDFE2C0521B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClientTreeView.h : header file
//

#include "TooltipTreeView.h"
#include "ClientListView.h"
#include "JDImgeList.h"

typedef struct _tagTreeData
{
	int nID;
	int nRoleID;
	int nParentID;
	BOOL bIsFile;
	CString sName;
	CString sExt;
	BOOL bRequested;
	int nVersion;
	HTREEITEM hBusyNode;
	CString sTooltip;
	CString sPrivileges;
} TREEDATA, *LPTREEDATA;
typedef const TREEDATA* LPCTREEDATA;

typedef HTREEITEM (CALLBACK *fnTreeVisitProc)(CTreeCtrl& tc, HTREEITEM hItem, LPARAM lParam);

/////////////////////////////////////////////////////////////////////////////
// CClientTreeView view

class CClientTreeView : public CTooltipTreeView
{
protected:
	CClientTreeView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CClientTreeView)

// Attributes
public:
	enum { SEL_NONE = 0, SEL_ROLE_CAT, SEL_CAT, SEL_FILE };

// Operations
public:
	BOOL ChangeCategoryPriv(CLBPacket* pPacket);
	void Refresh();
	LPTREEDATA CanRefresh() const;
	void RemoveCut();
	void LockCut(BOOL bLock);
	CString GetCutText() const;
	LPTREEDATA GetCutData() const;
	BOOL IsParentOf(HTREEITEM h1, HTREEITEM h2) const;
	void Paste(CLBPacket* pPacket);
	void Cut();
	BOOL CanPaste() const;
	BOOL CanCut() const;
	void SetMenu(CMenu* pMenu);
	BOOL SelectionHasChildren() const;
	HTREEITEM GetSelectionParent() const;

	BOOL VerifySelectionFileRole(int nPrivilege) const;
	BOOL VerifySelectionCategoryRole(int nPrivilege, BOOL bAllowRoot) const;
	BOOL OnListError(int nCatID);
	CString GetTreeItemText(HTREEITEM hItem) const;
	int GetSelectionRoleID() const;
	BOOL DeleteCategory(int nCatID);
	BOOL AddOrUpdateCategory(CLBPacket* pPacket, BOOL bSort = FALSE);
	BOOL AddCategory(CLBPacket *pPacket, BOOL bSort = FALSE);
	LPTREEDATA GetSelectionData() const;
	BOOL DeleteFile(int nFileID);
	int GetSelectionID() const;
	BOOL IsSelectionFile() const;
	BOOL IsSelectionCat(BOOL bAllowRole) const;
	CString GetSelectionText() const;
	int GetSelectionStatus() const;
	void OnLogout();
	void OnLogin();
	void SetFileView(CClientListView* pFileView);
	BOOL AddOrUpdateFile(CLBPacket* pPacket, BOOL bSort = FALSE);
	BOOL UpdateCategory(CLBPacket *pPacket);
	LPTREEDATA GetTreeData(HTREEITEM hItem) const;
	void AddRoot();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientTreeView)
	protected:
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CClientTreeView();
#ifdef _DEBUG
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CClientTreeView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	HTREEITEM GetInsertPos(HTREEITEM hParent, LPCTSTR lpszName, LPCTSTR lpszExt = NULL) const;
	void RemoveBusyNode(HTREEITEM hItem);
	void AddBusyNode(HTREEITEM hItem);
	int GetItemFileID(HTREEITEM hItem) const;
	virtual LPCTSTR OnTooltipRequest(HTREEITEM hItem);
	static DWORD CreateData(int nID, int nParentID, int nRoleID, BOOL bIsFile, LPCTSTR lpszName, LPCTSTR lpszExt, LPCTSTR lpszTooltip, LPCTSTR lpszPrivilege = NULL);

	CMenu* m_pMenu;
	HTREEITEM m_hRoot;
	CClientListView* m_pFileView;
	CJDImgeList m_il;
	HTREEITEM m_hSelection;
	int m_nSelStatus;
	int m_nSelID;
	int m_nSelRoleID;
	int m_nCx;
	LPTREEDATA m_pCutData;
	HTREEITEM m_hCutItem;
	LPTREEDATA m_pSelData;
	BOOL m_bLockCut;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTTREEVIEW_H__F6B3D46D_A5C6_4176_9046_EBDFE2C0521B__INCLUDED_)
