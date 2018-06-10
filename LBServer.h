#ifndef __LBSERVER_H__
#define __LBSERVER_H__

#include "LBNetwork.h"

/////////////////////////////////////////////////////////////
// CLBServerSession <Abstract class>
//
// Represents a running server session which does nothing other
// than sitting in a separate thread, receving and dispatching
// packets sent from a remote client.
/////////////////////////////////////////////////////////////

class __declspec(dllexport) CLBServerSession : public CLBThreadSession
{
public:		
	
	CLBServerSession();
	virtual ~CLBServerSession();

protected:

	virtual void OnReceive(DWORD dwPacketID, CLBPacket* pPacket) = 0;

private:
	
	void ThreadProc();
};


/////////////////////////////////////////////////////////////
// CLBServer
//
// Represents a server application and only run on the server,
// it listens to a particular port, accepts incoming client
// connections and manages all sessions. A CLBServer
// lasts as long as the server application does.
/////////////////////////////////////////////////////////////

class __declspec(dllexport) CLBServer
{
public:		

	CLBServer();
	virtual ~CLBServer();	
	
	BOOL IsRunning() const;
	BOOL SetMaxSessions(int nMaxSessions);
	int GetMaxSessions() const;	
	int GetSessionCount() const;
	CLBServerSession* FindSession(DWORD dwAddr) const;

	virtual BOOL Start(int nPort);
	virtual BOOL Stop();

protected:

	virtual void OnStart();
	virtual void OnStop();
	virtual void OnCreateSession(DWORD dwAddr, int nPort, CLBServerSession** ppNewSession) = 0;	
	virtual void OnSessionStart(CLBServerSession* pSession);
	virtual void OnSessionEnd(CLBServerSession* pSession);

private:

	void DeleteSession(int nIndex);
	BOOL AddSession(CLBServerSession* pSession);

	static DWORD WINAPI __ListenThreadProc(LPVOID lpParameter);
	static DWORD WINAPI __InspectThreadProc(LPVOID lpParameter);

	void Lock();
	void Unlock();	
	int FindFirstLocation() const;
	void DeleteAllSessions();
	void ListenThreadProc();
	void InspectThreadProc();		

	int m_nMaxSessions;
	int m_nSessions;
	CLBServerSession** m_aSessions;
	HANDLE m_hListenSocket;
	HANDLE m_hListenThread;
	HANDLE m_hInspectThread;
	volatile BOOL m_bRun;	
	LPCRITICAL_SECTION m_pcs;
};

#endif // __LBSERVER_H__
