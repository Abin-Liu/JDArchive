//////////////////////////////////////////////////////
// LBClient.h
//
// Defines API's and classes needed for establishing a client
// application based on LBNetwork library.
//
// Abin (abinn32@yahoo.com)
// 2011-9-12
//////////////////////////////////////////////////////

#ifndef __LBCLIENT_H__
#define __LBCLIENT_H__

#include "..\\LBNetwork.h"

/////////////////////////////////////////////////////////////
// CLBClientSession <Abstract class>
//
// Represents a running task-oriented client session processes
// tasks added by the user. Tasks are stored in an internal
// queue and are processed on a first-in-first-out basis.
/////////////////////////////////////////////////////////////

class __declspec(dllexport) CLBClientSession : public CLBThreadSession
{
public:

	CLBClientSession();
	virtual ~CLBClientSession();
	
	virtual void AddTask(const CLBPacket* pPacket, BOOL bAllowDupe = TRUE); // Add a new packet to the sending queue	
	virtual void DeleteAllTasks(); // Delete all queued tasks
	CLBPacket* FindTask(const CLBPacket* pPacket) const;
	BOOL IsIdle() const;
	
protected:

	/////////////////////////////////////////////////////////////////////////
	// OnTaskDeparture
	//
	// Pure virtual, called by the framework when a packet is about to be sent to
	// server. Return TRUE to proceed and the packet will be sent right away, return
	// FALSE to abort the task.	
	/////////////////////////////////////////////////////////////////////////
	virtual BOOL OnDeparture(DWORD dwPacketID, CLBPacket* pPacket) = 0;

	/////////////////////////////////////////////////////////////////////////
	// OnReceive
	//
	// Pure virtual, called by the framework when a packet from server arrives.
	/////////////////////////////////////////////////////////////////////////
	virtual void OnReceive(DWORD dwPacketID, CLBPacket* pPacket) = 0;	
	virtual void OnDisconnect();

private:
	
	void ThreadProc();

	struct TASK
	{
		CLBPacket* pPacket;
		TASK* pNext;
	};

	TASK* m_pHead;
	TASK* m_pTail;
	BOOL m_bIdle;
};


#endif // __LBCLIENT_H__
