// SnowControlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WeatherControl.h"
#include "SnowControlDlg.h"
#include "afxdialogex.h"

#include "WeatherControlView.h"
#include "MainFrm.h"
// CSnowControlDlg dialog

IMPLEMENT_DYNAMIC(CSnowControlDlg, CDialogEx)

CSnowControlDlg::CSnowControlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSnowControlDlg::IDD, pParent)
{

}

CSnowControlDlg::~CSnowControlDlg()
{
}

void CSnowControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SNOW_ACTION, m_Action);
}


BEGIN_MESSAGE_MAP(CSnowControlDlg, CDialogEx)
	ON_WM_HSCROLL()
	ON_COMMAND(IDC_RAIN_ACTION, &CSnowControlDlg::OnRainAction)
END_MESSAGE_MAP()


// CSnowControlDlg message handlers


BOOL CSnowControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CSnowControlDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CSnowControlDlg::OnRainAction()
{
	CMainFrame *MainFrame = (CMainFrame *)((CWeatherControlApp*)AfxGetApp())->GetMainWnd();
	CWeatherControlView *View = (CWeatherControlView *)MainFrame->GetActiveView();
  	CEngine *Engine = ((CWeatherControlApp*)AfxGetApp())->m_Engine;
	Ogre::Root *Root = Engine->GetRoot();

	View->m_Snow->setVisible(!View->m_Snow->getVisible());

	if (View->m_Snow->getVisible())
	{
		m_Action.SetWindowTextA("Stop");
		View->SetTimer(ID_SNOW_TIMER, 1, 0);
	}
	else
	{
		m_Action.SetWindowTextA("Start");
		View->KillTimer(ID_SNOW_TIMER);
	}

	Root->renderOneFrame();
}
