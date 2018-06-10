/////////////////////////////////////////////////////////////////////////////
// CTooltipTreeView
//
// A CTreeView derived class that implements tooltips for every single tree
// items based on user specified conditions.
//
// Abin (abinn32@yahoo.com)
// 2011-9-12
/////////////////////////////////////////////////////////////////////////////

#ifndef __TOOLTIPTREEVIEW_H__
#define __TOOLTIPTREEVIEW_H__

#include <afxcview.h>

class CTooltipTreeView : public CTreeView
{
public:
	CTooltipTreeView();
	virtual ~CTooltipTreeView();

protected:

	// Override this member function in your CTreeView class, and that's all!
	virtual LPCTSTR OnTooltipRequest(HTREEITEM hItem) = 0;

protected:
	
	//{{AFX_MSG(CTooltipTreeView)
	afx_msg BOOL OnToolTipText(UINT id, NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTooltipTreeView)
	protected:
	//}}AFX_VIRTUAL
	virtual INT_PTR CTooltipTreeView::OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __TOOLTIPTREEVIEW_H__
