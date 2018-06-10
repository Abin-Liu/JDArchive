// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__0D392943_C1CF_4E72_A4AB_ECCFF72D2F96__INCLUDED_)
#define AFX_STDAFX_H__0D392943_C1CF_4E72_A4AB_ECCFF72D2F96__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
//#define _WIN32_WINNT	0x0500 
#define WINVER 0x0400

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#define USE_NEW_DOCK_BAR
#define USE_NEW_MENU_BAR
#include "NewMenu\\NewMenu.h"
#include "NewMenu\\NewToolbar.h"
#include "NewMenu\\NewMenuBar.h"
#include "NewMenu\\NewStatusBar.h"

#include "..\\LBNetwork.h"
#include "..\LBClient.h"
#include "..\\JDArchiveCommDef.h"

#define APP_TITLE			_T("金都资料库")
#define APP_VERSION			132

#define APP_COMPANY			_T("无锡金都机械设备有限公司")
#define APP_COMPANY_URL		_T("http://www.jindu-wx.com")
#define DEFAULT_SERVER_IP	_T("192.168.1.2")

#define APP_CLASS_NAME		_T("JDArchive")

enum {
	WM_ACTIVITY = WM_APP + 100,
	WM_DISCONNECT,
	WM_SEARCH,
	WM_CONNECT,
	WM_SERVER_RESPONSE,
	WM_SERVER_ERROR,
	WM_SERVER_LIST_ERROR,
	WM_TASK_START,
	WM_DATA_NOTIFY,	
	WM_PATCH_INSTALL,
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__0D392943_C1CF_4E72_A4AB_ECCFF72D2F96__INCLUDED_)
