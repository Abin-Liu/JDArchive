// DownloadTreeManager.h: interface for the CDownloadTreeManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOWNLOADTREEMANAGER_H__4CF495F2_48A8_48F1_8427_AAA5D681DE07__INCLUDED_)
#define AFX_DOWNLOADTREEMANAGER_H__4CF495F2_48A8_48F1_8427_AAA5D681DE07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

class CDownloadTreeManager  
{
public:	
	
	CDownloadTreeManager();
	virtual ~CDownloadTreeManager();

	BOOL LookupFullPath(DWORD dwCatID, CString& rFullPath) const;
	void AddCategory(DWORD dwCatID, LPCTSTR lpszCatPath);
	void Initialize(int nCatCount = 0, int nFileCount = 0, LPCTSTR lpszClientPath = NULL);
	void StepIt();
	int GetProgressTotal() const;
	int GetProgressDone() const;

private:

	CMap<DWORD, DWORD, CString, LPCTSTR> m_aCatMap;
	
	int m_nCatCount;
	int m_nFileCount;
	CString m_sClientPath;
	int m_nCompltedCount;
};

#endif // !defined(AFX_DOWNLOADTREEMANAGER_H__4CF495F2_48A8_48F1_8427_AAA5D681DE07__INCLUDED_)
