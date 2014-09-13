// ParticleSystemControlDlg.cpp : implementation file
//
#pragma once
#include "stdafx.h"

#include "Ogre.h"

#include "ParticleSystem.h"
#include "ParticleSystemControlDlg.h"
#include "afxdialogex.h"

#include "MainFrm.h"

#include "RingEmitterDlg.h"
#include "PointEmitterDlg.h"
#include "BoxEmitterDlg.h"
#include "EllipsoidEmitterDlg.h"
#include "CylinderEmitterDlg.h"

#include "DeflectorPlaneAffectorDlg.h"
#include "DirectionRandomiserDlg.h"
// CParticleSystemControlDlg dialog

IMPLEMENT_DYNAMIC(CParticleSystemControlDlg, CDialogEx)

CParticleSystemControlDlg::CParticleSystemControlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CParticleSystemControlDlg::IDD, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif
	m_ParticleSystem = NULL;
}

CParticleSystemControlDlg::~CParticleSystemControlDlg()
{
}

void CParticleSystemControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SYSTEM_TREE, m_ParticleSystemTree);
}


BEGIN_MESSAGE_MAP(CParticleSystemControlDlg, CDialogEx)
	ON_WM_HSCROLL()
	ON_COMMAND(ID_EMITTERS_ADDPOINTEMITTER, &CParticleSystemControlDlg::OnEmittersAddPointEmitter)
	ON_COMMAND(ID_EMITTERS_ADDBOXEMITTER, &CParticleSystemControlDlg::OnEmittersAddBoxEmitter)
	ON_COMMAND(ID_EMITTERS_ADDCYLINDEREMITTER, &CParticleSystemControlDlg::OnEmittersAddCylinderEmitter)
	ON_COMMAND(ID_EMITTERS_ADDELLIPSOIDEMITTER, &CParticleSystemControlDlg::OnEmittersAddellipsoidemitter)
	ON_COMMAND(ID_EMITTERS_ADDHOLLOWELLIPSOIDEMITTER, &CParticleSystemControlDlg::OnEmittersAddhollowellipsoidemitter)
	ON_COMMAND(ID_EMITTERS_ADDRINGEMITTER, &CParticleSystemControlDlg::OnEmittersAddringemitter)
	ON_COMMAND(ID_AFFECTORS_ADDLINEARFORCEAFFECTOR, &CParticleSystemControlDlg::OnAffectorsAddlinearforceaffector)
	ON_COMMAND(ID_AFFECTORS_ADDROTATORAFFECTOR, &CParticleSystemControlDlg::OnAffectorsAddrotatoraffector)
	ON_COMMAND(ID_AFFECTORS_ADDDEFLECTORPLANEAFFECTOR, &CParticleSystemControlDlg::OnAffectorsAdddeflectorplaneaffector)
	ON_COMMAND(ID_AFFECTORS_ADDCOLOURFADERAFFECTOR, &CParticleSystemControlDlg::OnAffectorsAddcolourfaderaffector)
	ON_BN_CLICKED(IDC_CREATE_PARTICLE_SYSTEM, &CParticleSystemControlDlg::OnBnClickedCreateParticleSystem)
	ON_BN_CLICKED(IDC_GET_PARTICLE_SYSTEM, &CParticleSystemControlDlg::OnBnClickedGetParticleSystem)
	ON_BN_CLICKED(IDC_TRY_PARTICLE_SYSTEM, &CParticleSystemControlDlg::OnBnClickedTryParticleSystem)
	ON_COMMAND(ID_AFFECTORS_ADDDIRECTIONRANDOMISERAFFECTOR, &CParticleSystemControlDlg::OnAffectorsAdddirectionrandomiseraffector)
END_MESSAGE_MAP()


// CParticleSystemControlDlg message handlers


BOOL CParticleSystemControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	HTREEITEM ParticleSystemItem = m_ParticleSystemTree.InsertItem("Particle System");
	m_EmittersItem = m_ParticleSystemTree.InsertItem("Emitters", ParticleSystemItem);
	m_AffectorsItem = m_ParticleSystemTree.InsertItem("Affectors", ParticleSystemItem);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CParticleSystemControlDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CParticleSystemControlDlg::OnEmittersAddPointEmitter()
{
	CPointEmitterDlg PointEmitterDlg;

	if (IDOK == PointEmitterDlg.DoModal())
	{
		HTREEITEM EmitterItem = m_ParticleSystemTree.InsertItem(PointEmitterDlg.m_EmitterName, m_EmittersItem);
		m_ParticleSystemTree.EnsureVisible(EmitterItem);
	}
}


void CParticleSystemControlDlg::OnEmittersAddBoxEmitter()
{
	CBoxEmitterDlg BoxEmitterDlg;

	if (IDOK == BoxEmitterDlg.DoModal())
	{
		HTREEITEM EmitterItem = m_ParticleSystemTree.InsertItem(BoxEmitterDlg.m_EmitterName, m_EmittersItem);
		m_ParticleSystemTree.EnsureVisible(EmitterItem);

		Ogre::ParticleEmitter *BoxEmitter = m_ParticleSystem->addEmitter("Box");

		BoxEmitter->setAngle(Ogre::Radian(Ogre::Math::PI));
		BoxEmitter->setColour(Ogre::ColourValue(1, 1, 1, 1));
		BoxEmitter->setColourRangeEnd(Ogre::ColourValue(1, 1, 1, 1));
		BoxEmitter->setColourRangeStart(Ogre::ColourValue(1, 1, 1, 1));
		BoxEmitter->setDirection(Ogre::Vector3(0, 1, 0));
		BoxEmitter->setDuration(0);
		BoxEmitter->setEmissionRate(30);
		BoxEmitter->setEnabled(true);
		BoxEmitter->setMaxDuration(0);
		BoxEmitter->setMaxParticleVelocity(1);
		BoxEmitter->setMaxRepeatDelay(0);
		BoxEmitter->setMaxTimeToLive(20);
		BoxEmitter->setMinDuration(0);
		BoxEmitter->setMinParticleVelocity(50);
		BoxEmitter->setMinRepeatDelay(0);
		BoxEmitter->setMinTimeToLive(20);
		BoxEmitter->setName("BoxEmitter");
		BoxEmitter->setParticleVelocity(50);
		BoxEmitter->setRepeatDelay(0);
		BoxEmitter->setTimeToLive(20);
	}
}


void CParticleSystemControlDlg::OnEmittersAddCylinderEmitter()
{
	CCylinderEmitterDlg CylinderEmitterDlg;

	if (IDOK == CylinderEmitterDlg.DoModal())
	{
		m_ParticleSystemTree.InsertItem(CylinderEmitterDlg.m_EmitterName, m_EmittersItem);
	}
}


void CParticleSystemControlDlg::OnEmittersAddellipsoidemitter()
{
	CEllipsoidEmitterDlg EllipsoidEmitterDlg;

	if (IDOK == EllipsoidEmitterDlg.DoModal())
	{
		m_ParticleSystemTree.InsertItem(EllipsoidEmitterDlg.m_EmitterName, m_EmittersItem);
	}
}


void CParticleSystemControlDlg::OnEmittersAddhollowellipsoidemitter()
{
	// TODO: Add your command handler code here
}


void CParticleSystemControlDlg::OnEmittersAddringemitter()
{
	CRingEmitterDlg RingEmitterDlg;

	if (IDOK == RingEmitterDlg.DoModal())
	{
		m_ParticleSystemTree.InsertItem(RingEmitterDlg.m_EmitterName, m_EmittersItem);
	}
}


void CParticleSystemControlDlg::OnAffectorsAddlinearforceaffector()
{
	// TODO: Add your command handler code here
}


void CParticleSystemControlDlg::OnAffectorsAddrotatoraffector()
{
	// TODO: Add your command handler code here
}


void CParticleSystemControlDlg::OnAffectorsAdddeflectorplaneaffector()
{
	CDeflectorPlaneAffectorDlg DeflectorPlaneAffectorDlg;

	if (IDOK == DeflectorPlaneAffectorDlg.DoModal())
	{
		m_ParticleSystemTree.InsertItem(DeflectorPlaneAffectorDlg.m_AffectorName, m_AffectorsItem);
			
		Ogre::ParticleAffector *Plane = m_ParticleSystem->addAffector("DeflectorPlane");
		
		Plane->setParameter("plane_point", "0 -50 0");
		Plane->setParameter("plane_normal", "0 1 0");
		Plane->setParameter("bounce", "1");
	}
}


void CParticleSystemControlDlg::OnAffectorsAddcolourfaderaffector()
{
	CDeflectorPlaneAffectorDlg DeflectorPlaneAffectorDlg;

	if (IDOK == DeflectorPlaneAffectorDlg.DoModal())
	{
		m_ParticleSystemTree.InsertItem(DeflectorPlaneAffectorDlg.m_AffectorName, m_AffectorsItem);
		Ogre::ParticleAffector *Color = m_ParticleSystem->addAffector("ColourFader");
		Color->setParameter("red", "-0.05");
		Color->setParameter("green", "0");
		Color->setParameter("blue", "0");
		Color->setParameter("alpha", "0");
	}
}


void CParticleSystemControlDlg::OnBnClickedCreateParticleSystem()
{
	CMainFrame *MainFrame = (CMainFrame *)((CParticleSystemApp*)AfxGetApp())->GetMainWnd();

	CEngine *Engine = ((CParticleSystemApp*)AfxGetApp())->m_Engine;
	Ogre::Root *Root = Engine->GetRoot();
	Ogre::SceneManager *SceneManager = Root->getSceneManager("ParticleSystem");

	m_ParticleSystem = SceneManager->createParticleSystem("Sun");

	m_ParticleSystem->setDefaultDimensions(12, 24);
	m_ParticleSystem->setSortingEnabled(true);
	m_ParticleSystem->setMaterialName("Examples/Flare2");
	m_ParticleSystem->setCullIndividually(true);
	m_ParticleSystem->setParticleQuota(3000);
	m_ParticleSystem->setRenderer("billboard");
	m_ParticleSystem->setKeepParticlesInLocalSpace(false);
}


void CParticleSystemControlDlg::OnBnClickedGetParticleSystem()
{
	// TODO: Add your control notification handler code here
}


void CParticleSystemControlDlg::OnBnClickedTryParticleSystem()
{
	CMainFrame *MainFrame = (CMainFrame *)((CParticleSystemApp*)AfxGetApp())->GetMainWnd();
	CEngine *Engine = ((CParticleSystemApp*)AfxGetApp())->m_Engine;
	Ogre::Root *Root = Engine->GetRoot();
	Ogre::SceneManager *SceneManager = Root->getSceneManager("ParticleSystem");

	Ogre::SceneNode *SceneNode = SceneManager->getRootSceneNode()->createChildSceneNode();
	SceneNode->attachObject(m_ParticleSystem);
	SceneNode->setPosition(0, 10, 0);
	m_ParticleSystem->setVisible(true);
	Root->renderOneFrame();
	MainFrame->GetActiveView()->SetTimer(1,1,0);
}


void CParticleSystemControlDlg::OnAffectorsAdddirectionrandomiseraffector()
{
	CDirectionRandomiserDlg DirectionRandomiserDlg;

	if (IDOK == DirectionRandomiserDlg.DoModal())
	{
		m_ParticleSystemTree.InsertItem(DirectionRandomiserDlg.m_AffectorName, m_AffectorsItem);
		Ogre::ParticleAffector *Direction = m_ParticleSystem->addAffector("DirectionRandomiser");
		Direction->setParameter("randomness", "60");
	}
}