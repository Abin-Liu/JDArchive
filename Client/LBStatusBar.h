//////////////////////////////////////////////////////
// LBStatusBar.h
//
// CLBStatusBar, a CStatusBar derived class with ability of
// displaying one small icon on each panel, also offers methods
// to create CWnd derived controls on panels and resizes them
// automatically.
//
// Abin (abinn32@yahoo.com)
// 2011-10-16
//////////////////////////////////////////////////////

#ifndef __LBSTATUSBAR_H__
#define __LBSTATUSBAR_H__

#include <afxtempl.h>

#ifndef __CNewMenu_H_
typedef CStatusBar CNewStatusBar;
#endif

class CLBStatusBar : public CNewStatusBar
{
public:
	CLBStatusBar();
	virtual ~CLBStatusBar();

public:
	
	virtual BOOL SetIndicators(const UINT* lpIDArray, int nIDCount);
	virtual BOOL IsValidPane(int nPane) const;

	virtual BOOL SetIcon(int nPane, LPCTSTR lpszResource, UINT nSize = 16, BOOL bAutoDestroy = TRUE);
	virtual BOOL SetIcon(int nPane, UINT nResourceID, UINT nSize = 16, BOOL bAutoDestroy = TRUE);
	virtual BOOL RemoveIcon(int nPane);
	virtual BOOL ClearPane(int nPane);

	virtual BOOL SetPaneWidth(int nPane, int nWidth);
	virtual int GetPaneWidth(int nPane) const;

	virtual BOOL SetPaneCtrl(int nPane, CWnd* pWnd);
	virtual CWnd* GetPaneCtrl(int nPane) const;

protected:
	//{{AFX_MSG(CLBStatusBar)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:

	virtual void UpdatePaneCtrlPosition(int nPane) const;
	virtual void CleanUp();

	struct PANELDATA
	{
		HICON hIcon;
		BOOL bAutoDestroy;
		UINT nIndicatorID;
		UINT nIconSize;
		CWnd* pwndCtrl;
	};
	
	CArray<PANELDATA, const PANELDATA&> m_aPaneData;
};

#endif // __LBSTATUSBAR_H__
