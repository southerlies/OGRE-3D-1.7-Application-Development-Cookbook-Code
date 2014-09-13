// SunControlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WeatherControl.h"
#include "SunControlDlg.h"
#include "afxdialogex.h"

#include "WeatherControlView.h"
#include "MainFrm.h"
// CSunControlDlg dialog

IMPLEMENT_DYNAMIC(CSunControlDlg, CDialogEx)

CSunControlDlg::CSunControlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSunControlDlg::IDD, pParent)
{

}

CSunControlDlg::~CSunControlDlg()
{
}

void CSunControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUN_ACTION, m_Action);
}


BEGIN_MESSAGE_MAP(CSunControlDlg, CDialogEx)
	ON_BN_CLICKED(IDC_SUN_ACTION, &CSunControlDlg::OnBnClickedSunAction)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CSunControlDlg message handlers


void CSunControlDlg::OnBnClickedSunAction()
{
	CMainFrame *MainFrame = (CMainFrame *)((CWeatherControlApp*)AfxGetApp())->GetMainWnd();
	CWeatherControlView *View = (CWeatherControlView *)MainFrame->GetActiveView();
  	CEngine *Engine = ((CWeatherControlApp*)AfxGetApp())->m_Engine;
	Ogre::Root *Root = Engine->GetRoot();

	View->m_Sun->setVisible(!View->m_Sun->getVisible());

	if (View->m_Sun->getVisible())
	{
		m_Action.SetWindowTextA("Stop");
		View->SetTimer(ID_SUN_TIMER, 1, 0);
	}
	else
	{
		m_Action.SetWindowTextA("Start");
		View->KillTimer(ID_SUN_TIMER);
	}

	Root->renderOneFrame();
}

BOOL CSunControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CSunControlDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
