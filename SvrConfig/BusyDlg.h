#if !defined(AFX_BUSYDLG_H__EDA8B4E0_62CD_4E46_9F2F_7A91272E5CC4__INCLUDED_)
#define AFX_BUSYDLG_H__EDA8B4E0_62CD_4E46_9F2F_7A91272E5CC4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BusyDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBusyDlg dialog

class CBusyDlg : public CDialog
{
// Construction
public:
	CBusyDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBusyDlg)
	enum { IDD = IDD_BUSYDLG };
	CStatic	m_wndText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBusyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	static BOOL ProcService(CStatic& wnd);
	static DWORD WINAPI ThreadProc(LPVOID lpParameter);

	// Generated message map functions
	//{{AFX_MSG(CBusyDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BUSYDLG_H__EDA8B4E0_62CD_4E46_9F2F_7A91272E5CC4__INCLUDED_)
