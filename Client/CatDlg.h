#if !defined(AFX_CATDLG_H__D1903F44_4F6B_4A38_B60E_49CE0CB3E7B8__INCLUDED_)
#define AFX_CATDLG_H__D1903F44_4F6B_4A38_B60E_49CE0CB3E7B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CatDlg.h : header file
//

#include "EditEx.h"

/////////////////////////////////////////////////////////////////////////////
// CCatDlg dialog

class CCatDlg : public CNewDialog
{
// Construction
public:
	CCatDlg(int nRoleID, int nParentCatID, LPCTSTR lpszParentName, int nCatID = 0, LPCTSTR lpszName = NULL, LPCTSTR lpszDesc = NULL, CWnd* pParent = NULL);   // standard constructor

	int m_nCatID;
	int m_nRoleID;
	int m_nParentCatID;

// Dialog Data
	//{{AFX_DATA(CCatDlg)
	enum { IDD = IDD_CATDLG };
	CEditEx	m_wndDesc;
	CEditEx	m_wndName;
	CString	m_sName;
	CString	m_sParentName;
	CString	m_sDesc;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCatDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCatDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CATDLG_H__D1903F44_4F6B_4A38_B60E_49CE0CB3E7B8__INCLUDED_)
