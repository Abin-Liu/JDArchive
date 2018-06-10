/////////////////////////////////////////////////////////////
// CLBClientSession
//
// Implementation file
/////////////////////////////////////////////////////////////

#include "..\\LBClient.h"

CLBClientSession::CLBClientSession()
{
	m_pHead = NULL;
	m_pTail = NULL;
	m_bIdle = TRUE;
}

CLBClientSession::~CLBClientSession()
{
	Stop();
	DeleteAllTasks();
}

void CLBClientSession::DeleteAllTasks()
{
	Lock();
	while (m_pHead)
	{
		TASK* p = m_pHead;
		m_pHead = m_pHead->pNext;
		delete p->pPacket;
		delete p;
	}
	m_pHead = NULL;
	m_pTail = NULL;
	Unlock();
}

void CLBClientSession::AddTask(const CLBPacket* pPacket, BOOL bAllowDupe)
{
	if (pPacket == NULL)
		return;

	if (!bAllowDupe && FindTask(pPacket))
		return;

	TASK* pNew = new TASK;
	pNew->pPacket = new CLBPacket(pPacket);
	pNew->pNext = NULL;

	Lock();
	if (!m_pHead)
	{
		m_pHead = pNew;
		m_pTail = pNew;
	}	
	else
	{
		m_pTail->pNext = pNew;
		m_pTail = pNew;		
	}
	Unlock();
}

void CLBClientSession::ThreadProc()
{
	while (!IsStopping())
	{
		CLBPacket* pPacket = NULL;	
		Lock();		
		if (m_pHead)
		{
			TASK* p = m_pHead;
			m_pHead = m_pHead->pNext;
			if (!m_pHead)
				m_pTail = NULL;	
			pPacket = p->pPacket;
			delete p;
		}		
		Unlock();

		m_bIdle = !pPacket;		
		if (pPacket)
		{
			// Check if the client wants to abort
			BOOL bContinue = OnDeparture(pPacket->GetPacketID(), pPacket);
			if	(bContinue)
				bContinue = Send(pPacket);

			while (bContinue && !IsStopping())
			{
				bContinue = Receive(pPacket);				
				if (bContinue && !IsStopping())
				{
					DWORD dwPacketID = pPacket->GetPacketID();
					BOOL bSequencial = pPacket->IsSequencial();
					BOOL bSeqEnd = pPacket->IsSequenceEnd();
					OnReceive(dwPacketID, pPacket);
					if (bSequencial && !IsStopping())						
						AcknowledgeSequence(dwPacketID);
					bContinue = !bSeqEnd;
				}
			}
			delete pPacket;	
		}
		else
		{
			::Sleep(100);
		}				
	}	
}

void CLBClientSession::OnDisconnect()
{
	DeleteAllTasks();
}

CLBPacket* CLBClientSession::FindTask(const CLBPacket* pPacket) const
{	
	if (pPacket == NULL)
		return NULL;

	CLBPacket* pResult = NULL;
	Lock();
	TASK* p = m_pHead;
	while (p && p->pPacket != pPacket)
		p = p->pNext;
	if (p)
		pResult = p->pPacket;	
	Unlock();
	return pResult;
}

BOOL CLBClientSession::IsIdle() const
{
	return m_bIdle;
}
