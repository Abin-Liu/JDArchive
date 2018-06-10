////////////////////////////////////////////////////////////////
// LogFile.h
//
// Abin (abinn32@yahoo.com)
// 2011-9-17
////////////////////////////////////////////////////////////////

#ifndef __LOGFILE_H__
#define __LOGFILE_H__

class CLogFile : public CStdioFile
{
public:	

	enum { NONE = 0, TIME_ONLY, DATE_TIME };

	CLogFile();
	virtual ~CLogFile();

	virtual BOOL Open(LPCTSTR lpszPathName, BOOL bTruncate = TRUE);
	virtual void Close();
	virtual void Write(LPCTSTR lpszText, ...);
	void SetTimestamp(int nTimestamp = CLogFile::TIME_ONLY);

protected:

	BOOL m_bCreated;
	int m_nTimestamp;
	LPCRITICAL_SECTION m_pcs; // Critical section object for synchronization
};

#endif // __LOGFILE_H__