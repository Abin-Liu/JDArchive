#if !defined(AFX_PATCHUPLOADDLG_H__62FDBEF1_0F45_4A0D_AF92_EF9AEF480BA1__INCLUDED_)
#define AFX_PATCHUPLOADDLG_H__62FDBEF1_0F45_4A0D_AF92_EF9AEF480BA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PatchUploadDlg.h : header file
//

#include "BrowseCtrl.h"
#include "EditEx.h"

/////////////////////////////////////////////////////////////////////////////
// CPatchUploadDlg dialog

class CPatchUploadDlg : public CNewDialog
{
// Construction
public:
	CPatchUploadDlg(CWnd* pParent = NULL);   // standard constructor
	CString m_sFileName;
	CString m_sFilePath;

// Dialog Data
	//{{AFX_DATA(CPatchUploadDlg)
	enum { IDD = IDD_PATCHDLG };
	CBrowseCtrl	m_wndPath;
	CString	m_sVersion;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPatchUploadDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPatchUploadDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PATCHUPLOADDLG_H__62FDBEF1_0F45_4A0D_AF92_EF9AEF480BA1__INCLUDED_)
