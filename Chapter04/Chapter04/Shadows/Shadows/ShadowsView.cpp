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

// ShadowsView.cpp : implementation of the CShadowsView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Shadows.h"
#endif

#include "ShadowsDoc.h"
#include "ShadowsView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CShadowsView

IMPLEMENT_DYNCREATE(CShadowsView, CView)

BEGIN_MESSAGE_MAP(CShadowsView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CShadowsView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_SHADOWSCONTROL_SHADOWSCONTROL, &CShadowsView::OnShadowsControlShadowsControl)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_TIMER()
	ON_COMMAND(ID_HEIGHT, &CShadowsView::OnHeight)
	ON_COMMAND(ID_TIME, &CShadowsView::OnTime)
END_MESSAGE_MAP()

// CShadowsView construction/destruction

CShadowsView::CShadowsView()
{
	m_First = true;
	m_ShadowControlDlg = NULL;
}

CShadowsView::~CShadowsView()
{
}

BOOL CShadowsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CShadowsView drawing

void CShadowsView::OnDraw(CDC* /*pDC*/)
{
	CShadowsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CShadowsView printing


void CShadowsView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CShadowsView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CShadowsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CShadowsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CShadowsView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CShadowsView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CShadowsView diagnostics

#ifdef _DEBUG
void CShadowsView::AssertValid() const
{
	CView::AssertValid();
}

void CShadowsView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CShadowsDoc* CShadowsView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CShadowsDoc)));
	return (CShadowsDoc*)m_pDocument;
}
#endif //_DEBUG


// CShadowsView message handlers


void CShadowsView::OnShadowsControlShadowsControl()
{
	if (m_ShadowControlDlg == NULL)
	{
		m_ShadowControlDlg = new CShadowControlDlg();
		m_ShadowControlDlg->Create(IDD_SHADOW_CONTROL);
	}

	m_ShadowControlDlg->ShowWindow(SW_SHOW);
}


void CShadowsView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CView::OnPaint() for painting messages
	CShadowsDoc *Document = GetDocument();
	CEngine *Engine = ((CShadowsApp*)AfxGetApp())->m_Engine;
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


void CShadowsView::EngineSetup(void)
{
	Ogre::Root *Root = ((CShadowsApp*)AfxGetApp())->m_Engine->GetRoot();
	Ogre::Light *Sun = NULL;
	Ogre::SceneManager *SceneManager = NULL;

	SceneManager = Root->createSceneManager(Ogre::ST_GENERIC, "Shadows");
	SceneManager->setShadowColour(Ogre::ColourValue(0.5, 0.5, 0.5));
	SceneManager->setAmbientLight(Ogre::ColourValue::Black);
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

	Ogre::RenderTarget *RenderWindow = Root->getRenderTarget("Shadows");

	if (RenderWindow == NULL)
	{
	try
	{
		m_RenderWindow = Root->createRenderWindow("Shadows", rect.Width(), rect.Height(), false, &parms);
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
	m_Camera->setCastShadows(true);
	m_Camera->setUseRenderingDistance(true);
	m_Camera->setPosition(Ogre::Vector3(3800.0, 950.0, 1900.0));
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

	Ogre::Plane Ground(Ogre::Vector3::UNIT_Y, 0);
	Ogre::MeshPtr GroundMesh = Ogre::MeshManager::getSingleton().createPlane("Ground",
            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ground,
            10000, 10000,20,20,true,1,5,5, Ogre::Vector3::UNIT_Z);
	
	Ogre::Entity *GroundEntity = SceneManager->createEntity("Ground", "Ground");
	Ogre::SceneNode *GroundNode = SceneManager->getRootSceneNode()->createChildSceneNode("Ground");
	GroundNode->attachObject(GroundEntity);
	GroundEntity->setCastShadows(false);

	Ogre::Vector3 InsertionPoint;

	Ogre::Entity *HouseEntity = SceneManager->createEntity("House", "tudorhouse.mesh");
	Ogre::AxisAlignedBox HouseBox = HouseEntity->getBoundingBox();
	InsertionPoint = - HouseBox.getCorner(Ogre::AxisAlignedBox::NEAR_LEFT_BOTTOM);
	Ogre::SceneNode *HouseNode = SceneManager->getRootSceneNode()->createChildSceneNode(InsertionPoint);
	HouseNode->attachObject(HouseEntity);
	HouseEntity->setCastShadows(false);

	Ogre::Entity *BoxEntity = SceneManager->createEntity("Box", Ogre::SceneManager::PrefabType::PT_CUBE);
	BoxEntity->setMaterialName("Examples/BeachStones");
	Ogre::AxisAlignedBox Box = BoxEntity->getBoundingBox();
	Ogre::SceneNode *BoxNode = SceneManager->getRootSceneNode()->createChildSceneNode("Box");
	BoxNode->attachObject(BoxEntity);
	BoxNode->setScale(Ogre::Vector3(5, 20, 5));
	BoxNode->setPosition(Ogre::Vector3(-1000, 0, 1000));
	BoxEntity->setCastShadows(true);

	SceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
	Sun = SceneManager->createLight("Sun"); 
	Sun->setType(Ogre::Light::LT_POINT); 
	Sun->setPosition(2500 * Ogre::Math::Cos(0), 1000, 2500 * Ogre::Math::Sin(0));
	Sun->setDiffuseColour(0.35, 0.35, 0);
	Sun->setSpecularColour(0.9, 0.9, 0);  
	Sun->setVisible(true);

	Ogre::Vector3 Center = HouseBox.getCenter();
	m_Camera->lookAt(Center);
}


void CShadowsView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnLButtonDown(nFlags, point);
}


void CShadowsView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnLButtonUp(nFlags, point);
}


void CShadowsView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnMouseMove(nFlags, point);
}


BOOL CShadowsView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[2] = 0.1 * zDelta;

	CEngine * Engine = ((CShadowsApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return false;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return false;
	m_Camera->moveRelative(CameraMove);

	Root->renderOneFrame();
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CShadowsView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnTimer(nIDEvent);
}


void CShadowsView::OnHeight()
{
	CMainFrame *MainFrame = (CMainFrame *)((CShadowsApp*)AfxGetApp())->GetMainWnd();
	CMFCRibbonBar* RibbonBar = MainFrame->GetRibbonBar();

	CMFCRibbonSlider* Slider = DYNAMIC_DOWNCAST(CMFCRibbonSlider, RibbonBar->FindByID(ID_HEIGHT));  

	CEngine *Engine = ((CShadowsApp*)AfxGetApp())->m_Engine;
	Ogre::Root *Root = Engine->GetRoot();
	Ogre::SceneManager *SceneManager = Root->getSceneManager("Shadows");
	Ogre::SceneNode *BoxNode = SceneManager->getSceneNode("Box");
	BoxNode->setScale(5, Slider->GetPos(), 5);

	if (Root != NULL)
    {
       Root->renderOneFrame();
	}
}


void CShadowsView::OnTime()
{
	CMainFrame *MainFrame = (CMainFrame *)((CShadowsApp*)AfxGetApp())->GetMainWnd();
	CMFCRibbonBar* RibbonBar = MainFrame->GetRibbonBar();

	CMFCRibbonSlider* Slider = DYNAMIC_DOWNCAST(CMFCRibbonSlider, RibbonBar->FindByID(ID_TIME));  

	Ogre::Radian Angle = Ogre::Radian(Ogre::Math::TWO_PI * (double)Slider->GetPos() / 24);

	CEngine *Engine = ((CShadowsApp*)AfxGetApp())->m_Engine;
	Ogre::Root *Root = Engine->GetRoot();
	Ogre::SceneManager *SceneManager = Root->getSceneManager("Shadows");
	Ogre::Light *Sun = SceneManager->getLight("Sun");
	Sun->setPosition(2500 * Ogre::Math::Cos(Angle), 1000, 2500 * Ogre::Math::Sin(Angle));
	
	if (Root != NULL)
    {
       Root->renderOneFrame();
	}
}
