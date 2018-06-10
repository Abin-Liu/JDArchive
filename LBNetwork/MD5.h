/////////////////////////////////////////////////////
// MD5.h
//
// Calculation MD5 checksum for data blocks or files.
//
// Abin (abinn32@yahoo.com)
// 2011-9-25
/////////////////////////////////////////////////////

#ifndef __CHECKSUM_H__
#define __CHECKSUM_H__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

/////////////////////////////////////////////////////
// Callback function called during calculation process of MD5
// checksum, return non-zero to proceed, or zero to abort.
/////////////////////////////////////////////////////
typedef BOOL (CALLBACK *fnMD5Callback)(int nSequenceCount, int nSequenceID, LPVOID lParam);

class CMD5
{
public:
	
	CMD5();
	~CMD5();	

	LPCSTR CalcData(LPCVOID lpData, DWORD dwDataSize);
	LPCSTR CalcText(LPCSTR lpszText);
	LPCSTR CalcText(LPCWSTR lpszText);
	LPCSTR CalcFile(LPCSTR lpszFilePath, fnMD5Callback fnCallback = NULL, LPVOID lParam = 0);
	LPCSTR CalcFile(LPCWSTR lpszFilePath, fnMD5Callback fnCallback = NULL, LPVOID lParam = 0);	

	// The following functions are for accumulated calculation only!	
	void Initialize();
	void Update(LPCVOID lpData, DWORD dwDataSize);
	LPCSTR Final();

private:

	BOOL CalcFile(HANDLE hFile, fnMD5Callback fnCallback, LPVOID lParam);
	void Transform(const BYTE* lpBlock);

	static void FF( DWORD& A, DWORD B, DWORD C, DWORD D, DWORD X, DWORD S, DWORD T);
	static void GG( DWORD& A, DWORD B, DWORD C, DWORD D, DWORD X, DWORD S, DWORD T);
	static void HH( DWORD& A, DWORD B, DWORD C, DWORD D, DWORD X, DWORD S, DWORD T);
	static void II( DWORD& A, DWORD B, DWORD C, DWORD D, DWORD X, DWORD S, DWORD T);
	static DWORD RotateLeft(DWORD x, int n);
	static void DWordToByte(LPBYTE lpBuff, const DWORD* lpData, DWORD dwDataSize);
	static void ByteToDWord(LPDWORD aOutput, const BYTE* pInput, DWORD dwLength);

	BYTE m_aMD5[16];
	BYTE m_aTemp[64];
	DWORD m_aCounts[2];
	DWORD m_aStates[4];
	CHAR m_szResult[40];
};

#endif // __CHECKSUM_H__
