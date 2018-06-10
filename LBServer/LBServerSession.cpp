/////////////////////////////////////////////////////////////
// CLBServerSession
//
// Implementation file
/////////////////////////////////////////////////////////////

#include "..\\LBServer.h"

CLBServerSession::CLBServerSession()
{
}

CLBServerSession::~CLBServerSession()
{
	Stop();
}

void CLBServerSession::ThreadProc()
{
	CLBPacket packet;
	while (!IsStopping() && Receive(&packet))
	{
		DWORD dwPacketID = packet.GetPacketID();
		BOOL bSequencial = packet.IsSequencial();
		OnReceive(packet.GetPacketID(), &packet);
		if (bSequencial)
			AcknowledgeSequence(dwPacketID);
	}
}