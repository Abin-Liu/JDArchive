#if !defined(AFX_CHANGEPWDDLG_H__D932E37B_BB32_452E_931C_77851ECCFF73__INCLUDED_)
#define AFX_CHANGEPWDDLG_H__D932E37B_BB32_452E_931C_77851ECCFF73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChangePwdDlg.h : header file
//
#include "EditEx.h"
/////////////////////////////////////////////////////////////////////////////
// CChangePwdDlg dialog

class CChangePwdDlg : public CNewDialog
{
// Construction
public:
	CString GetNewPassword() const;
	CString GetOldPassword() const;
	CChangePwdDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CChangePwdDlg)
	enum { IDD = IDD_CHANGEPWDDLG };
	CEditEx	m_wndNewPassword2;
	CEditEx	m_wndNewPassword;
	CEditEx	m_wndOldPassword;
	CString	m_sOldPassword;
	CString	m_sNewPassword;
	CString	m_sNewPassword2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChangePwdDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChangePwdDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHANGEPWDDLG_H__D932E37B_BB32_452E_931C_77851ECCFF73__INCLUDED_)
