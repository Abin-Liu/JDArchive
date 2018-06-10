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
		sText = _T("操作成功");
		break;

	case JDRESPONSE_INTERNAL_ERROR:
		sText = _T("服务器内部错误");
		break;
		
	case JDRESPONSE_DATA_ERROR:
		sText = _T("数据校验错误");
		break;

	case JDRESPONSE_DENY:
		sText = _T("访问被拒绝");
		break;

	case JDRESPONSE_ALREADY_EXISTS:
		sText = _T("对象已经存在");
		break;

	case JDRESPONSE_NOT_EXISTS:
		sText = _T("对象不存在");
		break;

	case JDRESPONSE_PARENT_NOT_EXISTS:
		sText = _T("上级对象不存在");
		break;

	case JDRESPONSE_TARGET_NOT_EMPTY:
		sText = _T("对象含有子项目");
		break;

	case JDRESPONSE_LOGIN_FAIL:
		sText = _T("无效的用户名或密码");
		break;

	case JDRESPONSE_USER_ABORT:
		sText = _T("操作被用户取消");
		break;

	case JDRESPONSE_VER_ALREADY_CHECKED:
		sText = _T("版本重复审批");
		break;

	case JDRESPONSE_VER_NOT_CHECKED:
		sText = _T("版本尚未被审批");
		break;

	case JDRESPONSE_VER_ALREADY_DELETED:
		sText = _T("版本重复删除");
		break;

	case JDRESPONSE_VER_VERSION_ERROR:
		sText = _T("版本号低于资料库现有版本号");
		break;

	case JDRESPONSE_NAME_CONFLICTS:
		sText = _T("目标位置已有同名对象存在");
		break;

	case JDRESPONSE_FILE_LOCKED:
		sText = _T("文件已被其它用户锁定");
		break;

	case JDRESPONSE_RECOVERY_DATA_NOTFOUND:
		sText = _T("未找到符合指定条件的可恢复数据");
		break;

	case JDRESPONSE_CLIENT_VER:
		sText = _T("客户端版本过低");
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
		sText = _T("文件传输成功");
		break;

	case CLBSession::FILE_ERROR_FILE_SYSTEM:
		sText = _T("无法访问文件");
		break;	

	case CLBSession::FILE_ERROR_ZERO_LENGTH:
		sText = _T("文件内容为空");
		break;

	case CLBSession::FILE_ERROR_TOO_LARGE:
		sText = _T("文件长度超出资料库限制");
		break;

	case CLBSession::FILE_ERROR_USER_ABORT:
		sText = _T("用户已取消传输");
		break;

	case CLBSession::FILE_ERROR_PEER_ABORT:
		sText = _T("对方用户已取消传输");
		break;

	case CLBSession::FILE_ERROR_NETWORK:
		sText = _T("网络错误");
		break;

	case CLBSession::FILE_ERROR_MD5_MISMATCH:
		sText = _T("文件MD5校验失败");
		break;

	case 0xffff:
		sText = _T("文件数据解密失败");
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
		sText = _T("用户登录");
		break;

	case JD_PACKET_CHANGE_PASSWORD:
		sText = _T("更改密码");
		break;

	case JD_PACKET_USER_LIST:
		sText = _T("获取用户列表");
		break;

	case JD_PACKET_USER_ADD:
		sText = _T("创建用户");
		break;

	case JD_PACKET_USER_CHANGE: 
		sText = _T("修改用户"); 
		break;

	case JD_PACKET_USER_RESETPASSWORD:
		sText = _T("重置用户密码"); 
		break;
	
	case JD_PACKET_ROLE_LIST:
		sText = _T("获取用户组列表");
		break;

	case JD_PACKET_ROLE_ADD: 
		sText = _T("创建用户组");
		break;

	case JD_PACKET_ROLE_CHANGE: 
		sText = _T("修改用户组");
		break;	

	case JD_PACKET_CAT_LIST:
		sText = _T("获取目录列表");
		break;

	case JD_PACKET_CAT_ADD: 
		sText = _T("创建目录");
		break;

	case JD_PACKET_CAT_CHANGE: 
		sText = _T("修改目录"); 
		break;

	case JD_PACKET_CAT_DELETE: 
		sText = _T("删除目录");
		break;

	case JD_PACKET_CAT_ROOT:
		sText = _T("获取用户组根目录");
		break;

	case JD_PACKET_FILE_LIST:
		sText = _T("获取文件列表");
		break;
		
	case JD_PACKET_FILE_ADD: 
		sText = _T("创建文件");
		break;

	case JD_PACKET_FILE_DELETE:
		sText = _T("删除文件");
		break;
	
	case JD_PACKET_VER_LIST:
		sText = _T("获取版本列表");
		break;
		
	case JD_PACKET_VER_CHECK:
		sText = _T("审核版本"); 
		break;

	case JD_PACKET_VER_UNCHECK:
		sText = _T("反审核版本"); 
		break;

	case JD_PACKET_VER_DELETE: 
		sText = _T("删除版本"); 
		break;
	
	case JD_PACKET_VER_DOWNLOAD:
		sText = _T("下载版本");
		break;
	
	case JD_PACKET_VER_UPLOAD: 
		sText = _T("创建版本"); 
		break;	

	case JD_PACKET_REQUEST_USER:
		sText = _T("请求用户数据"); 
		break;

	case JD_PACKET_REQUEST_ROLE:
		sText = _T("请求用户组数据"); 
		break;

	case JD_PACKET_REQUEST_CAT:
		sText = _T("请求目录数据"); 
		break;

	case JD_PACKET_REQUEST_FILE:
		sText = _T("请求文件数据"); 
		break;

	case JD_PACKET_REQUEST_VER:
		sText = _T("请求版本数据"); 
		break;

	case JD_PACKET_VER_DOWNLOAD_DATA_BEGIN:
		sText = _T("下载版本数据开始"); 
		break;

	case JD_PACKET_CHECK_UPDATE:
		sText = _T("获取程序更新数据"); 
		break;

	case JD_PACKET_UPDATE_DOWNLOAD:
		sText = _T("下载更新程序");
		break;

	case JD_PACKET_UPLOAD_APP:
		sText = _T("上传程序更新文件");
		break;

	case JD_PACKET_UPLOAD_APP_RESULT:
		sText = _T("上传程序更新文件");
		break;

	case JD_PACKET_CAT_INIT_START:
		sText = _T("目录初始化开始");
		break;

	case JD_PACKET_CAT_INIT_CAT:
		sText = _T("目录初始化-上传目录");
		break;

	case JD_PACKET_CAT_INIT_FILE:
		sText = _T("目录初始化-上传文件");
		break;

	case JD_PACKET_CAT_INIT_END:
		sText = _T("目录初始化结束");
		break;

	case JD_PACKET_PASTE_FILE:
		sText = _T("粘贴文件");
		break;

	case JD_PACKET_PASTE_CAT:
		sText = _T("粘贴目录");
		break;

	case JD_PACKET_REQUEST_UPLOAD_VER:
		sText = _T("申请上传版本");
		break;

	case JD_PACKET_CANCEL_REQUEST_UPLOAD_VER:
		sText = _T("取消申请上传版本");
		break;

	case JD_PACKET_FILE_MANAGE_VER_REQUEST:
		sText = _T("申请版本号管理");
		break;

	case JD_PACKET_FILE_MANAGE_VER_APPLY:
		sText = _T("提交版本号管理");
		break;

	case JD_PACKET_DATA_RECOVERY_LIST:
		sText = _T("申请数据恢复");
		break;

	case JD_PACKET_RECOVERY:
	case JD_PACKET_RECOVERY_DOWNLOAD:
		sText = _T("数据恢复");
		break;

	case JD_PACKET_CAT_CHANGE_PRIV:
		sText = _T("修改目录共享权限");
		break;

	case JD_PACKET_DELTREE:
		sText = _T("删除目录树");
		break;

	case JD_PACKET_FILESYSTEM_REQUEST:
		sText = _T("目录树下载申请");
		break;

	case JD_PACKET_FILESYSTEM_NEXT:
		sText = _T("目录树下载-内容");
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