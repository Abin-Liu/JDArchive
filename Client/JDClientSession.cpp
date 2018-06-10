// JDClientSession.cpp: implementation of the CJDClientSession class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JDArchive.h"
#include "JDClientSession.h"
#include "ZipArchive\\ZipArchive.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define OUT_RDY			CLBPacket packet; packet.PrepareForWrite();
#define OUT_INT(x)		packet.WriteInt(x);
#define OUT_STR(x)		packet.WriteString(x);
#define OUT_GO(x)		if (m_nUserID || (x) == JD_PACKET_LOGIN) { packet.Encode(x); AddTask(&packet); }


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJDClientSession::CJDClientSession()
{
	m_pProgWnd = NULL;
	m_nUserID = 0;
	m_nRoleID = 0;
	m_nPrivilege = 0;
	m_hRoleNofity = NULL;
	m_hUserNotify = NULL;
	m_hDeletedVerNotify = NULL;
	m_nFileSystemFlag = 0;
}

CJDClientSession::~CJDClientSession()
{
	Abandon();
}

void CJDClientSession::SetProgWnd(CProgressDlg *pWnd)
{
	m_pProgWnd = pWnd;
}

CString CJDClientSession::GetServerAddr() const
{
	return m_sServerAddr;
}

CString CJDClientSession::GetUserName() const
{
	return m_sUserName;
}

CString CJDClientSession::GetRoleName() const
{
	return m_sRoleName;
}

void CJDClientSession::SetServerAddr(LPCTSTR lpszServerAddr)
{
	m_sServerAddr = lpszServerAddr;
}

void CJDClientSession::Abandon()
{
	CLBClientSession::Abandon();
	m_sUserName.Empty();
	m_sPassword.Empty();
	m_sRoleName.Empty();
	m_nUserID = 0;
	m_nRoleID = 0;
	m_nPrivilege = 0;
}

void CJDClientSession::PostMessage(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	if (::IsWindow(hWnd))
		::PostMessage(hWnd, nMsg, wParam, lParam);
}

BOOL CJDClientSession::IsLoggedin() const
{
	return m_nUserID != 0;
}

void CJDClientSession::RegisterRoleNotify(HWND hWnd)
{
	m_hRoleNofity = hWnd;
}

void CJDClientSession::RegisterUserNotify(HWND hWnd)
{
	m_hUserNotify = hWnd;
}

void CJDClientSession::OnDisconnect()
{
	CLBClientSession::OnDisconnect();
	_CACHE.Cleanup();
	_POST(WM_DISCONNECT, m_nUserID);
	m_sUserName = _T("");
	m_sRoleName = _T("");	
	m_nUserID = 0;
	m_nRoleID = 0;
	m_nPrivilege = 0;
	_LOG(_T("Session closed"));
}

BOOL CJDClientSession::OnDeparture(DWORD dwPacketID, CLBPacket* pPacket)
{
	_LOG("OnTaskDeparture: %d", dwPacketID);

	if (dwPacketID == JD_PACKET_LOGIN )
	{	
		_POST(WM_CONNECT, 1);
		if (!Connect(m_sServerAddr, JDARCHIVE_SERVER_PORT))
		{
			_POST(WM_CONNECT, 0);
			return FALSE;
		}
	}	

	if (!IsBackgroundPacket(dwPacketID) && !IsSilentPacket(dwPacketID))
		_POST(WM_TASK_START, dwPacketID);
	return TRUE;
}

void CJDClientSession::OnServerError(DWORD dwPacketID, DWORD dwResult, int nID, BOOL bBkgnd)
{	
	if (dwPacketID == JD_PACKET_LOGIN)
		Abandon();

	if (dwPacketID == JD_PACKET_CAT_LIST || dwPacketID == JD_PACKET_FILE_LIST)
		_POST(WM_SERVER_LIST_ERROR, dwPacketID, nID);
	else if (!bBkgnd && !IsSilentPacket(dwPacketID))
		_POST(WM_SERVER_ERROR, dwPacketID, dwResult);
}

void CJDClientSession::OnReceive(DWORD dwPacketID, CLBPacket* pPacket)
{
	_LOG(_T("Server responded: %d"), dwPacketID);
	BOOL bBkgnd = IsBackgroundPacket(dwPacketID);
	DWORD dwResult = pPacket->ReadDword();
	if (dwResult != JDRESPONSE_SUCCESS)
	{
		m_pProgWnd->Hide();
		OnServerError(dwPacketID, dwResult, pPacket->ReadInt(), bBkgnd);
		return;
	}	

	// By reaching here the packet is healthy
	int nTempID = 0;
	switch (dwPacketID)
	{
	case JD_PACKET_LOGIN:
		OnUserLogin(pPacket);
		break;
		
	case JD_PACKET_CHANGE_PASSWORD:
		m_sPassword = ReadPacketString(pPacket);
		break;	

	case JD_PACKET_USER_LIST:
	case JD_PACKET_USER_ADD:
	case JD_PACKET_USER_CHANGE:
	case JD_PACKET_REQUEST_USER:
		nTempID = _CACHE.AddUser(pPacket);
		PostMessage(m_hUserNotify, WM_DATA_NOTIFY, 0, nTempID);
		break;

	case JD_PACKET_USER_RESETPASSWORD:
		PostMessage(m_hUserNotify, WM_DATA_NOTIFY, 1, pPacket->ReadInt());
		break;

	case JD_PACKET_ROLE_LIST:
	case JD_PACKET_REQUEST_ROLE:
		nTempID = _CACHE.AddRole(pPacket);
		PostMessage(m_hRoleNofity, WM_DATA_NOTIFY, 0, nTempID);
		break;	
		
	case JD_PACKET_ROLE_ADD:
	case JD_PACKET_ROLE_CHANGE:
		nTempID = _CACHE.AddRole(pPacket);		
		TaskRequestRootCategory(nTempID);
		PostMessage(m_hRoleNofity, WM_DATA_NOTIFY, 1, nTempID);
		break;

	case JD_PACKET_VER_UPLOAD:
		OnUploadVersion(pPacket);
		break;

	case JD_PACKET_VER_DOWNLOAD:
		OnDownloadVersion(pPacket);
		break;

	case JD_PACKET_UPDATE_DOWNLOAD:
		DownloadPatch(pPacket);
		break;

	case JD_PACKET_UPLOAD_APP:
		UploadPatch(pPacket);
		break;

	case JD_PACKET_CAT_INIT_START:
		InitCategory(pPacket);
		break;

	case JD_PACKET_CAT_INIT_END:
		TaskRequestCategory(pPacket->ReadInt());
		break;

	case JD_PACKET_RECOVERY:
		OnRecoverData(pPacket);
		break;

	case JD_PACKET_FILESYSTEM_REQUEST:
		OnDownloadTreeReady(pPacket);
		break;

	case JD_PACKET_FILESYSTEM_NEXT:
		OnDownloadTreeNext(pPacket);
		break;	

	default:
		break;
	}
	
	if (pPacket->IsSequenceEnd())
		m_pProgWnd->Hide();
	
	if (!bBkgnd)
		_POST(WM_SERVER_RESPONSE, dwPacketID, (LPARAM)(new CLBPacket(pPacket)));
}

void CJDClientSession::OnUserLogin(CLBPacket *pPacket)
{
	_LOG("Login success");
	m_nUserID = pPacket->ReadInt();
	m_nRoleID = pPacket->ReadInt();
	m_nPrivilege = pPacket->ReadInt();
	m_sUserName = ReadPacketString(pPacket);
	m_sPassword = ReadPacketString(pPacket);
	m_sRoleName = ReadPacketString(pPacket);	

	_LOG("Requesting role list");
	TaskListRole();
		
	_LOG("Requesting user list");
	TaskListUser();

	TaskCheckUpdate();
}

int CJDClientSession::GetRoleID() const
{
	return m_nRoleID;
}

int CJDClientSession::GetUserPrivilege() const
{
	return m_nPrivilege;
}

CString CJDClientSession::GetUserPrivilegeName(int nPrivilege)
{
	if (nPrivilege >= 0 && nPrivilege < _PRIVILEGES_COUNT)
		return _PRIVILEGES[nPrivilege];
	return _T("");
}

CString CJDClientSession::GetUserPrivilegeName() const
{
	return GetUserPrivilegeName(m_nPrivilege);
}

BOOL CJDClientSession::VerifyPrivilege(int nRequiredPrivilege, int nRequiredRole) const
{
	if (m_nUserID == 0 || m_nPrivilege < JDPRIV_USER || m_nPrivilege < nRequiredPrivilege)
		return FALSE;

	if (m_nPrivilege >= JDPRIV_ADMIN)
		return TRUE;

	return nRequiredRole ? m_nRoleID == nRequiredRole : TRUE;
}

void CJDClientSession::OnUploadVersion(CLBPacket* pPacket)
{
	m_pProgWnd->Hide();
	int nFileID = pPacket->ReadInt();
	CString sFilePath = ReadPacketString(pPacket);
	CString sFileName = AfxGetFileName(sFilePath);
	CString sTempPath;

	m_pProgWnd->Show(_T("数据上传"), CString(_T("正在加密 ")) + sFileName);
	if (!Zip(sFileName, sFilePath, sTempPath))
	{
		m_pProgWnd->Hide();
		SendInvalidPacket();
		AfxMessageBox(_T("数据加密失败，上传取消。"), MB_ICONEXCLAMATION);
		return;
	}

	m_pProgWnd->SetText(CString(_T("正在上传 ")) + sFileName);

	int nResult = SendFile(sTempPath);
	::DeleteFile(sTempPath);
	m_pProgWnd->Hide();

	CString sMsg;
	if (nResult == CLBSession::FILE_ERROR_SUCCESS)
	{
		// Wait for JD_PACKET_VER_LIST
		Receive(pPacket);
		OnReceive(pPacket->GetPacketID(), pPacket);

		// Request file data change
		OUT_RDY;
		OUT_INT(nFileID);
		OUT_GO(JD_PACKET_REQUEST_FILE);

		sMsg = _T("版本数据上传成功");
		AfxMessageBox(sMsg, MB_ICONEXCLAMATION);
	}
	else
	{
		LookupFileError(nResult, sMsg);
		AfxMessageBox(sMsg, MB_ICONINFORMATION);
	}	
}

void CJDClientSession::OnDownloadVersion(CLBPacket *pPacket)
{	
	m_pProgWnd->Hide();
	
	int nVerID = pPacket->ReadInt();
	CString sFileName = ReadPacketString(pPacket);
	DWORD dwSize = pPacket->ReadInt();
	CString sExt;
	CString sFilter = GetFilter(sFileName, sExt);	

	CFileDialog dlg(FALSE, sExt, sFileName, OFN_OVERWRITEPROMPT | OFN_ENABLESIZING, sFilter);
	if (GetVersion() < 0x80000000)
		dlg.m_ofn.lStructSize = 88;

	if (dlg.DoModal() != IDOK)
	{
		SendInvalidPacket();
		return;
	}

	CString sPathName = dlg.GetPathName();
	CString sNewName = dlg.GetFileName();
	int nPos = sPathName.ReverseFind(_T('\\'));
	VERIFY(nPos != -1);
	CString sFolder = sPathName.Left(nPos) + _T('\\');	
	CString sTempPath = _TEMP(sNewName);

	m_pProgWnd->Show(_T("数据下载"), CString(_T("正在下载 ")) + sFileName);

	pPacket->PrepareForWrite();
	pPacket->Encode(JD_PACKET_VER_DOWNLOAD_DATA_BEGIN);
	Send(pPacket);
	
	int nResult = ReceiveFile(sTempPath);

	if (nResult == CLBSession::FILE_ERROR_SUCCESS)
	{
		m_pProgWnd->SetText(CString(_T("正在解密 ")) + sNewName);
		if (!Unzip(sTempPath, sFolder, sNewName))
			nResult = 0xffff;		
	}
	
	m_pProgWnd->Hide();

	if (nResult == CLBSession::FILE_ERROR_SUCCESS)
	{
		AfxMessageBox(_T("版本数据下载成功"), MB_ICONINFORMATION);
		/*
		CString sParamLine;
		sParamLine.Format(_T("/n, /select, %s"), (LPCTSTR)sPathName);
		::ShellExecute(NULL, _T("open"), _T("explorer.exe"), sParamLine, NULL, SW_SHOW);
		*/
	}
	else
	{
		CString sMsg;
		LookupFileError(nResult, sMsg);
		AfxMessageBox(sMsg, MB_ICONEXCLAMATION);
	}	
}

void CJDClientSession::TaskLogin(LPCTSTR lpszUserName, LPCTSTR lpszPassword)
{
	OUT_RDY;
	OUT_STR(lpszUserName);
	OUT_STR(lpszPassword);
	OUT_INT(APP_VERSION);
	OUT_GO(JD_PACKET_LOGIN);
}

void CJDClientSession::TaskChangePassword(LPCTSTR lpszOldPassword, LPCTSTR lpszNewPassword)
{
	OUT_RDY;
	OUT_STR(lpszOldPassword);
	OUT_STR(lpszNewPassword);
	OUT_GO(JD_PACKET_CHANGE_PASSWORD);
}

void CJDClientSession::TaskListUser()
{
	OUT_RDY;
	OUT_INT(0);
	OUT_GO(JD_PACKET_USER_LIST);
}

void CJDClientSession::TaskAddUser(int nRoleID, int nPrivilege, LPCTSTR lpszName)
{
	OUT_RDY;
	OUT_INT(nRoleID);
	OUT_INT(nPrivilege);
	OUT_STR(lpszName);
	OUT_GO(JD_PACKET_USER_ADD);
}

void CJDClientSession::TaskChangeUser(int nUserID, int nRoleID, int nPrivilege)
{
	OUT_RDY;
	OUT_INT(nUserID);
	OUT_INT(nRoleID);
	OUT_INT(nPrivilege);
	OUT_GO(JD_PACKET_USER_CHANGE);
}

void CJDClientSession::TaskResetPassword(int nUserID)
{
	OUT_RDY;
	OUT_INT(nUserID);
	OUT_GO(JD_PACKET_USER_RESETPASSWORD);
}

void CJDClientSession::TaskListRole()
{
	OUT_RDY;
	OUT_INT(0);
	OUT_GO(JD_PACKET_ROLE_LIST);
}


void CJDClientSession::TaskAddRole(LPCTSTR lpszName)
{
	OUT_RDY;
	OUT_STR(lpszName);
	OUT_GO(JD_PACKET_ROLE_ADD);
}

void CJDClientSession::TaskChangeRole(int nRoleID, LPCTSTR lpszNewName)
{
	OUT_RDY;
	OUT_INT(nRoleID);
	OUT_STR(lpszNewName);
	OUT_GO(JD_PACKET_ROLE_CHANGE);
}

void CJDClientSession::TaskListCategory(int nParentCatID)
{
	OUT_RDY;
	OUT_INT(nParentCatID);
	OUT_GO(JD_PACKET_CAT_LIST);
}

void CJDClientSession::TaskAddCategory(int nParentCatID, LPCTSTR lpszName, LPCTSTR lpszDesc)
{
	OUT_RDY;
	OUT_INT(nParentCatID);
	OUT_STR(lpszName);
	OUT_STR(lpszDesc);
	OUT_GO(JD_PACKET_CAT_ADD);
}

void CJDClientSession::TaskChangeCategory(int nCatID, LPCTSTR lpszNewName, LPCTSTR lpszNewDesc)
{
	OUT_RDY;
	OUT_INT(nCatID);
	OUT_STR(lpszNewName);
	OUT_STR(lpszNewDesc);
	OUT_GO(JD_PACKET_CAT_CHANGE);
}

void CJDClientSession::TaskChangeCategoryPriv(int nCatID, LPCTSTR lpszNewPriv, BOOL bApplySubCats)
{
	OUT_RDY;
	OUT_INT(nCatID);
	OUT_STR(lpszNewPriv);
	OUT_INT(bApplySubCats);
	OUT_GO(JD_PACKET_CAT_CHANGE_PRIV);
}

void CJDClientSession::TaskDeleteCategory(int nCatID)
{
	OUT_RDY;
	OUT_INT(nCatID);	
	OUT_GO(JD_PACKET_CAT_DELETE);
}

void CJDClientSession::TaskListFile(int nParentCatID)
{
	OUT_RDY;
	OUT_INT(nParentCatID);	
	OUT_GO(JD_PACKET_FILE_LIST);
}

void CJDClientSession::TaskAddFile(int nParentCatID, LPCTSTR lpszName, LPCTSTR lpszDesc)
{
	OUT_RDY;
	OUT_INT(nParentCatID);
	OUT_STR(lpszName);
	OUT_STR(lpszDesc);
	OUT_GO(JD_PACKET_FILE_ADD);
}

void CJDClientSession::TaskListVersion(int nFileID)
{
	OUT_RDY;
	OUT_INT(nFileID);	
	OUT_GO(JD_PACKET_VER_LIST);
}

void CJDClientSession::TaskCheckVersion(int nVerID)
{
	OUT_RDY;
	OUT_INT(nVerID);	
	OUT_GO(JD_PACKET_VER_CHECK);
}

void CJDClientSession::TaskUncheckVersion(int nVerID)
{
	OUT_RDY;
	OUT_INT(nVerID);	
	OUT_GO(JD_PACKET_VER_UNCHECK);
}

void CJDClientSession::TaskRequestRootCategory(int nRoleID)
{
	OUT_RDY;
	OUT_INT(nRoleID);	
	OUT_GO(JD_PACKET_CAT_ROOT);
}

void CJDClientSession::TaskUploadVersion(int nFileID, LPCTSTR lpszDesc, int nVersion, LPCTSTR lpszFilePath)
{
	OUT_RDY;
	OUT_INT(nFileID);
	OUT_INT(nVersion);
	OUT_STR(lpszDesc);
	OUT_STR(lpszFilePath);
	OUT_INT(TRUE);
	OUT_GO(JD_PACKET_VER_UPLOAD);
}

void CJDClientSession::TaskDownloadVersion(int nVersionID)
{
	OUT_RDY;
	OUT_INT(nVersionID);
	OUT_GO(JD_PACKET_VER_DOWNLOAD);
}

void CJDClientSession::TaskDeleteFile(int nFileID)
{
	OUT_RDY;
	OUT_INT(nFileID);
	OUT_GO(JD_PACKET_FILE_DELETE);
}

void CJDClientSession::TaskDeleteVersion(int nVerID)
{
	OUT_RDY;
	OUT_INT(nVerID);
	OUT_GO(JD_PACKET_VER_DELETE);
}

BOOL CJDClientSession::SendFileProc(LPCVOID lpszFilePath, DWORD dwBytesSent, DWORD dwBytesTotal, LPVOID lParam)
{
	return !IsStopping() && m_pProgWnd->SetProgress(dwBytesSent, dwBytesTotal);
}

BOOL CJDClientSession::ReceiveFileProc(LPCVOID lpszFilePath, DWORD dwBytesReceived, DWORD dwBytesTotal, LPVOID lParam)
{
	return !IsStopping() && m_pProgWnd->SetProgress(dwBytesReceived, dwBytesTotal);
}

void CJDClientSession::RegisterDeletedVerNotify(HWND hWnd)
{
	m_hDeletedVerNotify = hWnd;
}

void CJDClientSession::SendHeartbeat()
{
	if (IsIdle())
	{
		OUT_RDY;
		OUT_GO(JD_PACKET_HEARTBEAT);
	}	
}

void CJDClientSession::TaskCheckUpdate()
{
	OUT_RDY;
	OUT_INT(APP_VERSION);
	OUT_GO(JD_PACKET_CHECK_UPDATE);
}

void CJDClientSession::DownloadPatch(CLBPacket *pPacket)
{
	CString sFileName = ReadPacketString(pPacket);
	CString sFilePath = _TEMP(sFileName);

	m_pProgWnd->Show(_T("程序更新"), CString(_T("正在下载程序更新文件 ")) + sFileName);
	int nResult = ReceiveFile(sFilePath);
	m_pProgWnd->Hide();
	::Sleep(100);

	if (nResult == CLBSession::FILE_ERROR_SUCCESS)
	{
		// all good, install the patch now
		m_nUserID = 0;
		Abandon();
		m_sReserved = sFilePath;
		_POST(WM_PATCH_INSTALL, 0, (LPARAM)(LPCTSTR)m_sReserved);
	}
	else
	{	
		CString sMsg;
		LookupFileError(nResult, sMsg);
		AfxMessageBox(sMsg, MB_ICONEXCLAMATION);
	}	
}

void CJDClientSession::TaskUploadPatch(LPCTSTR lpszFileName, LPCTSTR lpszPath)
{
	if (m_nPrivilege < JDPRIV_SUPER_ADMIN)
		return;
	
	OUT_RDY;
	OUT_INT(APP_VERSION);
	OUT_STR(lpszFileName);
	OUT_STR(lpszPath);
	OUT_GO(JD_PACKET_UPLOAD_APP);
}

void CJDClientSession::UploadPatch(CLBPacket *pPacket)
{
	CString sFilePath = ReadPacketString(pPacket);
	CString sFileName = AfxGetFileName(sFilePath);	
	
	m_pProgWnd->Show(_T("安装文件上传"), CString(_T("正在上传程序更新文件 ") + sFileName));
	int nResult = SendFile(sFilePath);
	m_pProgWnd->Hide();
	::Sleep(100);

	if (nResult != CLBSession::FILE_ERROR_SUCCESS)
	{
		CString sMsg;
		LookupFileError(nResult, sMsg);
		AfxMessageBox(sMsg, MB_ICONEXCLAMATION);
		return;
	}

	// Wait for JD_PACKET_UPLOAD_APP_RESULT
	if (Receive(pPacket))
		OnReceive(pPacket->GetPacketID(), pPacket);
}

void CJDClientSession::TaskPatchApp(int nPatchID)
{
	OUT_RDY;
	OUT_INT(nPatchID);
	OUT_GO(JD_PACKET_UPDATE_DOWNLOAD);
}

CString CJDClientSession::GetPassword() const
{
	return m_sPassword;
}

void CJDClientSession::TaskCatInitStart(int nParentCatID, LPCTSTR lpszCatName, LPCTSTR lpszClientPath)
{
	OUT_RDY;
	OUT_INT(nParentCatID);
	OUT_STR(lpszCatName);
	OUT_STR(lpszClientPath);
	OUT_GO(JD_PACKET_CAT_INIT_START);
}

void CJDClientSession::InitCategory(CLBPacket *pPacket)
{	
	int nCatID = pPacket->ReadInt();
	CString sCatName = ReadPacketString(pPacket);
	CString sPathName = ReadPacketString(pPacket);
	CString sTitle;
	sTitle.Format(_T("目录初始化 - %s"), (LPCTSTR)sCatName);
	m_pProgWnd->Show(sTitle, sCatName);
	BatchInit(nCatID, sPathName.Find(sCatName), sCatName, sPathName);
	OUT_RDY;
	OUT_GO(JD_PACKET_CAT_INIT_END);
	m_pProgWnd->Hide();
}

// Behold, the sheer magic of recursive!
void CJDClientSession::BatchInit(int nParentCatID, int nStartPos, LPCTSTR lpszFolderName, LPCTSTR lpszPath)
{
	CFileFind ff;
	BOOL bOK = ff.FindFile(CString(lpszPath) + _T("\\*.*"));
	while (bOK && !IsStopping() && !m_pProgWnd->Cancelled())
	{
		bOK = ff.FindNextFile();
		if (ff.IsDots())
			continue;

		CString sName = ff.GetFileName();
		CString sPath = ff.GetFilePath();
		m_pProgWnd->SetText(sPath.Mid(nStartPos));

		if (ff.IsDirectory())
		{	
			CLBPacket packet;
			packet.PrepareForWrite();
			packet.WriteInt(nParentCatID);
			packet.WriteString(sName);
			packet.Encode(JD_PACKET_CAT_INIT_CAT);
			Send(&packet);
			Receive(&packet);
			ASSERT(packet.GetPacketID() == JD_PACKET_CAT_INIT_CAT);
			int nCatID = packet.ReadInt();
			BatchInit(nCatID, nStartPos, sName, sPath);
		}
		else
		{
			// upload file under nCatID
			CLBPacket packet;
			packet.PrepareForWrite();
			packet.WriteInt(nParentCatID);
			packet.WriteString(sName);
			packet.Encode(JD_PACKET_CAT_INIT_FILE);
			Send(&packet);
			Receive(&packet); // wait for JD_PACKET_CAT_INIT_FILE
			ASSERT(packet.GetPacketID() == JD_PACKET_CAT_INIT_FILE);
			CString sTemp;
			Zip(sName, sPath, sTemp);
			SendFile(sTemp);
			Receive(&packet); // Wait for JD_PACKET_CAT_INIT_FILE_OK
			ASSERT(packet.GetPacketID() == JD_PACKET_CAT_INIT_FILE_OK);
		}
	}
}

void CJDClientSession::TaskRequestFile(int nFileID)
{
	OUT_RDY;
	OUT_INT(nFileID);
	OUT_GO(JD_PACKET_REQUEST_FILE);
}

void CJDClientSession::TaskRequestCategory(int nCatID)
{
	OUT_RDY;
	OUT_INT(nCatID);
	OUT_GO(JD_PACKET_REQUEST_CAT);
}

void CJDClientSession::TaskPasteItem(BOOL bIsFile, int nID, int nParentCatID)
{
	OUT_RDY;
	OUT_INT(nID);
	OUT_INT(nParentCatID);
	OUT_GO(bIsFile ? JD_PACKET_PASTE_FILE : JD_PACKET_PASTE_CAT);
}

void CJDClientSession::TaskRequestUploadVer(int nFileID)
{
	OUT_RDY;
	OUT_INT(nFileID);
	OUT_GO(JD_PACKET_REQUEST_UPLOAD_VER);
}

void CJDClientSession::TaskCancelRequestUploadVer()
{
	OUT_RDY;
	OUT_GO(JD_PACKET_CANCEL_REQUEST_UPLOAD_VER);
}

void CJDClientSession::TaskRequestManageVer(int nFileID)
{
	OUT_RDY;
	OUT_INT(nFileID);
	OUT_GO(JD_PACKET_FILE_MANAGE_VER_REQUEST);
}

void CJDClientSession::TaskCancelRequestManageVer()
{
	TaskCancelRequestUploadVer();
}

void CJDClientSession::TaskApplyVerManage(int nFileID, int nVersion)
{
	OUT_RDY;
	OUT_INT(nFileID);
	OUT_INT(nVersion);
	OUT_GO(JD_PACKET_FILE_MANAGE_VER_APPLY);
}

void CJDClientSession::TaskRequestRecovery(int nRoleID, LPCTSTR lpszFileName)
{
	OUT_RDY;
	OUT_INT(nRoleID);
	OUT_STR(lpszFileName);
	OUT_GO(JD_PACKET_DATA_RECOVERY_LIST);
}

void CJDClientSession::TaskRecoverData(int nVersionID)
{
	OUT_RDY;
	OUT_INT(nVersionID);
	OUT_GO(JD_PACKET_RECOVERY);	
}

void CJDClientSession::OnRecoverData(CLBPacket *pPacket)
{
	CString sFileName = ReadPacketString(pPacket);
	CString sExt;
	CString sFilter = GetFilter(sFileName, sExt);

	CFileDialog dlg(FALSE, sExt, sFileName, OFN_OVERWRITEPROMPT | OFN_ENABLESIZING, sFilter);
	if (GetVersion() < 0x80000000)
		dlg.m_ofn.lStructSize = 88;

	if (dlg.DoModal() != IDOK)
	{
		SendInvalidPacket();
		return;
	}

	CString sPath = dlg.GetPathName();

	CLBPacket packet;
	packet.PrepareForWrite();
	packet.Encode(JD_PACKET_RECOVERY_DOWNLOAD);
	if (!Send(&packet))
		return;

	m_pProgWnd->Show(_T("数据恢复"), CString(_T("正在下载 ")) + sFileName);
	int nResult = ReceiveFile(sPath);
	m_pProgWnd->Hide();
	::Sleep(100);

	if (nResult == CLBSession::FILE_ERROR_SUCCESS)
	{
		AfxMessageBox(_T("数据恢复成功"), MB_ICONINFORMATION);
		CString sParamLine;
		sParamLine.Format(_T("/n, /select, %s"), (LPCTSTR)sPath);
		::ShellExecute(NULL, _T("open"), _T("explorer.exe"), sParamLine, NULL, SW_SHOW);
	}
	else
	{	
		CString sMsg;
		LookupFileError(nResult, sMsg);
		AfxMessageBox(sMsg, MB_ICONEXCLAMATION);
	}	
}

CString CJDClientSession::GetZipPassword() const
{
	// _T("{E44112D1-49CB-4A68-92AE-3A5F67808A4B}") just don't want it to appear in the exe file
	CString sPassword;
	sPassword.Format(_T("{%X-%X-%X-%X-%X%X}"), 0xE44112D1, 0x49CB, 0x4A68, 0x92AE, 0x3A5F, 0x67808A4B);
	return sPassword;
}

BOOL CJDClientSession::Zip(LPCTSTR lpszFileName, LPCTSTR lpszFilePath, CString& sTemp)
{
	CZipArchive zip;
	sTemp = _TEMP(lpszFileName);
	BOOL bResult = FALSE;
	try
	{
		zip.Open(sTemp, CZipArchive::create);
		zip.SetPassword(GetZipPassword());
		bResult = zip.AddNewFile(lpszFilePath);
		zip.Close();
	}
	catch (...)
	{
	}	
	return bResult;
}

BOOL CJDClientSession::Unzip(LPCTSTR lpszSrc, LPCTSTR lpszDestFolder, LPCTSTR lpszFileName)
{
	CZipArchive zip;
	BOOL bResult = FALSE;
	try
	{
		zip.Open(lpszSrc);
		zip.SetPassword(GetZipPassword());
		bResult = zip.ExtractFile(0, lpszDestFolder, TRUE, lpszFileName);
		zip.Close();
	}
	catch (...)
	{
		if (AfxMessageBox(_T("文件无法解密。如果此文件由旧版本客户端上传则无须解密，是否尝试不经解密直接使用？"), MB_ICONEXCLAMATION | MB_YESNO) == IDYES)
			bResult = ::CopyFile(lpszSrc, CString(lpszDestFolder) + _T("\\") + lpszFileName, FALSE);
	}
	
	::DeleteFile(lpszSrc);
	return bResult;
}

void CJDClientSession::TaskDelTree(int nCatID)
{
	OUT_RDY;
	OUT_INT(nCatID);
	OUT_GO(JD_PACKET_DELTREE);
}

void CJDClientSession::TaskDownloadTreeRequest(int nCatID, LPCTSTR lpszClientPath)
{
	OUT_RDY;
	OUT_INT(nCatID);
	OUT_STR(lpszClientPath);
	OUT_GO(JD_PACKET_FILESYSTEM_REQUEST);
	m_pProgWnd->Show(_T("目录树下载"), _T("下载请求已发送，请等待服务器回应......"));	
}

void CJDClientSession::OnDownloadTreeReady(CLBPacket *pPacket)
{
	CString sClientPath = ReadPacketString(pPacket);
	CString sCatName = ReadPacketString(pPacket);
	int nCatCount = pPacket->ReadInt();
	int nFileCount = pPacket->ReadInt();

	m_dtm.Initialize(nCatCount, nFileCount, sClientPath);
	m_pProgWnd->Show(_T("目录树下载"), _T("准备接受目录树结构"));
	OUT_RDY;
	OUT_GO(JD_PACKET_FILESYSTEM_NEXT);
}

void CJDClientSession::OnDownloadTreeNext(CLBPacket* pPacket)
{	
	if (IsStopping() || m_pProgWnd->Cancelled())
		return;

	BOOL bHasMore = pPacket->ReadInt();
	DWORD dwFileID = pPacket->ReadInt();
	DWORD dwCatID = pPacket->ReadInt();
	CString sPathOrName = ReadPacketString(pPacket);

	if (dwFileID)
	{
		CString sFullPath;
		m_dtm.LookupFullPath(dwCatID, sFullPath);
		m_pProgWnd->SetText(CString(_T("接收文件：")) + sPathOrName);

		CString sTemp = _TEMP(sPathOrName);
		if (ReceiveFile(sTemp) == FILE_ERROR_SUCCESS)
			Unzip(sTemp, sFullPath, sPathOrName);
	}
	else if (dwCatID)
	{
		m_dtm.AddCategory(dwCatID, sPathOrName);
		m_pProgWnd->SetText(CString(_T("创建目录：")) + sPathOrName);
	}

	OUT_RDY;

	if (bHasMore)
	{		
		OUT_GO(JD_PACKET_FILESYSTEM_NEXT);	
	}
	else
	{
		OUT_GO(JD_PACKET_FILESYSTEM_FINISH);
		m_dtm.Initialize(0, 0, NULL);
		m_pProgWnd->Hide();
		SendInvalidPacket();
		AfxMessageBox(_T("目录树下载结束。"), MB_ICONINFORMATION);
	}	
}
