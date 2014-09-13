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

// ZoomManagerView.cpp : implementation of the CZoomManagerView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ZoomManager.h"
#endif

#include "ZoomManagerDoc.h"
#include "ZoomManagerView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CZoomManagerView

IMPLEMENT_DYNCREATE(CZoomManagerView, CView)

BEGIN_MESSAGE_MAP(CZoomManagerView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_COMMAND(ID_ZOOM, &CZoomManagerView::OnZoom)
	ON_COMMAND(ID_EXTENTS, &CZoomManagerView::OnExtents)
	ON_COMMAND(ID_WINDOW, &CZoomManagerView::OnWindow)
END_MESSAGE_MAP()

// CZoomManagerView construction/destruction

CZoomManagerView::CZoomManagerView()
{
	m_First = true;
	m_SelectMode = false;
	m_zoomFactor = 1.0;
}

CZoomManagerView::~CZoomManagerView()
{
}

BOOL CZoomManagerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CZoomManagerView drawing

void CZoomManagerView::OnDraw(CDC* /*pDC*/)
{
	CZoomManagerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CZoomManagerView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CZoomManagerView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CZoomManagerView diagnostics

#ifdef _DEBUG
void CZoomManagerView::AssertValid() const
{
	CView::AssertValid();
}

void CZoomManagerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CZoomManagerDoc* CZoomManagerView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CZoomManagerDoc)));
	return (CZoomManagerDoc*)m_pDocument;
}
#endif //_DEBUG


// CZoomManagerView message handlers


void CZoomManagerView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_SelectMode)
	{
		m_Start = point;
		m_rubberBand.SetRect(m_Start, m_Start);
		CClientDC dc(this);
		dc.DrawFocusRect(m_rubberBand);
	}

	CView::OnLButtonDown(nFlags, point);
}


void CZoomManagerView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_SelectMode) 
	{
		m_SelectMode = false;
		Invalidate(FALSE);

		CRect rect;
		GetClientRect(&rect);
		Ogre::Vector3 Position = m_Camera->getPosition();
		float ratio = std::min(m_rubberBand.Width()/rect.Width(), m_rubberBand.Height()/rect.Height());
		Position[2] *= ratio;
		m_Camera->setPosition(Position);
		CEngine * Engine = ((CZoomManagerApp*)AfxGetApp())->m_Engine;
		Ogre::Root *Root = Engine->GetRoot();
		Root->renderOneFrame();
	}

	CView::OnLButtonUp(nFlags, point);
}


void CZoomManagerView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_SelectMode) 
	{
		m_rubberBand.SetRect(m_Start, point);
		CClientDC dc(this);
		dc.DrawFocusRect(m_rubberBand);
		Invalidate(FALSE);
	}
	
	CView::OnMouseMove(nFlags, point);
}


BOOL CZoomManagerView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[2] = 0.1 * zDelta;

	CEngine * Engine = ((CZoomManagerApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return false;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return false;
	
	m_Camera->moveRelative(CameraMove);
	
	Root->renderOneFrame();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CZoomManagerView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CZoomManagerDoc *Document = GetDocument();
	CEngine *Engine = ((CZoomManagerApp*)AfxGetApp())->m_Engine;
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

	// Do not call CView::OnPaint() for painting messages
}


void CZoomManagerView::OnTimer(UINT_PTR nIDEvent)
{
	m_WalkAnimation->addTime(0.001);

	CEngine *Engine = ((CZoomManagerApp*)AfxGetApp())->m_Engine;
	Ogre::Root *Root = Engine->GetRoot();
    Root->renderOneFrame();

	CView::OnTimer(nIDEvent);
}

void CZoomManagerView::EngineSetup(void)
{
	Ogre::Root *Root = ((CZoomManagerApp*)AfxGetApp())->m_Engine->GetRoot();

	m_SceneManager = Root->createSceneManager(Ogre::ST_GENERIC, "ZoomManager");

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

	try
	{
		m_RenderWindow = Root->createRenderWindow("ZoomManager", rect.Width(), rect.Height(), false, &parms);
	}
    catch(...)
	{
		MessageBox("Cannot initialize\nCheck that graphic-card driver is up-to-date", "Initialize Render System", MB_OK | MB_ICONSTOP);
		exit(EXIT_SUCCESS);
	}
// Load resources
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    // Create the camera
    m_Camera = m_SceneManager->createCamera("Camera");
    m_Camera->setNearClipDistance(0.5);
	m_Camera->setFarClipDistance(5000); 
	m_Camera->setCastShadows(false);
	m_Camera->setUseRenderingDistance(true);

	Ogre::SceneNode *CameraNode = NULL;
	CameraNode = m_SceneManager->getRootSceneNode()->createChildSceneNode("CameraNode");

	Ogre::Viewport* Viewport = NULL;
	
	if (0 == m_RenderWindow->getNumViewports())
	{
		Viewport = m_RenderWindow->addViewport(m_Camera);
		Viewport->setBackgroundColour(Ogre::ColourValue(0.8f, 1.0f, 0.8f));
	}

    // Alter the camera aspect ratio to match the viewport
    m_Camera->setAspectRatio(Ogre::Real(rect.Width()) / Ogre::Real(rect.Height()));
		
	Ogre::Entity *RobotEntity = m_SceneManager->createEntity("Robot", "robot.mesh");
	Ogre::SceneNode *RobotNode = m_SceneManager->getRootSceneNode()->createChildSceneNode("Robot");
	RobotNode->attachObject(RobotEntity);

	Ogre::AxisAlignedBox Box = RobotEntity->getBoundingBox();
	Ogre::Vector3 Center = Box.getCenter();
	m_Camera->setPosition(Ogre::Vector3(Center[0], Center[1], Center[2] + 500.0));
	m_Camera->lookAt(Center);

	Root->renderOneFrame();
}



void CZoomManagerView::OnZoom()
{
	CMainFrame *MainFrame = (CMainFrame *)((CZoomManagerApp*)AfxGetApp())->GetMainWnd();
	CMFCRibbonBar* RibbonBar = MainFrame->GetRibbonBar();

	CMFCRibbonSlider* ZoomSlider = DYNAMIC_DOWNCAST(CMFCRibbonSlider, RibbonBar->FindByID(ID_ZOOM));  

	CEngine *Engine = ((CZoomManagerApp*)AfxGetApp())->m_Engine;
	Ogre::Root *Root = Engine->GetRoot();
	int Pos = ZoomSlider->GetPos();
	int Min = ZoomSlider->GetRangeMin();
	int Max = ZoomSlider->GetRangeMax();

	int Middle = (Max + Min) / 2; 
	ZoomSlider->SetPos(Middle);
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	Pos = Pos - Middle;
	CameraMove[2] = 0.1 * Pos;
	m_Camera->moveRelative(CameraMove);
	
	if (Root != NULL)
    {
       Root->renderOneFrame();
	}
}


void CZoomManagerView::OnExtents()
{
	Ogre::Radian fieldOfView = m_Camera->getFOVy();
	CEngine *Engine = ((CZoomManagerApp*)AfxGetApp())->m_Engine;
	Ogre::Root *Root = Engine->GetRoot();
	Ogre::Entity* Robot = m_SceneManager->getEntity("Robot");
	Ogre::AxisAlignedBox Box = Robot->getBoundingBox();
		
	Ogre::Vector3 Center = Box.getCenter();
	Ogre::Vector3 Size = Box.getSize();
	float Width = Size[0];
	float Length = Size[1];
	float Height = 0.5 * std::max(Width, Length) / Ogre::Math::Sin(fieldOfView / 2.0);

	m_Camera->setPosition(Center[0], Center[1], Height);
	m_Camera->lookAt(Center);
	Root->renderOneFrame();
}


void CZoomManagerView::OnWindow()
{
	m_SelectMode = true;
}

void CZoomManagerView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
}
