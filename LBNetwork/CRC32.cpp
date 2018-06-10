/////////////////////////////////////////////////////
// CRC32.h
//
// CRC32 calculation for data blocks or files.
//
// Abin (abinn32@yahoo.com)
// 2011-9-04
/////////////////////////////////////////////////////

//#include "stdafx.h" // include if needed
#include "CRC32.h"

static const DWORD CRC32_POLYNOMIAL = 0x04C11DB7;
static const DWORD CRC32_MASK = 0xffffffff;
static const int CRC32_TABLE_SIZE = 256;

DWORD __CRC32Reflect(DWORD dwRef, BYTE ch)
{
	DWORD dwValue = 0;
	for (int i = 1; i < (ch + 1); i++)
	{
		if (dwRef & 1)
			dwValue |= 1 << (ch - i);
		dwRef >>= 1;
	}
	return dwValue;
}

void __CRC32InitTable(LPDWORD pTable)
{
	for (int i = 0; i < CRC32_TABLE_SIZE; i++)
	{
		pTable[i] = __CRC32Reflect(i, 8) << 24;
		for (int j = 0; j < 8; j++)
			pTable[i] = (pTable[i] << 1) ^ (pTable[i] & (1 << 31) ? CRC32_POLYNOMIAL : 0);
		pTable[i] = __CRC32Reflect(pTable[i], 32);
	}
}

void __CRC32Calculate(LPDWORD pTable, LPCVOID lpData, int nDataSize, DWORD& rCRC32)
{
    const BYTE* p = (const BYTE*)lpData;
    while (nDataSize--)
        rCRC32 = (rCRC32 >> 8) ^ pTable[(rCRC32 & 0xFF) ^ *p++];
}

DWORD CRC32Calculate(LPCVOID lpData, int nDataSize)
{
	if (lpData == NULL || nDataSize < 1)
        return 0;

	DWORD dwCRC32 = CRC32_MASK;
	DWORD aTable[CRC32_TABLE_SIZE];
	__CRC32InitTable(aTable);
	__CRC32Calculate(aTable, lpData, nDataSize, dwCRC32);
	return dwCRC32 ^ CRC32_MASK;
}