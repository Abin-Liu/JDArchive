#if !defined(AFX_CATPRIVDLG_H__6B2AFF1F_2121_48A2_BA81_D88ABC137FFF__INCLUDED_)
#define AFX_CATPRIVDLG_H__6B2AFF1F_2121_48A2_BA81_D88ABC137FFF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CatPrivDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCatPrivDlg dialog

class CCatPrivDlg : public CNewDialog
{
// Construction
public:
	CString GetPrivileges() const;
	CCatPrivDlg(int nRoleID, LPCTSTR lpszPrivileges, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCatPrivDlg)
	enum { IDD = IDD_CATPRIVDLG };
	CListCtrl	m_wndList;
	BOOL	m_bApplySubCats;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCatPrivDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	static BOOL CALLBACK EnumRoleProc(int nRoleID, LPCTSTR lpszRoleName, LPVOID lParam);

	// Generated message map functions
	//{{AFX_MSG(CCatPrivDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:

	int m_nRoleID;
	CString m_sPrivs;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CATPRIVDLG_H__6B2AFF1F_2121_48A2_BA81_D88ABC137FFF__INCLUDED_)
