#if !defined(AFX_RECOVERLISTDLG_H__7D1E6522_FEEF_4CEC_B94A_0035D07A5756__INCLUDED_)
#define AFX_RECOVERLISTDLG_H__7D1E6522_FEEF_4CEC_B94A_0035D07A5756__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecoverListDlg.h : header file
//

#include "LBListCtrl.h"
#include <afxtempl.h>

struct RECOVERDATA
{
	int nVerID;
	TCHAR szFileName[65];
	DWORD dwSize;
	int nVersion;
	int nCreatorID;
	COleDateTime createDate;
	int nDeleterID;
	COleDateTime deleteDate;
	TCHAR szDesc[80];
};

/////////////////////////////////////////////////////////////////////////////
// CRecoverListDlg dialog

class CRecoverListDlg : public CNewDialog
{
// Construction
public:
	CRecoverListDlg(CWnd* pParent = NULL);   // standard constructor
	CArray<RECOVERDATA, const RECOVERDATA&> m_aData;
	int m_nVerID;

// Dialog Data
	//{{AFX_DATA(CRecoverListDlg)
	enum { IDD = IDD_RECOVERLISTDLG };
	CLBListCtrl	m_wndList;
	CButton	m_wndOK;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecoverListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRecoverListDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECOVERLISTDLG_H__7D1E6522_FEEF_4CEC_B94A_0035D07A5756__INCLUDED_)
