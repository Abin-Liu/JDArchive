/////////////////////////////////////////////////////////////
// LBPacket.cpp
/////////////////////////////////////////////////////////////

#include "..\\LBNetwork.h"
#include "CRC32.h"

//////////////////////////////////////////////////////
// Indices for a packet stream
//
// CRC32(4) | PACKET_ID(4) | SEQUENCE_COUNT(4) | SEQUENCE_ID(4) | DATA_SIZE(4) | DATA(various) | TAIL_ZERO(4)
//////////////////////////////////////////////////////

enum {
	PACKET_INDEX_CRC32 = 0,
	PACKET_INDEX_PACKETID,
	PACKET_INDEX_SEQUENCECOUNT,
	PACKET_INDEX_SEQUENCEID,
	PACKET_INDEX_DATASIZE,	
	PACKET_INDEX_DATA // Various portion
};

static const int DWORDSIZE = sizeof(DWORD);
static const int HEADERSIZE = PACKET_INDEX_DATA * DWORDSIZE;
static const int TAILSIZE = DWORDSIZE;

CLBPacket::CLBPacket(const CLBPacket* pSrc)
{
	m_dwPacketID = LBPACKET_INVALID_ID;
	m_pData = new BYTE[LBPACKET_SIZE];
	::memset(m_pData, 0, LBPACKET_SIZE);
	m_nDataSize = 0;
	m_nStatus = UNKNOWN;
	m_nOffset = 0;

	if (pSrc)
		Copy(*pSrc);
}

CLBPacket::CLBPacket(const CLBPacket& rhs)
{
	Copy(rhs);
}

CLBPacket::~CLBPacket()
{
	delete [] m_pData;
}

void CLBPacket::Copy(const CLBPacket &rhs)
{
	if (this == &rhs)
		return;

	m_dwPacketID = rhs.m_dwPacketID;	
	m_nDataSize = rhs.m_nDataSize;
	m_nStatus = rhs.m_nStatus;	
	m_nOffset = rhs.m_nOffset;	
	::memcpy(m_pData, rhs.m_pData, LBPACKET_SIZE);	
}

const CLBPacket& CLBPacket::operator=(const CLBPacket& rhs)
{
	Copy(rhs);
	return *this;
}

BOOL CLBPacket::operator==(const CLBPacket& rhs) const
{
	if (this == &rhs)
		return TRUE;

	int nSize = GetPacketSize();
	return (nSize == rhs.GetPacketSize() && ::memcmp(m_pData, rhs.m_pData, nSize) == 0);
}

LPVOID CLBPacket::GetData()
{
	return m_pData;
}

LPCVOID CLBPacket::GetData() const
{
	return m_pData;
}

void CLBPacket::ResetCursor()
{
	if (m_nStatus == READING)
		m_nOffset = 0;
}

int CLBPacket::GetDataSize() const
{
	return m_nDataSize;
}

int CLBPacket::GetPacketSize() const
{
	return HEADERSIZE + m_nDataSize + TAILSIZE;
}

int CLBPacket::GetStatus() const
{
	return m_nStatus;
}

DWORD CLBPacket::GetPacketID() const
{
	return m_dwPacketID;
}

BOOL CLBPacket::VerifyWrite(LPCVOID lpData, int nDataSize) const
{
	return m_nStatus == WRITING && lpData && nDataSize > 0 && HEADERSIZE + m_nDataSize + nDataSize + TAILSIZE <= LBPACKET_SIZE;
}

BOOL CLBPacket::VerifyRead(LPVOID lpData, int nDataSize) const
{
	return m_nStatus == READING && lpData && nDataSize > 0 && m_nOffset < m_nDataSize;
}

void CLBPacket::PrepareForWrite()
{
	m_dwPacketID = LBPACKET_INVALID_ID;
	m_nDataSize = 0;
	m_nStatus = WRITING;
	m_nOffset = 0;
	::memset(m_pData, 0, HEADERSIZE);
}

int CLBPacket::WriteData(LPCVOID lpData, int nDataSize)
{	
	if (!VerifyWrite(lpData, nDataSize))
		return 0;

	::memcpy(m_pData + HEADERSIZE + m_nDataSize, lpData, nDataSize);
	m_nDataSize += nDataSize;
	return nDataSize;
}

int CLBPacket::WriteDword(DWORD val)
{
	return WriteData(&val, sizeof(val));
}

int CLBPacket::WriteInt(int val)
{
	return WriteData(&val, sizeof(val));
}

int CLBPacket::WriteWord(WORD val)
{
	return WriteData(&val, sizeof(val));
}

int CLBPacket::WriteShort(short val)
{
	return WriteData(&val, sizeof(val));
}

int CLBPacket::WriteByte(BYTE val)
{
	return WriteData(&val, sizeof(val));
}

int CLBPacket::WriteChar(char val)
{
	return WriteData(&val, sizeof(val));
}

int CLBPacket::WriteDouble(double val)
{
	return WriteData(&val, sizeof(val));
}

int CLBPacket::WriteFloat(float val)
{
	return WriteData(&val, sizeof(val));
}

int CLBPacket::WriteString(LPCSTR lpsz)
{
	if (lpsz == NULL)
		return WriteChar(0);

	const int LEN = ::strlen(lpsz);
	if (LEN < 1)
		return WriteChar(0);

	return WriteData(lpsz, LEN + 1);	
}

int CLBPacket::WriteString(LPCWSTR lpsz)
{
	if (lpsz == NULL)
		return WriteChar(0);

	int WLEN = ::wcslen(lpsz);
	if (WLEN < 1)
		return WriteChar(0);

	int nLen = (WLEN + 1) * sizeof(wchar_t);
	LPSTR psz = new char[nLen];
	nLen = ::WideCharToMultiByte(CP_ACP, 0, lpsz, -1, psz, nLen, NULL, NULL);
	nLen = WriteData(psz, nLen);
	delete [] psz;
	return nLen;
}

int CLBPacket::Encode(DWORD dwPacketID, int nSequenceCount, int nSequenceID)
{
	if (m_nStatus != WRITING)
		return 0;
	
	m_nStatus = UNKNOWN;
	LPDWORD p = (LPDWORD)m_pData;
	p[PACKET_INDEX_PACKETID] = dwPacketID;
	p[PACKET_INDEX_SEQUENCECOUNT] = (DWORD)nSequenceCount;
	p[PACKET_INDEX_SEQUENCEID] = (DWORD)nSequenceID;
	p[PACKET_INDEX_DATASIZE] = m_nDataSize;
	::memset(m_pData + HEADERSIZE + m_nDataSize, 0, TAILSIZE);
	DWORD dwCRC = CRC32Calculate(m_pData + DWORDSIZE, m_nDataSize + HEADERSIZE + TAILSIZE - DWORDSIZE);
	if (dwCRC == 0)
		return 0;

	p[PACKET_INDEX_CRC32] = dwCRC;
	m_nStatus = READY;
	m_dwPacketID = dwPacketID;
	m_nOffset = 0;
	return GetPacketSize();
}

BOOL CLBPacket::Decode(int nReceivedDataSize)
{
	m_dwPacketID = LBPACKET_INVALID_ID;
	m_nOffset = 0;
	m_nStatus = UNKNOWN;
	m_nDataSize = nReceivedDataSize - HEADERSIZE - TAILSIZE;
	if (m_nDataSize < 0 || nReceivedDataSize > LBPACKET_SIZE)
		return FALSE;	

	LPDWORD p = (LPDWORD)m_pData;
	if ((int)(p[PACKET_INDEX_DATASIZE]) != m_nDataSize)
		return FALSE;

	if (*(m_pData + HEADERSIZE + m_nDataSize) != 0)
		return FALSE;

	DWORD dwCRC32 = CRC32Calculate((LPBYTE)m_pData + DWORDSIZE, nReceivedDataSize - DWORDSIZE);
	if (dwCRC32 != p[PACKET_INDEX_CRC32])
		return FALSE;
	
	m_dwPacketID = p[PACKET_INDEX_PACKETID];
	m_nStatus = READING;
	return TRUE;
}

int CLBPacket::ReadData(LPVOID lpData, int nDataSize)
{
	if (!VerifyRead(lpData, nDataSize))
		return 0;

	int nCopied = min(m_nDataSize - m_nOffset, nDataSize);
	::memcpy(lpData, m_pData + HEADERSIZE + m_nOffset, nCopied);
	m_nOffset += nCopied;
	return nCopied;
}

DWORD CLBPacket::ReadDword(DWORD defaultVal)
{
	DWORD val;
	return (ReadData(&val, sizeof(val))) == sizeof(val) ? val : defaultVal;
}

int CLBPacket::ReadInt(int defaultVal)
{
	int val;
	return (ReadData(&val, sizeof(val))) == sizeof(val) ? val : defaultVal;
}

WORD CLBPacket::ReadWord(WORD defaultVal)
{
	WORD val;
	return (ReadData(&val, sizeof(val))) == sizeof(val) ? val : defaultVal;
}

short CLBPacket::ReadShort(short defaultVal)
{
	short val;
	return (ReadData(&val, sizeof(val))) == sizeof(val) ? val : defaultVal;
}

BYTE CLBPacket::ReadByte(BYTE defaultVal)
{
	BYTE val;
	return (ReadData(&val, sizeof(val))) == sizeof(val) ? val : defaultVal;
}

char CLBPacket::ReadChar(char defaultVal)
{
	char val;
	return (ReadData(&val, sizeof(val))) == sizeof(val) ? val : defaultVal;
}

double CLBPacket::ReadDouble(double defaultVal)
{
	double val;
	return (ReadData(&val, sizeof(val))) == sizeof(val) ? val : defaultVal;
}

float CLBPacket::ReadFloat(float defaultVal)
{
	float val;
	return (ReadData(&val, sizeof(val))) == sizeof(val) ? val : defaultVal;
}

int CLBPacket::GetNextStringLength() const
{
	return (m_nStatus == READING) ? ::strlen((LPCSTR)(m_pData + HEADERSIZE + m_nOffset)) : 0;
}

int CLBPacket::ReadString(LPSTR lpszBuff, int nMaxChars)
{
	if (lpszBuff == NULL || nMaxChars < 1)
		return 0;

	LPCSTR p = (LPCSTR)(m_pData + HEADERSIZE + m_nOffset);
	int nLen = ::strlen(p);
	int nCopied = min(nMaxChars, nLen);
	::memcpy(lpszBuff, p, nCopied);
	if (nCopied < nMaxChars)
		lpszBuff[nCopied] = 0;

	m_nOffset += (nLen + 1);
	return nCopied;
}

int CLBPacket::ReadString(LPWSTR lpszBuff, int nMaxChars)
{
	if (lpszBuff == NULL || nMaxChars < 1)
		return 0;

	*lpszBuff = 0;
	int nLen = GetNextStringLength() + 1;
	if (nLen < 2)
		return 0;

	LPSTR psz = new char[nLen];
	ReadData(psz, nLen);
	nLen = ::MultiByteToWideChar(CP_ACP, 0, psz, -1, lpszBuff, nMaxChars);
	delete [] psz;
	if (nLen < nMaxChars)
		lpszBuff[nLen] = 0;
	return nLen;
}

int CLBPacket::GetSequenceCount() const
{
	return int(((LPDWORD)m_pData)[PACKET_INDEX_SEQUENCECOUNT]);
}

int CLBPacket::GetSequenceID() const
{
	return int(((LPDWORD)m_pData)[PACKET_INDEX_SEQUENCEID]);
}

BOOL CLBPacket::IsSequenceStart() const
{
	return IsSequencial() && GetSequenceID() == 0;
}

BOOL CLBPacket::IsSequenceEnd() const
{
	int nSequenceCount = GetSequenceCount();
	if (nSequenceCount < 1)
		return TRUE;
	return GetSequenceID() + 1 >= nSequenceCount;
}

BOOL CLBPacket::IsSequencial() const
{
	return GetSequenceCount() > 0;
}
