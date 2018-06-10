// JDImgeList.h: interface for the CJDImgeList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JDIMGELIST_H__A8BDA2B5_FE93_41DC_AB05_3DD3AD9F6276__INCLUDED_)
#define AFX_JDIMGELIST_H__A8BDA2B5_FE93_41DC_AB05_3DD3AD9F6276__INCLUDED_

#include <afxtempl.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum {
	TREE_ICON_UNKNOWN = 0,
	TREE_ICON_BUSY,
	TREE_ICON_SERVER_DISCONNECT,
	TREE_ICON_SERVER_CONNECT,
	TREE_ICON_FOLDER_CLOSE,
	TREE_ICON_FOLDER_OPEN
};

static const UINT BUILTIN_ICONS[] = {
	IDI_TREE_UNKNOWN,
	IDI_TREE_BUSY,
	IDI_TREE_SERVER_DISCONNECT, 
	IDI_TREE_SERVER_CONNECT, 
	IDI_TREE_FOLDER_CLOSE,
	IDI_TREE_FOLDER_OPEN 
};

static const int BUILTIN_ICON_NUM = sizeof(BUILTIN_ICONS) / sizeof(UINT);

class CJDImgeList : public CImageList  
{
public:		

	CJDImgeList();
	virtual ~CJDImgeList();

	BOOL Create();
	int GetFileIcon(LPCTSTR lpszFileName);
	static CString GetFileExt(LPCTSTR lpszFileName, BOOL bIncludeDot = FALSE);

private:

	CMap<CString, LPCTSTR, int, int> m_map;
	HICON m_aInternalIcons[BUILTIN_ICON_NUM];
	int RegisterIcon(HICON hIcon, LPCTSTR lpszExt, BOOL bDestroy);
	HICON m_hHtml;
	HICON m_hTxt;
	HICON m_hBmp;
};

#endif // !defined(AFX_JDIMGELIST_H__A8BDA2B5_FE93_41DC_AB05_3DD3AD9F6276__INCLUDED_)
