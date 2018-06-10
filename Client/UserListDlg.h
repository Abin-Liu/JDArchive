#if !defined(AFX_USERLISTDLG_H__64F61ED7_60A3_4978_8A65_35FC9D04BFC7__INCLUDED_)
#define AFX_USERLISTDLG_H__64F61ED7_60A3_4978_8A65_35FC9D04BFC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserListDlg.h : header file
//

#include <afxtempl.h>
#include "LBListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CUserListDlg dialog

class CUserListDlg : public CNewDialog
{
// Construction
public:
	CUserListDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUserListDlg)
	enum { IDD = IDD_USERLISTDLG };
	CButton	m_wndResetPwd;
	CButton	m_wndEdit;
	CLBListCtrl	m_wndList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	LRESULT OnDataNotify(WPARAM wParam, LPARAM lParam);
	static BOOL CALLBACK EnumUserProc(int nUserID, LPCUSERDATA pUserData, LPVOID lParam);

	// Generated message map functions
	//{{AFX_MSG(CUserListDlg)
	afx_msg void OnAdd();
	afx_msg void OnEdit();
	afx_msg void OnResetpwd();
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERLISTDLG_H__64F61ED7_60A3_4978_8A65_35FC9D04BFC7__INCLUDED_)
