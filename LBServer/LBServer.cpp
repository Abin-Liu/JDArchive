
#include "..\\LBServer.h"
#include <time.h>

#pragma warning(disable: 4100)

///////////////////////////////////////////////////////////
// CLBServer
///////////////////////////////////////////////////////////

CLBServer::CLBServer()
{
	m_nMaxSessions = 0;
	m_nSessions = 0;
	m_aSessions = NULL;
	m_hListenSocket = NULL;
	m_hListenThread = NULL;
	m_hInspectThread = NULL;
	m_bRun = FALSE;
	m_pcs = new CRITICAL_SECTION;
	::InitializeCriticalSection(m_pcs);
}

CLBServer::~CLBServer()
{
	Stop();	
	DeleteAllSessions();
	if (m_aSessions != NULL)
		delete [] m_aSessions;
	m_aSessions = NULL;
	::DeleteCriticalSection(m_pcs);
	delete m_pcs;
}

void CLBServer::OnStart() {}
void CLBServer::OnStop() {}
void CLBServer::OnSessionStart(CLBServerSession* pSession) {}
void CLBServer::OnSessionEnd(CLBServerSession* pSession) {}

void CLBServer::Lock()
{
	::EnterCriticalSection(m_pcs);
}

void CLBServer::Unlock()
{
	::LeaveCriticalSection(m_pcs);
}

BOOL CLBServer::IsRunning() const
{
	return CLBThreadSession::__IsThreadRunning(m_hListenThread);
}

BOOL CLBServer::Start(int nPort)
{
	if (m_aSessions == NULL)
		return FALSE;
	
	if (IsRunning())
		return FALSE;

	if (nPort == 0)
		return FALSE;	
	
	if (!LBSocketCreate(m_hListenSocket))
		return FALSE;
	
	if (!LBSocketListen(m_hListenSocket, nPort))
	{
		LBSocketClose(m_hListenSocket);
		return FALSE;
	}

	m_bRun = TRUE;
	m_hListenThread = CLBThreadSession::__CreateThread(__ListenThreadProc, (LPVOID)this);
	m_hInspectThread = CLBThreadSession::__CreateThread(__InspectThreadProc, (LPVOID)this);
	return TRUE;
}

BOOL CLBServer::Stop()
{
	if (!IsRunning())
		return FALSE;

	m_bRun = FALSE;
	LBSocketClose(m_hListenSocket);	
	CLBThreadSession::__StopThread(m_hListenThread, 2000);
	CLBThreadSession::__StopThread(m_hInspectThread, 2000);
	DeleteAllSessions();
	return TRUE;
}

int CLBServer::GetMaxSessions() const
{
	return m_nMaxSessions;
}

void CLBServer::DeleteAllSessions()
{
	m_nSessions = 0;
	if (m_aSessions == NULL)
		return;
	
	for (int i = 0; i < m_nMaxSessions; i++)
	{
		CLBServerSession* p = m_aSessions[i];
		if (p)
		{
			p->Abandon();
			OnSessionEnd(p);
			delete p;
			m_aSessions[i] = NULL;
		}
	}
}

CLBServerSession* CLBServer::FindSession(DWORD dwAddr) const
{
	if (m_aSessions == NULL)
		return NULL;

	CLBServerSession* p;
	for (int i = 0; i < m_nMaxSessions; i++)
	{
		p = m_aSessions[i];
		if (p && p->GetPeerAddr() == dwAddr)
			return p;
	}
	return NULL;
}

int CLBServer::FindFirstLocation() const
{
	if (m_aSessions == NULL)
		return -1;

	for (int i = 0; i < m_nMaxSessions; i++)
	{
		if (m_aSessions[i] == NULL)
			return i;
	}
	return -1;
}

DWORD WINAPI CLBServer::__ListenThreadProc(LPVOID lpParameter)
{
	CLBServer* pServer = (CLBServer*)lpParameter;	
	pServer->OnStart();
	pServer->ListenThreadProc();
	pServer->OnStop();
	return 0;
}

DWORD WINAPI CLBServer::__InspectThreadProc(LPVOID lpParameter)
{
	CLBServer* pServer = (CLBServer*)lpParameter;
	pServer->InspectThreadProc();
	return 0;
}

void CLBServer::ListenThreadProc()
{
	while(m_bRun)
	{
		while (m_bRun && m_nSessions >= m_nMaxSessions)
			::Sleep(100);

		if (!m_bRun)
			break;

		HANDLE socket = LBSocketAccept(m_hListenSocket);	
		if (!m_bRun)
		{
			LBSocketClose(socket);
			break;
		}
		
		int nPort = 0;
		DWORD dwAddr = LBSocketGetPeerIP(socket, &nPort);
		if (!dwAddr)
			continue;
		
		Lock();
		CLBServerSession* pExists = FindSession(dwAddr);
		Unlock();
		if (pExists)
			continue; // already connected for some reason

		// All OK, request a CLBServerSession inherited object now...
		CLBServerSession* pNewSession = NULL;
		OnCreateSession(dwAddr, nPort, &pNewSession);
		if (!pNewSession)
		{
			// The server application denied to create a new session
			LBSocketClose(socket);
			continue;
		}				

		Lock();
		BOOL bAdded = AddSession(pNewSession);		
		Unlock();

		if (bAdded)
		{
			pNewSession->Create(socket);
			OnSessionStart(pNewSession);
			pNewSession->Start();			
		}
		else
		{
			delete pNewSession;
		}
	}
}

void CLBServer::InspectThreadProc()
{	
	while (m_bRun)
	{	
		DWORD dwNextTick = ::GetTickCount() + 1000;
		while (m_bRun && ::GetTickCount() < dwNextTick)
			::Sleep(100);

		if (!m_bRun)
			return;
		
		Lock();
		for (int i = 0; m_bRun && i < m_nMaxSessions; i++)
		{
			CLBServerSession* p = m_aSessions[i];
			if (p && !p->IsConnected())			
			{
				p->Abandon();
				OnSessionEnd(p);
				DeleteSession(i);
			}
		}
		Unlock();		
	}
}

BOOL CLBServer::AddSession(CLBServerSession *pSession)
{
	if (m_aSessions == NULL || pSession == NULL)
		return FALSE;

	for (int i = 0; i < m_nMaxSessions; i++)
	{
		if (m_aSessions[i] == NULL)
		{
			m_aSessions[i] = pSession;
			m_nSessions++;
			return TRUE;
		}
	}

	return FALSE;
}

void CLBServer::DeleteSession(int nIndex)
{
	if (m_aSessions != NULL && nIndex >= 0 && nIndex < m_nMaxSessions)
	{
		CLBServerSession* p = m_aSessions[nIndex];
		if (p)
		{
			delete p;
			m_aSessions[nIndex] = NULL;
			m_nSessions--;
			return;
		}
	}
}

BOOL CLBServer::SetMaxSessions(int nMaxSessions)
{
	if (nMaxSessions < 1 || nMaxSessions > 65535)
		return FALSE;

	if (nMaxSessions == m_nMaxSessions)
		return TRUE;

	CLBServerSession** pTemp = new CLBServerSession* [nMaxSessions];
	::memset(pTemp, 0, sizeof(CLBServerSession*) * nMaxSessions);
	int nCount = 0;

	Lock();
	if (m_aSessions != NULL && m_nSessions > 0)
	{
		for (int i = 0; i < m_nMaxSessions; i++)
		{
			CLBServerSession* p = m_aSessions[i];
			if (p == NULL)
				continue;

			if (nCount < nMaxSessions)
			{
				pTemp[nCount++] = p;
			}
			else
			{
				p->Abandon();
				OnSessionEnd(p);
				delete p;
				m_aSessions[i] = NULL;
				m_nSessions--;
			}
		}

		delete [] m_aSessions;
	}
	m_aSessions = pTemp;
	m_nMaxSessions = nMaxSessions;
	Unlock();
	
	return TRUE;	
}

int CLBServer::GetSessionCount() const
{
	return m_nSessions;
}
