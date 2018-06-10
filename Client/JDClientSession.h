// JDClientSession.h: interface for the CJDClientSession class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JDCLIENTSESSION_H__ACA1CE18_90F5_4F1B_8C33_A9F693A308F6__INCLUDED_)
#define AFX_JDCLIENTSESSION_H__ACA1CE18_90F5_4F1B_8C33_A9F693A308F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\LBClient.h"
#include "ProgressDlg.h"
#include "DownloadTreeManager.h"

class CJDClientSession : public CLBClientSession  
{
public:	
	void TaskDownloadTreeRequest(int nCatID, LPCTSTR lpszClientPath);
	void TaskDelTree(int nCatID);
	void TaskChangeCategoryPriv(int nCatID, LPCTSTR lpszNewPriv, BOOL bApplySubCats);
	void TaskRecoverData(int nVersionID);
	void TaskRequestRecovery(int nRoleID, LPCTSTR lpszFileName);
	void TaskApplyVerManage(int nFileID, int nVersion);
	void TaskCancelRequestManageVer();
	void TaskRequestManageVer(int nFileID);
	void TaskCancelRequestUploadVer();
	void TaskRequestUploadVer(int nFileID);
	void TaskRequestCategory(int nCatID);
	void TaskRequestFile(int nFileID);
	void TaskPasteItem(BOOL bIsFile, int nID, int nParentCatID);
	void TaskCatInitStart(int nParentCatID, LPCTSTR lpszCatName, LPCTSTR lpszClientPath);
	CString GetPassword() const;
	void TaskPatchApp(int nPatchID);
	void TaskCheckUpdate();
	void TaskUploadPatch(LPCTSTR lpszFileName, LPCTSTR lpszPath);
	void SendHeartbeat();	
	void RegisterDeletedVerNotify(HWND hWnd);
	void TaskDeleteVersion(int nVerID);
	void TaskDeleteFile(int nFileID);
	void SetProgWnd(CProgressDlg* pWnd);
	void TaskDownloadVersion(int nVersionID);
	void TaskUploadVersion(int nFileID, LPCTSTR lpszDesc, int nVersion, LPCTSTR lpszFilePath);
	void TaskRequestRootCategory(int nRoleID);
	void SetServerAddr(LPCTSTR lpszServerAddr);
	void TaskResetPassword(int nUserID);
	static CString GetUserPrivilegeName(int nPrivilege);
	void RegisterUserNotify(HWND hWnd);
	void RegisterRoleNotify(HWND hWnd);
	void TaskUncheckVersion(int nVerID);
	void TaskCheckVersion(int nVerID);
	void TaskListVersion(int nFileID);
	void TaskAddFile(int nParentCatID, LPCTSTR lpszName, LPCTSTR lpszDesc);
	void TaskListFile(int nParentCatID);
	void TaskDeleteCategory(int nCatID);
	void TaskChangeCategory(int nCatID, LPCTSTR lpszNewName, LPCTSTR lpszNewDesc);
	void TaskAddCategory(int nParentCatID, LPCTSTR lpszName, LPCTSTR lpszDesc);
	void TaskListRole();
	void TaskListUser();
	void TaskListCategory(int nParentCatID);
	void TaskChangeRole(int nRoleID, LPCTSTR lpszNewName);
	void TaskChangeUser(int nUserID, int nRoleID, int nPrivilege);
	void TaskAddUser(int nRoleID, int nPrivilege, LPCTSTR lpszName);
	void TaskChangePassword(LPCTSTR lpszOldPassword, LPCTSTR lpszNewPassword);
	void TaskLogin(LPCTSTR lpszUserName, LPCTSTR lpszPassword);
	void TaskAddRole(LPCTSTR lpszName);
	
	CJDClientSession();
	virtual ~CJDClientSession();

	CString GetServerAddr() const;

	int GetRoleID() const;
	BOOL IsLoggedin() const;
	void Abandon();
	CString GetRoleName() const;
	CString GetUserName() const;
	int GetUserPrivilege() const;
	CString GetUserPrivilegeName() const;
	BOOL VerifyPrivilege(int nRequiredPrivilege, int nRequiredRole = 0) const;

protected:
	
	void OnDownloadTreeReady(CLBPacket *pPacket);
	void OnDownloadTreeNext(CLBPacket* pPacket);

	BOOL Zip(LPCTSTR lpszFileName, LPCTSTR lpszFilePath, CString& sTemp);
	BOOL Unzip(LPCTSTR lpszSrc, LPCTSTR lpszDestFolder, LPCTSTR lpszFileName);

	void OnRecoverData(CLBPacket* pPacket);
	void BatchInit(int nParentCatID, int nStartPos, LPCTSTR lpszFolderName, LPCTSTR lpszPath);
	void InitCategory(CLBPacket* pPacket);
	void UploadPatch(CLBPacket* pPacket);
	void DownloadPatch(CLBPacket* pPacket);

	BOOL ReceiveFileProc(LPCVOID lpszFilePath, DWORD dwBytesReceived, DWORD dwBytesTotal, LPVOID lParam);
	BOOL SendFileProc(LPCVOID lpszFilePath, DWORD dwBytesSent, DWORD dwBytesTotal, LPVOID lParam);
	void OnDownloadVersion(CLBPacket* pPacket);
	void OnUploadVersion(CLBPacket* pPacket);
	void PostMessage(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam = 0);
	void OnServerError(DWORD dwPacketID, DWORD dwResult, int nID, BOOL bBkgnd);
	void OnUserLogin(CLBPacket* pPacket);
	
	BOOL OnDeparture(DWORD dwPacketID, CLBPacket* pPacket);
	void OnReceive(DWORD dwPacketID, CLBPacket* pPacket);
		
	void OnDisconnect();

	int m_nUserID;	
	int m_nRoleID;
	int m_nPrivilege;
	CString m_sUserName;
	CString m_sRoleName;
	CString m_sServerAddr;
	HWND m_hRoleNofity;
	HWND m_hUserNotify;
	HWND m_hDeletedVerNotify;
	CProgressDlg* m_pProgWnd;
	CString m_sPassword;
	CString m_sReserved;

	int m_nFileSystemFlag;
	CString m_sFileSystemClientPath;
	CDownloadTreeManager m_dtm;

private:
	CString GetZipPassword() const;
};

#endif // !defined(AFX_JDCLIENTSESSION_H__ACA1CE18_90F5_4F1B_8C33_A9F693A308F6__INCLUDED_)
