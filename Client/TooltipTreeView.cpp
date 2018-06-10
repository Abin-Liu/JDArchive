/////////////////////////////////////////////////////////////////////////////
// CTooltipTreeView
//
// Implementation file
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TooltipTreeView.h"

CTooltipTreeView::CTooltipTreeView()
{
}

CTooltipTreeView::~CTooltipTreeView()
{
}

BEGIN_MESSAGE_MAP(CTooltipTreeView, CTreeView)
	//{{AFX_MSG_MAP(CTooltipTreeView)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
END_MESSAGE_MAP()

INT_PTR CTooltipTreeView::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	// TODO: Add your specialized code here and/or call the base class	
	UINT nFlags = 0;
	HTREEITEM hItem = GetTreeCtrl().HitTest(point, &nFlags);
	if ((nFlags & TVHT_ONITEM) && hItem)
	{
		RECT rect;
		GetTreeCtrl().GetItemRect(hItem, &rect, FALSE);
		pTI->hwnd = m_hWnd;
		pTI->uId = (UINT_PTR)hItem;
		pTI->lpszText = LPSTR_TEXTCALLBACK;
		pTI->rect = rect;
		return pTI->uId;
	}
	return -1;
}

BOOL CTooltipTreeView::OnToolTipText(UINT id, NMHDR *pNMHDR, LRESULT *pResult)
{
	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;

	// Do not process the message from built in tooltip 
	UINT nCode = pNMHDR->code;	
	if (pNMHDR->idFrom == (UINT)m_hWnd && ((nCode == TTN_NEEDTEXTA && pTTTA->uFlags & TTF_IDISHWND) || (nCode == TTN_NEEDTEXTW && pTTTW->uFlags & TTF_IDISHWND)))
		return FALSE;

	// Get the mouse position
	const MSG* pMessage = GetCurrentMessage(); // get mouse pos 
	if (!pMessage)
		return FALSE;
	
	CPoint pt = pMessage->pt;
	ScreenToClient(&pt);

	CTreeCtrl& tc = GetTreeCtrl();
	UINT nFlags = 0;
	HTREEITEM hItem = tc.HitTest(pt, &nFlags); // Get item pointed by mouse
	if ((nFlags & TVHT_ONITEM) == 0 || hItem == NULL)
		return FALSE;

	// Request tooltip text from derived class
	CString sTooltip(OnTooltipRequest(hItem));
	if (sTooltip.IsEmpty())
		return FALSE;
	
#ifdef UNICODE
	if (nCode == TTN_NEEDTEXTA)
		::WideCharToMultiByte(CP_ACP, 0, sTooltip, -1, pTTTA->szText, 80, NULL, NULL);
	else
		::wcsncpy(pTTTW->szText, sTooltip, 80);	
#else // UNICODE
	if (nCode == TTN_NEEDTEXTA)
		::strncpy(pTTTA->szText, sTooltip, 80);
	else
		::MultiByteToWideChar(CP_ACP, 0, sTooltip, -1, pTTTW->szText, 80);	
#endif // UNICODE

	*pResult = 0;
	return TRUE;
}
