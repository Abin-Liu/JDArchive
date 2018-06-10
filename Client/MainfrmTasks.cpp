#include "stdafx.h"
#include "JDArchive.h"
#include "MainFrm.h"
#include "ChangePwdDlg.h"
#include "LoginDlg.h"
#include "CatDlg.h"
#include "CatPrivDlg.h"
#include "UserListDlg.h"
#include "RoleListDlg.h"
#include "FileDlg.h"
#include "PatchUploadDlg.h"
#include "CatInitDlg.h"
#include "RecoverDlg.h"
#include "DownloadTreeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define _RDY		CLBPacket packet; packet.PrepareForWrite();
#define _INT(x)		packet.WriteInt(x);
#define _STR(x)		packet.WriteString(x);
#define _ADD(x)		packet.Encode(x);	_SESSION.AddTask(&packet);	_LOG(_T("Task added: %d"), x);

LRESULT CMainFrame::OnTaskStart(WPARAM wParam, LPARAM lParam)
{
	if (IsSilentPacket(wParam))
		return 0;
	
	CString sStatusText;
	if (LookupPacket(wParam, sStatusText))
		SetProgressText(sStatusText);

	return 0;
}

BOOL CMainFrame::VerifyAdmin()
{
	CLoginDlg dlg(this);
	return dlg.DoModal() == IDOK;
}

void CMainFrame::OnPromptLogin()
{
	// TODO: Add your command handler code here
	if (_SESSION.IsLoggedin())
		return;

	CLoginDlg dlg(this);
	if (dlg.DoModal() == IDOK)
		_SESSION.TaskLogin(dlg.GetUserName(), dlg.GetPassword());
}

void CMainFrame::OnChangepassword() 
{
	// TODO: Add your command handler code here
	CChangePwdDlg dlg;
	if (dlg.DoModal() != IDOK)
		return;

	if (!_SESSION.IsLoggedin())
		return;

	_SESSION.TaskChangePassword(dlg.GetOldPassword(), dlg.GetNewPassword());	
}

void CMainFrame::OnCatPriv() 
{
	// TODO: Add your command handler code here
	if (!m_pCatView->IsSelectionCat(FALSE))
		return;

	LPCTREEDATA pData = m_pCatView->GetSelectionData();
	if (!pData || pData->bIsFile)
		return;
	
	int nCatID = pData->nID;
	CString sDesc = pData->sTooltip;
	CCatPrivDlg dlg(pData->nRoleID, pData->sPrivileges, this);
	if (dlg.DoModal() == IDOK)
		_SESSION.TaskChangeCategoryPriv(nCatID, dlg.GetPrivileges(), dlg.m_bApplySubCats);	
}

void CMainFrame::OnCatAdd() 
{
	// TODO: Add your command handler code here	
	if (!m_pCatView->IsSelectionCat(TRUE))
		return;

	LPTREEDATA pData = m_pCatView->GetSelectionData();
	if (!pData)
		return;

	int nRoleID = pData->nRoleID;
	int nParentCatID = pData->nID;
	CString sParentName = m_pCatView->GetSelectionText();

	CCatDlg dlg(nRoleID, nParentCatID, sParentName);
	if (dlg.DoModal() == IDOK)
		_SESSION.TaskAddCategory(nParentCatID, dlg.m_sName, dlg.m_sDesc);	
}

void CMainFrame::OnCatAddfile() 
{
	// TODO: Add your command handler code here
	if (!m_pCatView->IsSelectionCat(FALSE))
		return;

	int nParentCatID = m_pCatView->GetSelectionID();
	if (nParentCatID < 1)
		return;

	CFileDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_sFilePath = dlg.m_sFilePath;
		_SESSION.TaskAddFile(nParentCatID, dlg.m_sFileName, dlg.m_sDesc);
	}
}

void CMainFrame::OnFileAddVer() 
{
	// TODO: Add your command handler code here
	LPTREEDATA pData = m_pCatView->GetSelectionData();
	if (pData && pData->bIsFile)
		_SESSION.TaskRequestUploadVer(pData->nID);	
}

void CMainFrame::OnVerDownload() 
{
	// TODO: Add your command handler code here	
	int nVerID = m_pFileView->GetSelectedVerID();
	if (nVerID > 0)
		_SESSION.TaskDownloadVersion(nVerID);	
}

void CMainFrame::OnCatChange() 
{
	if (!m_pCatView->IsSelectionCat(FALSE))
		return;

	LPTREEDATA pData = m_pCatView->GetSelectionData();
	if (!pData)
		return;

	int nCatID = pData->nID;
	int nRoleID = pData->nRoleID;
	CString sName = m_pCatView->GetSelectionText();
	CString sDesc = pData->sTooltip;

	HTREEITEM hItem = m_pCatView->GetSelectionParent();
	if (hItem == NULL)
		return;
	
	CString sParentName = m_pCatView->GetTreeCtrl().GetItemText(hItem);
	CCatDlg dlg(nRoleID, 0, sParentName, nCatID, sName, sDesc, this);
	if (dlg.DoModal() == IDOK)
	{
		if (dlg.m_sName.Compare(sName))
			sName = dlg.m_sName;
		else
			sName.Empty();

		_SESSION.TaskChangeCategory(nCatID, sName, dlg.m_sDesc);
	}
}

void CMainFrame::OnCatDelete() 
{
	if (m_pCatView->IsSelectionCat(FALSE))
		_SESSION.TaskDeleteCategory(m_pCatView->GetSelectionID());	
}

void CMainFrame::OnFileDelete() 
{
	if (m_pCatView->IsSelectionFile())
		_SESSION.TaskDeleteFile(m_pCatView->GetSelectionID());
}

void CMainFrame::OnVerDelete() 
{
	int nVerID = m_pFileView->GetSelectedVerID();
	if (nVerID < 1)
		return;

	LPCVERDATA p = _CACHE.GetVersion(nVerID);
	if (p == NULL)
		return;

	if (p->nCheckerID > 0)
	{
		ShowMessage(_T("版本已审核，请先反审核后再删除。"));
		return;
	}

	if (ShowMessage(_T("此操作将使选定的版本从资料库中删除，\n确定吗？"), MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2) == IDYES)
		_SESSION.TaskDeleteVersion(nVerID);
}

void CMainFrame::OnVerCheck() 
{
	int nVerID = m_pFileView->GetSelectedVerID();
	if (nVerID > 0)
		_SESSION.TaskCheckVersion(nVerID);
}

void CMainFrame::OnVerUncheck() 
{
	int nVerID = m_pFileView->GetSelectedVerID();
	if (nVerID > 0)
		_SESSION.TaskUncheckVersion(nVerID);
}

void CMainFrame::OnAdminUsers() 
{
	// TODO: Add your command handler code here
	if (!VerifyAdmin())
		return;

	CUserListDlg dlg;
	dlg.DoModal();
}

void CMainFrame::OnAdminRoles() 
{
	// TODO: Add your command handler code here
	if (!VerifyAdmin())
		return;
	CRoleListDlg dlg;
	dlg.DoModal();
}

void CMainFrame::OnUploadpatch() 
{
	if (!VerifyAdmin())
		return;
	CPatchUploadDlg dlg(this);
	if (dlg.DoModal() == IDOK)
		_SESSION.TaskUploadPatch(dlg.m_sFileName, dlg.m_sFilePath);
}

void CMainFrame::OnAdminCatInit() 
{
	if (!m_pCatView->VerifySelectionCategoryRole(JDPRIV_SUPERVISOR, TRUE))
		return;
		
	int nParentCatID = m_pCatView->GetSelectionID();	
	if (nParentCatID < 1)
		return;

	//_SESSION.TaskCatInitStart(nParentCatID, "Abin", "D:\\Abin\\Desktop\\abin"); // debug

	if (!VerifyAdmin())
		return;

	CCatInitDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		if (MessageBox(_T("警告：\n此操作将把整个目录（包括多级子目录和文件）上传到资料库中，并全部以你的名义通过审核。确定要继续吗？"), _T("目录上传"), MB_ICONEXCLAMATION | MB_YESNO | MB_DEFBUTTON2) == IDYES)
			_SESSION.TaskCatInitStart(nParentCatID, AfxGetFileName(dlg.m_sPathName), dlg.m_sPathName);
	}
}

void CMainFrame::OnEditCut() 
{
	if (GetFocus() == m_pCatView)
		m_pCatView->Cut();
}

void CMainFrame::OnEditPaste() 
{
	if (GetFocus() != m_pCatView)
		return;
		
	LPTREEDATA p = m_pCatView->GetCutData();
	if (p == NULL)
		return;

	BOOL bIsFile = p->bIsFile;
	int nID = p->nID;
	if (!m_pCatView->CanPaste())
		return;

	CString sMsg;
	sMsg.Format(_T("此操作将把%s\"%s\"移动到目录\"%s\"中，确定要继续吗？"), bIsFile ? _T("文件") : _T("目录"), (LPCTSTR)m_pCatView->GetCutText(), (LPCTSTR)m_pCatView->GetSelectionText());
	if (ShowMessage(sMsg, MB_ICONQUESTION | MB_YESNO) == IDYES)
	{
		m_pCatView->LockCut(TRUE);
		_SESSION.TaskPasteItem(bIsFile, nID, m_pCatView->GetSelectionID());
	}
}

void CMainFrame::OnFileManagever() 
{
	if (m_pCatView->GetSelectionStatus() == CClientTreeView::SEL_FILE)
		_SESSION.TaskRequestManageVer(m_pCatView->GetSelectionID());
}

void CMainFrame::OnAdminRecovery() 
{
	if (!VerifyAdmin())
		return;

	CRecoverDlg dlg;
	if (dlg.DoModal())
	{
		m_wndRecoverList.m_aData.RemoveAll();
		_SESSION.TaskRequestRecovery(dlg.m_nRoleID, dlg.m_sFileName);	
	}
}

void CMainFrame::OnAdminDeltree() 
{
	if (!VerifyAdmin())
		return;

	LPCTREEDATA pData = m_pCatView->GetSelectionData();
	if (pData->bIsFile || pData->nID < 1 || pData->nParentID < 1)
		return;

	int nCatID = pData->nID;
	CString sMsg;
	sMsg.Format(_T("此操作将对目录\"%s\"进行整体删除，该目录自身及其下属所有子目录、文件和版本都将被永久删除，并且无法恢复。\n确定要继续吗？"), (LPCTSTR)pData->sName);
	if (MessageBox(sMsg, _T("删除目录树"), MB_ICONQUESTION | MB_OKCANCEL) == IDOK)
		_SESSION.TaskDelTree(nCatID);
}

void CMainFrame::OnAdminDownloadtree() 
{
	if (!m_pCatView->VerifySelectionCategoryRole(JDPRIV_SUPERVISOR, TRUE))
		return;
		
	int nCatID = m_pCatView->GetSelectionID();	
	if (nCatID < 1)
		return;

	if (!VerifyAdmin())
		return;

	CDownloadTreeDlg dlg;
	if (dlg.DoModal() == IDOK)
		_SESSION.TaskDownloadTreeRequest(nCatID, dlg.m_sPathName);
}






