#include "stdafx.h"
#include "JDArchive.h"
#include "MainFrm.h"
#include "UploadDlg.h"
#include "PatchConfirmDlg.h"
#include "VerManageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

LRESULT CMainFrame::OnServerError(WPARAM wParam, LPARAM lParam)
{
	if (wParam == JD_PACKET_PASTE_FILE || wParam == JD_PACKET_PASTE_CAT)
		m_pCatView->RemoveCut();

	CString sEvent, sError, sMsg;
	LookupPacket(wParam, sEvent);
	LookupError(lParam, sError);		
	sMsg.Format(_T("操作(%04d)：%s\n错误(%04d)：%s"), wParam, (LPCTSTR)sEvent, lParam, (LPCTSTR)sError);
	MessageBox(sMsg, _T("访问错误"));	
	return 0;
}

// Processes data sent from the server
LRESULT CMainFrame::OnServerResponse(WPARAM wParam, LPARAM lParam)
{
	CLBPacket* pPacket = (CLBPacket*)lParam;
	ASSERT(pPacket);
	OnServerResponse((DWORD)wParam, pPacket);
	delete pPacket;
	return (LRESULT)0;
}

void CMainFrame::OnServerResponse(DWORD dwPacketID, CLBPacket *pPacket)
{	
	CString sEvent;
	LookupPacket(dwPacketID, sEvent);	

	BOOL bShowPopup = FALSE;
	switch (dwPacketID)
	{
	case JD_PACKET_LOGIN:
		OnLogin();		
		break;

	case JD_PACKET_CHANGE_PASSWORD:
		bShowPopup = TRUE;
		break;

	case JD_PACKET_CAT_ADD:
		m_pCatView->AddOrUpdateCategory(pPacket, TRUE);
		bShowPopup = TRUE;
		break;

	case JD_PACKET_CAT_LIST:
	case JD_PACKET_REQUEST_CAT:
	case JD_PACKET_CAT_CHANGE:
	case JD_PACKET_CAT_ROOT:
		m_pCatView->AddOrUpdateCategory(pPacket);
		break;

	case JD_PACKET_CAT_DELETE:
		m_pCatView->DeleteCategory(pPacket->ReadInt());
		bShowPopup = TRUE;
		break;

	case JD_PACKET_FILE_LIST:
	case JD_PACKET_REQUEST_FILE:
		m_pCatView->AddOrUpdateFile(pPacket);		
		break;

	case JD_PACKET_FILE_ADD:
		m_pCatView->AddOrUpdateFile(pPacket, TRUE);
		OnResponseFileAdd(pPacket);
		break;

	case JD_PACKET_FILE_DELETE:
		m_pCatView->DeleteFile(pPacket->ReadInt());
		bShowPopup = TRUE;
		break;

	case JD_PACKET_VER_LIST:
		m_pFileView->AddVersion(pPacket);
		break;	

	case JD_PACKET_VER_DELETE:
		m_pFileView->DeleteVer(pPacket->ReadInt());
		ShowMessage(_T("版本数据已从资料库中移除，并将在一年以后从服务器数据库中彻底删除。\n如果你需要恢复这一版本数据，请在一年之内联系系统管理员。"), MB_ICONINFORMATION);
		break;

	case JD_PACKET_VER_CHECK:
		m_pFileView->CheckVersion(pPacket);
		bShowPopup = TRUE;
		break;

	case JD_PACKET_VER_UNCHECK:
		m_pFileView->UncheckVersion(pPacket);
		bShowPopup = TRUE;
		break;

	case JD_PACKET_UPLOAD_APP_RESULT:
		bShowPopup = TRUE;
		break;

	case JD_PACKET_CHECK_UPDATE:
		OnUpdateConfirm(pPacket);
		break;

	case JD_PACKET_CAT_INIT_END:
		bShowPopup = TRUE;
		break;

	case JD_PACKET_PASTE_FILE:
	case JD_PACKET_PASTE_CAT:
		m_pCatView->Paste(pPacket);
		bShowPopup = TRUE;
		break;

	case JD_PACKET_REQUEST_UPLOAD_VER:
		UploadVersion(pPacket);
		break;

	case JD_PACKET_FILE_MANAGE_VER_REQUEST:
		ManageVersion(pPacket);
		break;

	case JD_PACKET_FILE_MANAGE_VER_APPLY:
		bShowPopup = TRUE;
		break;

	case JD_PACKET_DATA_RECOVERY_LIST:
		OnRecoverList(pPacket);
		break;	

	case JD_PACKET_CAT_CHANGE_PRIV:
		m_pCatView->ChangeCategoryPriv(pPacket);
		bShowPopup = pPacket->IsSequenceEnd();
		break;

	case JD_PACKET_DELTREE:
		OnDelTree(pPacket);		
		break;

	default:
		break;
	}	

	if (bShowPopup)
		ShowMessage(sEvent + _T("成功"), MB_ICONINFORMATION);
}

void CMainFrame::ManageVersion(CLBPacket *pPacket)
{
	int nFileID = pPacket->ReadInt();
	int nVersion = pPacket->ReadInt();
	CString sFileName = ReadPacketString(pPacket);
	
	CVerManageDlg dlg(sFileName, nVersion, this);
	if (dlg.DoModal() == IDOK)
		_SESSION.TaskApplyVerManage(nFileID, dlg.m_nVersion);
}

void CMainFrame::UploadVersion(CLBPacket *pPacket)
{
	int nFileID = pPacket->ReadInt();
	int nVersion = pPacket->ReadInt();
	CString sFileName = ReadPacketString(pPacket);
	CUploadDlg dlg(sFileName, nVersion, m_sFilePath, this);
	m_sFilePath.Empty();
	if (dlg.DoModal() == IDOK)
		_SESSION.TaskUploadVersion(nFileID, dlg.m_sVerDesc, nVersion, dlg.m_sFilePath);
}

void CMainFrame::OnResponseFileAdd(CLBPacket *pPacket)
{
	pPacket->ResetCursor();
	pPacket->ReadDword();
	_SESSION.TaskRequestUploadVer(pPacket->ReadInt());
}

void CMainFrame::OnUpdateConfirm(CLBPacket *pPacket)
{
	CPatchConfirmDlg dlg(pPacket, this);
	if (dlg.DoModal() == IDOK)
		_SESSION.TaskPatchApp(dlg.m_nPatchID);
}

void CMainFrame::OnRecoverList(CLBPacket *pPacket)
{
	RECOVERDATA data;
	data.szDesc[0] = 0;
	data.szFileName[0] = 0;
	data.nVerID = pPacket->ReadInt();
	pPacket->ReadString(data.szFileName, 64);
	data.dwSize = pPacket->ReadDword();
	data.nVersion = pPacket->ReadInt();
	data.nCreatorID = pPacket->ReadInt();
	pPacket->ReadData(&data.createDate, sizeof(COleDateTime));
	data.nDeleterID = pPacket->ReadInt();
	pPacket->ReadData(&data.deleteDate, sizeof(COleDateTime));
	pPacket->ReadString(data.szDesc, 79);	
	m_wndRecoverList.m_aData.Add(data);
	
	if (pPacket->IsSequenceEnd())
	{
		if (m_wndRecoverList.DoModal() == IDOK)
			_SESSION.TaskRecoverData(m_wndRecoverList.m_nVerID);	
	}
}

void CMainFrame::OnDelTree(CLBPacket *pPacket)
{
	int nCatID = pPacket->ReadInt();
	int nCats = pPacket->ReadInt();
	int nFiles = pPacket->ReadInt();
	int nVers = pPacket->ReadInt();
	m_pCatView->DeleteCategory(nCatID);

	CString sMsg;
	sMsg.Format(_T("操作成功。\n共计删除目录%d个、文件%d个、版本%d个。"), nCats, nFiles, nVers);
	ShowMessage(sMsg, MB_ICONINFORMATION);
}




