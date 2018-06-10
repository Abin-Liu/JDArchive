/////////////////////////////////////////////////////////////
// LBThreadSession
//
// Implementation file
/////////////////////////////////////////////////////////////

#include "..\\LBNetwork.h"

CLBThreadSession::CLBThreadSession()
{
	m_bStopping = TRUE;
	m_hThread = NULL;
	m_pcs = new CRITICAL_SECTION;
	::InitializeCriticalSection(m_pcs);
}

CLBThreadSession::~CLBThreadSession()
{
	Stop(2000);
	::DeleteCriticalSection(m_pcs);
	delete m_pcs;
}

void CLBThreadSession::OnStart() {}
void CLBThreadSession::OnStop() {}

HANDLE CLBThreadSession::__CreateThread(LPTHREAD_START_ROUTINE lpfn, LPVOID lpParameter)
{
 	DWORD dwDummy;
 	return ::CreateThread(NULL, 0, lpfn, lpParameter, 0, &dwDummy);
}

void CLBThreadSession::__StopThread(HANDLE &rThread, DWORD dwWaitFor)
{
	if (rThread == NULL)
		return;

	if (__IsThreadRunning(rThread) && ::WaitForSingleObject(rThread, dwWaitFor) == WAIT_TIMEOUT)
		::TerminateThread(rThread, 0xffffffff);
	
	::CloseHandle(rThread);
	rThread = NULL;
}

BOOL CLBThreadSession::__IsThreadRunning(HANDLE hThread)
{
	if (hThread == NULL)
		return FALSE;

	DWORD dwExitCode = 0;
	::GetExitCodeThread(hThread, &dwExitCode);
	return dwExitCode == STILL_ACTIVE;
}

void CLBThreadSession::Lock() const
{
	::EnterCriticalSection(m_pcs);
}

void CLBThreadSession::Unlock() const
{
	::LeaveCriticalSection(m_pcs);
}

DWORD WINAPI CLBThreadSession::__SessionThreadProc(LPVOID lpParameter)
{
	CLBThreadSession* pSession = (CLBThreadSession*)lpParameter;
	if (pSession == NULL)
		return 1;

	pSession->OnStart();
	pSession->ThreadProc();
	pSession->OnStop();
	return 0;
}

BOOL CLBThreadSession::Start()
{
	if (IsRunning())
		return FALSE;

	if (m_hThread != NULL)
		::CloseHandle(m_hThread);
	
	m_bStopping = FALSE;	
	m_hThread = __CreateThread(__SessionThreadProc, this);
	return m_hThread != NULL;	
}

void CLBThreadSession::Stop(DWORD dwMilliSeconds)
{
	m_bStopping = TRUE;
	Abandon();
	__StopThread(m_hThread, dwMilliSeconds);
}

BOOL CLBThreadSession::IsRunning() const
{
	return __IsThreadRunning(m_hThread);
}

BOOL CLBThreadSession::IsStopping() const
{
	return m_bStopping;
}
