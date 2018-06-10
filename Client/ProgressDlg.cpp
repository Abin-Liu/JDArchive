// ProgressDlg.cpp : implementation file
//

#include "stdafx.h"
#include "JDArchive.h"
#include "ProgressDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgressDlg dialog


CProgressDlg::CProgressDlg(CWnd* pParent /*=NULL*/)
	: CNewDialog(CProgressDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProgressDlg)
	//}}AFX_DATA_INIT
	m_bCancelled = FALSE;
}

void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CNewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProgressDlg)
	DDX_Control(pDX, IDCANCEL, m_wndCancel);
	DDX_Control(pDX, IDC_TEXT, m_wndText);
	DDX_Control(pDX, IDC_PROGRESS1, m_wndProgress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProgressDlg, CNewDialog)
	//{{AFX_MSG_MAP(CProgressDlg)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProgressDlg message handlers

BOOL CProgressDlg::OnInitDialog() 
{
	CNewDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	Hide();
	m_wndProgress.SetDrawEdge(FALSE);
	m_wndProgress.SetWindowText(NULL);
	m_wndProgress.SetShowPercent(TRUE);
	m_wndProgress.SetRange32(0, 100);
	m_wndProgress.SetPos(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CProgressDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	m_bCancelled = TRUE;
	Hide();
}

void CProgressDlg::SetTitle(LPCTSTR lpszTitle)
{
	SetWindowText(lpszTitle);
}

void CProgressDlg::SetText(LPCTSTR lpszText)
{
	m_wndText.SetWindowText(lpszText);
}

BOOL CProgressDlg::SetProgress(int nCompleted, int nTotal)
{
	if (!IsWindowVisible())
		ShowWindow(SW_SHOW);
		
	if (nTotal > 0)
		m_wndProgress.SetPos(int((double)nCompleted / (double)nTotal * 100.0));

	PeekAndPump();
	return !m_bCancelled;
}

void CProgressDlg::Hide()
{
	ShowWindow(SW_HIDE);
	m_pParent->EnableWindow(TRUE);
}

BOOL CProgressDlg::Cancelled() const
{
	return m_bCancelled;
}

void CProgressDlg::Show(LPCTSTR lpszTitle, LPCTSTR lpszText)
{
	m_bCancelled = FALSE;
	SetTitle(lpszTitle);
	SetText(lpszText);
	m_wndProgress.SetPos(0);
	ShowWindow(SW_SHOW);
}

void CProgressDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CNewDialog::OnShowWindow(bShow, nStatus);	
	m_pParent->EnableWindow(!bShow);
	CenterWindow(m_pParent);
}

BOOL CProgressDlg::Create(CWnd* pParentWnd) 
{
	m_pParent = pParentWnd;
	return CNewDialog::Create(IDD, pParentWnd);
}

void CProgressDlg::PeekAndPump(BOOL bCancelOnESCkey /*= TRUE*/)
{    
    MSG msg;
    while (!m_bCancelled && ::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
    {
        if (bCancelOnESCkey && (msg.message == WM_CHAR) && (msg.wParam == VK_ESCAPE))
            OnCancel();

        if (msg.message == WM_LBUTTONUP)
        {
            CRect rect;
            m_wndCancel.GetWindowRect(rect);
            if (rect.PtInRect(msg.pt))
                OnCancel();
        }
  
        if (!AfxGetApp()->PumpMessage())
        {
            ::PostQuitMessage(0);
            return;
        } 
    }
}
