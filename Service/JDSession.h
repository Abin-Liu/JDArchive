// JDSession.h: interface for the CJDSession class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JDSESSION_H__3FB02CE6_1CF6_4079_80CD_4289E228997D__INCLUDED_)
#define AFX_JDSESSION_H__3FB02CE6_1CF6_4079_80CD_4289E228997D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\\LBNetwork.h"
#include "..\\LBServer.h"
#include "ado.h"
#include "JDDatabase.h"
#include "SendTreeManager.h"

class CJDSession : public CLBServerSession 
{
public:

	enum { PERMISSION_NONE, PERMISSION_SUPER, PERMISSION_ADMIN };

	CJDSession(LPCTSTR lpszConnectString, LPCTSTR lpszRootFolder);
	virtual ~CJDSession();	
	
	COleDateTime GetConnectionTime() const;
	CString GetRoleName() const;
	CString GetUserName() const;
	LPCTSTR GetPeerName() const;

private:
	
	void CycleDelTree(DWORD nCatID, int& nCats, int& nFiles, int& nVers);
	DWORD DelTree(DWORD dwPacketID, CLBPacket* pPacket);
	void UpdateCatPrivProc(CDWordArray &aList, DWORD nCatID, LPCTSTR lpszNewPriv);
	DWORD ChangeCategoryPriv(DWORD dwPacketID, CLBPacket* pPacket);
	DWORD RecoverData(DWORD dwPacketID, CLBPacket *pPacket);
	DWORD ListRecovable(DWORD dwPacketID, CLBPacket* pPacket);
	DWORD ApplyManagerVer(DWORD dwPacketID, CLBPacket *pPacket);
	DWORD RequestManagerVer(DWORD dwPacketID, CLBPacket* pPacket);
	void UnlockFilesForUser();
	void OnDisconnect();
	DWORD RequestUploadVer(DWORD dwPacketID, CLBPacket* pPacket);
	DWORD CancelRequestUploadVer(DWORD dwPacketID, CLBPacket* pPacket);
	DWORD PasteCategory(DWORD dwPacketID, CLBPacket* pPacket);
	DWORD PasteFile(DWORD dwPacketID, CLBPacket* pPacket);
	DWORD CatInitStart(DWORD dwPacketID, CLBPacket* pPacket);
	DWORD DownloadPatch(DWORD dwPacketID, CLBPacket* pPacket);
	DWORD CheckPatch(DWORD dwPacketID, CLBPacket* pPacket);
	static CString GenerateGUID();
	DWORD UploadPatch(DWORD dwPacketID, CLBPacket *pPacket);
	DWORD RequestVersion(DWORD nVersionID);
	DWORD RequestCategory(DWORD nCatID);
	DWORD DeleteFile(DWORD dwPacketID, CLBPacket *pPacket);
	DWORD DeleteVersion(DWORD dwPacketID, CLBPacket *pPacket);
	DWORD UncheckVersion(DWORD dwPacketID, CLBPacket *pPacket);
	DWORD CheckVersion(DWORD dwPacketID, CLBPacket *pPacket);
	BOOL ReceiveFileProc(LPCVOID lpszFilePath, DWORD dwBytesReceived, DWORD dwBytesTotal, LPVOID lParam);
	BOOL SendFileProc(LPCVOID lpszFilePath, DWORD dwBytesSent, DWORD dwBytesTotal, LPVOID lParam);
	BOOL SendSequencialProc(DWORD dwPacketID, CLBPacket* pPacket, int nCount, int nSequence, LPVOID lParam);
	DWORD AfterUpload(CLBPacket *pPacket, CString sMD5, DWORD dwFileSize, CString sDiskName, DWORD nFileID, DWORD nRoleID, DWORD nVersion, CString sDesc);

	DWORD DownloadVer(DWORD dwPacketID, CLBPacket* pPacket);
	DWORD ListVersion(DWORD dwPacketID, CLBPacket* pPacket);
	DWORD UploadVer(DWORD dwPacketID, CLBPacket* pPacket);
	DWORD ChangeRole(DWORD dwPacketID, CLBPacket *pPacket);
	DWORD AddRole(DWORD dwPacketID, CLBPacket *pPacket);
	DWORD ListRole(DWORD dwPacketID, CLBPacket *pPacket);
	DWORD RequestData(DWORD dwPacketID, CLBPacket *pPacket, LPCTSTR lpszSQL, int nType);
	
	// A helper class to force closing database on function exits
	class CDBCloser
	{
	public:
		CDBCloser(CADODatabase* pDB, CADORecordset* pRS): m_pDB(pDB), m_pRS(pRS) {}
		~CDBCloser() { m_pRS->Close(); m_pDB->Close(); }

	private:
		CADODatabase* m_pDB;
		CADORecordset* m_pRS;
	};

	#define AUTO_CLOSE	CDBCloser __autoCloser(&m_db, &m_rs);

	void CloseDB();
	BOOL VerifyPermission(DWORD nRoleID, int nType) const;
	DWORD PackVerData(CLBPacket &packet);
	DWORD PackFileData(CLBPacket &packet);
	DWORD PackUserData(CLBPacket& packet);
	DWORD PackRoleData(CLBPacket &packet);
	DWORD PackCatData(CLBPacket& packet);
	DWORD AddFile(DWORD dwPacketID, CLBPacket *pPacket);
	DWORD ListFile(DWORD dwPacketID, CLBPacket *pPacket);
	DWORD ChangeUser(DWORD dwPacketID, CLBPacket *pPacket);
	void OnStop();
	void OnStart();

	DWORD AddUser(DWORD dwPacketID, CLBPacket *pPacket);
	DWORD ListUsers(DWORD dwPacketID, CLBPacket *pPacket);
	static DWORD TranslateResult(DWORD dwResult, DWORD dwReturnIfExists = JDRESPONSE_NAME_CONFLICTS);
	DWORD DeleteCategory(DWORD dwPacketID, CLBPacket *pPacket);
	DWORD ChangeCategory(DWORD dwPacketID, CLBPacket* pPacket);
	DWORD AddCategory(DWORD dwPacketID, CLBPacket *pPacket);
	DWORD ListCategory(DWORD dwPacketID, CLBPacket* pPacket);
	DWORD RequestRootCategory(DWORD dwPacketID, CLBPacket *pPacket);

	DWORD UserLogin(DWORD dwPacketID, CLBPacket* pPacket);
	DWORD ChangePassword(DWORD dwPacketID, CLBPacket* pPacket);

	DWORD Execute(LPCTSTR lpszSQL, ...);
	DWORD Query(LPCTSTR lpszSQL, ...);

	static CString ReadString(CLBPacket* pPacket);

	void OnConnect();
	void SendErrorPacket(DWORD dwPacketID, DWORD dwError, int nValue);
	void OnReceive(DWORD dwPacketID, CLBPacket* pPacket);

	DWORD CompareUserPrivilege(DWORD nUserID);
	DWORD ResetPassword(DWORD dwPacketID, CLBPacket* pPacket);
	BOOL InsertRecord(LPCTSTR lpszTableName);
	DWORD PackRecovableData(CLBPacket &packet);

	DWORD DownloadFileSystemStructure(DWORD dwPacketID, CLBPacket* pPacket);
	DWORD SendFileSystemNext(DWORD dwPacketID, CLBPacket* pPacket);
	DWORD FinishFileSystemStructure(DWORD dwPacketID, CLBPacket* pPacket);

	void GetSubFolders(DWORD dwParentID, const CString& sParentPath);
	void GetSubFiles(DWORD dwParentID);

	DWORD m_nUserID;
	DWORD m_nRoleID;
	DWORD m_nPrivilege;
	CString m_sUserName;
	CString m_sRoleName;
	COleDateTime m_connectionTime;
	CString m_sConnectString;
	CString m_sRootPath;
	TCHAR m_szPeerName[32];
	CADODatabase m_db;
	CADORecordset m_rs;	
	int m_nClientVer;
	CSendTreeManager m_stm;	
};

#endif // !defined(AFX_JDSESSION_H__3FB02CE6_1CF6_4079_80CD_4289E228997D__INCLUDED_)
