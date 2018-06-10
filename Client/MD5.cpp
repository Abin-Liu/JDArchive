/////////////////////////////////////////////////////
// MD5.h
//
// Calculation MD5 checksum for data blocks or files.
//
// Abin (abinn32@yahoo.com)
// 2011-9-25
/////////////////////////////////////////////////////

#include "stdafx.h" // Include if needed
#include "MD5.h"

static const int FILE_BLOCK_SIZE = 8192;

//Magic initialization constants
#define MD5_INIT_STATE_0 0x67452301
#define MD5_INIT_STATE_1 0xefcdab89
#define MD5_INIT_STATE_2 0x98badcfe
#define MD5_INIT_STATE_3 0x10325476

//Constants for Transform routine.
#define MD5_S11  7
#define MD5_S12 12
#define MD5_S13 17
#define MD5_S14 22
#define MD5_S21  5
#define MD5_S22  9
#define MD5_S23 14
#define MD5_S24 20
#define MD5_S31  4
#define MD5_S32 11
#define MD5_S33 16
#define MD5_S34 23
#define MD5_S41  6
#define MD5_S42 10
#define MD5_S43 15
#define MD5_S44 21

//Transformation Constants - Round 1
#define MD5_T01  0xd76aa478 //Transformation Constant 1 
#define MD5_T02  0xe8c7b756 //Transformation Constant 2
#define MD5_T03  0x242070db //Transformation Constant 3
#define MD5_T04  0xc1bdceee //Transformation Constant 4
#define MD5_T05  0xf57c0faf //Transformation Constant 5
#define MD5_T06  0x4787c62a //Transformation Constant 6
#define MD5_T07  0xa8304613 //Transformation Constant 7
#define MD5_T08  0xfd469501 //Transformation Constant 8
#define MD5_T09  0x698098d8 //Transformation Constant 9
#define MD5_T10  0x8b44f7af //Transformation Constant 10
#define MD5_T11  0xffff5bb1 //Transformation Constant 11
#define MD5_T12  0x895cd7be //Transformation Constant 12
#define MD5_T13  0x6b901122 //Transformation Constant 13
#define MD5_T14  0xfd987193 //Transformation Constant 14
#define MD5_T15  0xa679438e //Transformation Constant 15
#define MD5_T16  0x49b40821 //Transformation Constant 16

//Transformation Constants - Round 2
#define MD5_T17  0xf61e2562 //Transformation Constant 17
#define MD5_T18  0xc040b340 //Transformation Constant 18
#define MD5_T19  0x265e5a51 //Transformation Constant 19
#define MD5_T20  0xe9b6c7aa //Transformation Constant 20
#define MD5_T21  0xd62f105d //Transformation Constant 21
#define MD5_T22  0x02441453 //Transformation Constant 22
#define MD5_T23  0xd8a1e681 //Transformation Constant 23
#define MD5_T24  0xe7d3fbc8 //Transformation Constant 24
#define MD5_T25  0x21e1cde6 //Transformation Constant 25
#define MD5_T26  0xc33707d6 //Transformation Constant 26
#define MD5_T27  0xf4d50d87 //Transformation Constant 27
#define MD5_T28  0x455a14ed //Transformation Constant 28
#define MD5_T29  0xa9e3e905 //Transformation Constant 29
#define MD5_T30  0xfcefa3f8 //Transformation Constant 30
#define MD5_T31  0x676f02d9 //Transformation Constant 31
#define MD5_T32  0x8d2a4c8a //Transformation Constant 32

//Transformation Constants - Round 3
#define MD5_T33  0xfffa3942 //Transformation Constant 33
#define MD5_T34  0x8771f681 //Transformation Constant 34
#define MD5_T35  0x6d9d6122 //Transformation Constant 35
#define MD5_T36  0xfde5380c //Transformation Constant 36
#define MD5_T37  0xa4beea44 //Transformation Constant 37
#define MD5_T38  0x4bdecfa9 //Transformation Constant 38
#define MD5_T39  0xf6bb4b60 //Transformation Constant 39
#define MD5_T40  0xbebfbc70 //Transformation Constant 40
#define MD5_T41  0x289b7ec6 //Transformation Constant 41
#define MD5_T42  0xeaa127fa //Transformation Constant 42
#define MD5_T43  0xd4ef3085 //Transformation Constant 43
#define MD5_T44  0x04881d05 //Transformation Constant 44
#define MD5_T45  0xd9d4d039 //Transformation Constant 45
#define MD5_T46  0xe6db99e5 //Transformation Constant 46
#define MD5_T47  0x1fa27cf8 //Transformation Constant 47
#define MD5_T48  0xc4ac5665 //Transformation Constant 48

//Transformation Constants - Round 4
#define MD5_T49  0xf4292244 //Transformation Constant 49
#define MD5_T50  0x432aff97 //Transformation Constant 50
#define MD5_T51  0xab9423a7 //Transformation Constant 51
#define MD5_T52  0xfc93a039 //Transformation Constant 52
#define MD5_T53  0x655b59c3 //Transformation Constant 53
#define MD5_T54  0x8f0ccc92 //Transformation Constant 54
#define MD5_T55  0xffeff47d //Transformation Constant 55
#define MD5_T56  0x85845dd1 //Transformation Constant 56
#define MD5_T57  0x6fa87e4f //Transformation Constant 57
#define MD5_T58  0xfe2ce6e0 //Transformation Constant 58
#define MD5_T59  0xa3014314 //Transformation Constant 59
#define MD5_T60  0x4e0811a1 //Transformation Constant 60
#define MD5_T61  0xf7537e82 //Transformation Constant 61
#define MD5_T62  0xbd3af235 //Transformation Constant 62
#define MD5_T63  0x2ad7d2bb //Transformation Constant 63
#define MD5_T64  0xeb86d391 //Transformation Constant 64


//Null data (except for first BYTE) used to finalise the checksum calculation
static const BYTE PADDING[64] = {
  0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

CMD5::CMD5()
{
	Initialize();
}

CMD5::~CMD5()
{
}

void CMD5::FF( DWORD& A, DWORD B, DWORD C, DWORD D, DWORD X, DWORD S, DWORD T)
{
	DWORD F = (B & C) | (~B & D);
	A += F + X + T;
	A = RotateLeft(A, S);
	A += B;
}

void CMD5::GG( DWORD& A, DWORD B, DWORD C, DWORD D, DWORD X, DWORD S, DWORD T)
{
	DWORD G = (B & D) | (C & ~D);
	A += G + X + T;
	A = RotateLeft(A, S);
	A += B;
}

void CMD5::HH( DWORD& A, DWORD B, DWORD C, DWORD D, DWORD X, DWORD S, DWORD T)
{
	DWORD H = (B ^ C ^ D);
	A += H + X + T;
	A = RotateLeft(A, S);
	A += B;
}

void CMD5::II( DWORD& A, DWORD B, DWORD C, DWORD D, DWORD X, DWORD S, DWORD T)
{
	DWORD I = (C ^ (B | ~D));
	A += I + X + T;
	A = RotateLeft(A, S);
	A += B;
}

void CMD5::Initialize()
{	
	::memset(m_aMD5, 0, sizeof(m_aMD5));
	::memset(m_aTemp, 0, sizeof(m_aTemp));
	::memset(m_aCounts, 0, sizeof(m_aCounts));
	::memset(m_szResult, 0, sizeof(m_szResult));
		
	m_aStates[0] = MD5_INIT_STATE_0;
	m_aStates[1] = MD5_INIT_STATE_1;
	m_aStates[2] = MD5_INIT_STATE_2;
	m_aStates[3] = MD5_INIT_STATE_3;
}

void CMD5::Update(LPCVOID lpData, DWORD dwDataSize)
{
	if (lpData == NULL || dwDataSize < 1)
		return;

	//Compute number of bytes mod 64
	UINT nIndex = (UINT)((m_aCounts[0] >> 3) & 0x3F);

	//Update number of bits
	if ((m_aCounts[0] += dwDataSize << 3) < (dwDataSize << 3))
		m_aCounts[1]++;

	m_aCounts[1] += (dwDataSize >> 29);

	//Transform as many times as possible.
	const BYTE* p = (const BYTE*)lpData;
	UINT i = 0;		
	UINT nPartLen = 64 - nIndex;
	if (dwDataSize >= nPartLen) 	
	{
		::memcpy(&m_aTemp[nIndex], lpData, nPartLen);
		Transform(m_aTemp);
		for (i = nPartLen; i + 63 < dwDataSize; i += 64) 
			Transform(&p[i]);

		nIndex = 0;
	} 
	else 
	{
		i = 0;
	}

	// Buffer remaining input
	::memcpy(&m_aTemp[nIndex], &p[i], dwDataSize - i);
}

void CMD5::Transform(const BYTE *lpBlock)
{
	//initialise local data with current checksum
	DWORD a = m_aStates[0];
	DWORD b = m_aStates[1];
	DWORD c = m_aStates[2];
	DWORD d = m_aStates[3];

	//copy BYTES from input 'Block' to an array of ULONGS 'X'
	DWORD X[16];
	ByteToDWord(X, lpBlock, 64);

	//Perform Round 1 of the transformation
	FF (a, b, c, d, X[ 0], MD5_S11, MD5_T01); 
	FF (d, a, b, c, X[ 1], MD5_S12, MD5_T02); 
	FF (c, d, a, b, X[ 2], MD5_S13, MD5_T03); 
	FF (b, c, d, a, X[ 3], MD5_S14, MD5_T04); 
	FF (a, b, c, d, X[ 4], MD5_S11, MD5_T05); 
	FF (d, a, b, c, X[ 5], MD5_S12, MD5_T06); 
	FF (c, d, a, b, X[ 6], MD5_S13, MD5_T07); 
	FF (b, c, d, a, X[ 7], MD5_S14, MD5_T08); 
	FF (a, b, c, d, X[ 8], MD5_S11, MD5_T09); 
	FF (d, a, b, c, X[ 9], MD5_S12, MD5_T10); 
	FF (c, d, a, b, X[10], MD5_S13, MD5_T11); 
	FF (b, c, d, a, X[11], MD5_S14, MD5_T12); 
	FF (a, b, c, d, X[12], MD5_S11, MD5_T13); 
	FF (d, a, b, c, X[13], MD5_S12, MD5_T14); 
	FF (c, d, a, b, X[14], MD5_S13, MD5_T15); 
	FF (b, c, d, a, X[15], MD5_S14, MD5_T16); 

	//Perform Round 2 of the transformation
	GG (a, b, c, d, X[ 1], MD5_S21, MD5_T17); 
	GG (d, a, b, c, X[ 6], MD5_S22, MD5_T18); 
	GG (c, d, a, b, X[11], MD5_S23, MD5_T19); 
	GG (b, c, d, a, X[ 0], MD5_S24, MD5_T20); 
	GG (a, b, c, d, X[ 5], MD5_S21, MD5_T21); 
	GG (d, a, b, c, X[10], MD5_S22, MD5_T22); 
	GG (c, d, a, b, X[15], MD5_S23, MD5_T23); 
	GG (b, c, d, a, X[ 4], MD5_S24, MD5_T24); 
	GG (a, b, c, d, X[ 9], MD5_S21, MD5_T25); 
	GG (d, a, b, c, X[14], MD5_S22, MD5_T26); 
	GG (c, d, a, b, X[ 3], MD5_S23, MD5_T27); 
	GG (b, c, d, a, X[ 8], MD5_S24, MD5_T28); 
	GG (a, b, c, d, X[13], MD5_S21, MD5_T29); 
	GG (d, a, b, c, X[ 2], MD5_S22, MD5_T30); 
	GG (c, d, a, b, X[ 7], MD5_S23, MD5_T31); 
	GG (b, c, d, a, X[12], MD5_S24, MD5_T32); 

	//Perform Round 3 of the transformation
	HH (a, b, c, d, X[ 5], MD5_S31, MD5_T33); 
	HH (d, a, b, c, X[ 8], MD5_S32, MD5_T34); 
	HH (c, d, a, b, X[11], MD5_S33, MD5_T35); 
	HH (b, c, d, a, X[14], MD5_S34, MD5_T36); 
	HH (a, b, c, d, X[ 1], MD5_S31, MD5_T37); 
	HH (d, a, b, c, X[ 4], MD5_S32, MD5_T38); 
	HH (c, d, a, b, X[ 7], MD5_S33, MD5_T39); 
	HH (b, c, d, a, X[10], MD5_S34, MD5_T40); 
	HH (a, b, c, d, X[13], MD5_S31, MD5_T41); 
	HH (d, a, b, c, X[ 0], MD5_S32, MD5_T42); 
	HH (c, d, a, b, X[ 3], MD5_S33, MD5_T43); 
	HH (b, c, d, a, X[ 6], MD5_S34, MD5_T44); 
	HH (a, b, c, d, X[ 9], MD5_S31, MD5_T45); 
	HH (d, a, b, c, X[12], MD5_S32, MD5_T46); 
	HH (c, d, a, b, X[15], MD5_S33, MD5_T47); 
	HH (b, c, d, a, X[ 2], MD5_S34, MD5_T48); 

	//Perform Round 4 of the transformation
	II (a, b, c, d, X[ 0], MD5_S41, MD5_T49); 
	II (d, a, b, c, X[ 7], MD5_S42, MD5_T50); 
	II (c, d, a, b, X[14], MD5_S43, MD5_T51); 
	II (b, c, d, a, X[ 5], MD5_S44, MD5_T52); 
	II (a, b, c, d, X[12], MD5_S41, MD5_T53); 
	II (d, a, b, c, X[ 3], MD5_S42, MD5_T54); 
	II (c, d, a, b, X[10], MD5_S43, MD5_T55); 
	II (b, c, d, a, X[ 1], MD5_S44, MD5_T56); 
	II (a, b, c, d, X[ 8], MD5_S41, MD5_T57); 
	II (d, a, b, c, X[15], MD5_S42, MD5_T58); 
	II (c, d, a, b, X[ 6], MD5_S43, MD5_T59); 
	II (b, c, d, a, X[13], MD5_S44, MD5_T60); 
	II (a, b, c, d, X[ 4], MD5_S41, MD5_T61); 
	II (d, a, b, c, X[11], MD5_S42, MD5_T62); 
	II (c, d, a, b, X[ 2], MD5_S43, MD5_T63); 
	II (b, c, d, a, X[ 9], MD5_S44, MD5_T64); 

	//add the transformed values to the current checksum
	m_aStates[0] += a;
	m_aStates[1] += b;
	m_aStates[2] += c;
	m_aStates[3] += d;
}

void CMD5::ByteToDWord(LPDWORD aOutput, const BYTE *pInput, DWORD dwLength)
{
	//initialisations
	DWORD i = 0;	//index to Output array
	DWORD j = 0;	//index to Input array

	//transfer the data by shifting and copying
	for (; j < dwLength; i++, j += 4)
		aOutput[i] = (DWORD)pInput[j] | (DWORD)pInput[j + 1] << 8 | (DWORD)pInput[j + 2] << 16 | (DWORD)pInput[j + 3] << 24;
}

void CMD5::DWordToByte(LPBYTE lpBuff, const DWORD *lpData, DWORD dwDataSize)
{
	DWORD i = 0;
	DWORD j = 0;
	for ( ; j < dwDataSize; i++, j += 4) 
	{
		lpBuff[j] =   (BYTE)(lpData[i] & 0xff);
		lpBuff[j + 1] = (BYTE)((lpData[i] >> 8) & 0xff);
		lpBuff[j + 2] = (BYTE)((lpData[i] >> 16) & 0xff);
		lpBuff[j + 3] = (BYTE)((lpData[i] >> 24) & 0xff);
	}
}

DWORD CMD5::RotateLeft(DWORD x, int n)
{
	return (x << n) | (x >> (32 - n));
}

LPCSTR CMD5::Final()
{
	//Save number of bits
	BYTE aBits[8];
	DWordToByte(aBits, m_aCounts, 8);

	//Pad out to 56 mod 64.
	UINT nIndex = (UINT)((m_aCounts[0] >> 3) & 0x3f);
	UINT nPadLen = (nIndex < 56) ? (56 - nIndex) : (120 - nIndex);
	Update(PADDING, nPadLen);

	//Append length (before padding)
	Update(aBits, 8);

	//Store final state in md5
	DWordToByte(m_aMD5, m_aStates, 16);

	LPSTR psz = m_szResult;
	for (int i = 0; i < 16; i++)
	{
		::sprintf(psz, "%02x", m_aMD5[i]);
		psz = &psz[2];
	}
	return m_szResult;
}

LPCSTR CMD5::CalcData(LPCVOID lpData, DWORD dwDataSize)
{
	Initialize();
	Update(lpData, dwDataSize);
	return Final();
}

LPCSTR CMD5::CalcText(LPCSTR lpszText)
{
	return CalcData(lpszText, lpszText ? ::strlen(lpszText) : 0);
}

LPCSTR CMD5::CalcText(LPCWSTR lpszText)
{
	return CalcData(lpszText, lpszText ? (::wcslen(lpszText) * sizeof(WCHAR)) : 0);
}

BOOL CMD5::CalcFile(HANDLE hFile, fnMD5Callback fnCallback, LPVOID lParam)
{	
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	LARGE_INTEGER li = { 0 };
	if (!::GetFileSizeEx(hFile, &li))
		return FALSE;

	LONGLONG lSize = li.QuadPart;
	if (lSize == 0)
		return TRUE;
	
	int nCount = int(lSize / (LONGLONG)FILE_BLOCK_SIZE);
	if (lSize % FILE_BLOCK_SIZE)
		nCount++;

	BYTE buff[FILE_BLOCK_SIZE];
	DWORD dwRead = 0;
	int nSequence = 0;
	BOOL bOK = ::ReadFile(hFile, buff, FILE_BLOCK_SIZE, &dwRead, NULL);
	while(bOK && dwRead > 0)
	{		
		if (fnCallback && !fnCallback(nCount, nSequence, lParam))
			return FALSE;

		Update(buff, dwRead);
		bOK = ::ReadFile(hFile, buff, FILE_BLOCK_SIZE, &dwRead, NULL);
		nSequence++;
	}
	return TRUE;
}

LPCSTR CMD5::CalcFile(LPCSTR lpszFilePath, fnMD5Callback fnCallback, LPVOID lParam)
{
	Initialize();
	HANDLE hFile = ::CreateFileA(lpszFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	BOOL bResult = CalcFile(hFile, fnCallback, lParam);
	::CloseHandle(hFile);
	Final();
	return bResult ? m_szResult : NULL;
}

LPCSTR CMD5::CalcFile(LPCWSTR lpszFilePath, fnMD5Callback fnCallback, LPVOID lParam)
{
	Initialize();
	HANDLE hFile = ::CreateFileW(lpszFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	BOOL bResult = CalcFile(hFile, fnCallback, lParam);
	::CloseHandle(hFile);
	Final();
	return bResult ? m_szResult : NULL;
}