//////////////////////////////////////////////////////
// LBNetwork.cpp
//
// Implementation of LBNetwork interface.
//
// Abin (abinn32@yahoo.com)
// 2011-9-03
//////////////////////////////////////////////////////

#include <stdio.h>
#include <Winsock2.h>
#include "..\\LBNetwork.h"

BOOL __declspec(dllexport) LBNetworkStartup(BYTE nMajorVer, BYTE nMinorVer)
{
	WORD wVersion = MAKEWORD(nMajorVer, nMinorVer);
	WSADATA wsaData;
	if (::WSAStartup(wVersion, &wsaData) != 0)
		return FALSE;

	if (LOBYTE(wsaData.wVersion) != nMajorVer || HIBYTE(wsaData.wVersion) != nMinorVer)
	{
		::WSACleanup();
		return FALSE;
	}

	return TRUE;
}

BOOL __declspec(dllexport) LBNetworkCleanup()
{
	return ::WSACleanup() == 0;
}

DWORD __declspec(dllexport) LBNetworkComposeIP(BYTE b1, BYTE b2, BYTE b3, BYTE b4)
{
	return DWORD(MAKELPARAM(MAKEWORD(b1, b2), MAKEWORD(b3, b4)));
}

void __declspec(dllexport) LBNetworkDecomposeIP(DWORD dwIP, BYTE& b1, BYTE& b2, BYTE& b3, BYTE& b4)
{
	b1 = LOBYTE(LOWORD(dwIP));
	b2 = HIBYTE(LOWORD(dwIP));
	b3 = LOBYTE(HIWORD(dwIP));
	b4 = HIBYTE(HIWORD(dwIP));
}

void __declspec(dllexport) LBNetworkFormatIP(DWORD dwIP, LPSTR lpszBuff)
{
	BYTE b1, b2, b3, b4;
	LBNetworkDecomposeIP(dwIP, b1, b2, b3, b4);
	::sprintf(lpszBuff, "%d.%d.%d.%d", b1, b2, b3, b4);
}

void __declspec(dllexport) LBNetworkFormatIP(DWORD dwIP, LPWSTR lpszBuff)
{
	BYTE b1, b2, b3, b4;
	LBNetworkDecomposeIP(dwIP, b1, b2, b3, b4);
	::swprintf(lpszBuff, L"%d.%d.%d.%d", b1, b2, b3, b4);
}