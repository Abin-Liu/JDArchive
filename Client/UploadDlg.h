#if !defined(AFX_UPLOADDLG_H__1D5C098E_B952_4E77_AB8E_627747820B82__INCLUDED_)
#define AFX_UPLOADDLG_H__1D5C098E_B952_4E77_AB8E_627747820B82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UploadDlg.h : header file
//

#include "BrowseCtrl.h"
#include "EditEx.h"

/////////////////////////////////////////////////////////////////////////////
// CUploadDlg dialog

class CUploadDlg : public CNewDialog
{
// Construction
public:
	CUploadDlg(LPCTSTR lpszFileName, int nVersion, LPCTSTR lpszFilePath, CWnd* pParent = NULL);   // standard constructor
	CString m_sFilePath;
// Dialog Data
	//{{AFX_DATA(CUploadDlg)
	enum { IDD = IDD_UPLOADDLG };
	CEditEx	m_wndVerDesc;
	CBrowseCtrl	m_wndFilePath;
	CString	m_sVerDesc;
	CString	m_sFileName;
	CString	m_sVersion;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUploadDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUploadDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UPLOADDLG_H__1D5C098E_B952_4E77_AB8E_627747820B82__INCLUDED_)
