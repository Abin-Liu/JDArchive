// DownloadTreeManager.cpp: implementation of the CDownloadTreeManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JDArchive.h"
#include "DownloadTreeManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDownloadTreeManager::CDownloadTreeManager()
{
	Initialize();
}

CDownloadTreeManager::~CDownloadTreeManager()
{

}

void CDownloadTreeManager::Initialize( int nCatCount, int nFileCount, LPCTSTR lpszClientPath)
{
	m_nCompltedCount = 0;
	m_nCatCount = nCatCount;
	m_nFileCount = nFileCount;
	m_aCatMap.RemoveAll();

	if (lpszClientPath)
		m_sClientPath = lpszClientPath;
	else
		m_sClientPath.Empty();
}

void CDownloadTreeManager::AddCategory(DWORD dwCatID, LPCTSTR lpszCatPath)
{
	if (dwCatID && lpszCatPath && *lpszCatPath)
	{
		m_aCatMap.SetAt(dwCatID, lpszCatPath);
		::CreateDirectory(m_sClientPath + _T('\\') + lpszCatPath, NULL);
	}
}

BOOL CDownloadTreeManager::LookupFullPath(DWORD dwCatID, CString &rFullPath) const
{
	CString sCatPath;
	if (m_aCatMap.Lookup(dwCatID, sCatPath))
	{
		rFullPath = m_sClientPath + _T('\\') + sCatPath;
		return TRUE;			
	}
	return FALSE;
}

int CDownloadTreeManager::GetProgressDone() const
{
	return m_nCompltedCount;
}

int CDownloadTreeManager::GetProgressTotal() const
{
	return m_nCatCount + m_nFileCount;
}

void CDownloadTreeManager::StepIt()
{
	m_nCompltedCount++;
}
