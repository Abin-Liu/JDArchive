// JDServer.h: interface for the CJDServer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JDSERVER_H__AAC57697_CF96_4A46_A7F9_601A458781EF__INCLUDED_)
#define AFX_JDSERVER_H__AAC57697_CF96_4A46_A7F9_601A458781EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\\LBServer.h"
#include "ado.h"
#include <afxtempl.h>

class CJDServer : public CLBServer  
{
public:	
	BOOL Start(BOOL bBackupNow);
	
	CJDServer();
	virtual ~CJDServer();

	BOOL Stop();
	void ConfigServer(UINT nMaxConnections, LPCTSTR lpszRootFolder, LPCTSTR lpszHost, LPCTSTR lpszDatabase, LPCTSTR lpszUser, LPCTSTR lpszPassword);
	void SetBackupSchedule(LPCTSTR lpszFolder, int nInterval, int nHour, BOOL bAutoDelete, int nDeleteDays = 30);

private:
	BOOL Working() const;

	void CleanupThreadProc(CADODatabase& db, CADORecordset& rs);
	BOOL Cleanup();

	BOOL NeedBackup();
	BOOL Backup(BOOL bForceBackup = FALSE);
	void OnTimer(UINT nInterval);
	void RegisterTimer(UINT nInterval);

	void OnStart();
	void OnStop();
	void OnSessionEnd(CLBServerSession* pSession);
	void OnSessionStart(CLBServerSession* pSession);
	void OnCreateSession(DWORD dwAddr, int nPort, CLBServerSession** ppNewSession);

	static DWORD WINAPI __BackupThreadProc(LPVOID lpParameter);
	static DWORD WINAPI __TimerThreadProc(LPVOID lpParameter);
	static DWORD WINAPI __CleanupThreadProc(LPVOID lpParameter);
	BOOL BackupThreadProc(DWORD& nBackupVerID, CADODatabase& db, CADORecordset& rs);
	BOOL BackupDatabase(CADODatabase& db, CADORecordset& rs);
	BOOL BackupVersions(DWORD& nBackupVerID, CADODatabase& db, CADORecordset& rs);
	void DeleteOutdateBackups();

	BOOL m_bBackupNow;
	CString m_sRootFolder;	
	CString m_sHost;
	CString m_sDatabase;
	CString m_sUser;
	CString m_sPassword;
	CString m_sConnectString;
	CString m_sBackupFolder;
	int m_nBackupInterval;
	int m_nBackupHour;
	HANDLE m_hTimerThread;
	HANDLE m_hBackupThread;
	HANDLE m_hCleanupThread;
	BOOL m_bStop;
	BOOL m_bAutoDelete;
	int m_nAutoDeleteDays;
	CMap<UINT, UINT, COleDateTime, const COleDateTime&> m_aTimers;	
};

#endif // !defined(AFX_JDSERVER_H__AAC57697_CF96_4A46_A7F9_601A458781EF__INCLUDED_)
