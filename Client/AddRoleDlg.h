#if !defined(AFX_ADDROLEDLG_H__01BEE31F_F359_46CD_A17A_016B9BD477A6__INCLUDED_)
#define AFX_ADDROLEDLG_H__01BEE31F_F359_46CD_A17A_016B9BD477A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddRoleDlg.h : header file
//

#include "EditEx.h"

/////////////////////////////////////////////////////////////////////////////
// CAddRoleDlg dialog

class CAddRoleDlg : public CNewDialog
{
// Construction
public:
	CAddRoleDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAddRoleDlg)
	enum { IDD = IDD_ADDROLE };
	CEditEx	m_wndRoleName;
	CString	m_sRoleName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddRoleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAddRoleDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDROLEDLG_H__01BEE31F_F359_46CD_A17A_016B9BD477A6__INCLUDED_)
