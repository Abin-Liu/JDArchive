#if !defined(AFX_PROGRESSDLG_H__81CE2F20_B520_4A58_82D5_966B5C1F05BD__INCLUDED_)
#define AFX_PROGRESSDLG_H__81CE2F20_B520_4A58_82D5_966B5C1F05BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProgressDlg.h : header file
//

#include "TextProgressCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CProgressDlg dialog

class CProgressDlg : public CNewDialog
{
// Construction
public:		
	
	CProgressDlg(CWnd* pParent = NULL);   // standard constructor

	void SetTitle(LPCTSTR lpszTitle);
	void SetText(LPCTSTR lpszText);	
	void Show(LPCTSTR lpszTitle, LPCTSTR lpszText);
	BOOL SetProgress(int nCompleted, int nTotal);
	void Hide();
	BOOL Cancelled() const;
	void PeekAndPump(BOOL bCancelOnESCkey = TRUE);

// Dialog Data
	//{{AFX_DATA(CProgressDlg)
	enum { IDD = IDD_PROGDLG };
	CButton	m_wndCancel;
	CEdit	m_wndText;
	CTextProgressCtrl	m_wndProgress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgressDlg)
	public:
	virtual BOOL Create(CWnd* pParentWnd);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProgressDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:	

	BOOL m_bCancelled;
	CWnd* m_pParent;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGRESSDLG_H__81CE2F20_B520_4A58_82D5_966B5C1F05BD__INCLUDED_)
