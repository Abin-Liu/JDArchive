#if !defined(AFX_LBLISTCTRL_H__D5D1F718_72D3_4CDF_874A_B2FC8A5F7E7F__INCLUDED_)
#define AFX_LBLISTCTRL_H__D5D1F718_72D3_4CDF_874A_B2FC8A5F7E7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LBListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLBListCtrl window

class CLBListCtrl : public CListCtrl
{
// Construction
public:
	CLBListCtrl();
	virtual ~CLBListCtrl();

// Attributes
public:
	int GetSelection() const;
	DWORD GetSelectionData(DWORD dwDefault = 0) const;
	void InitializeStyle();

// Operations
public:
	int FindItem(DWORD dwItemData) const;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLBListCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	
	//{{AFX_MSG(CLBListCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:

	CImageList m_il;
	BOOL m_bInitialized;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LBLISTCTRL_H__D5D1F718_72D3_4CDF_874A_B2FC8A5F7E7F__INCLUDED_)
