#include "stdafx.h"
#include "..\\JDArchiveCommDef.h"

CString ReadPacketString(CLBPacket* pPacket)
{
	ASSERT(pPacket);
	CString str;
	pPacket->ReadString(str.GetBuffer(MAX_PATH + 1), MAX_PATH);
	str.ReleaseBuffer(-1);
	return str;
}

BOOL LookupError(DWORD dwErrorCode, CString& sText)
{
	BOOL bResult = TRUE;
	switch (dwErrorCode)
	{
	case JDRESPONSE_SUCCESS:
		sText = _T("�����ɹ�");
		break;

	case JDRESPONSE_INTERNAL_ERROR:
		sText = _T("�������ڲ�����");
		break;
		
	case JDRESPONSE_DATA_ERROR:
		sText = _T("����У�����");
		break;

	case JDRESPONSE_DENY:
		sText = _T("���ʱ��ܾ�");
		break;

	case JDRESPONSE_ALREADY_EXISTS:
		sText = _T("�����Ѿ�����");
		break;

	case JDRESPONSE_NOT_EXISTS:
		sText = _T("���󲻴���");
		break;

	case JDRESPONSE_PARENT_NOT_EXISTS:
		sText = _T("�ϼ����󲻴���");
		break;

	case JDRESPONSE_TARGET_NOT_EMPTY:
		sText = _T("����������Ŀ");
		break;

	case JDRESPONSE_LOGIN_FAIL:
		sText = _T("��Ч���û���������");
		break;

	case JDRESPONSE_USER_ABORT:
		sText = _T("�������û�ȡ��");
		break;

	case JDRESPONSE_VER_ALREADY_CHECKED:
		sText = _T("�汾�ظ�����");
		break;

	case JDRESPONSE_VER_NOT_CHECKED:
		sText = _T("�汾��δ������");
		break;

	case JDRESPONSE_VER_ALREADY_DELETED:
		sText = _T("�汾�ظ�ɾ��");
		break;

	case JDRESPONSE_VER_VERSION_ERROR:
		sText = _T("�汾�ŵ������Ͽ����а汾��");
		break;

	case JDRESPONSE_NAME_CONFLICTS:
		sText = _T("Ŀ��λ������ͬ���������");
		break;

	case JDRESPONSE_FILE_LOCKED:
		sText = _T("�ļ��ѱ������û�����");
		break;

	case JDRESPONSE_RECOVERY_DATA_NOTFOUND:
		sText = _T("δ�ҵ�����ָ�������Ŀɻָ�����");
		break;

	case JDRESPONSE_CLIENT_VER:
		sText = _T("�ͻ��˰汾����");
		break;

	default:
		bResult = FALSE;
		break;
	}
	return bResult;
}

BOOL LookupFileError(int nError, CString& sText)
{
	BOOL bResult = TRUE;
	sText.Empty();
	switch(nError)
	{
	case CLBSession::FILE_ERROR_SUCCESS:
		sText = _T("�ļ�����ɹ�");
		break;

	case CLBSession::FILE_ERROR_FILE_SYSTEM:
		sText = _T("�޷������ļ�");
		break;	

	case CLBSession::FILE_ERROR_ZERO_LENGTH:
		sText = _T("�ļ�����Ϊ��");
		break;

	case CLBSession::FILE_ERROR_TOO_LARGE:
		sText = _T("�ļ����ȳ������Ͽ�����");
		break;

	case CLBSession::FILE_ERROR_USER_ABORT:
		sText = _T("�û���ȡ������");
		break;

	case CLBSession::FILE_ERROR_PEER_ABORT:
		sText = _T("�Է��û���ȡ������");
		break;

	case CLBSession::FILE_ERROR_NETWORK:
		sText = _T("�������");
		break;

	case CLBSession::FILE_ERROR_MD5_MISMATCH:
		sText = _T("�ļ�MD5У��ʧ��");
		break;

	case 0xffff:
		sText = _T("�ļ����ݽ���ʧ��");
		break;

	default:
		bResult = FALSE;
		break;
	}
	return bResult;
}

BOOL LookupPacket(DWORD dwPacketID, CString& sText)
{
	sText.Empty();
	BOOL bResult = TRUE;
	switch (dwPacketID)
	{
	case JD_PACKET_LOGIN:
		sText = _T("�û���¼");
		break;

	case JD_PACKET_CHANGE_PASSWORD:
		sText = _T("��������");
		break;

	case JD_PACKET_USER_LIST:
		sText = _T("��ȡ�û��б�");
		break;

	case JD_PACKET_USER_ADD:
		sText = _T("�����û�");
		break;

	case JD_PACKET_USER_CHANGE: 
		sText = _T("�޸��û�"); 
		break;

	case JD_PACKET_USER_RESETPASSWORD:
		sText = _T("�����û�����"); 
		break;
	
	case JD_PACKET_ROLE_LIST:
		sText = _T("��ȡ�û����б�");
		break;

	case JD_PACKET_ROLE_ADD: 
		sText = _T("�����û���");
		break;

	case JD_PACKET_ROLE_CHANGE: 
		sText = _T("�޸��û���");
		break;	

	case JD_PACKET_CAT_LIST:
		sText = _T("��ȡĿ¼�б�");
		break;

	case JD_PACKET_CAT_ADD: 
		sText = _T("����Ŀ¼");
		break;

	case JD_PACKET_CAT_CHANGE: 
		sText = _T("�޸�Ŀ¼"); 
		break;

	case JD_PACKET_CAT_DELETE: 
		sText = _T("ɾ��Ŀ¼");
		break;

	case JD_PACKET_CAT_ROOT:
		sText = _T("��ȡ�û����Ŀ¼");
		break;

	case JD_PACKET_FILE_LIST:
		sText = _T("��ȡ�ļ��б�");
		break;
		
	case JD_PACKET_FILE_ADD: 
		sText = _T("�����ļ�");
		break;

	case JD_PACKET_FILE_DELETE:
		sText = _T("ɾ���ļ�");
		break;
	
	case JD_PACKET_VER_LIST:
		sText = _T("��ȡ�汾�б�");
		break;
		
	case JD_PACKET_VER_CHECK:
		sText = _T("��˰汾"); 
		break;

	case JD_PACKET_VER_UNCHECK:
		sText = _T("����˰汾"); 
		break;

	case JD_PACKET_VER_DELETE: 
		sText = _T("ɾ���汾"); 
		break;
	
	case JD_PACKET_VER_DOWNLOAD:
		sText = _T("���ذ汾");
		break;
	
	case JD_PACKET_VER_UPLOAD: 
		sText = _T("�����汾"); 
		break;	

	case JD_PACKET_REQUEST_USER:
		sText = _T("�����û�����"); 
		break;

	case JD_PACKET_REQUEST_ROLE:
		sText = _T("�����û�������"); 
		break;

	case JD_PACKET_REQUEST_CAT:
		sText = _T("����Ŀ¼����"); 
		break;

	case JD_PACKET_REQUEST_FILE:
		sText = _T("�����ļ�����"); 
		break;

	case JD_PACKET_REQUEST_VER:
		sText = _T("����汾����"); 
		break;

	case JD_PACKET_VER_DOWNLOAD_DATA_BEGIN:
		sText = _T("���ذ汾���ݿ�ʼ"); 
		break;

	case JD_PACKET_CHECK_UPDATE:
		sText = _T("��ȡ�����������"); 
		break;

	case JD_PACKET_UPDATE_DOWNLOAD:
		sText = _T("���ظ��³���");
		break;

	case JD_PACKET_UPLOAD_APP:
		sText = _T("�ϴ���������ļ�");
		break;

	case JD_PACKET_UPLOAD_APP_RESULT:
		sText = _T("�ϴ���������ļ�");
		break;

	case JD_PACKET_CAT_INIT_START:
		sText = _T("Ŀ¼��ʼ����ʼ");
		break;

	case JD_PACKET_CAT_INIT_CAT:
		sText = _T("Ŀ¼��ʼ��-�ϴ�Ŀ¼");
		break;

	case JD_PACKET_CAT_INIT_FILE:
		sText = _T("Ŀ¼��ʼ��-�ϴ��ļ�");
		break;

	case JD_PACKET_CAT_INIT_END:
		sText = _T("Ŀ¼��ʼ������");
		break;

	case JD_PACKET_PASTE_FILE:
		sText = _T("ճ���ļ�");
		break;

	case JD_PACKET_PASTE_CAT:
		sText = _T("ճ��Ŀ¼");
		break;

	case JD_PACKET_REQUEST_UPLOAD_VER:
		sText = _T("�����ϴ��汾");
		break;

	case JD_PACKET_CANCEL_REQUEST_UPLOAD_VER:
		sText = _T("ȡ�������ϴ��汾");
		break;

	case JD_PACKET_FILE_MANAGE_VER_REQUEST:
		sText = _T("����汾�Ź���");
		break;

	case JD_PACKET_FILE_MANAGE_VER_APPLY:
		sText = _T("�ύ�汾�Ź���");
		break;

	case JD_PACKET_DATA_RECOVERY_LIST:
		sText = _T("�������ݻָ�");
		break;

	case JD_PACKET_RECOVERY:
	case JD_PACKET_RECOVERY_DOWNLOAD:
		sText = _T("���ݻָ�");
		break;

	case JD_PACKET_CAT_CHANGE_PRIV:
		sText = _T("�޸�Ŀ¼����Ȩ��");
		break;

	case JD_PACKET_DELTREE:
		sText = _T("ɾ��Ŀ¼��");
		break;

	case JD_PACKET_FILESYSTEM_REQUEST:
		sText = _T("Ŀ¼����������");
		break;

	case JD_PACKET_FILESYSTEM_NEXT:
		sText = _T("Ŀ¼������-����");
		break;	

	default:
		bResult = FALSE;
		break;
	};
	return bResult;
}

// Background packets never borther the user in any way, at all
BOOL IsBackgroundPacket(DWORD dwPacketID)
{
	if (dwPacketID <= JD_PACKET_HEARTBEAT || dwPacketID >= JD_PACKET_DEFINE_END)
		return TRUE;

	BOOL bResult = TRUE;
	switch (dwPacketID)
	{
	case JD_PACKET_HEARTBEAT:
	case JD_PACKET_REQUEST_USER:
	case JD_PACKET_REQUEST_ROLE:
	//case JD_PACKET_VER_DOWNLOAD:
	//case JD_PACKET_VER_UPLOAD:
	//case JD_PACKET_CAT_INIT_START:
	case JD_PACKET_CANCEL_REQUEST_UPLOAD_VER:
	//case JD_PACKET_RECOVERY:
	case JD_PACKET_RECOVERY_DOWNLOAD:
	case JD_PACKET_UPDATE_DOWNLOAD:
		break;
		
	default:
		bResult = FALSE;
		break;
	}
	return bResult;
}

// Silent packets reflects the GUI in some way, but not at all when errors occur
BOOL IsSilentPacket(DWORD dwPacketID)
{
	if (IsBackgroundPacket(dwPacketID))
		return TRUE;

	BOOL bResult = TRUE;
	switch (dwPacketID)
	{
	case JD_PACKET_USER_LIST:
	case JD_PACKET_ROLE_LIST:
	case JD_PACKET_CAT_LIST:
	case JD_PACKET_FILE_LIST:
	case JD_PACKET_VER_LIST:
	case JD_PACKET_REQUEST_CAT:	
	case JD_PACKET_REQUEST_FILE:
	case JD_PACKET_REQUEST_VER:
	case JD_PACKET_CHECK_UPDATE:
	case JD_PACKET_CANCEL_REQUEST_UPLOAD_VER:
	case JD_PACKET_FILESYSTEM_NEXT:
		break;
		
	default:
		bResult = FALSE;
		break;
	}
	return bResult;
}