// JDSession.cpp: implementation of the CJDSession class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JDSession.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#pragma comment(lib, "Rpcrt4.lib")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJDSession::CJDSession(LPCTSTR lpszConnectString, LPCTSTR lpszRootFolder) : m_rs(&m_db)
{
	*m_szPeerName = 0;
	m_sConnectString = lpszConnectString;
	m_nUserID = 0;
	m_nRoleID = 0;
	m_nPrivilege = 0;
	m_nClientVer = 0;
	ASSERT(lpszConnectString && *lpszConnectString);
	m_db.SetConnectionString(lpszConnectString);
	m_sRootPath = lpszRootFolder;
	m_connectionTime = COleDateTime::GetCurrentTime();
}

CJDSession::~CJDSession() 
{
	Stop();
}

void CJDSession::OnStart()
{
	::CoInitialize(NULL);
}

void CJDSession::OnStop()
{
	::CoUninitialize();
}

void CJDSession::OnConnect()
{
	LBNetworkFormatIP(GetPeerAddr(), m_szPeerName);
}

LPCTSTR CJDSession::GetPeerName() const
{
	return m_szPeerName;
}

DWORD CJDSession::Execute(LPCTSTR lpszSQL, ...)
{
	if (!m_db.IsOpen() && !m_db.Open())
		return JDRESPONSE_INTERNAL_ERROR;

	va_list args;
	va_start(args, lpszSQL);
	CString sSql;
	sSql.FormatV(lpszSQL, args);
	va_end(args);

	if (!m_db.Execute((LPCTSTR)sSql))
		return JDRESPONSE_INTERNAL_ERROR;

	if (m_db.GetRecordsAffected() < 1)
		return JDRESPONSE_NOT_EXISTS;

	return JDRESPONSE_SUCCESS;
}

DWORD CJDSession::Query(LPCTSTR lpszSQL, ...)
{
	if (!m_db.IsOpen() && !m_db.Open())
		return JDRESPONSE_INTERNAL_ERROR;

	va_list args;
	va_start(args, lpszSQL);
	CString sSql;
	sSql.FormatV(lpszSQL, args);
	va_end(args);

	if (!m_rs.Open((LPCTSTR)sSql))
		return JDRESPONSE_INTERNAL_ERROR;

	if (m_rs.IsEOF())
		return JDRESPONSE_NOT_EXISTS;

	return JDRESPONSE_SUCCESS;
}

BOOL CJDSession::InsertRecord(LPCTSTR lpszTableName)
{
	if (!m_db.IsOpen() && !m_db.Open())
		return FALSE;

	CString sSql;
	sSql.Format(JDB_QUERY_ALL, lpszTableName);
	if (!m_rs.Open((LPCTSTR)sSql))
		return FALSE;

	return m_rs.AddNew();
}

CString CJDSession::ReadString(CLBPacket* pPacket)
{	
	CString sText;
	pPacket->ReadString(sText.GetBuffer(MAX_PATH + 1), MAX_PATH);
	sText.ReleaseBuffer();
	return sText;
}

CString CJDSession::GetUserName() const
{
	return m_sUserName;
}

CString CJDSession::GetRoleName() const
{
	return m_sRoleName;
}

COleDateTime CJDSession::GetConnectionTime() const
{
	return m_connectionTime;
}

BOOL CJDSession::VerifyPermission(DWORD nRoleID, int nType) const
{
	if (m_nPrivilege < JDPRIV_USER)
		return FALSE;
	
	if (m_nPrivilege >= JDPRIV_ADMIN)
		return TRUE;
	
	if (nRoleID && nRoleID != nRoleID)
		return FALSE;

	if (nType == PERMISSION_ADMIN)
		return m_nPrivilege >= JDPRIV_ADMIN;

	if (nType == PERMISSION_SUPER)
		return m_nPrivilege >= JDPRIV_SUPERVISOR;

	return TRUE;
}

void CJDSession::CloseDB()
{
	m_rs.Close();
	m_db.Close();
}

void CJDSession::SendErrorPacket(DWORD dwPacketID, DWORD dwError, int nValue)
{
	CLBPacket packet;
	packet.PrepareForWrite();
	packet.WriteDword(dwError);
	packet.WriteInt(nValue);
	packet.Encode(dwPacketID);
	Send(&packet);
}

DWORD CJDSession::TranslateResult(DWORD dwResult, DWORD dwReturnIfExists)
{
	if (dwResult == JDRESPONSE_SUCCESS)
		return dwReturnIfExists;

	if (dwResult == JDRESPONSE_NOT_EXISTS)
		return JDRESPONSE_SUCCESS;

	return dwResult;
}

#define SVR_PROC(x)		dwResult = x(dwPacketID, pPacket);
#define REQ_PROC(x,y)	dwResult = RequestData(dwPacketID, pPacket, (x), (y));
void CJDSession::OnReceive(DWORD dwPacketID, CLBPacket* pPacket)
{	
	// In release version no packets are processed before login
	if (!m_nUserID && dwPacketID != JD_PACKET_LOGIN)
	{
		SendErrorPacket(dwPacketID, JDRESPONSE_DENY, 0);
		return;
	}
	
	DWORD dwResult = JDRESPONSE_DATA_ERROR;
	switch (dwPacketID)
	{

	case JD_PACKET_LOGIN:
		SVR_PROC(UserLogin);
		break;

	case JD_PACKET_CHANGE_PASSWORD:
		SVR_PROC(ChangePassword);
		break;

	case JD_PACKET_USER_LIST:
		SVR_PROC(ListUsers);
		break;

	case JD_PACKET_USER_ADD:
		SVR_PROC(AddUser);
		break;

	case JD_PACKET_USER_CHANGE:
		SVR_PROC(ChangeUser);
		break;

	case JD_PACKET_USER_RESETPASSWORD:
		SVR_PROC(ResetPassword);
		break;

	case JD_PACKET_ROLE_LIST:
		SVR_PROC(ListRole);
		break;

	case JD_PACKET_ROLE_ADD:
		SVR_PROC(AddRole);
		break;

	case JD_PACKET_ROLE_CHANGE:
		SVR_PROC(ChangeRole);
		break;

	case JD_PACKET_CAT_LIST:
		SVR_PROC(ListCategory);
		break;

	case JD_PACKET_CAT_ADD:
		SVR_PROC(AddCategory);
		break;

	case JD_PACKET_CAT_CHANGE:
		SVR_PROC(ChangeCategory);
		break;

	case JD_PACKET_CAT_CHANGE_PRIV:
		SVR_PROC(ChangeCategoryPriv);
		break;

	case JD_PACKET_CAT_DELETE:
		SVR_PROC(DeleteCategory);
		break;	

	case JD_PACKET_CAT_ROOT:
		SVR_PROC(RequestRootCategory);
		break;

	case JD_PACKET_FILE_LIST:
		SVR_PROC(ListFile);
		break;

	case JD_PACKET_FILE_ADD:
		SVR_PROC(AddFile);
		break;
		
	case JD_PACKET_FILE_DELETE:
		SVR_PROC(DeleteFile);
		break;

	case JD_PACKET_REQUEST_USER:
		REQ_PROC(JDB_REQUEST_USER, JD_ACTION_USER);
		break;

	case JD_PACKET_REQUEST_ROLE:
		REQ_PROC(JDB_REQUEST_ROLE, JD_ACTION_ROLE);
		break;

	case JD_PACKET_REQUEST_CAT:
		REQ_PROC(JDB_REQUEST_CATEGORY, JD_ACTION_CAT);
		break;

	case JD_PACKET_REQUEST_FILE:
		REQ_PROC(JDB_REQUEST_FILE, JD_ACTION_FILE);
		break;

	case JD_PACKET_REQUEST_VER:
		REQ_PROC(JDB_REQUEST_VER, JD_ACTION_VER);
		break;

	case JD_PACKET_VER_UPLOAD:
		SVR_PROC(UploadVer);
		break;

	case JD_PACKET_VER_LIST:
		SVR_PROC(ListVersion);
		break;

	case JD_PACKET_VER_DOWNLOAD:
		SVR_PROC(DownloadVer);
		break;

	case JD_PACKET_VER_CHECK:
		SVR_PROC(CheckVersion);
		break;

	case JD_PACKET_VER_UNCHECK:
		SVR_PROC(UncheckVersion);
		break;

	case JD_PACKET_VER_DELETE:
		SVR_PROC(DeleteVersion);
		break;	

	case JD_PACKET_UPLOAD_APP:
		SVR_PROC(UploadPatch);
		break;

	case JD_PACKET_CHECK_UPDATE:
		SVR_PROC(CheckPatch);
		break;

	case JD_PACKET_UPDATE_DOWNLOAD:
		SVR_PROC(DownloadPatch);
		break;

	case JD_PACKET_CAT_INIT_START:
		SVR_PROC(CatInitStart);
		break;

	case JD_PACKET_PASTE_FILE:
		SVR_PROC(PasteFile);
		break;

	case JD_PACKET_PASTE_CAT:
		SVR_PROC(PasteCategory);
		break;

	case JD_PACKET_REQUEST_UPLOAD_VER:
		SVR_PROC(RequestUploadVer);
		break;

	case JD_PACKET_CANCEL_REQUEST_UPLOAD_VER:
		SVR_PROC(CancelRequestUploadVer);
		break;

	case JD_PACKET_FILE_MANAGE_VER_REQUEST:
		SVR_PROC(RequestManagerVer);
		break;

	case JD_PACKET_FILE_MANAGE_VER_APPLY:
		SVR_PROC(ApplyManagerVer);
		break;

	case JD_PACKET_DATA_RECOVERY_LIST:
		SVR_PROC(ListRecovable);
		break;

	case JD_PACKET_RECOVERY:
		SVR_PROC(RecoverData);
		break;

	case JD_PACKET_DELTREE:
		SVR_PROC(DelTree);
		break;

	case JD_PACKET_FILESYSTEM_REQUEST:
		SVR_PROC(DownloadFileSystemStructure);
		break;	

	case JD_PACKET_FILESYSTEM_NEXT:
		SVR_PROC(SendFileSystemNext);
		break;	

	case JD_PACKET_FILESYSTEM_FINISH:
		SVR_PROC(FinishFileSystemStructure);
		break;

	default:
		break;
	}

	CloseDB();

	if (dwResult != JDRESPONSE_SUCCESS)
	{
		pPacket->ResetCursor();
		SendErrorPacket(dwPacketID, dwResult, pPacket->ReadInt());
	}
}

DWORD CJDSession::CompareUserPrivilege(DWORD nUserID)
{	
	AUTO_CLOSE;
	DWORD dwResult = Query(JDB_REQUEST_USER, nUserID);
	if (dwResult != JDRESPONSE_SUCCESS)
		return dwResult;

	DWORD nPrivilege = 0;
	m_rs.GetFieldValue(_T("FPrivID"), nPrivilege);
	if (nPrivilege < 0)
		return JDRESPONSE_INTERNAL_ERROR;

	if (nPrivilege >= m_nPrivilege)
		return JDRESPONSE_DENY;
	
	return JDRESPONSE_SUCCESS;
}

BOOL CJDSession::SendFileProc(LPCVOID lpszFilePath, DWORD dwBytesSent, DWORD dwBytesTotal, LPVOID lParam)
{
	return !IsStopping();
}

BOOL CJDSession::ReceiveFileProc(LPCVOID lpszFilePath, DWORD dwBytesReceived, DWORD dwBytesTotal, LPVOID lParam)
{
	return !IsStopping();
}

CString CJDSession::GenerateGUID()
{
	UUID uuid = { 0 };
	::UuidCreate(&uuid);
	CString sGUID;
	sGUID.Format(_T("%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X"), uuid.Data1, uuid.Data2, uuid.Data3, uuid.Data4[0], uuid.Data4[1], uuid.Data4[2], uuid.Data4[3], uuid.Data4[4], uuid.Data4[5], uuid.Data4[6], uuid.Data4[7]);
	return sGUID;
}

void CJDSession::OnDisconnect()
{	
	UnlockFilesForUser();
	m_nUserID = 0;
	m_nRoleID = 0;
	m_nPrivilege = 0;
	m_nClientVer = 0;
	_LOG("User logout: %s", (LPCTSTR)m_sUserName);
}

void CJDSession::UnlockFilesForUser()
{
	Execute(JDB_FILE_UNLOCK, m_nUserID);
}
