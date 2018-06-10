// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__0467DAED_EF2E_43DA_9510_D501876DAFB9__INCLUDED_)
#define AFX_MAINFRM_H__0467DAED_EF2E_43DA_9510_D501876DAFB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxcview.h>
#include "ClientTreeView.h"
#include "RecoverListDlg.h"
#include "FlatSplitterWnd.h"
#include "LBStatusBar.h"
#include "TextToolBar.h"
#include "ProgressDlg.h"

class CMainFrame : public CNewFrameWnd
{
	
public:
	CMainFrame();

protected: 
	DECLARE_DYNAMIC(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:

	virtual ~CMainFrame();
#ifdef _DEBUG
#endif

protected:  // control bar embedded members

	CNewMenuBar m_wndMenuBar;
	CLBStatusBar  m_wndStatusBar;
	CTextToolBar m_wndToolBar;
	CFlatSplitterWnd m_wndSplitter;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdateStatusBarPanels(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLogin(CCmdUI* pCmdUI);
	afx_msg void OnLogout();
	afx_msg void OnUpdateLogout(CCmdUI* pCmdUI);
	afx_msg void OnServerConfig();
	afx_msg void OnChangepassword();
	afx_msg void OnUpdateChangepassword(CCmdUI* pCmdUI);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPromptLogin();
	afx_msg void OnAdminUsers();
	afx_msg void OnAdminRoles();
	afx_msg void OnUpdateCatAdd(CCmdUI* pCmdUI);
	afx_msg void OnCatAdd();
	afx_msg void OnUpdateCatPriv(CCmdUI* pCmdUI);
	afx_msg void OnCatPriv();
	afx_msg void OnUpdateCatAddfile(CCmdUI* pCmdUI);
	afx_msg void OnCatAddfile();
	afx_msg void OnUpdateFileAddVer(CCmdUI* pCmdUI);
	afx_msg void OnFileAddVer();
	afx_msg void OnVerDownload();
	afx_msg void OnUpdateVerDownload(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAdmin(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCatChange(CCmdUI* pCmdUI);
	afx_msg void OnCatChange();
	afx_msg void OnUpdateCatDelete(CCmdUI* pCmdUI);
	afx_msg void OnCatDelete();
	afx_msg void OnFileDelete();
	afx_msg void OnUpdateFileDelete(CCmdUI* pCmdUI);
	afx_msg void OnVerDelete();
	afx_msg void OnUpdateVerDelete(CCmdUI* pCmdUI);
	afx_msg void OnVerCheck();
	afx_msg void OnUpdateVerCheck(CCmdUI* pCmdUI);
	afx_msg void OnVerUncheck();
	afx_msg void OnUpdateVerUncheck(CCmdUI* pCmdUI);
	afx_msg void OnAppHelp();
	afx_msg void OnUploadpatch();
	afx_msg void OnUpdateUploadpatch(CCmdUI* pCmdUI);
	afx_msg void OnAdminCatInit();
	afx_msg void OnUpdateAdminCatInit(CCmdUI* pCmdUI);
	afx_msg void OnAdminRecovery();
	afx_msg void OnUpdateAdminRecovery(CCmdUI* pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnFileManagever();
	afx_msg void OnUpdateFileManagever(CCmdUI* pCmdUI);
	afx_msg void OnRefresh();
	afx_msg void OnUpdateRefresh(CCmdUI* pCmdUI);
	afx_msg void OnAdminDeltree();
	afx_msg void OnUpdateAdminDeltree(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnAdminDownloadtree();
	afx_msg void OnUpdateAdminDownloadtree(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void OnDelTree(CLBPacket* pPacket);
	LRESULT OnPatchInstall(WPARAM wParam, LPARAM lParam);
	void OnRecoverList(CLBPacket* pPacket);
	void ManageVersion(CLBPacket* pPacket);
	void UploadVersion(CLBPacket* pPacket);
	BOOL VerifyAdmin();
	void OnUpdateConfirm(CLBPacket* pPacket);
	
	LRESULT OnServerListError(WPARAM wParam, LPARAM lParam);
	LRESULT OnConnect(WPARAM wParam, LPARAM lParam);
	void OnResponseFileAdd(CLBPacket *pPacket);
	LRESULT OnTaskStart(WPARAM wParam, LPARAM lParam);
	void SetProgressText(LPCTSTR lpszText);
	void EndProgress();
	LRESULT OnServerError(WPARAM wParam, LPARAM lParam);
	void OnServerResponse(DWORD dwPacketID, CLBPacket* pPacket);
	LRESULT OnDisconnect(WPARAM wParam, LPARAM lParam);
	void OnLogin();
	
	int ShowMessage(CString sText, UINT nFlag = MB_ICONEXCLAMATION);

	LRESULT OnServerResponse(WPARAM wParam, LPARAM lParam);
		
	HICON m_hIcon;
	CNewMenu m_popup;
	CClientTreeView* m_pCatView;
	CClientListView* m_pFileView;
	CString m_sFilePath;
	CRecoverListDlg m_wndRecoverList;	
	CProgressDlg m_wndProgress;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__0467DAED_EF2E_43DA_9510_D501876DAFB9__INCLUDED_)
