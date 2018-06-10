// JDArchive.h : main header file for the JDARCHIVE application
//

#if !defined(AFX_JDARCHIVE_H__6686100C_6DC2_4E55_B62F_7DDF11B977F6__INCLUDED_)
#define AFX_JDARCHIVE_H__6686100C_6DC2_4E55_B62F_7DDF11B977F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "WinAppEx.h"
#include "LogFile.h"
#include "JDClientSession.h"
#include "DatabaseCache.h"
#include "PersistFrameImpl.h"

/////////////////////////////////////////////////////////////////////////////
// CJDArchiveApp:
// See JDArchive.cpp for the implementation of this class
//

class CJDArchiveApp : public CWinAppEx
{
public:

	CJDArchiveApp();

	CLogFile m_log;
	CDatabaseCache m_cache;
	CJDClientSession m_session;
	CString m_sVersion;
	BOOL m_bInitOK;
	CString m_sTempFolder;
	CPersistFrameImpl m_pfl;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJDArchiveApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	//}}AFX_VIRTUAL

// Implementation

public:
	//{{AFX_MSG(CJDArchiveApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()	
};

// Some global functions/macros for convenience
CDatabaseCache&		AfxGetCache();
CJDClientSession&	AfxGetSession();
BOOL IsBackgroundPacket(DWORD dwPacketID);
BOOL IsSilentPacket(DWORD dwPacketID);
BOOL LookupPacket(DWORD dwPacketID, CString& sText);
BOOL LookupError(DWORD dwErrorCode, CString& sText);
BOOL LookupFileError(int nError, CString& sText);
CString ReadPacketString(CLBPacket* pPacket);
void _POST(UINT nMsg, WPARAM wParam = 0, LPARAM lParam = 0);
CString GetFilter(const CString& sFile, CString& sExt);
LPCTSTR AfxGetVersionStr();
CString AfxSizeToStr(DWORD dwSize);
CString AfxGetFileName(const CString& sFullPath);
CString AfxParseVersion(int nVersion);
BOOL AfxIsExecutable(LPCTSTR lpszFileName);
int AfxGetLeftPanelWidth();
void AfxSetLeftPanelWidth(int nWidth);
CString AfxGetUserName();
void AfxSetUserName(LPCTSTR lpszUserName);
void AfxSaveFrame();
CString _TEMP(LPCTSTR lpszAppend = NULL);

#define APP_VER		AfxGetVersionStr()
#define _LOG		((CJDArchiveApp*)AfxGetApp())->m_log.Write
#define _CACHE		(AfxGetCache())
#define _SESSION	(AfxGetSession())

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JDARCHIVE_H__6686100C_6DC2_4E55_B62F_7DDF11B977F6__INCLUDED_)
