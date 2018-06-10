#include "stdafx.h"
#include "TextToolBar.h"

CTextToolBar::CTextToolBar()
{
}

CTextToolBar::~CTextToolBar()
{
}

BOOL CTextToolBar::LoadToolBar(UINT nIDResource)
{
	return LoadToolBar(MAKEINTRESOURCE(nIDResource));
}

BOOL CTextToolBar::LoadToolBar(LPCTSTR lpszResourceName)
{
	if (!CNewToolBar::LoadToolBar(lpszResourceName))
		return FALSE; 

	CRect rcMin(0, 0, 0, 0); 		
	CToolBarCtrl& tbCtrl = GetToolBarCtrl(); 
	int nCount = tbCtrl.GetButtonCount();
	for (int i = 0; i < nCount; i++)
	{
		UINT nID = GetItemID(i);
		if( ID_SEPARATOR == nID )
			continue;

		CString txt; 
		if(!txt.LoadString(nID) || txt.IsEmpty())
			continue;

		int iPos = txt.ReverseFind(_T('\n')); 
		if (iPos != -1)
			txt = txt.Mid(iPos + 1);
 
		SetButtonText(i, txt);			
		CRect rcBtn; 
		GetItemRect(i, &rcBtn);
 
		if( rcBtn.Width() > rcMin.Width() )
			rcMin.right = rcBtn.Width();
 
		if( rcBtn.Height() > rcMin.Height() )
			rcMin.bottom = rcBtn.Height();
	}
 
	IMAGEINFO ii = { 0 };
	CImageList* pil = tbCtrl.GetImageList(); 
	if (pil && pil->GetImageInfo(0, &ii))
		SetSizes(rcMin.Size(), CRect(ii.rcImage).Size()); 
	return TRUE;
}