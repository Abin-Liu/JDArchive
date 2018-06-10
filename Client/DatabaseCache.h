// DatabaseCache.h: interface for the CDatabaseCache class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATABASECACHE_H__EE8D64EC_5495_402F_A7CC_CF0A687419D2__INCLUDED_)
#define AFX_DATABASECACHE_H__EE8D64EC_5495_402F_A7CC_CF0A687419D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\\LBNetwork.h"
#include <afxtempl.h>

typedef struct _tagUserData
{
	int nUserID;
	int nRoleID;
	int nPrivilege;	
	CString sName;
} USERDATA, *LPUSERDATA;
typedef const USERDATA* LPCUSERDATA;

typedef struct _tagVerData
{
	int nVersionID;
	int nFileID;
	int nVersion;
	DWORD dwSize;
	CString sSize;	
	int nCreatorID;
	COleDateTime dtCreate;
	int nCheckerID;
	COleDateTime dtCheck;
	CString sDesc;
} VERDATA, *LPVERDATA;
typedef const VERDATA* LPCVERDATA;

typedef BOOL (CALLBACK *fnEnumUserProc)(int nUserID, LPCUSERDATA pUserData, LPVOID lParam);
typedef BOOL (CALLBACK *fnEnumRoleProc)(int nRoleID, LPCTSTR lpszRoleName, LPVOID lParam);
typedef BOOL (CALLBACK *fnEnumVersionProc)(int nVersionID, LPCVERDATA pVersionData, LPVOID lParam);

class CJDFile
{	
private:
	friend class CDatabaseCache;
	CJDFile(HTREEITEM hItem, int nParentCatID) { m_hItem = hItem; m_nParentCatID = nParentCatID; }
	~CJDFile();
	int FindVer(int nVerID) const;
	HTREEITEM m_hItem;
	int m_nParentCatID;
	CArray<LPVERDATA, LPVERDATA> m_aVersions;
};

class CDatabaseCache
{
public:		

	CDatabaseCache();
	~CDatabaseCache();
	void Cleanup();

	int AddRole(CLBPacket* pPacket);
	BOOL GetRole(int nRoleID, CString& sRoleName) const;

	int AddUser(CLBPacket* pPacket);
	BOOL GetUser(int nUserID, CString& sName, LPINT pRoleID = NULL, LPINT pPrivilege = NULL) const;

	BOOL AddCategory(int nCatID, int nParentCatID, HTREEITEM hItem);
	HTREEITEM GetCategory(int nCatID) const;
	void DeleteCategory(int nCatID);

	BOOL AddFile(int nFileID, int nParentCatID, HTREEITEM hItem);
	BOOL DeleteFile(int nFileID);
	HTREEITEM GetFile(int nFileID) const;
	void DeleteVersions(int nFileID);

	int AddVersion(CLBPacket* pPacket);
	int DeleteVersion(int nVersionID);
	int CheckVersion(int nVersionID, int nCheckerID, COleDateTime dtCheck);
	int UncheckVersion(int nVersionID);
	int VersionToFile(int nVerID) const;
	LPCVERDATA GetVersion(int nVerID) const;
	LPVERDATA GetVersion(int nVerID);


	BOOL EnumUsers(fnEnumUserProc fnCallback, LPVOID lParam) const;
	BOOL EnumRoles(fnEnumRoleProc fnCallback, LPVOID lParam) const;
	BOOL EnumVersions(int nFileID, fnEnumVersionProc fnCallback, LPVOID lParam) const;

private:

	CJDFile* VerToFile(int nVersionID, int& nFileID) const;

	CMap<int, int, CString, LPCTSTR> m_roles;
	CMap<int, int, LPUSERDATA, LPUSERDATA> m_users;	
	CMap<int, int, POINT, POINT> m_categories;
	CMap<int, int, CJDFile*, CJDFile*> m_files;
	CMap<int, int, int, int> m_verIdx;
};

#endif // !defined(AFX_DATABASECACHE_H__EE8D64EC_5495_402F_A7CC_CF0A687419D2__INCLUDED_)
