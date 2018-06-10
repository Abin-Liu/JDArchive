#if !defined(AFX_VERMANAGEDLG_H__57C1D4E7_F1E4_43E9_9A4E_6698FAB12F39__INCLUDED_)
#define AFX_VERMANAGEDLG_H__57C1D4E7_F1E4_43E9_9A4E_6698FAB12F39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VerManageDlg.h : header file
//

#include "EditEx.h"
/////////////////////////////////////////////////////////////////////////////
// CVerManageDlg dialog

class CVerManageDlg : public CNewDialog
{
// Construction
public:
	CVerManageDlg(LPCTSTR lpszFileName, int nVersion, CWnd* pParent = NULL);   // standard constructor
	int m_nVersion;

// Dialog Data
	//{{AFX_DATA(CVerManageDlg)
	enum { IDD = IDD_VERMANAGEDLG };
	CEditEx	m_wndBuild;
	CEditEx	m_wndMinor;
	CEditEx	m_wndMajor;
	CString	m_sCurMajor;
	CString	m_sCurMinor;
	CString	m_sFileName;
	CString	m_sMajor;
	CString	m_sMinor;
	CString	m_sBuild;
	CString	m_sCurBuild;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVerManageDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	int m_nCurMajor;
	int m_nCurMinor;
	int m_nCurBuild;
	int m_nCurVersion;


	// Generated message map functions
	//{{AFX_MSG(CVerManageDlg)
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VERMANAGEDLG_H__57C1D4E7_F1E4_43E9_9A4E_6698FAB12F39__INCLUDED_)
