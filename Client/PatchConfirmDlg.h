#if !defined(AFX_PATCHCONFIRMDLG_H__880F550D_0217_4E04_A8C0_CD755B3BD5C9__INCLUDED_)
#define AFX_PATCHCONFIRMDLG_H__880F550D_0217_4E04_A8C0_CD755B3BD5C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PatchConfirmDlg.h : header file
//
#include "..\\LBNetwork.h"

/////////////////////////////////////////////////////////////////////////////
// CPatchConfirmDlg dialog

class CPatchConfirmDlg : public CNewDialog
{
// Construction
public:
	CPatchConfirmDlg(CLBPacket* pPacket, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPatchConfirmDlg)
	enum { IDD = IDD_UPDATEDLG };
	CString	m_sAuthor;
	CString	m_sDate;
	CString	m_sFileName;
	CString	m_sVer;
	CString	m_sSize;
	//}}AFX_DATA

	int m_nPatchID;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPatchConfirmDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPatchConfirmDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PATCHCONFIRMDLG_H__880F550D_0217_4E04_A8C0_CD755B3BD5C9__INCLUDED_)
