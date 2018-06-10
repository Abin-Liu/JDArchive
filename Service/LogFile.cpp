////////////////////////////////////////////////////////////////
// LogFile.cpp
//
// Abin (abinn32@yahoo.com)
// 2011-9-17
////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogFile.h"

CLogFile::CLogFile()
{
	m_bCreated = FALSE;
	m_nTimestamp = DATE_TIME;
	m_pcs = new CRITICAL_SECTION;
	::InitializeCriticalSection(m_pcs);
}

CLogFile::~CLogFile()
{
	Close();
	::DeleteCriticalSection(m_pcs);
	delete m_pcs;
}

BOOL CLogFile::Open(LPCTSTR lpszPathName, BOOL bTruncate)
{
	Close();
	
	if (lpszPathName == NULL || *lpszPathName == 0)
		return FALSE;

	CString sPathName(lpszPathName);	
	if (sPathName.GetLength() <= 3 || sPathName[1] != _T(':'))
	{
		TCHAR sz[MAX_PATH + 1] = _T("");
		::GetModuleFileName(NULL, sz, MAX_PATH);
		LPTSTR p = _tcsrchr(sz, _T('\\'));
		if (p)
			*p = 0;

		sPathName.Format(_T("%s\\%s"), sz, lpszPathName);
	}

	m_bCreated = CStdioFile::Open(sPathName, CFile::modeWrite | (bTruncate ? CFile::modeCreate : CFile::modeNoTruncate) | CFile::shareDenyWrite | CFile::typeText);
	return m_bCreated;
}

void CLogFile::Write(LPCTSTR lpszText, ...)
{
	if (!m_bCreated)
		return;

	va_list args;
	va_start(args, lpszText);
	CString sText;
	sText.FormatV(lpszText, args);
	va_end(args);

	::EnterCriticalSection(m_pcs);
	if (m_nTimestamp)
	{
		WriteString(COleDateTime::GetCurrentTime().Format(m_nTimestamp == DATE_TIME ? 0 : VAR_TIMEVALUEONLY));
		WriteString(_T(" "));
	}
	WriteString(sText);
	WriteString(_T("\n"));
	Flush();
	::LeaveCriticalSection(m_pcs);
}

void CLogFile::Close()
{
	if (m_bCreated)
	{
		m_bCreated = FALSE;
		CStdioFile::Close();
	}
}

void CLogFile::SetTimestamp(int nTimestamp)
{
	m_nTimestamp = nTimestamp;
}
