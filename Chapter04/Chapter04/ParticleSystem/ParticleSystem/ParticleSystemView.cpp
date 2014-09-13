// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://msdn.microsoft.com/officeui.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// ParticleSystemView.cpp : implementation of the CParticleSystemView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ParticleSystem.h"
#endif

#include "ParticleSystemDoc.h"
#include "ParticleSystemView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CParticleSystemView

IMPLEMENT_DYNCREATE(CParticleSystemView, CView)

BEGIN_MESSAGE_MAP(CParticleSystemView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CParticleSystemView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_PARTICLECONTROL_PARTICLECONTROL, &CParticleSystemView::OnParticleControlParticleControl)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CParticleSystemView construction/destruction

CParticleSystemView::CParticleSystemView()
{
	m_First = true;
	m_ParticleSystemControlDlg = NULL;
}

CParticleSystemView::~CParticleSystemView()
{
}

BOOL CParticleSystemView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CParticleSystemView drawing

void CParticleSystemView::OnDraw(CDC* /*pDC*/)
{
	CParticleSystemDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CParticleSystemView printing


void CParticleSystemView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CParticleSystemView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CParticleSystemView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CParticleSystemView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CParticleSystemView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CParticleSystemView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CParticleSystemView diagnostics

#ifdef _DEBUG
void CParticleSystemView::AssertValid() const
{
	CView::AssertValid();
}

void CParticleSystemView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CParticleSystemDoc* CParticleSystemView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CParticleSystemDoc)));
	return (CParticleSystemDoc*)m_pDocument;
}
#endif //_DEBUG


// CParticleSystemView message handlers


void CParticleSystemView::OnParticleControlParticleControl()
{
	if (m_ParticleSystemControlDlg == NULL)
	{
		m_ParticleSystemControlDlg = new CParticleSystemControlDlg();
		m_ParticleSystemControlDlg->Create(IDD_PARTICLE_CONTROL);
	}

	m_ParticleSystemControlDlg->ShowWindow(SW_SHOW);
}


void CParticleSystemView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CView::OnPaint() for painting messages
	CParticleSystemDoc *Document = GetDocument();
	CEngine *Engine = ((CParticleSystemApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
	  return;

	Ogre::Root *Root = Engine->GetRoot();

	if (m_First && Root != NULL)
    {
        m_First = false;
        EngineSetup();
    }

	if (Root != NULL)
    {
       Root->renderOneFrame();
	}
}


void CParticleSystemView::EngineSetup(void)
{
	Ogre::Root *Root = ((CParticleSystemApp*)AfxGetApp())->m_Engine->GetRoot();

	Ogre::SceneManager *SceneManager = NULL;

	SceneManager = Root->createSceneManager(Ogre::ST_GENERIC, "ParticleSystem");

    //
    // Create a render window
    // This window should be the current ChildView window using the externalWindowHandle
    // value pair option.
    //

    Ogre::NameValuePairList parms;
    parms["externalWindowHandle"] = Ogre::StringConverter::toString((long)m_hWnd);
    parms["vsync"] = "true";

	CRect   rect;
    GetClientRect(&rect);

	Ogre::RenderTarget *RenderWindow = Root->getRenderTarget("ParticleSystem");

	if (RenderWindow == NULL)
	{
	try
	{
		m_RenderWindow = Root->createRenderWindow("ParticleSystem", rect.Width(), rect.Height(), false, &parms);
	}
    catch(...)
	{
		MessageBox("Cannot initialize\nCheck that graphic-card driver is up-to-date", "Initialize Render System", MB_OK | MB_ICONSTOP);
		exit(EXIT_SUCCESS);
	}
	}
// Load resources
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    // Create the camera
    m_Camera = SceneManager->createCamera("Camera");
    m_Camera->setNearClipDistance(0.5);
	m_Camera->setFarClipDistance(5000); 
	m_Camera->setCastShadows(false);
	m_Camera->setUseRenderingDistance(true);
	m_Camera->setPosition(Ogre::Vector3(200.0, 50.0, 100.0));
	Ogre::SceneNode *CameraNode = NULL;
	CameraNode = SceneManager->getRootSceneNode()->createChildSceneNode("CameraNode");

	Ogre::Viewport* Viewport = NULL;
	
	if (0 == m_RenderWindow->getNumViewports())
	{
		Viewport = m_RenderWindow->addViewport(m_Camera);
		Viewport->setBackgroundColour(Ogre::ColourValue(0.8f, 1.0f, 0.8f));
	}

    // Alter the camera aspect ratio to match the viewport
    m_Camera->setAspectRatio(Ogre::Real(rect.Width()) / Ogre::Real(rect.Height()));
		
	Ogre::Entity *RobotEntity = SceneManager->createEntity("Robot", "robot.mesh");
	Ogre::SceneNode *RobotNode = SceneManager->getRootSceneNode()->createChildSceneNode();
	RobotNode->attachObject(RobotEntity);

	Ogre::AxisAlignedBox Box = RobotEntity->getBoundingBox();
	Ogre::Vector3 Center = Box.getCenter();
	m_Camera->lookAt(Center);
}


void CParticleSystemView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnLButtonDown(nFlags, point);
}


void CParticleSystemView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnLButtonUp(nFlags, point);
}


void CParticleSystemView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnMouseMove(nFlags, point);
}


BOOL CParticleSystemView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[2] = 0.1 * zDelta;

	CEngine * Engine = ((CParticleSystemApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return false;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return false;
	m_Camera->moveRelative(CameraMove);

	Root->renderOneFrame();
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CParticleSystemView::OnTimer(UINT_PTR nIDEvent)
{
	CEngine * Engine = ((CParticleSystemApp*)AfxGetApp())->m_Engine;
	Ogre::Root *Root = Engine->GetRoot();
	Root->renderOneFrame();

	CView::OnTimer(nIDEvent);
}
