// SendTreeManager.cpp: implementation of the CSendTreeManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SendTreeManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSendTreeManager::CSendTreeManager()
{
	m_nCatIndex = 0;
	m_nFileIndex = 0;
}

CSendTreeManager::~CSendTreeManager()
{

}

void CSendTreeManager::AddCatInfo(DWORD dwCatID, LPCTSTR lpszCatPath)
{
	if (dwCatID && lpszCatPath && lpszCatPath && *lpszCatPath)
	{
		m_aCatIDs.Add(dwCatID);
		m_aCatPaths.Add(lpszCatPath);
	}
}

void CSendTreeManager::AddFileInfo(DWORD dwParentID, DWORD dwFileID)
{
	if (dwParentID && dwFileID)
	{
		m_aParentIDs.Add(dwParentID);
		m_aFileIDs.Add(dwFileID);
	}
}

int CSendTreeManager::GetCatCount() const
{
	return m_aCatIDs.GetSize();
}

int CSendTreeManager::GetFileCount() const
{
	return m_aFileIDs.GetSize();
}

DWORD CSendTreeManager::GetCatIDByIndex(int nIndex) const
{
	return m_aCatIDs[nIndex];
}

BOOL CSendTreeManager::HasMoreCats() const
{
	return m_nCatIndex < m_aCatIDs.GetSize();
}

BOOL CSendTreeManager::HasMoreFiles() const
{
	return m_nFileIndex < m_aFileIDs.GetSize();
}

void CSendTreeManager::Reset()
{
	m_aCatIDs.RemoveAll();
	m_aCatPaths.RemoveAll();
	m_nCatIndex = 0;

	m_aParentIDs.RemoveAll();
	m_aFileIDs.RemoveAll();
	m_nFileIndex = 0;
}

BOOL CSendTreeManager::GetNext(DWORD &rFileID, DWORD &rCatID, CString &rName)
{
	if (HasMoreCats())
	{
		rCatID = m_aCatIDs[m_nCatIndex];
		rName = m_aCatPaths[m_nCatIndex];
		m_nCatIndex++;
		return TRUE;
	}

	if (HasMoreFiles())
	{
		rFileID = m_aFileIDs[m_nFileIndex];
		rCatID = m_aParentIDs[m_nFileIndex];		
		m_nFileIndex++;
		return TRUE;
	}

	return FALSE;
}

BOOL CSendTreeManager::HasMore() const
{
	return HasMoreCats() || HasMoreFiles();
}
