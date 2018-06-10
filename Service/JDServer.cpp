// JDServer.cpp: implementation of the CJDServer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JDServer.h"
#include "JDSession.h"
#include "JDDatabase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static const UINT TMR_BACKUP = 300;
static const UINT TMR_CLEANUP = 3600;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJDServer::CJDServer()
{
	m_hBackupThread = NULL;
	m_hTimerThread = NULL;
	m_hCleanupThread = NULL;
	m_nBackupInterval = 3;
	m_nBackupHour = 2;
	m_bStop = TRUE;
	m_bBackupNow = FALSE;
	RegisterTimer(TMR_BACKUP);
	RegisterTimer(TMR_CLEANUP); // a day
}

CJDServer::~CJDServer()
{
	Stop();
}

void CJDServer::OnStart()
{
	_LOG(_T("Server started with run once tasks: \"backupnow\" %d"), m_bBackupNow);	
	m_bStop = FALSE;	
	if (m_bBackupNow)
	{
		//_LOG("run once: Backup");
		BOOL bOK = Backup(TRUE);
		//_LOG("call Backup result: %d", bOK);
		if (bOK)
			::WaitForSingleObject(m_hBackupThread, INFINITE);
	}
	
	m_hTimerThread = CLBThreadSession::__CreateThread(__TimerThreadProc, this);
	_LOG("create timer thread: %d", (DWORD)m_hTimerThread);
}

void CJDServer::OnStop()
{
	_LOG(_T("Server stopped"));
}

void CJDServer::OnCreateSession(DWORD dwAddr, int nPort, CLBServerSession **ppNewSession)
{
	TCHAR szIP[20];
	LBNetworkFormatIP(dwAddr, szIP);
	_LOG(_T("Session created for %s"), szIP);
	*ppNewSession = (CLBServerSession*)(new CJDSession(m_sConnectString, m_sRootFolder));	
}

void CJDServer::OnSessionStart(CLBServerSession *pSession)
{	
	_LOG(_T("Session started %s"), ((CJDSession*)pSession)->GetPeerName());
}

void CJDServer::OnSessionEnd(CLBServerSession *pSession)
{
	_LOG(_T("Session stopped %s"), ((CJDSession*)pSession)->GetPeerName());
}

void CJDServer::ConfigServer(UINT nMaxConnections, LPCTSTR lpszRootFolder, LPCTSTR lpszHost, LPCTSTR lpszDatabase, LPCTSTR lpszUser, LPCTSTR lpszPassword)
{
	SetMaxSessions(nMaxConnections);
	m_sRootFolder = lpszRootFolder;
	m_sHost = lpszHost;
	m_sDatabase = lpszDatabase;
	m_sUser = lpszUser;
	m_sPassword = lpszPassword;
	m_sConnectString.Format(JDB_CONN_STR, (LPCTSTR)m_sHost, (LPCTSTR)m_sDatabase, (LPCTSTR)m_sUser, (LPCTSTR)m_sPassword);
}

void CJDServer::SetBackupSchedule(LPCTSTR lpszFolder, int nInterval, int nHour, BOOL bAutoDelete, int nDeleteDays)
{
	m_sBackupFolder = lpszFolder;
	m_nBackupInterval = max(1, nInterval);
	if (nHour < 0 || nHour > 23)
		nHour = 2;
	m_nBackupHour = nHour;
	m_bAutoDelete = bAutoDelete;
	m_nAutoDeleteDays = nDeleteDays;	
}

BOOL CJDServer::NeedBackup()
{	
	COleDateTime now = COleDateTime::GetCurrentTime();
	if (now.GetHour() != m_nBackupHour)
		return FALSE;

	if (Working())
		return FALSE;
	
	CADODatabase db;
	db.SetConnectionString(m_sConnectString);
	if (!db.Open())
		return FALSE;

	CADORecordset rs(&db);
	if (!rs.Open(JDB_GET_BACKUP_RECORD))
	{
		db.Close();
		return FALSE;
	}

	BOOL bResult = TRUE;
	if (!rs.IsEOF())
	{		
		COleDateTime when = now;
		rs.GetFieldValue(_T("FDate"), when);		
		COleDateTimeSpan span = now - when;
		if (span.GetTotalMinutes() < 1380 * m_nBackupInterval)
			bResult = FALSE;
	}
	
	rs.Close();
	db.Close();
	return bResult;
}

BOOL CJDServer::Backup(BOOL bForceBackup)
{
	if (!bForceBackup && !NeedBackup())
		return FALSE;
	
	m_hBackupThread = CLBThreadSession::__CreateThread(__BackupThreadProc, this);
	//_LOG("create backup thread: %d", (DWORD)m_hBackupThread);
	return m_hBackupThread != NULL;
}

DWORD WINAPI CJDServer::__BackupThreadProc(LPVOID lpParameter)
{	
	CJDServer* pServer = (CJDServer*)lpParameter;
	::CoInitialize(NULL);
	CADODatabase db;
	db.SetConnectionString(pServer->m_sConnectString);
	if (!db.Open())
	{
		::CoUninitialize();
		return -1;
	}
	
	//_LOG("Starting backup");
	DWORD nBackupVerID = 0;
	CADORecordset rs(&db);
	BOOL bSuccess = pServer->BackupThreadProc(nBackupVerID, db, rs);
	rs.Close();
	_LOG("Backup finished with return: %d", bSuccess);

	if (bSuccess)
	{
		CString sSql;
		sSql.Format(JDB_UPDATE_BACKUP_RECORD, nBackupVerID);	
		db.Execute(sSql);
	}
	
	db.Close();
	::CoUninitialize();	
	return 0;
}

BOOL CJDServer::BackupThreadProc(DWORD& nBackupVerID, CADODatabase& db, CADORecordset& rs)
{
	if (!BackupDatabase(db, rs))
		return FALSE;

	if (!BackupVersions(nBackupVerID, db, rs))
		return FALSE;

	DeleteOutdateBackups();	
	return TRUE;	
}

BOOL CJDServer::Stop()
{
	m_bStop = TRUE;
	CLBThreadSession::__StopThread(m_hTimerThread, INFINITE);
	CLBThreadSession::__StopThread(m_hBackupThread, INFINITE);
	CLBThreadSession::__StopThread(m_hCleanupThread, INFINITE);
	return CLBServer::Stop();
}

BOOL CJDServer::BackupDatabase(CADODatabase& db, CADORecordset& rs)
{
	if (m_bStop)
		return FALSE;
	
	//_LOG("Backup database");
	CString sSql;

	// Backup the database
	CString sPath = m_sBackupFolder + _T("\\Database");
	::CreateDirectory(sPath, NULL);	
	COleDateTime now = COleDateTime::GetCurrentTime();
	CString sPathName;
	sPathName.Format(_T("%s\\%s_%d%02d%02d%02d%02d%02d.bak"), (LPCTSTR)sPath, (LPCTSTR)m_sDatabase, now.GetYear(), now.GetMonth(), now.GetDay(), now.GetHour(), now.GetMinute(), now.GetSecond());
	sSql.Format(JDB_GET_BACKUP_FULL, (LPCTSTR)m_sDatabase, (LPCTSTR)sPathName);
	return db.Execute(sSql);	
}

BOOL CJDServer::BackupVersions(DWORD& nBackupVerID, CADODatabase& db, CADORecordset& rs)
{
	nBackupVerID = 0;
	//_LOG("Backup files");
	if (m_bStop)
		return FALSE;	

	// Need to determine the last backup version ID
	if (!rs.Open(JDB_GET_BACKUP_RECORD))
		return FALSE;		

	DWORD nLastVerID = 0;
	if (!rs.IsEOF())
		rs.GetFieldValue(_T("FBackupID"), nLastVerID);		
	
	CString sSql;
	sSql.Format(JDB_GET_RECORDS_TO_BACKUP, nLastVerID);
	if (!rs.Open((LPCTSTR)sSql))
		return FALSE;	

	int nCount = rs.GetRecordCount();
	if (nCount < 1)
		return TRUE;	

	CString sPath = m_sBackupFolder + _T("\\Files");
	::CreateDirectory(sPath, NULL);	
	sPath += _T('\\');
	CString sDiskName;
	DWORD nVerID = 0;
	for (int i = 0; i < nCount; i++)
	{
		if (m_bStop)
			return 0;		

		rs.GetFieldValue(_T("FID"), nVerID);
		rs.GetFieldValue(_T("FDiskName"), sDiskName);
		if (nVerID > nBackupVerID)
			nBackupVerID = nVerID;
		::CopyFile(m_sRootFolder + _T('\\') + sDiskName, sPath + sDiskName, TRUE);
		rs.MoveNext();
	}
	return TRUE;
}

void CJDServer::DeleteOutdateBackups()
{
	//_LOG("Backup delete database redundancy");
	if (!m_bAutoDelete)
		return;

	int nDeleteDays = max(1, m_nAutoDeleteDays);
	CTime current = CTime::GetCurrentTime();
	CTime writtenTime;
	CTimeSpan age;
	CString sPattern = m_sDatabase + _T('_');
	CFileFind ff;
	BOOL bOK = ff.FindFile(m_sBackupFolder + _T("\\Database\\*.bak"));
	while (bOK && !m_bStop)
	{
		bOK = ff.FindNextFile();
		if (ff.IsDirectory() || ff.GetFileTitle().Find(sPattern) != 0)
			continue;

		writtenTime = current;
		ff.GetLastWriteTime(writtenTime);
		age = current - writtenTime;
		if (age.GetTotalMinutes() > nDeleteDays * 1440)
			::DeleteFile(ff.GetFilePath());
	}
}

DWORD WINAPI CJDServer::__TimerThreadProc(LPVOID lpParameter)
{
	_LOG(_T("Timer thread started"));

	CJDServer* pServer = (CJDServer*)lpParameter;
	UINT nInterval;
	COleDateTime prev;
	COleDateTimeSpan elapsed;
	
	while (!pServer->m_bStop)
	{
		::Sleep(200);
		POSITION pos = pServer->m_aTimers.GetStartPosition();
		while (pos)
		{
			pServer->m_aTimers.GetNextAssoc(pos, nInterval, prev);
			elapsed = COleDateTime::GetCurrentTime() - prev;
			if (elapsed.GetTotalSeconds() >= nInterval)
			{				
				pServer->OnTimer(nInterval);
				pServer->m_aTimers.SetAt(nInterval, COleDateTime::GetCurrentTime());
				::Sleep(100);
			}
		}		
	}

	_LOG(_T("Timer thread stopped"));
	return 0;
}

void CJDServer::RegisterTimer(UINT nInterval)
{
	m_aTimers.SetAt(nInterval, COleDateTime::GetCurrentTime());
}

void CJDServer::OnTimer(UINT nInterval)
{
	if (nInterval == TMR_BACKUP)
	{		
		Backup();
	}
	else if (nInterval == TMR_CLEANUP)
	{		
		Cleanup();
	}
}

DWORD WINAPI CJDServer::__CleanupThreadProc(LPVOID lpParameter)
{
	//_LOG("Run cleanup");
	CJDServer* pServer = (CJDServer*)lpParameter;
	::CoInitialize(NULL);

	CADODatabase db;
	db.SetConnectionString(pServer->m_sConnectString);
	if (db.Open())
	{
		CADORecordset rs(&db);	
		pServer->CleanupThreadProc(db, rs);
		rs.Close();
		db.Close();
		//_LOG("Cleanup successful");
	}	
	::CoUninitialize();
	return 0;
}

BOOL CJDServer::Cleanup()
{	
	if (Working())
		return FALSE;
	
	m_hCleanupThread = CLBThreadSession::__CreateThread(__CleanupThreadProc, this);
	//_LOG("create cleanup thread: %d", (DWORD)m_hCleanupThread);
	return m_hCleanupThread != NULL;
}

void CJDServer::CleanupThreadProc(CADODatabase& db, CADORecordset& rs)
{
	if (!rs.Open(_T(JDB_LIST_CLEANUP)))
		return;

	if (rs.IsEOF())
		return;

	CString sDiskName;
	while (!rs.IsEOF())
	{		
		rs.GetFieldValue(_T("FDiskName"), sDiskName);
		rs.MoveNext();
		::DeleteFile(m_sRootFolder + _T('\\') + sDiskName);
		//::DeleteFile(m_sBackupFolder + _T("\\Files\\") + sDiskName);
	}

	rs.Close();	
	db.Execute(JDB_RUN_CLEANUP);
}

BOOL CJDServer::Working() const
{
	return CLBThreadSession::__IsThreadRunning(m_hBackupThread) || CLBThreadSession::__IsThreadRunning(m_hCleanupThread);
}

BOOL CJDServer::Start(BOOL bBackupNow)
{
	m_bBackupNow = bBackupNow;
	return CLBServer::Start(JDARCHIVE_SERVER_PORT);
}
