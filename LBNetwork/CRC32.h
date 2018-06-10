/////////////////////////////////////////////////////
// CRC32.h
//
// CRC32 calculation for data blocks.
//
// Abin (abinn32@yahoo.com)
// 2011-9-04
/////////////////////////////////////////////////////

#ifndef __CRC32_H__
#define __CRC32_H__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

DWORD CRC32Calculate(LPCVOID lpBuff, int nDataSize);

#endif // __CRC32_H__