// LBListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "LBListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLBListCtrl

CLBListCtrl::CLBListCtrl()
{
	m_bInitialized = FALSE;
	m_il.Create(1, 20, ILC_COLOR24, 1, 1);
}

CLBListCtrl::~CLBListCtrl()
{
	m_il.DeleteImageList();
}


BEGIN_MESSAGE_MAP(CLBListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CLBListCtrl)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLBListCtrl message handlers

int CLBListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	InitializeStyle();
	return 0;
}

int CLBListCtrl::GetSelection() const
{
	POSITION pos = GetFirstSelectedItemPosition();
	if (pos)
		return GetNextSelectedItem(pos);
	return -1;
}

void CLBListCtrl::InitializeStyle()
{
	if (m_bInitialized)
		return;

	m_bInitialized = TRUE;
	SetExtendedStyle(GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	SetImageList(&m_il, LVSIL_SMALL);
}

void CLBListCtrl::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	CListCtrl::PreSubclassWindow();
	InitializeStyle();
}

int CLBListCtrl::FindItem(DWORD dwItemData) const
{
	for (int i = 0; i < GetItemCount(); i++)
	{
		if (GetItemData(i) == dwItemData)
			return i;
	}
	return -1;
}

DWORD CLBListCtrl::GetSelectionData(DWORD dwDefault) const
{
	int nSel = GetSelection();
	if (nSel == -1)
		return dwDefault;
	return GetItemData(nSel);
}
