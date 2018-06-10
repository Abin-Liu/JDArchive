#if !defined(AFX_USERDLG_H__C9D9FBC5_EC1D_47AC_BABF_3613347138F6__INCLUDED_)
#define AFX_USERDLG_H__C9D9FBC5_EC1D_47AC_BABF_3613347138F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUserDlg dialog

class CUserDlg : public CNewDialog
{
// Construction
public:
	CUserDlg(int nUserID = 0, CWnd* pParent = NULL);   // standard constructor
	int m_nUserID;
	int m_nRoleID;

// Dialog Data
	//{{AFX_DATA(CUserDlg)
	enum { IDD = IDD_USERDLG };
	CEdit	m_wndName;
	CComboBox	m_wndPrivileges;
	CComboBox	m_wndRoles;
	CString	m_sName;
	int		m_nRoleIndex;
	int		m_nPrivileges;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUserDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	static BOOL CALLBACK EnumRoleProc(int nRoleID, LPCTSTR lpszRoleName, LPVOID lParam);	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERDLG_H__C9D9FBC5_EC1D_47AC_BABF_3613347138F6__INCLUDED_)
