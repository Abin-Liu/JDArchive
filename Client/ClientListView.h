#if !defined(AFX_CLIENTLISTVIEW_H__448A821D_2F42_4F26_8F09_CFBE8BB1D377__INCLUDED_)
#define AFX_CLIENTLISTVIEW_H__448A821D_2F42_4F26_8F09_CFBE8BB1D377__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClientListView.h : header file
//
#include <afxcview.h>

/////////////////////////////////////////////////////////////////////////////
// CClientListView view

class CClientListView : public CListView
{
protected:
	CClientListView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CClientListView)

// Attributes
public:

// Operations
public:
	void SetMenu(CMenu* pMenu);
	void UncheckVersion(CLBPacket* pPacket);
	void CheckVersion(CLBPacket* pPacket);
	int GetSelectedVerCheckState() const;
	void DeleteVer(int nVerID);
	void AddVersion(CLBPacket* pPacket);
	void OnLogout();
	int GetSelectedVerID() const;
	int GetFileID() const;
	void SetFileID(int nFileID);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientListView)
	protected:
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CClientListView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CClientListView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	static BOOL CALLBACK EnumVersionProc(int nVersionID, LPCVERDATA pVersionData, LPVOID lParam);
	CImageList m_il;
	CMenu* m_pMenu;
	int m_nFileID;
	int m_nSelectedVerID;
	int m_nSelection;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTLISTVIEW_H__448A821D_2F42_4F26_8F09_CFBE8BB1D377__INCLUDED_)
