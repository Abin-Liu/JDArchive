// JDImgeList.cpp: implementation of the CJDImgeList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JDArchive.h"
#include "JDImgeList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJDImgeList::CJDImgeList()
{
	// Place our temporary files into system temp folder	
	for (int i = 0; i < BUILTIN_ICON_NUM; i++)
		m_aInternalIcons[i] = AfxGetApp()->LoadIcon(BUILTIN_ICONS[i]);	
	m_hHtml = AfxGetApp()->LoadIcon(IDI_HTML);
	m_hTxt = AfxGetApp()->LoadIcon(IDI_TXT);
	m_hBmp = AfxGetApp()->LoadIcon(IDI_BMP);
}

CJDImgeList::~CJDImgeList()
{	
}

BOOL CJDImgeList::Create()
{
	if (!CImageList::Create(16, 16, ILC_COLOR16 | ILC_MASK, 16, 16))
		return FALSE;
	
	// Add built-in icons first
	for (int i = 0; i < BUILTIN_ICON_NUM; i++)
		RegisterIcon(m_aInternalIcons[i], NULL, FALSE);

	RegisterIcon(m_hHtml, _T(".htm"), FALSE);
	RegisterIcon(m_hHtml, _T(".html"), FALSE);
	RegisterIcon(m_hTxt, _T(".txt"), FALSE);
	RegisterIcon(m_hTxt, _T(".log"), FALSE);
	RegisterIcon(m_hBmp, _T(".bmp"), FALSE);
	RegisterIcon(m_hBmp, _T(".dib"), FALSE);
	return TRUE;
}

int CJDImgeList::RegisterIcon(HICON hIcon, LPCTSTR lpszExt, BOOL bDestroy)
{
	if (hIcon == NULL)
		return TREE_ICON_UNKNOWN;

	//m_aRegisteredIcons.SetAt(hIcon, 1);
	int nID = Add(hIcon);
	if (nID == -1)
		return TREE_ICON_UNKNOWN;

	if (lpszExt && *lpszExt)
	{
		CString sExt(lpszExt);
		sExt.MakeLower();
		m_map.SetAt(sExt, nID);
	}

	if (bDestroy)
		::DestroyIcon(hIcon);

	return nID;
		
}

int CJDImgeList::GetFileIcon(LPCTSTR lpszFileName)
{
	CString sExt = GetFileExt(lpszFileName);
	if (sExt.IsEmpty())
		return TREE_ICON_UNKNOWN;

	sExt.MakeLower();
	int nID;
	if (m_map.Lookup(sExt, nID))
		return nID;

	m_map.SetAt(sExt, TREE_ICON_UNKNOWN);
	SHFILEINFO info = { 0 };
    BOOL bOK = ::SHGetFileInfo(sExt, 0, &info, sizeof(info), SHGFI_ICON | SHGFI_SMALLICON | SHGFI_USEFILEATTRIBUTES);
	return bOK ? RegisterIcon(info.hIcon, sExt, TRUE) : TREE_ICON_UNKNOWN;
}

CString CJDImgeList::GetFileExt(LPCTSTR lpszFileName, BOOL bIncludeDot)
{
	CString sExt(lpszFileName);
	int nDot = sExt.ReverseFind(_T('.'));
	if (nDot == -1)
		return _T("");

	sExt = sExt.Mid(nDot + bIncludeDot ? 0 : 1);
	sExt.TrimLeft();
	sExt.TrimRight();
	return sExt;
}
