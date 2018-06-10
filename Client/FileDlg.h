#if !defined(AFX_FILEDLG_H__9805EA9F_7BFC_4BA4_BC36_0097D4665FC1__INCLUDED_)
#define AFX_FILEDLG_H__9805EA9F_7BFC_4BA4_BC36_0097D4665FC1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFileDlg dialog

#include "BrowseCtrl.h"
#include "EditEx.h"

class CFileDlg : public CNewDialog
{
// Construction
public:
	CFileDlg(LPCTSTR lpszFileName = NULL, LPCTSTR lpszDesc = NULL, CWnd* pParent = NULL);   // standard constructor
	CString m_sFilePath;
// Dialog Data
	//{{AFX_DATA(CFileDlg)
	enum { IDD = IDD_FILEDLG };
	CEdit	m_wndFileName;
	CEditEx	m_wndDesc;
	CBrowseCtrl	m_wndBrowse;
	CString	m_sFileName;
	CString	m_sDesc;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFileDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEDLG_H__9805EA9F_7BFC_4BA4_BC36_0097D4665FC1__INCLUDED_)
