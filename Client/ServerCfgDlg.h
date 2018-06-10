#if !defined(AFX_SERVERCFGDLG_H__E4D34B7A_18E2_4456_9E58_41F9DDB70C50__INCLUDED_)
#define AFX_SERVERCFGDLG_H__E4D34B7A_18E2_4456_9E58_41F9DDB70C50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ServerCfgDlg.h : header file
//

#include "EditEx.h"

/////////////////////////////////////////////////////////////////////////////
// CServerCfgDlg dialog

class CServerCfgDlg : public CNewDialog
{
// Construction
public:
	CServerCfgDlg(LPCTSTR lpszAddress, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CServerCfgDlg)
	enum { IDD = IDD_SERVERCFGDLG };
	CEditEx	m_wndAddress;
	CString	m_sAddress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerCfgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CServerCfgDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERCFGDLG_H__E4D34B7A_18E2_4456_9E58_41F9DDB70C50__INCLUDED_)
