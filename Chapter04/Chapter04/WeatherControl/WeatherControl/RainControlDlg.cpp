// RainControlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WeatherControl.h"
#include "RainControlDlg.h"
#include "afxdialogex.h"

#include "WeatherControlView.h"
#include "MainFrm.h"

// CRainControlDlg dialog

IMPLEMENT_DYNAMIC(CRainControlDlg, CDialogEx)

CRainControlDlg::CRainControlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRainControlDlg::IDD, pParent)
	, m_PlaySound(false)
{

}

CRainControlDlg::~CRainControlDlg()
{
}

void CRainControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RAIN_ACTION, m_Action);
	DDX_Control(pDX, IDC_RAIN_PARTICLE_WIDTH, m_ParticleWidth);
	DDX_Control(pDX, IDC_RAIN_PARTICLE_HEIGHT, m_ParticleHeight);
}


BEGIN_MESSAGE_MAP(CRainControlDlg, CDialogEx)
	ON_BN_CLICKED(IDC_RAIN_ACTION, &CRainControlDlg::OnClickedAction)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_PLAY_SOUND, &CRainControlDlg::OnBnClickedPlaySound)
END_MESSAGE_MAP()


// CRainControlDlg message handlers


void CRainControlDlg::OnClickedAction()
{
	CWeatherControlApp* WeatherControlApp = (CWeatherControlApp*)AfxGetApp();
	CMainFrame *MainFrame = (CMainFrame *)WeatherControlApp->GetMainWnd();
	CWeatherControlView *View = (CWeatherControlView *)MainFrame->GetActiveView();
  	CEngine *Engine = ((CWeatherControlApp*)AfxGetApp())->m_Engine;
	Ogre::Root *Root = Engine->GetRoot();

	View->m_Rain->setVisible(!View->m_Rain->getVisible());

	if (View->m_Rain->getVisible())
	{
		m_Action.SetWindowTextA("Stop");
		View->SetTimer(ID_RAIN_TIMER, 1, 0);
	}
	else
	{
		m_Action.SetWindowTextA("Start");
		View->KillTimer(ID_RAIN_TIMER);
		WeatherControlApp->m_cpVoice->Pause();
	}

	Root->renderOneFrame();
}


BOOL CRainControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CMainFrame *MainFrame = (CMainFrame *)((CWeatherControlApp*)AfxGetApp())->GetMainWnd();
	CWeatherControlView *View = (CWeatherControlView *)MainFrame->GetActiveView();
 
	int ParticleWidth = View->m_Rain->getDefaultWidth();
	int ParticleHeight = View->m_Rain->getDefaultHeight();
	int ParticleQuota = View->m_Rain->getParticleQuota();
	
	Ogre::ParticleAffector *Affector = View->m_Rain->getAffector(0);
	Ogre::ParticleEmitter *Emitter = View->m_Rain->getEmitter(0);

	m_ParticleWidth.SetRange(0, 5 * ParticleWidth);
	m_ParticleWidth.SetTicFreq(ParticleWidth / 2);
	m_ParticleWidth.SetPos(ParticleWidth);
	
	m_ParticleHeight.SetRange(0, 5 * ParticleHeight);
	m_ParticleHeight.SetTicFreq(ParticleHeight / 2);
	m_ParticleHeight.SetPos(ParticleHeight);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CRainControlDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CMainFrame *MainFrame = (CMainFrame *)((CWeatherControlApp*)AfxGetApp())->GetMainWnd();
	CWeatherControlView *View = (CWeatherControlView *)MainFrame->GetActiveView();
 
	int ParticleWidth = m_ParticleWidth.GetPos();
	int ParticleHeight = m_ParticleHeight.GetPos();
	
	View->m_Rain->setDefaultDimensions(ParticleWidth, ParticleHeight);

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CRainControlDlg::OnBnClickedPlaySound()
{
	m_PlaySound = !m_PlaySound;
}
