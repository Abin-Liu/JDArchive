#include "stdafx.h"
#include "JDSession.h"
#include "JDDatabase.h"
#include <Rpcdce.h>

#define OUT_RDY		CLBPacket packet; packet.PrepareForWrite(); packet.WriteDword(dwResult);
#define OUT_INT(x)	packet.WriteInt(x);
#define OUT_STR(x)	packet.WriteString(x);
#define OUT_DAT(x)	packet.WriteData(&(x), sizeof(x))
#define OUT_GO		packet.Encode(dwPacketID); Send(&packet);

#define VERIFY_PERMISSION(x, y) if (!VerifyPermission((x), (y))) { return JDRESPONSE_DENY; }
#define VERIFY_RESULT			if (dwResult != JDRESPONSE_SUCCESS)	{ return dwResult; }

DWORD CJDSession::PackUserData(CLBPacket& packet)
{
	DWORD nUserID, nRoleID, nPriv;
	CString sName;
	
	m_rs.GetFieldValue(_T("FID"), nUserID);
	m_rs.GetFieldValue(_T("FRoleID"), nRoleID);
	m_rs.GetFieldValue(_T("FPrivID"), nPriv);	
	m_rs.GetFieldValue(_T("FName"), sName);	
	
	OUT_INT(nUserID);
	OUT_INT(nRoleID);
	OUT_INT(nPriv);	
	OUT_STR(sName);
	return nUserID;
}

DWORD CJDSession::PackRoleData(CLBPacket &packet)
{
	DWORD nRoleID;
	CString sName;
	m_rs.GetFieldValue(_T("FID"), nRoleID);
	m_rs.GetFieldValue(_T("FName"), sName);	
	packet.WriteInt(nRoleID);
	packet.WriteString(sName);	
	return nRoleID;
}

DWORD CJDSession::PackCatData(CLBPacket &packet)
{
	DWORD nCatID, nParentCatID, nRoleID;
	CString sName, sDesc, sPriv;

	m_rs.GetFieldValue(_T("FID"), nCatID);
	m_rs.GetFieldValue(_T("FParentID"), nParentCatID);
	m_rs.GetFieldValue(_T("FRoleID"), nRoleID);
	m_rs.GetFieldValue(_T("FName"), sName);
	m_rs.GetFieldValue(_T("FDesc"), sDesc);
	m_rs.GetFieldValue(_T("FPrivilege"), sPriv);
	
	OUT_INT(nCatID);
	OUT_INT(nParentCatID);
	OUT_INT(nRoleID);
	OUT_STR(sName);
	OUT_STR(sDesc);
	OUT_STR(sPriv);
	return nCatID;
}

DWORD CJDSession::PackFileData(CLBPacket &packet)
{
	DWORD nFileID, nParentCatID, nVersion;
	CString sName, sDesc, sExt;
	m_rs.GetFieldValue(_T("FID"), nFileID);
	m_rs.GetFieldValue(_T("FParentID"), nParentCatID);
	m_rs.GetFieldValue(_T("FName"), sName);
	m_rs.GetFieldValue(_T("FDesc"), sDesc);
	m_rs.GetFieldValue(_T("FVersion"), nVersion);
	m_rs.GetFieldValue(_T("FExt"), sExt);

	OUT_INT(nFileID);
	OUT_INT(nParentCatID);
	OUT_STR(sName);
	OUT_STR(sDesc);
	OUT_INT(nVersion);
	OUT_STR(sExt);
	return nFileID;
}

DWORD CJDSession::PackVerData(CLBPacket &packet)
{
	DWORD nVerID, nFileID, nVersion, nCreatorID, nCheckerID;
	DWORD dwSize;
	CString sDesc;
	COleDateTime dtCreate, dtCheck;

	m_rs.GetFieldValue(_T("FID"), nVerID);
	m_rs.GetFieldValue(_T("FFileID"), nFileID);
	m_rs.GetFieldValue(_T("FSize"), dwSize);
	m_rs.GetFieldValue(_T("FVersion"), nVersion);
	m_rs.GetFieldValue(_T("FCreatorID"), nCreatorID);
	m_rs.GetFieldValue(_T("FCreateDate"), dtCreate);
	m_rs.GetFieldValue(_T("FCheckerID"), nCheckerID);
	m_rs.GetFieldValue(_T("FCheckDate"), dtCheck);
	m_rs.GetFieldValue(_T("FDesc"), sDesc);

	OUT_INT(nVerID);
	OUT_INT(nFileID);
	OUT_INT(dwSize);
	OUT_INT(nVersion);
	OUT_INT(nCreatorID);
	OUT_DAT(dtCreate);
	OUT_INT(nCheckerID);
	OUT_DAT(dtCheck);
	OUT_STR(sDesc);
	return nVerID;
}

DWORD CJDSession::PackRecovableData(CLBPacket &packet)
{
	DWORD nVerID, nVersion, nCreatorID, nDeleterID;
	DWORD dwSize;
	CString sFileName, sDesc;
	COleDateTime dtCreate, dtDelete;

	m_rs.GetFieldValue(_T("FID"), nVerID);
	m_rs.GetFieldValue(_T("FOrigFileName"), sFileName);
	m_rs.GetFieldValue(_T("FSize"), dwSize);
	m_rs.GetFieldValue(_T("FVersion"), nVersion);
	m_rs.GetFieldValue(_T("FCreatorID"), nCreatorID);
	m_rs.GetFieldValue(_T("FCreateDate"), dtCreate);
	m_rs.GetFieldValue(_T("FDeleterID"), nDeleterID);
	m_rs.GetFieldValue(_T("FDeleteDate"), dtDelete);
	m_rs.GetFieldValue(_T("FDesc"), sDesc);

	OUT_INT(nVerID);
	OUT_STR(sFileName);
	OUT_INT(dwSize);
	OUT_INT(nVersion);
	OUT_INT(nCreatorID);
	OUT_DAT(dtCreate);
	OUT_INT(nDeleterID);
	OUT_DAT(dtDelete);
	OUT_STR(sDesc);
	return nVerID;
}

DWORD CJDSession::RequestCategory(DWORD nCatID)
{
	return Query(JDB_REQUEST_CATEGORY, nCatID, m_nPrivilege, JDPRIV_ADMIN, m_nRoleID, m_nRoleID);
}

DWORD CJDSession::RequestVersion(DWORD nVersionID)
{
	return Query(JDB_REQUEST_VER, nVersionID, m_nUserID, m_nRoleID, m_nPrivilege, JDPRIV_SUPERVISOR);
}

DWORD CJDSession::UserLogin(DWORD dwPacketID, CLBPacket* pPacket)
{	
	CString sName = ReadString(pPacket);
	CString sPassword = ReadString(pPacket);
	int nVersion = pPacket->ReadInt();

	// Beta versions no longer allowed
	if (nVersion < 120)
		return JDRESPONSE_DENY;

	AUTO_CLOSE;	
	DWORD dwResult = Query(JDB_LOGIN, (LPCTSTR)sName, (LPCTSTR)sPassword);
	if (dwResult == JDRESPONSE_NOT_EXISTS)
		dwResult = JDRESPONSE_LOGIN_FAIL;
	VERIFY_RESULT;

	m_nClientVer = nVersion;
	m_sUserName = sName;
	m_rs.GetFieldValue(_T("FID"), m_nUserID);
	m_rs.GetFieldValue(_T("FRoleID"), m_nRoleID);
	m_rs.GetFieldValue(_T("FPrivID"), m_nPrivilege);
	m_rs.GetFieldValue(_T("FRoleName"), m_sRoleName);	
	CloseDB();
	
	_LOG("User login: %s, client version: %d", (LPCTSTR)sName, nVersion);

	OUT_RDY;
	OUT_INT(m_nUserID);
	OUT_INT(m_nRoleID);
	OUT_INT(m_nPrivilege);
	OUT_STR(sName);
	OUT_STR(sPassword);
	OUT_STR(m_sRoleName);
	OUT_GO;
	return JDRESPONSE_SUCCESS;
}

DWORD CJDSession::ChangePassword(DWORD dwPacketID, CLBPacket* pPacket)
{
	AUTO_CLOSE;
	CString sOldPassword = ReadString(pPacket);
	CString sNewPassword = ReadString(pPacket);
	DWORD dwResult = Query(JDB_VERYFY_PASSWORD, m_nUserID, (LPCTSTR)sOldPassword);
	if (dwResult == JDRESPONSE_NOT_EXISTS)
		dwResult = JDRESPONSE_DENY;
	VERIFY_RESULT;

	dwResult = Execute(JDB_CHANGE_PASSWORD, (LPCTSTR)sNewPassword, m_nUserID);	
	VERIFY_RESULT;

	CloseDB();
	OUT_RDY;
	OUT_STR(sNewPassword);
	OUT_GO;
	return JDRESPONSE_SUCCESS;
}

DWORD CJDSession::ListUsers(DWORD dwPacketID, CLBPacket *pPacket)
{	
	AUTO_CLOSE;
	DWORD dwResult = Query(JDB_LIST_USER);
	VERIFY_RESULT;

	SendSequencial(dwPacketID, m_rs.GetRecordCount(), (LPVOID)JD_ACTION_USER);
	return JDRESPONSE_SUCCESS;
}

DWORD CJDSession::AddUser(DWORD dwPacketID, CLBPacket *pPacket)
{
	VERIFY_PERMISSION(0, PERMISSION_ADMIN);	

	DWORD nRoleID = pPacket->ReadInt();
	DWORD nPriv = pPacket->ReadInt();
	if (nPriv >= m_nPrivilege)
		return JDRESPONSE_DENY;
	
	CString sName = ReadString(pPacket);
	if (sName.IsEmpty())
		return JDRESPONSE_DATA_ERROR;

	AUTO_CLOSE;
	DWORD dwResult = Query(JDB_REQUEST_ROLE, nRoleID);
	VERIFY_RESULT; // Role not exists

	dwResult = Query(JDB_CHECK_USER_NAME, (LPCTSTR)sName);
	dwResult = TranslateResult(dwResult);
	VERIFY_RESULT; // Name already taken

	if (!InsertRecord(JDB_TABLE_USER))
		return JDRESPONSE_INTERNAL_ERROR;

	m_rs.SetFieldValue(_T("FName"), sName);
	m_rs.SetFieldValue(_T("FRoleID"), nRoleID);
	m_rs.SetFieldValue(_T("FPrivID"), nPriv);
	if (!m_rs.Update())
		return JDRESPONSE_INTERNAL_ERROR;

	OUT_RDY;
	DWORD nUserID = PackUserData(packet);
	CloseDB();
	OUT_GO;
	return JDRESPONSE_SUCCESS;
}

DWORD CJDSession::ChangeUser(DWORD dwPacketID, CLBPacket *pPacket)
{
	VERIFY_PERMISSION(0, PERMISSION_ADMIN);
	AUTO_CLOSE;

	DWORD nUserID = pPacket->ReadInt();
	DWORD nRoleID = pPacket->ReadInt();
	DWORD nPriv = pPacket->ReadInt();
	
	DWORD dwResult = CompareUserPrivilege(nUserID);
	VERIFY_RESULT;

	dwResult = Query(JDB_REQUEST_ROLE, nRoleID);
	VERIFY_RESULT;
	
	dwResult = Query(JDB_REQUEST_USER, nUserID);
	VERIFY_RESULT;

	BOOL nOldPriv = TRUE;
	m_rs.GetFieldValue(_T("FPrivID"), nOldPriv);
	if (nOldPriv >= JDPRIV_SUPER_ADMIN)
		return JDRESPONSE_DENY;

	m_rs.Edit();
	m_rs.SetFieldValue(_T("FRoleID"), nRoleID);
	m_rs.SetFieldValue(_T("FPrivID"), nPriv);
	if (!m_rs.Update())
		return JDRESPONSE_INTERNAL_ERROR;

	OUT_RDY;
	PackUserData(packet);
	CloseDB();
	OUT_GO;
	return JDRESPONSE_SUCCESS;
}

DWORD CJDSession::ResetPassword(DWORD dwPacketID, CLBPacket *pPacket)
{
	VERIFY_PERMISSION(0, PERMISSION_ADMIN);
	AUTO_CLOSE;
	DWORD nUserID = pPacket->ReadInt();
	DWORD dwResult = CompareUserPrivilege(nUserID);
	VERIFY_RESULT;
	
	dwResult = Execute(JDB_RESET_PASSWORD, nUserID);
	VERIFY_RESULT;

	OUT_RDY;
	OUT_INT(nUserID);
	OUT_GO;
	return JDRESPONSE_SUCCESS;
}

DWORD CJDSession::ListRole(DWORD dwPacketID, CLBPacket *pPacket)
{
	AUTO_CLOSE;
	DWORD dwResult = Query(JDB_LIST_ROLE);
	VERIFY_RESULT;

	SendSequencial(dwPacketID, m_rs.GetRecordCount(), (LPVOID)JD_ACTION_ROLE);
	return JDRESPONSE_SUCCESS;
}

DWORD CJDSession::AddRole(DWORD dwPacketID, CLBPacket *pPacket)
{
	VERIFY_PERMISSION(0, PERMISSION_ADMIN);
	CString sName = ReadString(pPacket);
	if (sName.IsEmpty())
		return JDRESPONSE_DATA_ERROR;

	DWORD dwResult = Query(JDB_CHECK_ROLE_NAME, (LPCTSTR)sName);
	dwResult = TranslateResult(dwResult);
	VERIFY_RESULT; // Name already taken?

	// Add role
	if (!InsertRecord(JDB_TABLE_ROLE))
		return JDRESPONSE_INTERNAL_ERROR;

	m_rs.SetFieldValue(_T("FName"), sName);	
	if (!m_rs.Update())
		return JDRESPONSE_INTERNAL_ERROR;

	OUT_RDY;
	DWORD nRoleID = PackRoleData(packet);
	
	// Add a root category for the new role
	if (!InsertRecord(JDB_TABLE_CAT))
		return JDRESPONSE_INTERNAL_ERROR;

	m_rs.SetFieldValue(_T("FName"), sName);	
	m_rs.SetFieldValue(_T("FParentID"), 0);
	m_rs.SetFieldValue(_T("FRoleID"), nRoleID);
	m_rs.SetFieldValue(_T("FUserID"), m_nUserID);
	m_rs.SetFieldValue(_T("FDesc"), sName + _T("根目录"));
	if (!m_rs.Update())
		return JDRESPONSE_INTERNAL_ERROR;

	DWORD nCatID = 0;
	m_rs.GetFieldValue(_T("FID"), nCatID);	
	CloseDB();

	OUT_INT(nCatID);
	OUT_GO;
	return JDRESPONSE_SUCCESS;
}

DWORD CJDSession::ChangeRole(DWORD dwPacketID, CLBPacket *pPacket)
{
	VERIFY_PERMISSION(0, PERMISSION_ADMIN);
	DWORD nRoleID = pPacket->ReadInt();
	CString sName = ReadString(pPacket);
	if (sName.IsEmpty())
		return JDRESPONSE_DATA_ERROR;

	DWORD dwResult = Query(JDB_REQUEST_ROLE, nRoleID);
	VERIFY_RESULT; 
	
	dwResult = Query(JDB_CHECK_ROLE_NAME, (LPCTSTR)sName);
	dwResult = TranslateResult(dwResult);
	VERIFY_RESULT; // Name already taken?

	dwResult = Execute(JDB_UPDATE_ROLE, (LPCTSTR)sName, nRoleID);
	VERIFY_RESULT;

	Execute(JDB_UPDATE_ROOT_CATEGORY, (LPCTSTR)sName, nRoleID);
	
	Query(JDB_REQUEST_ROOT_CATEGORY, nRoleID);
	DWORD nCatID = 0;
	m_rs.GetFieldValue(_T("FID"), nCatID);
	return JDRESPONSE_SUCCESS;
}

DWORD CJDSession::ListCategory(DWORD dwPacketID, CLBPacket *pPacket)
{
	AUTO_CLOSE;
	DWORD nParentCatID = pPacket->ReadInt();
	DWORD dwResult = Query(JDB_LIST_CATEGORY, nParentCatID, m_nPrivilege, JDPRIV_ADMIN, m_nRoleID, m_nRoleID);	
	VERIFY_RESULT;
	SendSequencial(dwPacketID, m_rs.GetRecordCount(), (LPVOID)JD_ACTION_CAT);	
	return JDRESPONSE_SUCCESS;
}

DWORD CJDSession::AddCategory(DWORD dwPacketID, CLBPacket *pPacket)
{
	AUTO_CLOSE;
	DWORD nParentCatID = pPacket->ReadInt();
	DWORD dwResult = RequestCategory(nParentCatID);
	if (dwResult == JDRESPONSE_NOT_EXISTS)
		dwResult = JDRESPONSE_PARENT_NOT_EXISTS;
	VERIFY_RESULT;

	DWORD nRoleID;
	m_rs.GetFieldValue(_T("FRoleID"), nRoleID);
	VERIFY_PERMISSION(nRoleID, PERMISSION_SUPER); // Parent category role must match

	// New category initially inherits privileges from its parent
	CString sPriv;
	m_rs.GetFieldValue(_T("FPrivilege"), sPriv);

	CString sName = ReadString(pPacket);
	dwResult = Query(JDB_CHECK_CATEGORY_NAME, nParentCatID, (LPCTSTR)sName);
	dwResult = TranslateResult(dwResult);	
	VERIFY_RESULT;

	CString sDesc = ReadString(pPacket);
	
	if (!InsertRecord(JDB_TABLE_CAT))
		return JDRESPONSE_INTERNAL_ERROR;

	m_rs.SetFieldValue(_T("FParentID"), nParentCatID);
	m_rs.SetFieldValue(_T("FRoleID"), nRoleID); // Admins can create categories anywhere but let them inherit role ID from their parents only
	m_rs.SetFieldValue(_T("FUserID"), m_nUserID);
	m_rs.SetFieldValue(_T("FName"), sName);
	m_rs.SetFieldValue(_T("FDesc"), sDesc);
	if (!sPriv.IsEmpty())
		m_rs.SetFieldValue(_T("FPrivilege"), sPriv);
	
	if (!m_rs.Update())
		return JDRESPONSE_INTERNAL_ERROR;

	OUT_RDY;
	PackCatData(packet);
	CloseDB();
	OUT_GO;
	return JDRESPONSE_SUCCESS;	
}

DWORD CJDSession::ChangeCategory(DWORD dwPacketID, CLBPacket *pPacket)
{
	AUTO_CLOSE;
	DWORD nCatID = pPacket->ReadInt();
	CString sNewName = ReadString(pPacket);
	CString sNewDesc = ReadString(pPacket);

	DWORD dwResult = RequestCategory(nCatID);
	VERIFY_RESULT;

	DWORD nRoleID, nParentCatID;	
	m_rs.GetFieldValue(_T("FRoleID"), nRoleID);
	VERIFY_PERMISSION(nRoleID, PERMISSION_SUPER);
	
	m_rs.GetFieldValue(_T("FParentID"), nParentCatID);
	
	if (!sNewName.IsEmpty())
	{
		// Check name conflicts
		dwResult = Query(JDB_CHECK_CATEGORY_NAME, nParentCatID, (LPCTSTR)sNewName);
		dwResult = TranslateResult(dwResult);	
		VERIFY_RESULT;
	}

	dwResult = RequestCategory(nCatID);
	VERIFY_RESULT;	

	m_rs.Edit();
	
	if (!sNewName.IsEmpty())
		m_rs.SetFieldValue(_T("FName"), sNewName);

	m_rs.SetFieldValue(_T("FDesc"), sNewDesc);
	m_rs.Update();

	OUT_RDY;
	PackCatData(packet);
	CloseDB();
	OUT_GO;
	return JDRESPONSE_SUCCESS;
}

void CJDSession::UpdateCatPrivProc(CDWordArray &aList, DWORD nCatID, LPCTSTR lpszNewPriv)
{
	aList.Add(nCatID);
	if (Execute(JDB_VATEGORY_PRIVILEGE_SUB, lpszNewPriv, nCatID) != JDRESPONSE_SUCCESS)
		return;

	if (Query(JDB_VATEGORY_GETID, nCatID) != JDRESPONSE_SUCCESS)
		return;

	CDWordArray aSub;
	DWORD nID;
	while (!m_rs.IsEOF())
	{			
		m_rs.GetFieldValue(_T("FID"), nID);
		aSub.Add(nID);
		m_rs.MoveNext();
	}
	m_rs.Close();

	for (int i = 0; i < aSub.GetSize(); i++)
		UpdateCatPrivProc(aList, aSub[i], lpszNewPriv);
}

DWORD CJDSession::ChangeCategoryPriv(DWORD dwPacketID, CLBPacket *pPacket)
{
	AUTO_CLOSE;
	DWORD nCatID = pPacket->ReadInt();
	CString sNewPriv = ReadString(pPacket);
	BOOL bApplySubCats = pPacket->ReadInt();
	_LOG("ChangeCategoryPriv, %d, %s, %d", nCatID, (LPCTSTR)sNewPriv, bApplySubCats);

	DWORD dwResult = RequestCategory(nCatID);
	VERIFY_RESULT;

	DWORD nRoleID;	
	m_rs.GetFieldValue(_T("FRoleID"), nRoleID);
	VERIFY_PERMISSION(nRoleID, PERMISSION_SUPER);
	
	m_rs.Edit();
	m_rs.SetFieldValue(_T("FPrivilege"), sNewPriv);
	m_rs.Update();
	CloseDB();

	// Only change a single category
	if (!bApplySubCats)
	{		
		OUT_RDY;
		OUT_INT(nCatID);
		OUT_STR(sNewPriv);
		OUT_GO;
		return JDRESPONSE_SUCCESS;
	}

	// Apply to all sub categories, needs some recursion...
	CDWordArray aList;
	UpdateCatPrivProc(aList, nCatID, sNewPriv);
	CloseDB();
	
	int nCount = aList.GetSize();
	//_LOG("Total %d affected sub categories: ", nCount);
	for (int i = 0; i < nCount && !IsStopping(); i++)
	{
		//_LOG("%d", aList[i]);
		pPacket->PrepareForWrite();
		pPacket->WriteDword(JDRESPONSE_SUCCESS);
		pPacket->WriteDword(aList[i]);
		pPacket->WriteString(sNewPriv);
		pPacket->Encode(dwPacketID, nCount, i);
		Send(pPacket);
		Receive(pPacket, FALSE);
	}
	
	return JDRESPONSE_SUCCESS;
}

DWORD CJDSession::DeleteCategory(DWORD dwPacketID, CLBPacket *pPacket)
{
	AUTO_CLOSE;	
	DWORD nCatID = pPacket->ReadInt();
	DWORD dwResult = RequestCategory(nCatID);
	VERIFY_RESULT;
	
	DWORD nRoleID, nParentCatID;	
	m_rs.GetFieldValue(_T("FRoleID"), nRoleID);
	VERIFY_PERMISSION(nRoleID, PERMISSION_SUPER);

	m_rs.GetFieldValue(_T("FParentID"), nParentCatID);
	if (nParentCatID == 0)
		return JDRESPONSE_DENY; // Nobody can delete a root category, even a super-admin!
	
	dwResult = Query(JDB_CHECK_CATEGORY_EMPTY_C, nCatID);
	dwResult = TranslateResult(dwResult, JDRESPONSE_TARGET_NOT_EMPTY);
	VERIFY_RESULT;

	dwResult = Query(JDB_CHECK_CATEGORY_EMPTY_F, nCatID);
	dwResult = TranslateResult(dwResult, JDRESPONSE_TARGET_NOT_EMPTY);
	VERIFY_RESULT;

	dwResult = Query(JDB_LIST_FILE, nCatID);
	dwResult = TranslateResult(dwResult, JDRESPONSE_TARGET_NOT_EMPTY);
	VERIFY_RESULT;

	dwResult = Execute(JDB_DELETE_CATEGORY, nCatID);
	VERIFY_RESULT;
	CloseDB();
	
	OUT_RDY;
	OUT_INT(nCatID);	
	OUT_GO;
	return JDRESPONSE_SUCCESS;
}

DWORD CJDSession::RequestRootCategory(DWORD dwPacketID, CLBPacket *pPacket)
{
	VERIFY_PERMISSION(0, PERMISSION_ADMIN);
	AUTO_CLOSE;	
	DWORD nRoleID = pPacket->ReadInt();
	DWORD dwResult = Query(JDB_REQUEST_ROOT_CATEGORY, nRoleID);
	VERIFY_RESULT;
	
	OUT_RDY;
	PackCatData(packet);
	CloseDB();
	OUT_GO;
	return JDRESPONSE_SUCCESS;
}

DWORD CJDSession::ListFile(DWORD dwPacketID, CLBPacket *pPacket)
{
	AUTO_CLOSE;
	DWORD nCatID = pPacket->ReadInt();
	DWORD dwResult = Query(JDB_LIST_FILE, nCatID);
	VERIFY_RESULT;

	SendSequencial(dwPacketID, m_rs.GetRecordCount(), (LPVOID)JD_ACTION_FILE);	
	return JDRESPONSE_SUCCESS;
}

DWORD CJDSession::AddFile(DWORD dwPacketID, CLBPacket *pPacket)
{
	AUTO_CLOSE;
	DWORD nCatID = pPacket->ReadInt();
	CString sName = ReadString(pPacket);
	CString sDesc = ReadString(pPacket);

	// make sure no name confilicts in parent category
	DWORD dwResult = Query(JDB_CHECK_FILE_NAME, nCatID, (LPCTSTR)sName);
	dwResult = TranslateResult(dwResult);	
	VERIFY_RESULT;

	dwResult = Query(JDB_QUERY_BY_ID, JDB_TABLE_CAT, nCatID);
	VERIFY_RESULT;

	// Files derive role id from parent category
	DWORD nRoleID = 0;
	m_rs.GetFieldValue(_T("FRoleID"), nRoleID);

	if (!InsertRecord(JDB_TABLE_FILE))
		return JDRESPONSE_INTERNAL_ERROR;

	DWORD nVersion = 1000000;
	CString sExt;
	int nDot = sName.ReverseFind(_T('.'));
	if (nDot != -1)
	{
		sExt = sName.Mid(nDot + 1);
		sExt.MakeLower();
	}

	m_rs.SetFieldValue(_T("FParentID"), nCatID);
	m_rs.SetFieldValue(_T("FName"), sName);
	m_rs.SetFieldValue(_T("FExt"), sExt);
	m_rs.SetFieldValue(_T("FVersion"), nVersion);
	m_rs.SetFieldValue(_T("FDesc"), sDesc);
	m_rs.SetFieldValue(_T("FRoleID"), nRoleID);
	
	if (!m_rs.Update())
		return JDRESPONSE_INTERNAL_ERROR;	

	OUT_RDY;
	PackFileData(packet);
	CloseDB();
	OUT_GO;
	
	return JDRESPONSE_SUCCESS;
}

DWORD CJDSession::RequestData(DWORD dwPacketID, CLBPacket *pPacket, LPCTSTR lpszSQL, int nType)
{
	AUTO_CLOSE;
	DWORD nID = pPacket->ReadInt();
	DWORD dwResult;
	if (nType == JD_ACTION_VER)
		dwResult = RequestVersion(nID);
	else if (nType == JD_ACTION_CAT)
		dwResult = RequestCategory(nID);
	else
		dwResult = Query(lpszSQL, nID);
	VERIFY_RESULT;
	
	OUT_RDY;
	switch (nType)
	{
	case JD_ACTION_USER:
		PackUserData(packet);
		break;

	case JD_ACTION_ROLE:
		PackRoleData(packet);
		break;

	case JD_ACTION_CAT:
		PackCatData(packet);
		break;

	case JD_ACTION_FILE:
		PackFileData(packet);
		break;

	case JD_ACTION_VER:
		PackVerData(packet);
		break;	

	default:
		ASSERT(FALSE);
		break;
	}

	CloseDB();
	OUT_GO;
	return JDRESPONSE_SUCCESS;
}

DWORD CJDSession::RequestManagerVer(DWORD dwPacketID, CLBPacket *pPacket)
{
	AUTO_CLOSE;
	UnlockFilesForUser();
	DWORD nFileID = pPacket->ReadInt();
	DWORD dwResult = Query(JDB_REQUEST_FILE, nFileID);	
	VERIFY_RESULT;
	
	DWORD nVersion, nLockerID, nRoleID;
	CString sFileName;
	m_rs.GetFieldValue(_T("FVersion"), nVersion);
	m_rs.GetFieldValue(_T("FRoleID"), nRoleID);
	m_rs.GetFieldValue(_T("FLockerID"), nLockerID);
	m_rs.GetFieldValue(_T("FName"), sFileName);

	VERIFY_PERMISSION(nRoleID, PERMISSION_SUPER);

	if (nLockerID)
		return JDRESPONSE_FILE_LOCKED;

	m_rs.Edit();
	m_rs.SetFieldValue(_T("FLockerID"), m_nUserID);
	m_rs.Update();

	CloseDB();
	
	OUT_RDY;
	OUT_INT(nFileID);
	OUT_INT(nVersion + 1);
	OUT_STR(sFileName);
	OUT_GO;
	return JDRESPONSE_SUCCESS;
}

DWORD CJDSession::ApplyManagerVer(DWORD dwPacketID, CLBPacket *pPacket)
{
	AUTO_CLOSE;
	DWORD nFileID = pPacket->ReadInt();
	DWORD nVersion = pPacket->ReadInt();
	nVersion--;
	DWORD dwResult = Query(JDB_REQUEST_FILE, nFileID);	
	VERIFY_RESULT;

	DWORD nCurVersion, nRoleID, nLockerID;
	m_rs.GetFieldValue(_T("FVersion"), nCurVersion);
	m_rs.GetFieldValue(_T("FRoleID"), nRoleID);
	m_rs.GetFieldValue(_T("FLockerID"), nLockerID);	
	
	VERIFY_PERMISSION(nRoleID, PERMISSION_SUPER);

	if (nLockerID != m_nUserID)
		return JDRESPONSE_FILE_LOCKED;	

	if (nCurVersion >= nVersion)
		return JDRESPONSE_VER_VERSION_ERROR;

	m_rs.Edit();
	m_rs.SetFieldValue(_T("FVersion"), nVersion);
	m_rs.Update();

	UnlockFilesForUser();
	OUT_RDY;
	OUT_GO;
	return JDRESPONSE_SUCCESS;	
}

DWORD CJDSession::RequestUploadVer(DWORD dwPacketID, CLBPacket *pPacket)
{
	AUTO_CLOSE;
	UnlockFilesForUser();
	DWORD nFileID = pPacket->ReadInt();
	DWORD dwResult = Query(JDB_REQUEST_FILE, nFileID);	
	VERIFY_RESULT;
	
	DWORD nVersion, nLockerID = 0;
	CString sFileName;
	m_rs.GetFieldValue(_T("FVersion"), nVersion);
	m_rs.GetFieldValue(_T("FLockerID"), nLockerID);
	m_rs.GetFieldValue(_T("FName"), sFileName);
	if (nLockerID)
		return JDRESPONSE_FILE_LOCKED;

	m_rs.Edit();
	m_rs.SetFieldValue(_T("FLockerID"), m_nUserID);
	m_rs.Update();

	CloseDB();
	
	OUT_RDY;
	OUT_INT(nFileID);
	OUT_INT(nVersion + 1);
	OUT_STR(sFileName);
	OUT_GO;
	return JDRESPONSE_SUCCESS;
}

DWORD CJDSession::CancelRequestUploadVer(DWORD dwPacketID, CLBPacket *pPacket)
{
	UnlockFilesForUser();
	CloseDB();
	DWORD dwResult = JDRESPONSE_SUCCESS;
	OUT_RDY;
	OUT_GO;
	return JDRESPONSE_SUCCESS;
}

DWORD CJDSession::UploadVer(DWORD dwPacketID, CLBPacket *pPacket)
{
	AUTO_CLOSE;

	DWORD nFileID = pPacket->ReadInt();
	DWORD nVersion = pPacket->ReadInt();
	CString sDesc = ReadString(pPacket);
	CString sTemp = ReadString(pPacket);
	DWORD dwResult = Query(JDB_REQUEST_FILE, nFileID);	
	VERIFY_RESULT;	

	DWORD nCurVersion, nRoleID, nLockerID;
	m_rs.GetFieldValue(_T("FVersion"), nCurVersion);
	m_rs.GetFieldValue(_T("FRoleID"), nRoleID);
	m_rs.GetFieldValue(_T("FLockerID"), nLockerID);
	CloseDB();	

	if (nLockerID != m_nUserID)
		return JDRESPONSE_FILE_LOCKED;	

	if (nCurVersion >= nVersion)
		return JDRESPONSE_VER_VERSION_ERROR;

	// Tell the client to upload right now
	OUT_RDY;
	OUT_INT(nFileID);
	OUT_STR(sTemp);
	OUT_GO;	

	// Ready to receive
	CString sDiskName = GenerateGUID();	
	DWORD dwFileSize;
	TCHAR szMD5[40] = _T("");
	int nResult = ReceiveFile(m_sRootPath + _T('\\') + sDiskName, szMD5, &dwFileSize);
	if (nResult != CLBSession::FILE_ERROR_SUCCESS)
	{
		UnlockFilesForUser();
		return JDRESPONSE_SUCCESS; // need to abort stuff;
	}

	dwResult = AfterUpload(pPacket, szMD5, dwFileSize, sDiskName, nFileID, nRoleID, nVersion, sDesc);
	UnlockFilesForUser();

	pPacket->PrepareForWrite();
	pPacket->WriteDword(dwResult);
	if (dwResult == JDRESPONSE_SUCCESS)
	{		
		PackVerData(*pPacket);
	}
	else
	{
		::DeleteFile(m_sRootPath + _T('\\') + sDiskName);
	}
	pPacket->Encode(JD_PACKET_VER_LIST);
	Send(pPacket);	
	return JDRESPONSE_SUCCESS;
}

DWORD CJDSession::AfterUpload(CLBPacket *pPacket, CString sMD5, DWORD dwFileSize, CString sDiskName, DWORD nFileID, DWORD nRoleID, DWORD nVersion, CString sDesc)
{
	// Check file existence
	DWORD dwResult = Query(JDB_REQUEST_FILE, nFileID);
	VERIFY_RESULT;
	
	m_rs.Edit();
	m_rs.SetFieldValue(_T("FVersion"), nVersion);
	m_rs.Update();

	InsertRecord(JDB_TABLE_VER);
	m_rs.SetFieldValue(_T("FFileID"), nFileID);
	m_rs.SetFieldValue(_T("FDiskName"), sDiskName);
	m_rs.SetFieldValue(_T("FMD5"), sMD5);
	m_rs.SetFieldValue(_T("FVersion"), nVersion);
	m_rs.SetFieldValue(_T("FSize"), dwFileSize);
	m_rs.SetFieldValue(_T("FCreatorID"), m_nUserID);
	m_rs.SetFieldValue(_T("FDesc"), sDesc);	
	m_rs.SetFieldValue(_T("FCreateDate"), COleDateTime::GetCurrentTime());
	m_rs.SetFieldValue(_T("FRoleID"), nRoleID); // Versions derive role id from parent file
	m_rs.Update();
	return JDRESPONSE_SUCCESS;
}

DWORD CJDSession::ListVersion(DWORD dwPacketID, CLBPacket *pPacket)
{
	AUTO_CLOSE;	
	DWORD nFileID = pPacket->ReadInt();
	DWORD dwResult = Query(JDB_LIST_VERSION, nFileID, m_nUserID, m_nRoleID, m_nPrivilege, JDPRIV_SUPERVISOR);
	VERIFY_RESULT;
	SendSequencial(dwPacketID, m_rs.GetRecordCount(), (LPVOID)JD_ACTION_VER);
	return JDRESPONSE_SUCCESS;
}

DWORD CJDSession::DownloadVer(DWORD dwPacketID, CLBPacket *pPacket)
{
	if (m_nClientVer < 124)
		return JDRESPONSE_CLIENT_VER;
	
	AUTO_CLOSE;	
	DWORD nVerID = pPacket->ReadInt();
	DWORD dwResult = RequestVersion(nVerID);
	VERIFY_RESULT;
	
	CString sFileName, sDiskName, sMD5;
	DWORD dwSize, nCheckerID, nRoleID, nCreatorID;

	m_rs.GetFieldValue(_T("FCheckerID"), nCheckerID);
	m_rs.GetFieldValue(_T("FRoleID"), nRoleID);
	m_rs.GetFieldValue(_T("FCreatorID"), nCreatorID);
	if (nCheckerID < 1 && nCreatorID != m_nUserID)
	{
		if (m_nPrivilege < JDPRIV_SUPERVISOR || (m_nPrivilege == JDPRIV_SUPERVISOR && m_nRoleID != nRoleID))
			return JDRESPONSE_DENY;
	}
	
	m_rs.GetFieldValue(_T("FFileName"), sFileName);
	m_rs.GetFieldValue(_T("FDiskName"), sDiskName);
	m_rs.GetFieldValue(_T("FSize"), dwSize);
	m_rs.GetFieldValue(_T("FMD5"), sMD5);
	CloseDB();

	if (sFileName.IsEmpty() || sDiskName.IsEmpty() || dwSize == 0)
		return JDRESPONSE_INTERNAL_ERROR;

	pPacket->PrepareForWrite();
	pPacket->WriteDword(JDRESPONSE_SUCCESS);
	pPacket->WriteInt(nVerID);
	pPacket->WriteString(sFileName);
	pPacket->WriteInt(dwSize);
	pPacket->Encode(JD_PACKET_VER_DOWNLOAD);
	Send(pPacket);

	Receive(pPacket);
	if (pPacket->GetPacketID() == JD_PACKET_VER_DOWNLOAD_DATA_BEGIN)
		SendFile(m_sRootPath + _T('\\') + sDiskName, sMD5);
	
	return JDRESPONSE_SUCCESS;
}

BOOL CJDSession::SendSequencialProc(DWORD dwPacketID, CLBPacket *pPacket, int nCount, int nSequence, LPVOID lParam)
{
	pPacket->WriteDword(JDRESPONSE_SUCCESS);

	if (!m_rs.IsEOF())
	{
		switch ((int)lParam)
		{
		case JD_ACTION_USER:
			PackUserData(*pPacket);
			break;

		case JD_ACTION_ROLE:
			PackRoleData(*pPacket);
			break;

		case JD_ACTION_CAT:
			PackCatData(*pPacket);
			break;

		case JD_ACTION_FILE:
			PackFileData(*pPacket);
			break;

		case JD_ACTION_VER:
			PackVerData(*pPacket);
			break;

		case JD_ACTION_RECOVABLE:
			PackRecovableData(*pPacket);
			break;

		default:
			ASSERT(FALSE);
			break;
		}

		m_rs.MoveNext();
	}	
		
	return !IsStopping();
}

DWORD CJDSession::CheckVersion(DWORD dwPacketID, CLBPacket *pPacket)
{
	AUTO_CLOSE;	
	DWORD nVerID = pPacket->ReadInt();
	DWORD dwResult = RequestVersion(nVerID);
	VERIFY_RESULT;

	dwResult = Execute(JDB_CHECK_VER, m_nUserID, nVerID);
	VERIFY_RESULT;

	DWORD nCheckerID;
	m_rs.GetFieldValue(_T("FCheckerID"), nCheckerID);
	if (nCheckerID)
		return JDRESPONSE_VER_ALREADY_CHECKED;	

	CloseDB();

	OUT_RDY;
	OUT_INT(nVerID);
	OUT_INT(m_nUserID);
	OUT_DAT(COleDateTime::GetCurrentTime());
	OUT_GO;
	
	return JDRESPONSE_SUCCESS;
}

DWORD CJDSession::UncheckVersion(DWORD dwPacketID, CLBPacket *pPacket)
{
	AUTO_CLOSE;	
	DWORD nVerID = pPacket->ReadInt();
	DWORD dwResult = RequestVersion(nVerID);
	VERIFY_RESULT;

	dwResult = Execute(JDB_UNCHECK_VER, nVerID);
	VERIFY_RESULT;	

	CloseDB();
	OUT_RDY;
	OUT_INT(nVerID);
	OUT_GO;
	return JDRESPONSE_SUCCESS;
}

DWORD CJDSession::DeleteVersion(DWORD dwPacketID, CLBPacket *pPacket)
{
	AUTO_CLOSE;	
	DWORD nVerID = pPacket->ReadInt();
	DWORD dwResult = RequestVersion(nVerID);
	CString sFileName; 
	m_rs.GetFieldValue(_T("FFileName"), sFileName);
	VERIFY_RESULT;	

	dwResult = Execute(JDB_DELETE_VER, m_nUserID, (LPCTSTR)sFileName, nVerID);
	VERIFY_RESULT;	
	CloseDB();
	OUT_RDY;
	OUT_INT(nVerID);
	OUT_GO;

	return JDRESPONSE_SUCCESS;
}

DWORD CJDSession::DeleteFile(DWORD dwPacketID, CLBPacket *pPacket)
{
	AUTO_CLOSE;	
	DWORD nFileID = pPacket->ReadInt();
	DWORD dwResult = Query(JDB_CHECK_FILE_EMPTY, nFileID);
	dwResult = TranslateResult(dwResult, JDRESPONSE_TARGET_NOT_EMPTY);
	VERIFY_RESULT; // File not empty
	dwResult = Execute(JDB_DELETE_FILE, nFileID);
	VERIFY_RESULT;

	CloseDB();
	OUT_RDY;
	OUT_INT(nFileID);
	OUT_GO;

	return JDRESPONSE_SUCCESS;
}

DWORD CJDSession::UploadPatch(DWORD dwPacketID, CLBPacket *pPacket)
{
	if (m_nPrivilege < JDPRIV_SUPER_ADMIN)
		return JDRESPONSE_DENY;

	DWORD nVersion = pPacket->ReadInt();
	CString sName = ReadString(pPacket);
	CString sTemp = ReadString(pPacket);

	if (sName.IsEmpty() || sTemp.IsEmpty())
		return JDRESPONSE_DATA_ERROR;

	// verify version
	DWORD dwResult = Query(JDB_PATCH_VERIFY_VER, nVersion);
	CloseDB();
	dwResult = TranslateResult(dwResult, JDRESPONSE_VER_VERSION_ERROR);
	VERIFY_RESULT; // version error

	pPacket->PrepareForWrite();
	pPacket->WriteDword(dwResult);
	pPacket->WriteString(sTemp);
	pPacket->Encode(dwPacketID);
	Send(pPacket);	

	CString sPath = m_sRootPath + _T("\\Patch");
	::CreateDirectory(sPath, NULL);

	DWORD dwFileSize = 0;
	TCHAR szMD5[40] = _T("");
	int nResult = ReceiveFile(sPath + _T('\\') + sName, szMD5, &dwFileSize);
	if (nResult != CLBSession::FILE_ERROR_SUCCESS)
		return JDRESPONSE_SUCCESS; // need to abort stuff;

	dwResult = Execute(JDB_PATCH_ADD, nVersion, (LPCTSTR)sName, dwFileSize, szMD5, m_nUserID);
	CloseDB();
	if (dwResult != JDRESPONSE_SUCCESS)
		::DeleteFile(sPath + _T('\\') + sName);

	pPacket->PrepareForWrite();
	pPacket->WriteDword(dwResult);
	pPacket->Encode(JD_PACKET_UPLOAD_APP_RESULT);
	Send(pPacket);

	return JDRESPONSE_SUCCESS;
}

DWORD CJDSession::CheckPatch(DWORD dwPacketID, CLBPacket *pPacket)
{
	DWORD nVersion = pPacket->ReadInt();

	// check for newer version
	AUTO_CLOSE;
	DWORD dwResult = Query(JDB_PATCH_GET_NEWEST, nVersion);
	VERIFY_RESULT; // no newer version avialable

	DWORD dwSize = 0;
	CString sFileName, sUserName;
	COleDateTime date;
	m_rs.GetFieldValue(_T("FID"), nVersion);
	m_rs.GetFieldValue(_T("FSize"), dwSize);
	m_rs.GetFieldValue(_T("FName"), sFileName);
	m_rs.GetFieldValue(_T("FUserName"), sUserName);
	m_rs.GetFieldValue(_T("FDate"), date);
	CloseDB();

	OUT_RDY;
	OUT_INT(nVersion);
	OUT_INT(dwSize);
	OUT_STR(sFileName);	
	OUT_STR(sUserName);
	OUT_DAT(date);
	OUT_GO;

	return JDRESPONSE_SUCCESS;
}

DWORD CJDSession::DownloadPatch(DWORD dwPacketID, CLBPacket *pPacket)
{
	DWORD nVersion = pPacket->ReadInt();
	AUTO_CLOSE;

	DWORD dwResult = Query(JDB_QUERY_BY_ID, JDB_VIEW_PATCH, nVersion);
	VERIFY_RESULT;

	CString sFileName, sMD5;
	m_rs.GetFieldValue(_T("FName"), sFileName);
	m_rs.GetFieldValue(_T("FMD5"), sMD5);

	OUT_RDY;
	OUT_STR(sFileName);
	OUT_GO;

	SendFile(m_sRootPath + _T("\\Patch\\") + sFileName, sMD5);
	return JDRESPONSE_SUCCESS;	
}

DWORD CJDSession::CatInitStart(DWORD dwPacketID, CLBPacket *pPacket)
{
	if (m_nClientVer < 124)
		return JDRESPONSE_CLIENT_VER;

	AUTO_CLOSE;
	DWORD nParentCatID = pPacket->ReadInt();
	CString sName = ReadString(pPacket);
	CString sTemp = ReadString(pPacket);

	DWORD dwResult = RequestCategory(nParentCatID);
	VERIFY_RESULT;

	DWORD nRoleID;	
	m_rs.GetFieldValue(_T("FRoleID"), nRoleID);
	VERIFY_PERMISSION(nRoleID, PERMISSION_SUPER);

	// Make sure no name conflicts under nParentCatID
	dwResult = Query(JDB_CHECK_CATEGORY_NAME, nParentCatID, (LPCTSTR)sName);
	dwResult = TranslateResult(dwResult);	
	VERIFY_RESULT;

	if (!InsertRecord(JDB_TABLE_CAT))
		return JDRESPONSE_INTERNAL_ERROR;

	m_rs.SetFieldValue(_T("FParentID"), nParentCatID);
	m_rs.SetFieldValue(_T("FRoleID"), nRoleID); // Admins can create categories anywhere but let them inherit role ID from their parents only
	m_rs.SetFieldValue(_T("FUserID"), m_nUserID);
	m_rs.SetFieldValue(_T("FName"), sName);
	
	if (!m_rs.Update())
		return JDRESPONSE_INTERNAL_ERROR;

	DWORD nCatID = 0;
	m_rs.GetFieldValue(_T("FID"), nCatID);
	CloseDB();

	const DWORD INIT_CAT_ID = nCatID;

	OUT_RDY;
	OUT_INT(nCatID);
	OUT_STR(sName);
	OUT_STR(sTemp);
	OUT_GO;

	CString sVerDesc(_T("初始化自动创建版本"));
	const DWORD INITVER = 1000001;

	// Wait for category initial data, keep receiving until JD_PACKET_CAT_INIT_END arrives	
	while (Receive(pPacket))
	{		
		dwPacketID = pPacket->GetPacketID();
		nParentCatID = pPacket->ReadInt();
		sName = ReadString(pPacket);
		
		if (dwPacketID == JD_PACKET_CAT_INIT_CAT || dwPacketID == JD_PACKET_CAT_INIT_FILE)
		{
			if (dwPacketID == JD_PACKET_CAT_INIT_CAT)
			{
				InsertRecord(JDB_TABLE_CAT);
				m_rs.SetFieldValue(_T("FParentID"), nParentCatID);
				m_rs.SetFieldValue(_T("FRoleID"), nRoleID); // Admins can create categories anywhere but let them inherit role ID from their parents only
				m_rs.SetFieldValue(_T("FUserID"), m_nUserID);
				m_rs.SetFieldValue(_T("FName"), sName);	
				m_rs.Update();
				m_rs.GetFieldValue(_T("FID"), nCatID);

				pPacket->PrepareForWrite();
				pPacket->WriteInt(nCatID);
				pPacket->Encode(dwPacketID);
				Send(pPacket);
			}
			else
			{
				InsertRecord(JDB_TABLE_FILE);
				CString sExt;
				int nDot = sName.ReverseFind(_T('.'));
				if (nDot != -1)
				{
					sExt = sName.Mid(nDot + 1);
					sExt.MakeLower();
				}
				m_rs.SetFieldValue(_T("FParentID"), nParentCatID);
				m_rs.SetFieldValue(_T("FRoleID"), nRoleID);
				m_rs.SetFieldValue(_T("FName"), sName);	
				m_rs.SetFieldValue(_T("FExt"), sExt);
				m_rs.SetFieldValue(_T("FVersion"), INITVER); // There will be an initial version coming soon
				m_rs.Update();

				DWORD nFileID = 0;
				m_rs.GetFieldValue(_T("FID"), nFileID);

				pPacket->PrepareForWrite();
				pPacket->Encode(dwPacketID);
				Send(pPacket);

				// wait for version upload
				CString sDiskName = GenerateGUID();	
				DWORD dwFileSize;
				TCHAR szMD5[40] = _T("");
				int nResult = ReceiveFile(m_sRootPath + _T('\\') + sDiskName, szMD5, &dwFileSize);
				if (nResult == CLBSession::FILE_ERROR_SUCCESS)
				{
					InsertRecord(JDB_TABLE_VER);
					m_rs.SetFieldValue(_T("FFileID"), nFileID);
					m_rs.SetFieldValue(_T("FDiskName"), sDiskName);
					m_rs.SetFieldValue(_T("FMD5"), CString(szMD5));
					m_rs.SetFieldValue(_T("FSize"), dwFileSize);
					m_rs.SetFieldValue(_T("FCreatorID"), m_nUserID);
					m_rs.SetFieldValue(_T("FRoleID"), nRoleID); // Versions derive role id from parent file
					m_rs.SetFieldValue(_T("FCheckerID"), m_nUserID);
					m_rs.SetFieldValue(_T("FCheckDate"), COleDateTime::GetCurrentTime());
					m_rs.SetFieldValue(_T("FDesc"), sVerDesc);

					m_rs.Update();

					DWORD nVerID = 0;
					m_rs.GetFieldValue(_T("FID"), nVerID);
				}
				else
				{
					::DeleteFile(m_sRootPath + _T('\\') + sDiskName);
				}

				pPacket->PrepareForWrite();
				pPacket->Encode(JD_PACKET_CAT_INIT_FILE_OK);
				Send(pPacket);
			}			
		}		
		else
		{
			// Likely JD_PACKET_CAT_INIT_END, but quit wanyway if we get here
			break;
		}
	}

	pPacket->PrepareForWrite();
	pPacket->WriteDword(JDRESPONSE_SUCCESS);
	pPacket->WriteInt(INIT_CAT_ID);
	pPacket->Encode(JD_PACKET_CAT_INIT_END);
	Send(pPacket);	
	return JDRESPONSE_SUCCESS;	
}

DWORD CJDSession::PasteFile(DWORD dwPacketID, CLBPacket *pPacket)
{
	AUTO_CLOSE;	
	
	DWORD nFileID = pPacket->ReadInt();
	DWORD nParentCatID = pPacket->ReadInt();
	DWORD dwResult = Query(JDB_QUERY_BY_ID, JDB_TABLE_FILE, nFileID);
	VERIFY_RESULT;

	DWORD nRoleID, nTemp;
	CString sName;
	m_rs.GetFieldValue(_T("FRoleID"), nRoleID);
	m_rs.GetFieldValue(_T("FName"), sName);

	VERIFY_PERMISSION(nRoleID, PERMISSION_SUPER);	

	dwResult = Query(JDB_QUERY_BY_ID, JDB_TABLE_CAT, nParentCatID);
	VERIFY_RESULT;
	m_rs.GetFieldValue(_T("FRoleID"), nTemp);

	if (nRoleID != nTemp)
		return JDRESPONSE_DENY;

	// Check name conflicts
	dwResult = Query(JDB_CHECK_FILE_NAME, nParentCatID, (LPCTSTR)sName);
	dwResult = TranslateResult(dwResult);
	VERIFY_RESULT;

	dwResult = Execute(JDB_FILE_PASTE, nParentCatID, nFileID);
	VERIFY_RESULT;

	OUT_RDY;
	OUT_INT(nFileID);
	OUT_INT(nParentCatID);
	OUT_GO;
	
	return JDRESPONSE_SUCCESS;	
}

DWORD CJDSession::PasteCategory(DWORD dwPacketID, CLBPacket *pPacket)
{
	AUTO_CLOSE;	
	DWORD nCatID = pPacket->ReadInt();
	DWORD nParentCatID = pPacket->ReadInt();

	DWORD dwResult = Query(JDB_QUERY_BY_ID, JDB_TABLE_CAT, nCatID);
	VERIFY_RESULT;

	DWORD nRoleID, nTemp;
	CString sName;
	m_rs.GetFieldValue(_T("FRoleID"), nRoleID);
	m_rs.GetFieldValue(_T("FName"), sName);

	VERIFY_PERMISSION(nRoleID, PERMISSION_SUPER);

	dwResult = Query(JDB_QUERY_BY_ID, JDB_TABLE_CAT, nParentCatID);
	VERIFY_RESULT;
	m_rs.GetFieldValue(_T("FRoleID"), nTemp);

	if (nRoleID != nTemp)
		return JDRESPONSE_DENY;

	// Check name conflicts
	dwResult = Query(JDB_CHECK_CATEGORY_NAME, nParentCatID, (LPCTSTR)sName);
	dwResult = TranslateResult(dwResult);
	VERIFY_RESULT;

	dwResult = Execute(JDB_CATEGORY_PASTE, nParentCatID, nCatID);
	VERIFY_RESULT;

	OUT_RDY;
	OUT_INT(nCatID);
	OUT_INT(nParentCatID);
	OUT_GO;

	return JDRESPONSE_SUCCESS;	
}

DWORD CJDSession::ListRecovable(DWORD dwPacketID, CLBPacket *pPacket)
{
	if (m_nPrivilege < JDPRIV_SUPER_ADMIN)
		return JDRESPONSE_DENY;

	AUTO_CLOSE;
	DWORD nRoleID = pPacket->ReadInt();
	CString sFileName = ReadString(pPacket);

	DWORD dwResult;
	if (sFileName.IsEmpty())
		dwResult = Query(JDB_LIST_RECOVABLE, nRoleID);
	else
		dwResult = Query(JDB_LIST_RECOVABLE_MATCH_NAME, nRoleID, (LPCTSTR)sFileName);

	VERIFY_RESULT;

	SendSequencial(dwPacketID, m_rs.GetRecordCount(), (LPVOID)JD_ACTION_RECOVABLE);	
	return JDRESPONSE_SUCCESS;
}

DWORD CJDSession::RecoverData(DWORD dwPacketID, CLBPacket *pPacket)
{
	AUTO_CLOSE;
	DWORD nVerID = pPacket->ReadInt();
	DWORD dwResult = Query(JDB_QUERY_BY_ID, JDB_TABLE_VER, nVerID);
	VERIFY_RESULT;
	
	CString sFileName, sDiskName, sMD5;
	m_rs.GetFieldValue(_T("FOrigFileName"), sFileName);
	m_rs.GetFieldValue(_T("FDiskName"), sDiskName);
	m_rs.GetFieldValue(_T("FMD5"), sMD5);
	CloseDB();

	pPacket->PrepareForWrite();
	pPacket->WriteDword(JDRESPONSE_SUCCESS);
	pPacket->WriteString(sFileName);
	pPacket->Encode(dwPacketID);
	Send(pPacket);

	Receive(pPacket); // Wait for JD_PACKET_RECOVERY_DOWNLOAD
	if (pPacket->GetPacketID() == JD_PACKET_RECOVERY_DOWNLOAD)
		SendFile(m_sRootPath + _T('\\') + sDiskName, sMD5);	
	return JDRESPONSE_SUCCESS;
}

void CJDSession::CycleDelTree(DWORD nCatID, int& nCats, int& nFiles, int& nVers)
{
	// Get a list of files belong to this category
	CDWordArray aList;
	if (Query(JDB_LIST_FILE, nCatID) == JDRESPONSE_SUCCESS)
	{
		while (!m_rs.IsEOF())
		{
			DWORD nFileID;
			m_rs.GetFieldValue(_T("FID"), nFileID);
			aList.Add(nFileID);
			m_rs.MoveNext();
		}
		m_rs.Close();
	}

	// delete versions belong to every file
	CString sDiskName;
	for (int i = 0; i < aList.GetSize(); i++)
	{
		if (Query(JDB_LIST_VER_ADMIN, aList[i]) != JDRESPONSE_SUCCESS)
			continue;

		nVers += m_rs.GetRecordCount();
		while (!m_rs.IsEOF())
		{
			m_rs.GetFieldValue(_T("FDiskName"), sDiskName);
			::DeleteFile(m_sRootPath + _T('\\') + sDiskName);			
			m_rs.MoveNext();
		}
		
		m_rs.Close();		
		Execute(JDB_DELETE_VERS, aList[i]);
	}

	// delete files
	nFiles += aList.GetSize();
	Execute(JDB_ADMIN_DELETE_FILES, nCatID);
	aList.RemoveAll();	

	// Get a list of sub-categories belong to this category
	if (Query(JDB_CHECK_CATEGORY_EMPTY_C, nCatID) == JDRESPONSE_SUCCESS)
	{
		while (!m_rs.IsEOF())
		{
			DWORD nCatID;
			m_rs.GetFieldValue(_T("FID"), nCatID);
			aList.Add(nCatID);
			m_rs.MoveNext();
		}
		m_rs.Close();

		for (int i = 0; i < aList.GetSize(); i++)
			CycleDelTree(aList[i], nCats, nFiles, nVers);
	}

	// delete subcategories
	nCats += aList.GetSize();
	Execute(JDB_DELETE_SUB_CATEGORIES, nCatID);

	// delete the category itself
	nCats++;
	Execute(JDB_DELETE_CATEGORY, nCatID);
}

DWORD CJDSession::DelTree(DWORD dwPacketID, CLBPacket *pPacket)
{
	_LOG("%s initiated DELTREE", (LPCTSTR)m_sUserName);
	if (m_nPrivilege < JDPRIV_SUPER_ADMIN)
		return JDRESPONSE_DENY;

	AUTO_CLOSE;
	DWORD nCatID = pPacket->ReadInt();
	DWORD dwResult = Query(JDB_REQUEST_SUB_CATEGORY, nCatID);
	VERIFY_RESULT;

	int nCats = 0, nFiles = 0, nVers = 0;
	CycleDelTree(nCatID, nCats, nFiles, nVers);

	OUT_RDY;
	OUT_INT(nCatID);
	OUT_INT(nCats);
	OUT_INT(nFiles);
	OUT_INT(nVers);
	OUT_GO;	
	_LOG("DELTREE completed");
	return JDRESPONSE_SUCCESS;
}

void CJDSession::GetSubFolders(DWORD dwParentID, const CString& sParentPath)
{
	DWORD dwResult = Query("SELECT FID, FName FROM t_Category WHERE FParentID=%d ORDER BY FName", dwParentID);
	if (dwResult != JDRESPONSE_SUCCESS)
		return;

	CDWordArray aCatIDs;
	CStringArray aCatPaths;

	while (!m_rs.IsEOF())
	{
		DWORD dwID = 0;
		CString sCatName;
		m_rs.GetFieldValue(_T("FID"), dwID);
		m_rs.GetFieldValue(_T("FName"), sCatName);
		m_rs.MoveNext();

		m_stm.AddCatInfo(dwID, sParentPath + _T('\\') + sCatName);
		
		aCatIDs.Add(dwID);
		aCatPaths.Add(sParentPath + _T('\\') + sCatName);
	}

	m_rs.Close();

	for (int i = 0; i < aCatIDs.GetSize(); i++)
		GetSubFolders(aCatIDs[i], aCatPaths[i]);
}

void CJDSession::GetSubFiles(DWORD dwParentID)
{
	//_LOG("GetSubFiles: %d", dwParentID);
	DWORD dwResult = Query("SELECT FID, FName FROM t_File WHERE FParentID=%d ORDER BY FName", dwParentID);
	if (dwResult != JDRESPONSE_SUCCESS)
		return;

	while (!m_rs.IsEOF())
	{
		DWORD dwID = 0;
		m_rs.GetFieldValue(_T("FID"), dwID);
		m_rs.MoveNext();
		m_stm.AddFileInfo(dwParentID, dwID);
	}

	m_rs.Close();
}

DWORD CJDSession::DownloadFileSystemStructure(DWORD dwPacketID, CLBPacket* pPacket)
{
	DWORD nCatID = pPacket->ReadInt();
	CString sClientPath = ReadString(pPacket);

	if (nCatID < 1 || sClientPath.IsEmpty())
		return JDRESPONSE_NOT_EXISTS;

	if (m_nPrivilege < JDPRIV_SUPERVISOR)
		return JDRESPONSE_DENY;

	AUTO_CLOSE;
	
	DWORD dwResult = Query(_T("SELECT * FROM t_Category WHERE FID=%d AND FParentID>0 AND (FRoleID=%d OR %d>%d)"), nCatID, m_nRoleID, m_nPrivilege, JDPRIV_SUPERVISOR);
	VERIFY_RESULT;

	CString sCatName;
	m_rs.GetFieldValue(_T("FName"), sCatName);
	m_rs.Close();

	m_stm.Reset();

	// Get a snapshot of the selected folder structure
	m_stm.AddCatInfo(nCatID, sCatName);
	GetSubFolders(nCatID, sCatName);

	for (int i = 0; i < m_stm.GetCatCount(); i++)
		GetSubFiles(m_stm.GetCatIDByIndex(i));		

	pPacket->PrepareForWrite();
	pPacket->WriteInt(JDRESPONSE_SUCCESS);
	pPacket->WriteString(sClientPath);
	pPacket->WriteString(sCatName);
	pPacket->WriteInt(m_stm.GetCatCount());
	pPacket->WriteInt(m_stm.GetFileCount());
	pPacket->Encode(JD_PACKET_FILESYSTEM_REQUEST);
	Send(pPacket);	

	return JDRESPONSE_SUCCESS;
}

DWORD CJDSession::SendFileSystemNext(DWORD dwPacketID, CLBPacket* pPacket)
{
	DWORD dwCatID = 0, dwFileID = 0;
	CString sPathOrName, sDiskName, sMD5;

	if (!m_stm.GetNext(dwFileID, dwCatID, sPathOrName))
		return JDRESPONSE_NOT_EXISTS;

	if (dwFileID)
	{
		AUTO_CLOSE;
		DWORD dwResult = Query(_T("SELECT * FROM v_Version WHERE FFileID=%d ORDER BY FVersion DESC"), dwFileID);
		if (dwResult == JDRESPONSE_SUCCESS)
		{
			m_rs.GetFieldValue(_T("FFileName"), sPathOrName);
			m_rs.GetFieldValue(_T("FDiskName"), sDiskName);
			m_rs.GetFieldValue(_T("FMD5"), sMD5);
		}
		
		if (sDiskName.IsEmpty())
		{
			// File invalid, but even in this case we still need to notify the client to keep up the "JD_PACKET_FILESYSTEM_NEXT" loops
			dwFileID = 0;
			dwCatID = 0;
			sPathOrName.Empty();
		}
	}

	pPacket->PrepareForWrite();
	pPacket->WriteDword(JDRESPONSE_SUCCESS);
	pPacket->WriteInt(m_stm.HasMore());
	pPacket->WriteInt(dwFileID);
	pPacket->WriteInt(dwCatID);
	pPacket->WriteString(sPathOrName);
	pPacket->Encode(JD_PACKET_FILESYSTEM_NEXT);
	Send(pPacket);

	if (dwFileID)
		SendFile(m_sRootPath + _T('\\') + sDiskName, sMD5);

	return JDRESPONSE_SUCCESS;
}

DWORD CJDSession::FinishFileSystemStructure(DWORD dwPacketID, CLBPacket* pPacket)
{
	//_LOG("FileSystemStructure clear.");
	m_stm.Reset();	
	return JDRESPONSE_SUCCESS;
}