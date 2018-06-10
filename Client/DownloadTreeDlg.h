#if !defined(AFX_DOWNLOADTREEDLG_H__F9DD85A2_E1AE_40C3_AA81_590F35C10A85__INCLUDED_)
#define AFX_DOWNLOADTREEDLG_H__F9DD85A2_E1AE_40C3_AA81_590F35C10A85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DownloadTreeDlg.h : header file
//

#include "BrowseCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CDownloadTreeDlg dialog

class CDownloadTreeDlg : public CDialog
{
// Construction
public:
	CDownloadTreeDlg(CWnd* pParent = NULL);   // standard constructor
	CString m_sPathName;

// Dialog Data
	//{{AFX_DATA(CDownloadTreeDlg)
	enum { IDD = IDD_DOWNLOADTREEDLG };
	CBrowseCtrl	m_wndPath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDownloadTreeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDownloadTreeDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOWNLOADTREEDLG_H__F9DD85A2_E1AE_40C3_AA81_590F35C10A85__INCLUDED_)
