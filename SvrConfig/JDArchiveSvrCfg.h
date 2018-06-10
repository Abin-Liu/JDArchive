// JDArchiveSvrCfg.h : main header file for the JDARCHIVESVRCFG application
//

#if !defined(AFX_JDARCHIVESVRCFG_H__70615FC5_1CA3_4239_B6F4_921C02730709__INCLUDED_)
#define AFX_JDARCHIVESVRCFG_H__70615FC5_1CA3_4239_B6F4_921C02730709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "WinAppEx.h"
#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CJDArchiveSvrCfgApp:
// See JDArchiveSvrCfg.cpp for the implementation of this class
//

class CJDArchiveSvrCfgApp : public CWinAppEx
{
public:
	CJDArchiveSvrCfgApp();
	BOOL m_bSilent;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJDArchiveSvrCfgApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CJDArchiveSvrCfgApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

BOOL AfxSilent();

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JDARCHIVESVRCFG_H__70615FC5_1CA3_4239_B6F4_921C02730709__INCLUDED_)
