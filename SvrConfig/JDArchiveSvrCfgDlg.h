// JDArchiveSvrCfgDlg.h : header file
//

#if !defined(AFX_JDARCHIVESVRCFGDLG_H__B59FBD99_58FA_4817_A640_E048189D2C16__INCLUDED_)
#define AFX_JDARCHIVESVRCFGDLG_H__B59FBD99_58FA_4817_A640_E048189D2C16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CJDArchiveSvrCfgDlg dialog

#include "EditEx.h"
#include "BrowseCtrl.h"

class CJDArchiveSvrCfgDlg : public CDialog
{
// Construction
public:
	
	CJDArchiveSvrCfgDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CJDArchiveSvrCfgDlg)
	enum { IDD = IDD_JDARCHIVESVRCFG_DIALOG };
	CButton	m_wndAutoDel;
	CEditEx	m_wndUserName;
	CBrowseCtrl	m_wndRootPath;
	CEditEx	m_wndPassword;
	CEditEx	m_wndMaxConn;
	CEditEx	m_wndInterval;
	CEditEx	m_wndHour;
	CEditEx	m_wndHost;
	CEditEx	m_wndDelDays;
	CEditEx	m_wndDBName;
	CBrowseCtrl	m_wndBackupPath;
	BOOL	m_bAutoDel;
	CString	m_sDBName;
	int		m_nDelDays;
	CString	m_sHost;
	int		m_nHour;
	int		m_nInterval;
	int		m_nMaxConn;
	CString	m_sPassword;
	CString	m_sUserName;
	BOOL	m_bDebug;
	BOOL	m_bBackupNow;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJDArchiveSvrCfgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	BOOL ValidateInput(CString& sValue, CEditEx& edit, LPCTSTR lpszName);
	static BOOL TestFolder(LPCTSTR lpszFolder);
	HICON m_hIcon;
	CString m_sRootPath;
	CString m_sBackupPath;

	// Generated message map functions
	//{{AFX_MSG(CJDArchiveSvrCfgDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnAutodel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JDARCHIVESVRCFGDLG_H__B59FBD99_58FA_4817_A640_E048189D2C16__INCLUDED_)
