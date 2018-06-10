// VerManageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "JDArchive.h"
#include "VerManageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVerManageDlg dialog


CVerManageDlg::CVerManageDlg(LPCTSTR lpszFileName, int nVersion, CWnd* pParent /*=NULL*/)
	: CNewDialog(CVerManageDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVerManageDlg)
	m_sCurMajor = _T("");
	m_sCurMinor = _T("");
	m_sFileName = _T("");
	m_sMajor = _T("");
	m_sMinor = _T("");
	m_sBuild = _T("");
	m_sCurBuild = _T("");
	//}}AFX_DATA_INIT
	m_sFileName = lpszFileName;
	m_nCurVersion = nVersion;
	m_nVersion = nVersion;

	m_nCurMajor = nVersion / 1000000;
	m_nCurMinor = (nVersion / 10000) % 100;
	m_nCurBuild = nVersion % 10000;
	
	m_sCurMajor.Format(_T("%d"), m_nCurMajor);
	m_sCurMinor.Format(_T("%02d"), m_nCurMinor);
	m_sCurBuild.Format(_T("%04d"), m_nCurBuild);
	
	m_sMajor = m_sCurMajor;
	m_sMinor = m_sCurMinor;
	m_sBuild = m_sCurBuild;
}


void CVerManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CNewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVerManageDlg)
	DDX_Control(pDX, IDC_BUILD, m_wndBuild);
	DDX_Control(pDX, IDC_MINOR, m_wndMinor);
	DDX_Control(pDX, IDC_MAJOR1, m_wndMajor);
	DDX_Text(pDX, IDC_CURMAJOR, m_sCurMajor);
	DDX_Text(pDX, IDC_CURMINOR, m_sCurMinor);
	DDX_Text(pDX, IDC_FILENAME, m_sFileName);
	DDX_Text(pDX, IDC_MAJOR1, m_sMajor);
	DDV_MaxChars(pDX, m_sMajor, 3);
	DDX_Text(pDX, IDC_MINOR, m_sMinor);
	DDV_MaxChars(pDX, m_sMinor, 2);
	DDX_Text(pDX, IDC_BUILD, m_sBuild);
	DDV_MaxChars(pDX, m_sBuild, 4);
	DDX_Text(pDX, IDC_CURBUILD, m_sCurBuild);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVerManageDlg, CNewDialog)
	//{{AFX_MSG_MAP(CVerManageDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVerManageDlg message handlers

void CVerManageDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!UpdateData(TRUE))
		return;

	int nMajor = _ttoi(m_sMajor);
	if (nMajor < 1 || nMajor > 255)
	{
		MessageBox(_T("主版本号必须介于1-255之间"), _T("版本号管理"), MB_ICONEXCLAMATION);
		m_wndMajor.SetFocus();
		m_wndMajor.Highlight();
		return;
	}

	int nMinor = _ttoi(m_sMinor);
	if (nMinor < 0 || nMinor > 99)
	{
		MessageBox(_T("副版本号必须介于00-99之间"), _T("版本号管理"), MB_ICONEXCLAMATION);
		m_wndMinor.SetFocus();
		m_wndMinor.Highlight();
		return;
	}
	
	int nBuild = _ttoi(m_sBuild);
	if (nBuild < 1 || nBuild > 9999)
	{
		MessageBox(_T("内部号必须介于0001-9999之间"), _T("版本号管理"), MB_ICONEXCLAMATION);
		m_wndBuild.SetFocus();
		m_wndBuild.Highlight();
		return;
	}

	if (nMajor > m_nCurMajor && nMinor > 0)
	{
		if (MessageBox(_T("主版本号提升后，副版本号从00开始有利于资料版本管理的连续性。\n是否要将副版本号改为00？"), _T("版本号管理"), MB_ICONQUESTION | MB_YESNO) == IDYES)
		{
			nMinor = 0;
			nBuild = 1;
			m_wndMinor.SetWindowText(_T("00"));
			m_wndBuild.SetWindowText(_T("0001"));
			return;
		}
	}

	if (nMajor * 100 + nMinor > m_nCurMajor * 100 + m_nCurMinor && nBuild > 1)
	{
		if (MessageBox(_T("主/副版本号提升后，内部号从0001开始有利于资料版本管理的连续性。\n是否要将内部号改为0001？"), _T("版本号管理"), MB_ICONQUESTION | MB_YESNO) == IDYES)
		{
			nBuild = 1;
			m_wndBuild.SetWindowText(_T("0001"));
			return;
		}
	}
	
	m_nVersion = nMajor * 1000000 + nMinor * 10000 + nBuild;
	if (m_nVersion == m_nCurVersion)
	{
		MessageBox(_T("版本号无变化"), _T("版本号管理"), MB_ICONEXCLAMATION);
		m_wndMinor.SetFocus();
		m_wndMinor.Highlight();
		return;
	}

	if (m_nVersion < m_nCurVersion)
	{
		CString sMsg;
		sMsg.Format(_T("资料库不允许版本倒退，新版本号不得低于%s"), (LPCTSTR)AfxParseVersion(m_nCurVersion));
		MessageBox(sMsg, _T("版本号管理"), MB_ICONEXCLAMATION);
		m_wndMinor.SetFocus();
		m_wndMinor.Highlight();
		return;
	}

	EndDialog(IDOK);
}

void CVerManageDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	_SESSION.TaskCancelRequestManageVer();
	CNewDialog::OnCancel();
}
