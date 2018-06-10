// SendTreeManager.h: interface for the CSendTreeManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SENDTREEMANAGER_H__8DB223A6_1253_4229_B208_878A70C066F5__INCLUDED_)
#define AFX_SENDTREEMANAGER_H__8DB223A6_1253_4229_B208_878A70C066F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

class CSendTreeManager  
{
public:		
	
	CSendTreeManager();
	virtual ~CSendTreeManager();

	BOOL GetNext(DWORD& rFileID, DWORD& rCatID, CString& rName);
	void Reset();

	BOOL HasMore() const;
	BOOL HasMoreFiles() const;
	BOOL HasMoreCats() const;
	int GetFileCount() const;
	int GetCatCount() const;
	void AddFileInfo(DWORD dwParentID, DWORD dwFileID);
	void AddCatInfo(DWORD dwCatID, LPCTSTR lpszCatPath);
	DWORD GetCatIDByIndex(int nIndex) const;

private:
	
	CDWordArray m_aCatIDs;
	CStringArray m_aCatPaths;
	CDWordArray m_aParentIDs;
	CDWordArray m_aFileIDs;
	int m_nCatIndex;
	int m_nFileIndex;
};

#endif // !defined(AFX_SENDTREEMANAGER_H__8DB223A6_1253_4229_B208_878A70C066F5__INCLUDED_)
