#if !defined(AFX_CATINITDLG_H__6D4FCD0C_EEDF_477D_9767_82FF5CDC9D7B__INCLUDED_)
#define AFX_CATINITDLG_H__6D4FCD0C_EEDF_477D_9767_82FF5CDC9D7B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CatInitDlg.h : header file
//
#include "BrowseCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CCatInitDlg dialog

class CCatInitDlg : public CNewDialog
{
// Construction
public:
	CCatInitDlg(CWnd* pParent = NULL);   // standard constructor
	CString m_sPathName;
// Dialog Data
	//{{AFX_DATA(CCatInitDlg)
	enum { IDD = IDD_CATINITDLG };
	CBrowseCtrl	m_wndPath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCatInitDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCatInitDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CATINITDLG_H__6D4FCD0C_EEDF_477D_9767_82FF5CDC9D7B__INCLUDED_)
