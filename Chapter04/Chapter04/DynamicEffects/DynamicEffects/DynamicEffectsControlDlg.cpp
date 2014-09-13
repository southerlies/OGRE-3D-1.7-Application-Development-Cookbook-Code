// DynamicEffectsControlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DynamicEffects.h"
#include "DynamicEffectsControlDlg.h"
#include "afxdialogex.h"
#include "DynamicEffectsView.h"
#include "MainFrm.h"
// CDynamicEffectsControlDlg dialog

IMPLEMENT_DYNAMIC(CDynamicEffectsControlDlg, CDialogEx)

CDynamicEffectsControlDlg::CDynamicEffectsControlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDynamicEffectsControlDlg::IDD, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}

CDynamicEffectsControlDlg::~CDynamicEffectsControlDlg()
{
}

void CDynamicEffectsControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ACTION, m_Action);
	DDX_Control(pDX, IDC_ANGLE, m_Angle);
	DDX_Control(pDX, IDC_INTENSITY, m_Intensity);
	DDX_Control(pDX, IDC_TV, m_TV);
}


BEGIN_MESSAGE_MAP(CDynamicEffectsControlDlg, CDialogEx)
	ON_BN_CLICKED(IDC_ACTION, &CDynamicEffectsControlDlg::OnBnClickedAction)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_TV, &CDynamicEffectsControlDlg::OnBnClickedTv)
END_MESSAGE_MAP()


// CDynamicEffectsControlDlg message handlers


BOOL CDynamicEffectsControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_Angle.SetRange(0, 7);
	m_Angle.SetTicFreq(1);
	m_Angle.SetPos(0);

	m_Intensity.SetRange(0,100);
	m_Intensity.SetTicFreq(10);
	m_Intensity.SetPos(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDynamicEffectsControlDlg::OnBnClickedAction()
{
	CString Caption;
	m_Action.GetWindowTextA(Caption);

	CMainFrame *MainFrame = (CMainFrame *)((CDynamicEffectsApp*)AfxGetApp())->GetMainWnd();
	CDynamicEffectsView *View = (CDynamicEffectsView *)MainFrame->GetActiveView();
 
	if (Caption == "Start")
	{
		m_Action.SetWindowTextA("Stop");
		View->SetTimer(1, 10, 0);
	}
	else
	{
		m_Action.SetWindowTextA("Start");
		View->KillTimer(1);
	}
}


void CDynamicEffectsControlDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int Angle = m_Angle.GetPos();
	
	CMainFrame *MainFrame = (CMainFrame *)((CDynamicEffectsApp*)AfxGetApp())->GetMainWnd();
	CDynamicEffectsView *View = (CDynamicEffectsView *)MainFrame->GetActiveView();
 
//	View->m_Camera->yaw/*roll*/(Ogre::Radian::Radian(Ogre::Degree::Degree(Angle)));

	CEngine * Engine = ((CDynamicEffectsApp*)AfxGetApp())->m_Engine;
	Ogre::Root *Root = Engine->GetRoot();
	
//	View->m_Camera->lookAt(View->lights[Angle]->getPosition());

		Ogre::Real red = (double)m_Intensity.GetPos()/100.0;
		Ogre::Real green = (double)m_Intensity.GetPos()/100.0;
		Ogre::Real blue = 0.0;

		View->Light->setDiffuseColour(red, green, blue);      
		View->Light->setSpecularColour(red, green, blue);    

	int numSegBase = 24;
	int numSegHeight = 24;
	Ogre::Real radius = 10.0;
	Ogre::Real height = 20.0;

	int offset = 0;
	Ogre::Real deltaAngle = (Ogre::Math::TWO_PI / numSegBase);
	Ogre::Real deltaHeight = height/(Ogre::Real)numSegHeight;
	View->Cone->beginUpdate(0);
	
	Ogre::Vector3 refNormal = Ogre::Vector3(radius, height, 0.f).normalisedCopy();
	Ogre::Quaternion q;

	for (int i = 0; i <= numSegHeight; i++)
	{
		Ogre::Real r0 = radius * (1 - i / (Ogre::Real)numSegHeight);

		for (int j = 0; j <= numSegBase; j++)
		{
			Ogre::Real x0 = r0* cosf(j*deltaAngle);
			Ogre::Real z0 = r0 * sinf(j*deltaAngle);
			q.FromAngleAxis(Ogre::Radian(-j*deltaAngle), Ogre::Vector3::NEGATIVE_UNIT_Y);
			View->Cone->normal(q*refNormal);
			View->Cone->position(x0, i*deltaHeight, z0);		
			View->Cone->colour(m_Intensity.GetPos()/100.0, m_Intensity.GetPos()/100.0, 0.0, 1.0);
			View->Cone->textureCoord(j/(Ogre::Real)numSegBase, i/(Ogre::Real)numSegHeight);

			if (i != numSegHeight&& j != numSegBase)
			{
				View->Cone->index(offset + numSegBase + 2);
				View->Cone->index(offset);
				View->Cone->index(offset + numSegBase + 1);
				View->Cone->index(offset + numSegBase + 2);
				View->Cone->index(offset + 1);
				View->Cone->index(offset);
			}

			offset ++;
		}
	}

	//low cap
	int centerIndex = offset;
	
	View->Cone->position(0,0,0);
	View->Cone->normal(Ogre::Vector3::NEGATIVE_UNIT_Y);
	View->Cone->textureCoord(0.0,1.0);
	offset++;
	for (int j=0; j<=numSegBase; j++)
	{
		Ogre::Real x0 = radius * cosf(j*deltaAngle);
		Ogre::Real z0 = radius * sinf(j*deltaAngle);

		View->Cone->position(x0, 0.0f, z0);
		View->Cone->colour(1.0, 1.0, 0.0, 1.0);
		View->Cone->normal(Ogre::Vector3::NEGATIVE_UNIT_Y);
		View->Cone->textureCoord(j/(Ogre::Real)numSegBase,0.0);
		if (j!=numSegBase)
		{
			View->Cone->index(centerIndex);
			View->Cone->index(offset);
			View->Cone->index(offset+1);
		}
		offset++;
	}
	
	View->Cone->end();

	Root->renderOneFrame();

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CDynamicEffectsControlDlg::OnBnClickedTv()
{
	CString Caption;
	m_TV.GetWindowTextA(Caption);

	CMainFrame *MainFrame = (CMainFrame *)((CDynamicEffectsApp*)AfxGetApp())->GetMainWnd();
	CDynamicEffectsView *View = (CDynamicEffectsView *)MainFrame->GetActiveView();
 
	CEngine * Engine = ((CDynamicEffectsApp*)AfxGetApp())->m_Engine;
	Ogre::Root *Root = Engine->GetRoot();

	Ogre::Real red = 0.0;
	Ogre::Real green = 0.0;
	Ogre::Real blue = 0.0;

	if (Caption == "TV Start")
	{
		m_TV.SetWindowTextA("TV Stop");
	}
	else
	{
		m_TV.SetWindowTextA("TV Start");
		red = 1.0;
		green = 0.0;
		blue = 0.0;
	}

	View->Light->setDiffuseColour(red, green, blue);      
	View->Light->setSpecularColour(red, green, blue);    
  

	Root->renderOneFrame();
}
