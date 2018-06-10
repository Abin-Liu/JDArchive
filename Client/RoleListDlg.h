#if !defined(AFX_ROLELISTDLG_H__676C7933_7B93_4F83_8C10_A05118E1FB14__INCLUDED_)
#define AFX_ROLELISTDLG_H__676C7933_7B93_4F83_8C10_A05118E1FB14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RoleListDlg.h : header file
//

#include "LBListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CRoleListDlg dialog

class CRoleListDlg : public CNewDialog
{
// Construction
public:
	CRoleListDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRoleListDlg)
	enum { IDD = IDD_ROLEDLG };
	CLBListCtrl	m_wndList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRoleListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	LRESULT OnDataNotify(WPARAM wParam, LPARAM lParam);
	static BOOL CALLBACK EnumRoleProc(int nRoleID, LPCTSTR lpszRoleName, LPVOID lParam);

	// Generated message map functions
	//{{AFX_MSG(CRoleListDlg)
	afx_msg void OnAdd();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROLELISTDLG_H__676C7933_7B93_4F83_8C10_A05118E1FB14__INCLUDED_)
