#include "stdafx.h"
#include "JDArchive.h"
#include "MainFrm.h"
#include "ServerCfgDlg.h"
#include "ClientListView.h"

#define _CHECK(x)	pCmdUI->Enable(x)
#define _IS_USER	pCmdUI->Enable(_SESSION.IsLoggedin())
#define _IS_ADMIN	pCmdUI->Enable(_SESSION.VerifyPrivilege(JDPRIV_ADMIN))

void CMainFrame::OnUpdateLogin(CCmdUI* pCmdUI) 
{
	_CHECK(!_SESSION.IsLoggedin());
}

void CMainFrame::OnUpdateLogout(CCmdUI* pCmdUI) 
{
	_IS_USER;
}

void CMainFrame::OnUpdateChangepassword(CCmdUI* pCmdUI) 
{
	_IS_USER;
}

void CMainFrame::OnUpdateAdmin(CCmdUI* pCmdUI)
{
	_IS_ADMIN;
}

void CMainFrame::OnUpdateCatAdd(CCmdUI* pCmdUI) 
{
	_CHECK(m_pCatView->VerifySelectionCategoryRole(JDPRIV_SUPERVISOR, TRUE));
}

void CMainFrame::OnUpdateCatPriv(CCmdUI* pCmdUI) 
{
	_CHECK(m_pCatView->VerifySelectionCategoryRole(JDPRIV_SUPERVISOR, FALSE));
}

void CMainFrame::OnUpdateCatAddfile(CCmdUI* pCmdUI) 
{
	_CHECK(m_pCatView->VerifySelectionCategoryRole(JDPRIV_POWER_USER, FALSE));
}

void CMainFrame::OnUpdateFileAddVer(CCmdUI* pCmdUI) 
{
	_CHECK(m_pCatView->VerifySelectionFileRole(JDPRIV_POWER_USER));
}

void CMainFrame::OnUpdateVerDownload(CCmdUI* pCmdUI) 
{
	_CHECK(m_pFileView->GetSelectedVerID());
}

void CMainFrame::OnUpdateCatChange(CCmdUI* pCmdUI) 
{
	_CHECK(m_pCatView->VerifySelectionCategoryRole(JDPRIV_SUPERVISOR, FALSE));
}

void CMainFrame::OnUpdateCatDelete(CCmdUI* pCmdUI) 
{
	_CHECK(!m_pCatView->SelectionHasChildren() && m_pCatView->VerifySelectionCategoryRole(JDPRIV_SUPERVISOR, FALSE));
}

void CMainFrame::OnUpdateFileDelete(CCmdUI* pCmdUI) 
{
	_CHECK(m_pCatView->VerifySelectionFileRole(JDPRIV_SUPERVISOR));
}

void CMainFrame::OnUpdateVerDelete(CCmdUI* pCmdUI) 
{
	_CHECK(m_pFileView->GetSelectedVerCheckState() == 0 && m_pCatView->VerifySelectionFileRole(JDPRIV_SUPERVISOR));	
}

void CMainFrame::OnUpdateVerCheck(CCmdUI* pCmdUI) 
{
	_CHECK(m_pFileView->GetSelectedVerCheckState() == 0 && m_pCatView->VerifySelectionFileRole(JDPRIV_SUPERVISOR));
}

void CMainFrame::OnUpdateVerUncheck(CCmdUI* pCmdUI) 
{
	_CHECK(m_pFileView->GetSelectedVerCheckState() == 1 && m_pCatView->VerifySelectionFileRole(JDPRIV_SUPERVISOR));
}

void CMainFrame::OnUpdateUploadpatch(CCmdUI* pCmdUI) 
{
	_CHECK(_SESSION.VerifyPrivilege(JDPRIV_SUPER_ADMIN));
}

void CMainFrame::OnUpdateAdminCatInit(CCmdUI* pCmdUI) 
{		
	_CHECK(m_pCatView->VerifySelectionCategoryRole(JDPRIV_SUPERVISOR, TRUE));
}

void CMainFrame::OnUpdateAdminRecovery(CCmdUI* pCmdUI) 
{
	_CHECK(_SESSION.VerifyPrivilege(JDPRIV_SUPER_ADMIN));
}

void CMainFrame::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	_CHECK(m_pCatView->CanCut());
}

void CMainFrame::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	_CHECK(m_pCatView->CanPaste());
}

void CMainFrame::OnUpdateFileManagever(CCmdUI* pCmdUI) 
{
	_CHECK(m_pCatView->VerifySelectionFileRole(JDPRIV_SUPERVISOR));
}

void CMainFrame::OnUpdateAdminDeltree(CCmdUI* pCmdUI) 
{
	_CHECK(m_pCatView->GetSelectionStatus() == CClientTreeView::SEL_CAT && _SESSION.VerifyPrivilege(JDPRIV_SUPER_ADMIN));
}

void CMainFrame::OnUpdateAdminDownloadtree(CCmdUI* pCmdUI) 
{
	_CHECK(m_pCatView->GetSelectionStatus() == CClientTreeView::SEL_CAT && _SESSION.VerifyPrivilege(JDPRIV_SUPERVISOR, TRUE));
}


