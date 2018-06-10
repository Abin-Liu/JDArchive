#if !defined(AFX_LOGINDLG_H__3FD3B630_4066_43D2_929F_74907339B346__INCLUDED_)
#define AFX_LOGINDLG_H__3FD3B630_4066_43D2_929F_74907339B346__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LoginDlg.h : header file
//
#include "EditEx.h"
/////////////////////////////////////////////////////////////////////////////
// CLoginDlg dialog

class CLoginDlg : public CNewDialog
{
// Construction
public:
	CString GetPassword() const;
	CString GetUserName() const;
	CLoginDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLoginDlg)
	enum { IDD = IDD_LOGINDLG };
	CEditEx	m_wndUserName;
	CEditEx	m_wndPassword;
	CString	m_sUserName;
	CString	m_sPassword;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoginDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLoginDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	
	BOOL m_bIsConfirm;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGINDLG_H__3FD3B630_4066_43D2_929F_74907339B346__INCLUDED_)
