/////////////////////////////////////////////////////////////
// LBSocket.cpp
/////////////////////////////////////////////////////////////

#include <Winsock2.h>
#include "..\\LBNetwork.h"

BOOL __declspec(dllexport) LBSocketCreate(HANDLE& rSocket)
{
	rSocket = NULL;
	SOCKET soc = ::socket(AF_INET, SOCK_STREAM, 0);
	if (soc == INVALID_SOCKET)
		return FALSE;

	rSocket = (HANDLE)soc;
	return TRUE;
}

BOOL __declspec(dllexport) LBSocketClose(HANDLE& rSocket)
{
	if (rSocket != NULL)
	{
		::shutdown((SOCKET)rSocket, SD_BOTH);
		int nResult = ::closesocket((SOCKET)rSocket);
		rSocket = NULL;
		return nResult == 0;
	}

	return FALSE;
}

DWORD __declspec(dllexport) LBSocketGetLocalIP(HANDLE hSocket, LPINT lpPort)
{
	if (hSocket == NULL)
		return 0;

	SOCKADDR_IN addr = { 0 };
	int nLen = sizeof(addr);
	if (::getsockname((SOCKET)hSocket, (LPSOCKADDR)&addr, &nLen) == 0)
	{
		if (lpPort)
			*lpPort = addr.sin_port;
		return addr.sin_addr.S_un.S_addr;
	}	

	return 0;
}

DWORD __declspec(dllexport) LBSocketGetPeerIP(HANDLE hSocket, LPINT lpPort)
{
	if (hSocket == NULL)
		return 0;
	
	SOCKADDR_IN addr = { 0 };
	int nLen = sizeof(addr);
	if (::getpeername((SOCKET)hSocket, (LPSOCKADDR)&addr, &nLen) == 0)
	{
		if (lpPort)
			*lpPort = addr.sin_port;
		return addr.sin_addr.S_un.S_addr;
	}	

	return 0;
}

int __declspec(dllexport) LBSocketReceive(HANDLE hSocket, LPVOID lpBuff, int nBuffSize)
{
	return ::recv((SOCKET)hSocket, (LPSTR)lpBuff, nBuffSize, 0);	
}

int __declspec(dllexport) LBSocketSend(HANDLE hSocket, LPCVOID lpPacket, int nPacketSize)
{	
	return ::send((SOCKET)hSocket, (LPSTR)lpPacket, nPacketSize, 0);
}

BOOL __declspec(dllexport) LBSocketListen(HANDLE hSocket, int nPort, int nBacklog)
{
	SOCKADDR_IN addr;
	addr.sin_addr.S_un.S_addr = ::htonl(INADDR_ANY);
	addr.sin_family = AF_INET;
	addr.sin_port = (USHORT)::htons((USHORT)nPort);
	::bind((SOCKET)hSocket, (LPSOCKADDR)&addr, sizeof(addr));
	return ::listen((SOCKET)hSocket, nBacklog) == 0;
}

HANDLE __declspec(dllexport) LBSocketAccept(HANDLE hSocket)
{
	SOCKADDR_IN addr = { 0 };
	int nLen = sizeof(addr);
	return (HANDLE)::accept((SOCKET)hSocket, (LPSOCKADDR)&addr, &nLen);	
}

BOOL __declspec(dllexport) LBSocketConnect(HANDLE hSocket, LPCSTR lpszAddr, int nPort)
{
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = ::inet_addr(lpszAddr);
	addr.sin_port = (USHORT)::htons((USHORT)nPort); 
	return ::connect((SOCKET)hSocket, (LPSOCKADDR)&addr, sizeof(addr)) == 0;
}

BOOL __declspec(dllexport) LBSocketConnect(HANDLE hSocket, LPCWSTR lpszAddr, int nPort)
{
	CHAR szAddr[20];
	::WideCharToMultiByte(CP_ACP, 0, lpszAddr, -1, szAddr, 19, NULL, NULL);	
	return LBSocketConnect(hSocket, szAddr, nPort);
}
