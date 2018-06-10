// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__869FF6C4_8E6F_4409_B106_1AE5D6A47817__INCLUDED_)
#define AFX_STDAFX_H__869FF6C4_8E6F_4409_B106_1AE5D6A47817__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#define _ATL_APARTMENT_THREADED

#include <afxwin.h>
#include <afxext.h>
#include <afxdisp.h>
#include <atlbase.h>
#include "..\\JDArchiveCommDef.h"
#include "..\\LBNetwork.h"
#include "..\\LBServer.h"
#include "..\\LBServer.h"
#include "LogFile.h"

#define SVR_DESC	_T("金都资料库服务器")

//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module

class CServiceModule : public CComModule
{
public:
	HRESULT RegisterServer(BOOL bRegTypeLib, BOOL bService);
	HRESULT UnregisterServer();
	void Init(_ATL_OBJMAP_ENTRY* p, HINSTANCE h, UINT nServiceNameID, const GUID* plibid = NULL);
    void Start();
	void ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);
    void Handler(DWORD dwOpcode);
    void Run();
    BOOL IsInstalled();
    BOOL Install();
    BOOL Uninstall();
	LONG Unlock();
	void LogEvent(LPCTSTR pszFormat, ...);
    void SetServiceStatus(DWORD dwState);
    void SetupAsLocalServer();

//Implementation
private:
	static void WINAPI _ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);
    static void WINAPI _Handler(DWORD dwOpcode);

// data members
public:
    TCHAR m_szServiceName[256];
    SERVICE_STATUS_HANDLE m_hServiceStatus;
    SERVICE_STATUS m_status;
	DWORD dwThreadID;
	BOOL m_bService;
	CLogFile m_log;
};

extern CServiceModule _Module;
#include <atlcom.h>

#define _LOG	_Module.m_log.Write

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__869FF6C4_8E6F_4409_B106_1AE5D6A47817__INCLUDED)
