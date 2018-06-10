#if !defined(AFX_RECOVERDLG_H__2E39775A_2A82_4FD9_BC2B_B98978A8E833__INCLUDED_)
#define AFX_RECOVERDLG_H__2E39775A_2A82_4FD9_BC2B_B98978A8E833__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecoverDlg.h : header file
//

#include "EditEx.h"

/////////////////////////////////////////////////////////////////////////////
// CRecoverDlg dialog

class CRecoverDlg : public CNewDialog
{
// Construction
public:
	CRecoverDlg(CWnd* pParent = NULL);   // standard constructor
	int m_nRoleID;

// Dialog Data
	//{{AFX_DATA(CRecoverDlg)
	enum { IDD = IDD_RECOVERDLG };
	CEditEx	m_wndFileName;
	CComboBox	m_wndRoles;
	CString	m_sFileName;
	int		m_nRoleIndex;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecoverDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRecoverDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	static BOOL CALLBACK EnumRoleProc(int nRoleID, LPCTSTR lpszRoleName, LPVOID lParam);	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECOVERDLG_H__2E39775A_2A82_4FD9_BC2B_B98978A8E833__INCLUDED_)
