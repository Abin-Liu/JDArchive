// DatabaseCache.cpp: implementation of the CDatabaseCache class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JDArchive.h"
#include "DatabaseCache.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CJDFile::~CJDFile()
{
	for (int i = 0; i < m_aVersions.GetSize(); i++)
		delete m_aVersions[i];
}

int CJDFile::FindVer(int nVerID) const
{
	for (int i = 0; i < m_aVersions.GetSize(); i++)
	{
		LPVERDATA p = m_aVersions[i];
		if (p->nVersionID == nVerID)
			return i;
	}
	return -1;
}

CDatabaseCache::CDatabaseCache()
{

}

CDatabaseCache::~CDatabaseCache()
{
	POSITION pos = m_users.GetStartPosition();
	while (pos)
	{
		int nID;
		LPUSERDATA p = NULL;
		m_users.GetNextAssoc(pos, nID, p);
		if (p)
			delete p;
	}
	
	pos = m_files.GetStartPosition();
	while (pos)
	{
		int nID;
		CJDFile* p = NULL;
		m_files.GetNextAssoc(pos, nID, p);
		if (p)
			delete p;
	}
}

CJDFile* CDatabaseCache::VerToFile(int nVersionID, int& nFileID) const
{
	nFileID = 0;
	if (!m_verIdx.Lookup(nVersionID, nFileID))
		return NULL;
	
	CJDFile* pFile;
	if (!m_files.Lookup(nFileID, pFile))
		return NULL;
	return pFile;
}

int CDatabaseCache::AddRole(CLBPacket* pPacket)
{
	int nRoleID = pPacket->ReadInt();
	if (nRoleID < 0)
		return 0;

	m_roles.SetAt(nRoleID, ReadPacketString(pPacket));
	return nRoleID;
}

BOOL CDatabaseCache::GetRole(int nRoleID, CString &sRoleName) const
{
	return m_roles.Lookup(nRoleID, sRoleName);
}

int CDatabaseCache::AddUser(CLBPacket* pPacket)
{
	int nUserID = pPacket->ReadInt();
	if (nUserID < 1)
		return 0;

	LPUSERDATA p = NULL;
	if (!m_users.Lookup(nUserID, p))
	{
		p = new USERDATA;
		m_users.SetAt(nUserID, p);
	}	

	p->nUserID = nUserID;
	p->nRoleID = pPacket->ReadInt();
	p->nPrivilege = pPacket->ReadInt();
	p->sName = ReadPacketString(pPacket);	
	return nUserID;
}

BOOL CDatabaseCache::GetUser(int nUserID, CString& sName, LPINT pRoleID, LPINT pPrivilege) const
{
	LPUSERDATA p = NULL;
	m_users.Lookup(nUserID, p);
	if (!m_users.Lookup(nUserID, p))
		return FALSE;

	sName = p->sName;	

	if (pRoleID)
		*pRoleID = p->nRoleID;

	if (pPrivilege)
		*pPrivilege = p->nPrivilege;
	
	return TRUE;
}

BOOL CDatabaseCache::AddFile(int nFileID, int nParentCatID, HTREEITEM hItem)
{
	if (nFileID < 1 || hItem == NULL)
		return FALSE;

	CJDFile* p = NULL;
	if (m_files.Lookup(nFileID, p))
		return TRUE;

	p = new CJDFile(hItem, nParentCatID);
	m_files.SetAt(nFileID, p);
	return TRUE;
}

BOOL CDatabaseCache::DeleteFile(int nFileID)
{
	CJDFile* p = NULL;
	if (!m_files.Lookup(nFileID, p))
		return FALSE;

	delete p;
	m_files.RemoveKey(nFileID);
	return TRUE;
}

int CDatabaseCache::AddVersion(CLBPacket* pPacket)
{
	int nVerID = pPacket->ReadInt();
	if (nVerID < 1)
		return 0;

	int nFileID = pPacket->ReadInt();
	CJDFile* pFile = NULL;
	if (!m_files.Lookup(nFileID, pFile))
		return 0;

	m_verIdx.SetAt(nVerID, nFileID);
	
	LPVERDATA p = new VERDATA;
	p->nVersionID = nVerID;
	p->nFileID = nFileID;
	p->dwSize = pPacket->ReadDword();	
	p->sSize = AfxSizeToStr(p->dwSize);		
	p->nVersion = pPacket->ReadInt();
	p->nCreatorID = pPacket->ReadInt();
	pPacket->ReadData(&p->dtCreate, sizeof(COleDateTime));
	p->nCheckerID = pPacket->ReadInt();
	pPacket->ReadData(&p->dtCheck, sizeof(COleDateTime));
	p->sDesc = ReadPacketString(pPacket);
	pFile->m_aVersions.Add(p);
	return nVerID;
}

int CDatabaseCache::DeleteVersion(int nVersionID)
{
	int nFileID = 0;
	CJDFile* pFile = VerToFile(nVersionID, nFileID);
	if (pFile == NULL)
		return 0;

	m_verIdx.RemoveKey(nVersionID);
	int idx = pFile->FindVer(nVersionID);
	if (idx != -1)
	{
		delete pFile->m_aVersions[idx];
		pFile->m_aVersions.RemoveAt(idx);
		return nFileID;
	}
	
	return 0;
}

int CDatabaseCache::CheckVersion(int nVersionID, int nCheckerID, COleDateTime dtCheck)
{
	int nFileID = 0;
	CJDFile* pFile = VerToFile(nVersionID, nFileID);
	if (pFile == NULL)
		return 0;	

	int idx = pFile->FindVer(nVersionID);
	if (idx != -1)
	{
		LPVERDATA p = pFile->m_aVersions[idx];
		p->nCheckerID = nCheckerID;
		p->dtCheck = dtCheck;
		return nFileID;
	}
	return 0;
}

int CDatabaseCache::UncheckVersion(int nVersionID)
{
	int nFileID = 0;
	CJDFile* pFile = VerToFile(nVersionID, nFileID);
	if (pFile == NULL)
		return 0;	
	
	int idx = pFile->FindVer(nVersionID);
	if (idx != -1)
	{
		LPVERDATA p = pFile->m_aVersions[idx];
		p->nCheckerID = 0;
		p->dtCheck.SetStatus(COleDateTime::invalid);
		return nFileID;
	}
	return 0;
}

BOOL CDatabaseCache::EnumUsers(fnEnumUserProc fnCallback, LPVOID lParam) const
{
	if (fnCallback == NULL)
		return TRUE;

	int nUserID;
	LPUSERDATA pData;
	POSITION pos = m_users.GetStartPosition();
	while (pos)
	{
		m_users.GetNextAssoc(pos, nUserID, pData);
		if (!fnCallback(nUserID, pData, lParam))
			return FALSE;
	}
	return TRUE;
}

BOOL CDatabaseCache::EnumRoles(fnEnumRoleProc fnCallback, LPVOID lParam) const
{
	if (fnCallback == NULL)
		return TRUE;

	int nRoleID;
	CString sName;
	POSITION pos = m_roles.GetStartPosition();
	while (pos)
	{
		m_roles.GetNextAssoc(pos, nRoleID, sName);
		if (!fnCallback(nRoleID, sName, lParam))
			return FALSE;
	}
	return TRUE;
}

BOOL CDatabaseCache::EnumVersions(int nFileID, fnEnumVersionProc fnCallback, LPVOID lParam) const
{
	if (fnCallback == NULL)
		return TRUE;

	CJDFile* pFile = NULL;
	if (!m_files.Lookup(nFileID, pFile))
		return FALSE;

	if (pFile == NULL)
		return FALSE;

	int nCount = pFile->m_aVersions.GetSize();
	if (nCount < 1)
		return FALSE;

	for (int i = 0; i < nCount; i++)
	{
		LPCVERDATA pData = pFile->m_aVersions[i];
		if (!fnCallback(pData->nVersionID, pData, lParam))
			return FALSE;
	}	
	return TRUE;
}

BOOL CDatabaseCache::AddCategory(int nCatID, int nParentCatID, HTREEITEM hItem)
{
	if (nCatID < 1 || hItem == NULL)
		return FALSE;

	POINT pt;
	pt.x = nParentCatID;
	pt.y = (LONG)hItem;
	
	m_categories.SetAt(nCatID, pt);
	return TRUE;
}

HTREEITEM CDatabaseCache::GetCategory(int nCatID) const
{
	POINT pt;
	if (m_categories.Lookup(nCatID, pt))
		return (HTREEITEM)pt.y;
	return NULL;
}

HTREEITEM CDatabaseCache::GetFile(int nFileID) const
{
	CJDFile* pFile = NULL;
	if (m_files.Lookup(nFileID, pFile))
		return pFile->m_hItem;
	return NULL;
}

int CDatabaseCache::VersionToFile(int nVerID) const
{
	int nFileID = 0;
	if (m_verIdx.Lookup(nVerID, nFileID))
		return nFileID;
	return 0;
}

void CDatabaseCache::DeleteCategory(int nCatID)
{
	POINT pt;
	if (!m_categories.Lookup(nCatID, pt))
		return;

	m_categories.RemoveKey(nCatID);
}

void CDatabaseCache::Cleanup()
{
	m_categories.RemoveAll();
	m_files.RemoveAll();
	m_roles.RemoveAll();
	m_users.RemoveAll();
	m_verIdx.RemoveAll();
}

LPCVERDATA CDatabaseCache::GetVersion(int nVerID) const
{
	int nFileID;
	CJDFile* pFile = VerToFile(nVerID, nFileID);
	if (pFile == NULL)
		return NULL;

	int nIdx = pFile->FindVer(nVerID);
	if (nIdx == -1)
		return NULL;

	return pFile->m_aVersions[nIdx];
}

LPVERDATA CDatabaseCache::GetVersion(int nVerID)
{
	int nFileID;
	CJDFile* pFile = VerToFile(nVerID, nFileID);
	if (pFile == NULL)
		return NULL;

	int nIdx = pFile->FindVer(nVerID);
	if (nIdx == -1)
		return NULL;

	return pFile->m_aVersions[nIdx];
}

void CDatabaseCache::DeleteVersions(int nFileID)
{
	CJDFile* pFile = NULL;
	if (m_files.Lookup(nFileID, pFile))
		pFile->m_aVersions.RemoveAll();
}

