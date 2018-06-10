/////////////////////////////////////////////////////////////
// Session
//
// Implementation file
/////////////////////////////////////////////////////////////

#include "..\\LBNetwork.h"
#include <time.h>
#include <string.h>
#include "MD5.h"

#pragma warning(disable: 4100)
#pragma warning(disable: 4127)

static const DWORD FILE_BLOCK_SIZE =	min(LBPACKET_SIZE - 128, LBPACKET_SIZE / 2);

static const DWORD PACKET_ACKNOWLEDGE = 0xFFFF1225;
static const DWORD PACKET_ABORT =		0xFFFF1226;
static const DWORD PACKET_FILE_START =	0xFFFF1227;
static const DWORD PACKET_FILE_DATA =	0xFFFF1228;
static const DWORD PACKET_FILE_END =	0xFFFF1229;

///////////////////////////////////////////////////////////
// CLBSession
///////////////////////////////////////////////////////////

CLBSession::CLBSession()
{
	m_hSocket = NULL;	
}

CLBSession::~CLBSession()
{	
	Abandon();
}

void CLBSession::OnConnect() {}
void CLBSession::OnDisconnect() {}
BOOL CLBSession::SendSequencialProc(DWORD dwPacketID, CLBPacket *pPacket, int nCount, int nSequence, LPVOID lParam) { return TRUE; }
BOOL CLBSession::ReceiveSequencialProc(DWORD dwPacketID, CLBPacket *pPacket, int nCount, int nSequence, LPVOID lParam) { return TRUE; }
BOOL CLBSession::SendFileProc(LPCVOID lpszFilePath, DWORD dwBytesSent, DWORD dwBytesTotal, LPVOID lParam) { return TRUE; }
BOOL CLBSession::ReceiveFileProc(LPCVOID lpszFilePath, DWORD dwBytesReceived, DWORD dwBytesTotal, LPVOID lParam) { return TRUE; }

BOOL CLBSession::Create(HANDLE hSocket)
{
	if (m_hSocket != NULL)
		return FALSE;

	if (hSocket)
	{
		m_hSocket = hSocket;
		if (IsConnected())
			OnConnect();

		return TRUE;
	}
	
	return LBSocketCreate(m_hSocket);
}

void CLBSession::Abandon()
{	
	if (m_hSocket == NULL)
		return;

	BOOL bWasConnected = IsConnected();
	LBSocketClose(m_hSocket);
	if (bWasConnected)
		OnDisconnect();
}

DWORD CLBSession::GetLocalAddr(LPINT lpPort) const
{
	return LBSocketGetLocalIP(m_hSocket, lpPort);
}

DWORD CLBSession::GetPeerAddr(LPINT lpPort) const
{
	return LBSocketGetPeerIP(m_hSocket, lpPort);
}

BOOL CLBSession::IsConnected() const
{
	return GetPeerAddr() != 0;
}

BOOL CLBSession::Connect(LPCSTR lpszAddr, int nPort)
{
	if (IsConnected())
		return FALSE;

	if (m_hSocket == NULL && !Create())
		return FALSE;

	BOOL bResult = LBSocketConnect(m_hSocket, lpszAddr, nPort);
	if (bResult)
		OnConnect();
	return bResult;
}

BOOL CLBSession::Connect(LPCWSTR lpszAddr, int nPort)
{
	if (IsConnected())
		return FALSE;

	if (m_hSocket == NULL && !Create())
		return FALSE;
	
	BOOL bResult = LBSocketConnect(m_hSocket, lpszAddr, nPort);
	if (bResult)
		OnConnect();

	return bResult;
}

int CLBSession::Send(const CLBPacket* pPacket)
{
	if (m_hSocket == NULL || pPacket == NULL || pPacket->GetStatus() != CLBPacket::READY)
		return 0;

	int nSent = LBSocketSend(m_hSocket, pPacket->GetData(), pPacket->GetPacketSize());
	if (nSent < 1)
	{
		Abandon();
		return 0;
	}

	return nSent;
}

int CLBSession::Receive(CLBPacket* pPacket, BOOL bAutoDecode)
{
	if (m_hSocket == NULL || pPacket == NULL)
		return 0;

	int nReceived = LBSocketReceive(m_hSocket, pPacket->GetData(), LBPACKET_SIZE);
	if (nReceived < 1 || (bAutoDecode && !pPacket->Decode(nReceived)))
	{
		Abandon();
		return 0;
	}

	return nReceived;
}

void CLBSession::SendInvalidPacket()
{
	CLBPacket packet;
	packet.PrepareForWrite();
	packet.Encode(LBPACKET_INVALID_ID);
	Send(&packet);
}

BOOL CLBSession::SendSequencial(DWORD dwPacketID, int nCount, LPVOID lParam)
{
	int nErrorCount = 0;
	CLBPacket packet;
	for (int i = 0; i < nCount; i++)
	{
		packet.PrepareForWrite();
		if (!SendSequencialProc(dwPacketID, &packet, nCount, i, lParam))
		{
			// Abort
			packet.Encode(PACKET_ABORT);
			Send(&packet);
			return FALSE;
		}

		packet.Encode(dwPacketID, nCount, i);
		if (!Send(&packet))
			return FALSE;

		// Receive acknowledgement
		int nReceived = Receive(&packet);
		if (nReceived < 1)
			return FALSE;
				
		if (packet.Decode(nReceived) && packet.GetPacketID() == PACKET_ACKNOWLEDGE && packet.ReadDword() == dwPacketID)
		{			
			if (packet.ReadInt())
				return FALSE;

			nErrorCount = 0;
		}
		else
		{
			nErrorCount++;
			if (nErrorCount > 100)
				return FALSE;

			i--; // Resend the previous packet
		}
	}
	return TRUE;
}

BOOL CLBSession::ReceiveSequencial(DWORD dwPacketID, LPVOID lParam)
{
	CLBPacket packet;
	BOOL bCompleted = FALSE;	
	while (!bCompleted)
	{
		if (!Receive(&packet))
			return FALSE;

		if (packet.GetPacketID() != dwPacketID)
			return FALSE;

		if (!ReceiveSequencialProc(dwPacketID, &packet, packet.GetSequenceCount(), packet.GetSequenceID(), lParam))
		{
			AcknowledgeSequence(dwPacketID, TRUE);
			return FALSE;
		}

		AcknowledgeSequence(dwPacketID);
		bCompleted = packet.IsSequenceEnd();		
	}	
	return bCompleted;
}

BOOL CLBSession::AcknowledgeSequence(DWORD dwPacketID, BOOL bAbort)
{
	CLBPacket packet;
	packet.PrepareForWrite();
	packet.WriteDword(dwPacketID);
	packet.WriteInt(bAbort);
	packet.Encode(PACKET_ACKNOWLEDGE);
	return Send(&packet);
}

int __GetFileHeaderInfo(HANDLE hFile, DWORD& dwSize)
{
	dwSize = 0;
	if (hFile == INVALID_HANDLE_VALUE)
		return CLBSession::FILE_ERROR_FILE_SYSTEM;

	DWORD dwHi = 0;
	dwSize = ::GetFileSize(hFile, &dwHi);
	if (dwSize == 0)
		return CLBSession::FILE_ERROR_ZERO_LENGTH;

	if (dwHi)
		return CLBSession::FILE_ERROR_TOO_LARGE;	
	return CLBSession::FILE_ERROR_SUCCESS;
}

int CLBSession::__SendFile(HANDLE hFile, LPCSTR lpszMD5, LPDWORD pSize, LPVOID lParam, LPCVOID lpFilePath)
{
	// Send file header info: // size
	DWORD dwSize = 0;
	int nResult = __GetFileHeaderInfo(hFile, dwSize);
	if (nResult != FILE_ERROR_SUCCESS)
	{
		SendInvalidPacket();
		return nResult;
	}

	CLBPacket packet;
	packet.PrepareForWrite();	
	packet.WriteDword(dwSize);
	packet.Encode(PACKET_FILE_START);

	if (!Send(&packet))
		return FILE_ERROR_NETWORK;	

	// Wait the peer to command start of transfer
	if (!Receive(&packet))
		return FILE_ERROR_NETWORK;

	// The peer aborts by sending an unknown packet
	if (packet.GetPacketID() != PACKET_FILE_START)
		return FILE_ERROR_PEER_ABORT;

	// All good, transfer away
	CMD5 md5;
	BYTE aData[FILE_BLOCK_SIZE];
	DWORD dwSent = 0;
	DWORD dwRead = 0;
	BOOL bOK = ::ReadFile(hFile, aData, FILE_BLOCK_SIZE, &dwRead, NULL);
	while (bOK && dwRead)
	{		
		if (SendFileProc(lpFilePath, dwSent, dwSize, lParam))
		{			
			packet.PrepareForWrite();
			packet.WriteData(aData, dwRead);
			if (lpszMD5 == NULL)
				md5.Update(aData, dwRead);
			packet.Encode(PACKET_FILE_DATA);
		}
		else
		{
			// Tell the peer that the sending is aborted
			SendInvalidPacket();
			return FILE_ERROR_USER_ABORT;
		}
		
		if (!Send(&packet))
			return FILE_ERROR_NETWORK;		

		// Wait for acknowedgement
		if (!Receive(&packet))
			return FILE_ERROR_NETWORK;

		// The peer aborts by sending an unknown packet
		if (packet.GetPacketID() != PACKET_FILE_DATA)
			return FILE_ERROR_PEER_ABORT;		

		dwSent += dwRead;
		dwRead = 0;
		bOK = ::ReadFile(hFile, aData, FILE_BLOCK_SIZE, &dwRead, NULL);
	}

	// Send tail packet: md5
	if (lpszMD5 == NULL)
		lpszMD5 = md5.Final();

	packet.PrepareForWrite();	
	packet.WriteString(lpszMD5);
	packet.Encode(PACKET_FILE_END);
	if (!Send(&packet))
		return FILE_ERROR_NETWORK;

	if (pSize)
		*pSize = dwSize;
	
	return FILE_ERROR_SUCCESS;
}

int CLBSession::SendFile(LPCSTR lpszFilePath, LPCSTR lpszMD5, LPDWORD pSize, LPVOID lParam)
{
	if (pSize)
		*pSize = 0;
	HANDLE hFile = ::CreateFileA(lpszFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	int nResult = __SendFile(hFile, lpszMD5, pSize, lParam, (LPCVOID)lpszFilePath);
	::CloseHandle(hFile);
	return nResult;
}

int CLBSession::SendFile(LPCWSTR lpszFilePath, LPCWSTR lpszMD5, LPDWORD pSize, LPVOID lParam)
{
	if (pSize)
		*pSize = 0;
	HANDLE hFile = ::CreateFileW(lpszFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

	char szTemp[40] = "";
	if (lpszMD5)
		::WideCharToMultiByte(CP_ACP, 0, lpszMD5, -1, szTemp, 39, NULL, NULL);

	int nResult = __SendFile(hFile, lpszMD5 ? szTemp : NULL, pSize, lParam, (LPCVOID)lpszFilePath);	
	::CloseHandle(hFile);
	return nResult;
}

int CLBSession::__ReceiveFile(HANDLE hFile, LPSTR lpszMD5, LPDWORD pSize, LPVOID lParam, LPCVOID lpFilePath)
{
	CLBPacket packet;
	if (!Receive(&packet))
		return FILE_ERROR_NETWORK;

	if (packet.GetPacketID() != PACKET_FILE_START)
		return FILE_ERROR_PEER_ABORT;

	if (hFile == INVALID_HANDLE_VALUE)
	{
		SendInvalidPacket();
		return FILE_ERROR_FILE_SYSTEM;
	}

	// Tell the peer to start sending file data
	DWORD dwSize = packet.ReadDword();	
	packet.PrepareForWrite();
	packet.Encode(PACKET_FILE_START);
	if (!Send(&packet))
		return FILE_ERROR_NETWORK;

	// Start receiving file data
	CMD5 md5;
	BYTE aData[FILE_BLOCK_SIZE];
	DWORD dwReceived = 0;
	DWORD dwWritten = 0;
	while (TRUE)
	{		
		if (!Receive(&packet))
			return FILE_ERROR_NETWORK;

		DWORD dwPacketID = packet.GetPacketID();
		if (dwPacketID == PACKET_FILE_DATA)
		{
			if (!ReceiveFileProc(lpFilePath, dwReceived, dwSize, lParam))		
			{
				SendInvalidPacket();
				return FILE_ERROR_USER_ABORT;
			}

			int nSize = packet.GetDataSize();
			int nDataSize = packet.ReadData(aData, min(nSize, FILE_BLOCK_SIZE));
			md5.Update(aData, nSize);
			::WriteFile(hFile, aData, nDataSize, &dwWritten, NULL);
			dwReceived += dwWritten;

			packet.PrepareForWrite();
			packet.Encode(PACKET_FILE_DATA);
			if (!Send(&packet))
				return FILE_ERROR_NETWORK;
		}
		else if (dwPacketID == PACKET_FILE_END)
		{
			break;
		}
		else
		{
			return FILE_ERROR_PEER_ABORT;
		}						
	}
	
	// By reaching here we've got the tail packet: md5
	LPCSTR pszMD5 = md5.Final();
	CHAR szMD5[40] = "";
	packet.ReadString(szMD5, 33);
	if (::stricmp(szMD5, pszMD5))
		return FILE_ERROR_MD5_MISMATCH;

	if (lpszMD5)
		::strcpy(lpszMD5, pszMD5);
	
	if (pSize)
		*pSize = dwSize;
	
	return FILE_ERROR_SUCCESS;
}

int CLBSession::ReceiveFile(LPCSTR lpszFilePath, LPSTR lpszMD5, LPDWORD pSize, LPVOID lParam)
{	
	if (pSize)
		*pSize = 0;
	HANDLE hFile = ::CreateFileA(lpszFilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);	
	int nResult = __ReceiveFile(hFile, lpszMD5, pSize, lParam, (LPCVOID)lpszFilePath);
	::CloseHandle(hFile);	
	if (nResult != FILE_ERROR_SUCCESS)
		::DeleteFileA(lpszFilePath);
		
	return nResult;	
}

int CLBSession::ReceiveFile(LPCWSTR lpszFilePath, LPWSTR lpszMD5, LPDWORD pSize, LPVOID lParam)
{
	if (pSize)
		*pSize = 0;
	HANDLE hFile = ::CreateFileW(lpszFilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	char szTemp[40] = "";
	int nResult = __ReceiveFile(hFile, szTemp, pSize, lParam, (LPCVOID)lpszFilePath);
	::CloseHandle(hFile);	
	if (nResult != FILE_ERROR_SUCCESS)
		::DeleteFileW(lpszFilePath);

	if (lpszMD5)
		::MultiByteToWideChar(CP_ACP, 0, szTemp, -1, lpszMD5, 33);
		
	return nResult;	
}