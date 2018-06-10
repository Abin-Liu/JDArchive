//////////////////////////////////////////////////////
// LBNetwork.h
//
// Wraps windows socket API, manipulates data exchange
// over the network.
//
// Abin (abinn32@yahoo.com)
// 2011-9-03
//////////////////////////////////////////////////////

#ifndef __LBNETWORK_H__
#define __LBNETWORK_H__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// General utilities
BOOL __declspec(dllexport) LBNetworkStartup(BYTE nMajorVer = 2, BYTE nMinorVer = 2); // Initiate use of winsock, must call it once in your process
BOOL __declspec(dllexport) LBNetworkCleanup(); // Release winsock, must call this function at end of your process to free resources
DWORD __declspec(dllexport) LBNetworkComposeIP(BYTE b1, BYTE b2, BYTE b3, BYTE b4); // Compose an IP address from bytes into DWORD
void __declspec(dllexport) LBNetworkDecomposeIP(DWORD dwIP, BYTE& b1, BYTE& b2, BYTE& b3, BYTE& b4); // Decompose an IP address from DWORD into bytes
void __declspec(dllexport) LBNetworkFormatIP(DWORD dwIP, LPSTR lpszBuff); // Format an IP address into a char string
void __declspec(dllexport) LBNetworkFormatIP(DWORD dwIP, LPWSTR lpszBuff); // Format an IP address into a wide-char string

// Socket manipulation
BOOL __declspec(dllexport) LBSocketCreate(HANDLE& rSocket); // Create a socket
BOOL __declspec(dllexport) LBSocketClose(HANDLE& rSocket); // Close a socket
DWORD __declspec(dllexport) LBSocketGetLocalIP(HANDLE hSocket, LPINT lpPort = NULL); // Retrieve local IP address and port number (optional)
DWORD __declspec(dllexport) LBSocketGetPeerIP(HANDLE hSocket, LPINT lpPort = NULL); // Retrieve remote peer IP address and port number (optional)
int __declspec(dllexport) LBSocketReceive(HANDLE hSocket, LPVOID lpBuff, int nBuffSize); // Receive a packet
int __declspec(dllexport) LBSocketSend(HANDLE hSocket, LPCVOID lpPacket, int nPacketSize); // Send a packet
BOOL __declspec(dllexport) LBSocketListen(HANDLE hSocket, int nPort, int nBacklog = 5); // Listen to a port (server application)
HANDLE __declspec(dllexport) LBSocketAccept(HANDLE hSocket); // Accept an incoming connection (server application)
BOOL __declspec(dllexport) LBSocketConnect(HANDLE hSocket, LPCSTR lpszAddr, int nPort); // Connect to a server using the specified IP address and port number (client application)
BOOL __declspec(dllexport) LBSocketConnect(HANDLE hSocket, LPCWSTR lpszAddr, int nPort); // Connect to a server using the specified IP address and port number (client application)


//////////////////////////////////////////////////////
// CLBPacket class
//
// This class implements a "packet" which is the smallest data
// unit exchanged between 2 network terminals. Data pattern within
// a packet is described below:
//
// CRC32(4) | PACKET_ID(4) | SEQUENCE_COUNT(4) | SEQUENCE_ID(4) | DATA_SIZE(4) | DATA(various) | TAIL_ZERO(4)
//////////////////////////////////////////////////////

#define LBPACKET_SIZE		(4096) // Packet size
#define LBPACKET_INVALID_ID	(0xffffffff) // Invalid packet id

class __declspec(dllexport) CLBPacket
{
public:	

	enum { UNKNOWN = 0, READY, WRITING, READING };
	
	CLBPacket(const CLBPacket* pSrc = NULL);
	CLBPacket(const CLBPacket& rhs);
	~CLBPacket();

	const CLBPacket& operator=(const CLBPacket& rhs);	
	BOOL operator==(const CLBPacket& rhs) const;

	void Copy(const CLBPacket& rhs);
	void ResetCursor(); // Reset the reading cursor position, only available in READING status

	LPVOID GetData();
	LPCVOID GetData() const;
	int GetDataSize() const; // Used for local data processing
	int GetPacketSize() const; // Used for sending data to network
	DWORD GetPacketID() const;
	
	BOOL IsSequencial() const;
	int GetSequenceCount() const;
	int GetSequenceID() const;
	BOOL IsSequenceStart() const;
	BOOL IsSequenceEnd() const;

	int GetStatus() const;
	void PrepareForWrite(); // Start writing data
	int WriteData(LPCVOID lpData, int nDataSize);
	int WriteDword(DWORD val);
	int WriteInt(int val);
	int WriteWord(WORD val);
	int WriteShort(short val);
	int WriteByte(BYTE val);
	int WriteChar(char val);
	int WriteDouble(double val);
	int WriteFloat(float val);
	int WriteString(LPCSTR lpsz);
	int WriteString(LPCWSTR lpsz);
	int Encode(DWORD dwPacketID, int nSequenceCount = 0, int nSequenceID = 0); // End writing data

	BOOL Decode(int nReceivedDataSize); // Must call before extracting anything out of the packet
	int ReadData(LPVOID lpData, int nDataSize);
	DWORD ReadDword(DWORD defaultVal = 0);
	int ReadInt(int defaultVal = 0);
	WORD ReadWord(WORD defaultVal = 0);
	short ReadShort(short defaultVal = 0);
	BYTE ReadByte(BYTE defaultVal = 0);
	char ReadChar(char defaultVal = 0);
	double ReadDouble(double defaultVal = 0.0);
	float ReadFloat(float defaultVal = 0.0);
	int GetNextStringLength() const;
	int ReadString(LPSTR lpszBuff, int nMaxChars);
	int ReadString(LPWSTR lpszBuff, int nMaxChars);
	
private:

	BOOL VerifyWrite(LPCVOID lpData, int nDataSize) const;
	BOOL VerifyRead(LPVOID lpData, int nDataSize) const;

	LPBYTE m_pData;
	DWORD m_dwPacketID;
	int m_nDataSize;
	int m_nStatus;
	int m_nOffset; // Only used when extracting data
};

/////////////////////////////////////////////////////////////
// CLBSession
//
// Represents a running session, each session deals with a
// remote connection and processes incoming/outgoing packets.
/////////////////////////////////////////////////////////////

class __declspec(dllexport) CLBSession
{
public:

	CLBSession();
	virtual ~CLBSession();

	DWORD GetLocalAddr(LPINT lpPort = NULL) const;
	DWORD GetPeerAddr(LPINT lpPort = NULL) const;
	BOOL IsConnected() const;

	virtual BOOL Create(HANDLE hSocket = NULL); // NULL to call LBSocketCreate
	virtual void Abandon(); // Abandon the session (disconnect)
	virtual BOOL Connect(LPCSTR lpszAddr, int nPort); // Connect to a listening station (server) in the network
	virtual BOOL Connect(LPCWSTR lpszAddr, int nPort);
	virtual int Send(const CLBPacket* pPacket); // Send a packet	
	virtual int Receive(CLBPacket* pPacket, BOOL bAutoDecode = TRUE); // Receive a packet from peer
	
	// Sequencial packets
	virtual BOOL SendSequencial(DWORD dwPacketID, int nCount, LPVOID lParam = NULL);
	virtual BOOL ReceiveSequencial(DWORD dwPacketID, LPVOID lParam = NULL);
	virtual BOOL AcknowledgeSequence(DWORD dwPacketID, BOOL bAbort = FALSE); // Acknowledge a sequencial packet
	virtual void SendInvalidPacket(); // Send an invalid packet, usually used to tell the peer to abort its tasks

	enum
	{
		FILE_ERROR_SUCCESS = 0,
		FILE_ERROR_FILE_SYSTEM,
		FILE_ERROR_ZERO_LENGTH,
		FILE_ERROR_TOO_LARGE,
		FILE_ERROR_USER_ABORT,
		FILE_ERROR_PEER_ABORT,
		FILE_ERROR_NETWORK,
		FILE_ERROR_MD5_MISMATCH
	};
	
	virtual int SendFile(LPCSTR lpszFilePath, LPCSTR lpszMD5 = NULL, LPDWORD pSize = NULL, LPVOID lParam = NULL);
	virtual int SendFile(LPCWSTR lpszFilePath, LPCWSTR lpszMD5 = NULL, LPDWORD pSize = NULL, LPVOID lParam = NULL);
	virtual int ReceiveFile(LPCSTR lpszFilePath, LPSTR lpszMD5 = NULL, LPDWORD pSize = NULL, LPVOID lParam = NULL);
	virtual int ReceiveFile(LPCWSTR lpszFilePath, LPWSTR lpszMD5 = NULL, LPDWORD pSize = NULL, LPVOID lParam = NULL);

protected:

	virtual void OnConnect(); // Connection established
	virtual void OnDisconnect(); // Connection lost, either gracefully or not
	virtual BOOL SendSequencialProc(DWORD dwPacketID, CLBPacket* pPacket, int nCount, int nSequence, LPVOID lParam);
	virtual BOOL ReceiveSequencialProc(DWORD dwPacketID, CLBPacket* pPacket, int nCount, int nSequence, LPVOID lParam);
	virtual BOOL SendFileProc(LPCVOID lpszFilePath, DWORD dwBytesSent, DWORD dwBytesTotal, LPVOID lParam);
	virtual BOOL ReceiveFileProc(LPCVOID lpszFilePath, DWORD dwBytesReceived, DWORD dwBytesTotal, LPVOID lParam);

private:

	int __SendFile(HANDLE hFile, LPCSTR lpszMD5, LPDWORD pSize, LPVOID lParam, LPCVOID lpFilePath);
	int __ReceiveFile(HANDLE hFile, LPSTR lpszMD5, LPDWORD pSize, LPVOID lParam, LPCVOID lpFilePath);

	HANDLE m_hSocket; // The socket
};


/////////////////////////////////////////////////////////////
// CLBThreadSession <Abstract class>
//
// Represents a running session with ability to execute lengthy
// tasks in a separate thread to avoid freezing the main UI.
/////////////////////////////////////////////////////////////

class __declspec(dllexport) CLBThreadSession : public CLBSession
{
public:

	CLBThreadSession();
	virtual ~CLBThreadSession();

	void Lock() const; // Synchronization
	void Unlock() const;

	virtual BOOL Start(); // Start the thread with an optional user data
	virtual void Stop(DWORD dwMilliSeconds = INFINITE); // Stop the thread
	BOOL IsRunning() const; // Is the thread running
	BOOL IsStopping() const; // Is the thread stopping

	// Utility functions
	static HANDLE __CreateThread(LPTHREAD_START_ROUTINE lpfn, LPVOID lpParameter);
	static void __StopThread(HANDLE& rThread, DWORD dwWaitFor = INFINITE);
	static BOOL __IsThreadRunning(HANDLE hThread);

protected:
	
	virtual void OnStart(); // Thread starts (wanna call ::CoInitialize and stuff? There you go!)
	virtual void OnStop(); // Thread stops
	virtual void ThreadProc() = 0; // Pure virtual

private:	
	
	static DWORD WINAPI __SessionThreadProc(LPVOID lpParameter);
	HANDLE m_hThread; // Thread handle
	volatile BOOL m_bStopping; // Thread stopping signal
	LPCRITICAL_SECTION m_pcs; // Critical section object for synchronization	
};

#endif // __LBNETWORK_H__