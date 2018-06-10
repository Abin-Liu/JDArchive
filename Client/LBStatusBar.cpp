// LBStatusBar.cpp : implementation file
//

#include "stdafx.h"
#include "LBStatusBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CLBStatusBar::CLBStatusBar()
{
}

CLBStatusBar::~CLBStatusBar()
{
	CleanUp();	
}


BEGIN_MESSAGE_MAP(CLBStatusBar, CNewStatusBar)
	//{{AFX_MSG_MAP(CLBStatusBar)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CLBStatusBar::SetIndicators(const UINT* lpIDArray, int nIDCount)
{
	if (!CNewStatusBar::SetIndicators(lpIDArray, nIDCount))
		return FALSE;

	CleanUp();
	m_aPaneData.SetSize(nIDCount);
	if (lpIDArray && nIDCount > 0)
	{
		for (int i = 0; i < nIDCount; i++)
		{
			PANELDATA data = { 0 };
			data.nIndicatorID = lpIDArray[i];
			m_aPaneData[i] = data;
			SetPaneWidth(i, 100);
		}	
	}

	return TRUE;
}

BOOL CLBStatusBar::IsValidPane(int nPane) const
{
	return nPane >= 0 && nPane < m_aPaneData.GetSize();
}

void CLBStatusBar::CleanUp()
{
	for (int i = 0; i < m_aPaneData.GetSize(); i++)
	{
		PANELDATA& data = m_aPaneData.ElementAt(i);
		if (data.hIcon && data.bAutoDestroy)
			::DestroyIcon(data.hIcon);
	}
	m_aPaneData.RemoveAll();
}

BOOL CLBStatusBar::SetIcon(int nPane, LPCTSTR lpszResource, UINT nSize, BOOL bAutoDestroy)
{
	if (!IsValidPane(nPane))
	{
		ASSERT(FALSE);
		return FALSE;
	}

	PANELDATA& data = m_aPaneData.ElementAt(nPane);
	HICON hIcon = data.hIcon;
	if (hIcon)
	{
		if (data.bAutoDestroy)
			::DestroyIcon(hIcon);
		data.bAutoDestroy = FALSE;
		data.hIcon = NULL;
		data.nIconSize = 0;
	}

	BOOL bSuccess = TRUE;
	hIcon = NULL;
	if (lpszResource)
	{
		hIcon = (HICON)::LoadImage(AfxGetInstanceHandle(), lpszResource, IMAGE_ICON, nSize, nSize, LR_DEFAULTCOLOR);
		if (hIcon)
		{
			data.hIcon = hIcon;
			data.bAutoDestroy = bAutoDestroy;
			data.nIconSize = nSize;
		}
		else
		{
			bSuccess = FALSE;
		}
	}
	
	GetStatusBarCtrl().SetIcon(nPane, hIcon);
	return bSuccess;
}

BOOL CLBStatusBar::SetIcon(int nPane, UINT nResourceID, UINT nSize, BOOL bAutoDestroy)
{
	return SetIcon(nPane, nResourceID ? MAKEINTRESOURCE(nResourceID) : NULL, nSize, bAutoDestroy);	
}

BOOL CLBStatusBar::RemoveIcon(int nPane)
{
	return SetIcon(nPane, (LPCTSTR)NULL);
}

void CLBStatusBar::UpdatePaneCtrlPosition(int nPane) const
{
	PANELDATA data = m_aPaneData[nPane];
	CWnd* pWnd = data.pwndCtrl;
	if (pWnd && ::IsWindow(pWnd->m_hWnd))
	{
		CRect rect(0, 0, 0, 0);
		GetItemRect(nPane, &rect);
		rect.left += data.nIconSize;
		if (rect.Width() > 0 && rect.Height() > 0)
			pWnd->MoveWindow(&rect);
	}		
}

void CLBStatusBar::OnSize(UINT nType, int cx, int cy) 
{
	CNewStatusBar::OnSize(nType, cx, cy);
	for (int i = 0; i < m_aPaneData.GetSize(); i++)
		UpdatePaneCtrlPosition(i);
}

CWnd* CLBStatusBar::GetPaneCtrl(int nPane) const
{
	if (!IsValidPane(nPane))
	{
		ASSERT(FALSE);
		return NULL;
	}

	return m_aPaneData[nPane].pwndCtrl;
}

BOOL CLBStatusBar::SetPaneCtrl(int nPane, CWnd *pWnd)
{
	if (!IsValidPane(nPane))
	{
		ASSERT(FALSE);
		return FALSE;
	}

	PANELDATA& data = m_aPaneData.ElementAt(nPane);
	if (data.pwndCtrl != pWnd)
	{
		data.pwndCtrl = pWnd;
		UpdatePaneCtrlPosition(nPane);
	}
	
	return TRUE;
}

BOOL CLBStatusBar::SetPaneWidth(int nPane, int nWidth)
{
	if (!IsValidPane(nPane))
	{
		ASSERT(FALSE);
		return FALSE;
	}

	ASSERT(nWidth > 0);

	UINT nIndicator, nStyle;
	int nOldWidth = 0;
	GetPaneInfo(nPane, nIndicator, nStyle, nOldWidth);
	if (nOldWidth != nWidth)
		SetPaneInfo(nPane, nIndicator, nStyle, nWidth);
	return TRUE;
}

int CLBStatusBar::GetPaneWidth(int nPane) const
{
	if (!IsValidPane(nPane))
	{
		ASSERT(FALSE);
		return 0;
	}

	UINT nDummy;
	int nWidth = 0;
	GetPaneInfo(nPane, nDummy, nDummy, nWidth);
	return nWidth;
}

BOOL CLBStatusBar::ClearPane(int nPane)
{
	if (!IsValidPane(nPane))
	{
		ASSERT(FALSE);
		return FALSE;
	}

	SetPaneText(nPane, NULL);
	RemoveIcon(nPane);
	return TRUE;
}
