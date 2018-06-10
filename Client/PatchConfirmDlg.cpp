// PatchConfirmDlg.cpp : implementation file
//

#include "stdafx.h"
#include "JDArchive.h"
#include "PatchConfirmDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPatchConfirmDlg dialog


CPatchConfirmDlg::CPatchConfirmDlg(CLBPacket* pPacket, CWnd* pParent /*=NULL*/)
	: CNewDialog(CPatchConfirmDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPatchConfirmDlg)
	m_sAuthor = _T("");
	m_sDate = _T("");
	m_sFileName = _T("");
	m_sVer = _T("");
	m_sSize = _T("");
	//}}AFX_DATA_INIT

	m_nPatchID = pPacket->ReadInt();
	m_sVer.Format(_T("v%d.%02d"), m_nPatchID / 100, m_nPatchID % 100);
	DWORD dwSize = pPacket->ReadDword();
	m_sSize = AfxSizeToStr(dwSize);
	m_sFileName = ReadPacketString(pPacket);
	m_sAuthor = ReadPacketString(pPacket);
	COleDateTime date;
	pPacket->ReadData(&date, sizeof(date));
	m_sDate = date.Format();
}


void CPatchConfirmDlg::DoDataExchange(CDataExchange* pDX)
{
	CNewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPatchConfirmDlg)
	DDX_Text(pDX, IDC_AUTHOR, m_sAuthor);
	DDX_Text(pDX, IDC_DATE, m_sDate);
	DDX_Text(pDX, IDC_FILENAME, m_sFileName);
	DDX_Text(pDX, IDC_VER, m_sVer);
	DDX_Text(pDX, IDC_FILELEN, m_sSize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPatchConfirmDlg, CNewDialog)
	//{{AFX_MSG_MAP(CPatchConfirmDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPatchConfirmDlg message handlers

