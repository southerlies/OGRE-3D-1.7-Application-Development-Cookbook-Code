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

// OrbitViewView.cpp : implementation of the COrbitViewView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "OrbitView.h"
#endif

#include "OrbitViewDoc.h"
#include "OrbitViewView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COrbitViewView

IMPLEMENT_DYNCREATE(COrbitViewView, CView)

BEGIN_MESSAGE_MAP(COrbitViewView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_COMMAND(ID_ORBIT, &COrbitViewView::OnOrbit)
END_MESSAGE_MAP()

// COrbitViewView construction/destruction

COrbitViewView::COrbitViewView()
{
	m_First = true;
	m_Orbit = false;
	m_MouseNavigation = false;
	m_Camera = NULL;
}

COrbitViewView::~COrbitViewView()
{
}

BOOL COrbitViewView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// COrbitViewView drawing

void COrbitViewView::OnDraw(CDC* /*pDC*/)
{
	COrbitViewDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void COrbitViewView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void COrbitViewView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// COrbitViewView diagnostics

#ifdef _DEBUG
void COrbitViewView::AssertValid() const
{
	CView::AssertValid();
}

void COrbitViewView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COrbitViewDoc* COrbitViewView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COrbitViewDoc)));
	return (COrbitViewDoc*)m_pDocument;
}
#endif //_DEBUG


// COrbitViewView message handlers


void COrbitViewView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_MousePosition = point;
	m_MouseNavigation = true;

	CView::OnLButtonDown(nFlags, point);
}


void COrbitViewView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_MouseNavigation = false;
	CView::OnLButtonUp(nFlags, point);
}


void COrbitViewView::OnMouseMove(UINT nFlags, CPoint point)
{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CEngine * Engine = ((COrbitViewApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
	  return;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
	  return;

	if (m_MouseNavigation)
	{
	if (m_Orbit)
	{
		Ogre::SceneNode* CameraNode = m_SceneManager->getSceneNode("CameraNode");
		CameraMove[1] = m_MousePosition.y - point.y;
		m_Camera->moveRelative(CameraMove);
		CameraNode->yaw(Ogre::Radian(0.01 * (m_MousePosition.x - point.x)));
	}
	else
	{
		CameraMove[0] = -(m_MousePosition.x - point.x);
		CameraMove[1] = m_MousePosition.y - point.y;
		m_Camera->moveRelative(CameraMove);
	}

		m_MousePosition = point;
		Root->renderOneFrame();
	}

	CView::OnMouseMove(nFlags, point);
}


BOOL COrbitViewView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[2] = 0.1 * zDelta;

	CEngine * Engine = ((COrbitViewApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return false;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return false;
	
	m_Camera->moveRelative(CameraMove);
	
	Root->renderOneFrame();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void COrbitViewView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	COrbitViewDoc *Document = GetDocument();
	CEngine *Engine = ((COrbitViewApp*)AfxGetApp())->m_Engine;
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


void COrbitViewView::OnTimer(UINT_PTR nIDEvent)
{
	m_WalkAnimation->addTime(0.001);

	CEngine *Engine = ((COrbitViewApp*)AfxGetApp())->m_Engine;
	Ogre::Root *Root = Engine->GetRoot();
    Root->renderOneFrame();

	CView::OnTimer(nIDEvent);
}

void COrbitViewView::EngineSetup(void)
{
	Ogre::Root *Root = ((COrbitViewApp*)AfxGetApp())->m_Engine->GetRoot();

	m_SceneManager = Root->createSceneManager(Ogre::ST_GENERIC, "OrbitViewt");

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
		m_RenderWindow = Root->createRenderWindow("OrbitViewt", rect.Width(), rect.Height(), false, &parms);
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
	CameraNode = m_SceneManager->getRootSceneNode()->createChildSceneNode("CameraNode", Ogre::Vector3::ZERO);
	CameraNode->attachObject(m_Camera);

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



void COrbitViewView::OnOrbit()
{
	int ViewIndex = CMFCRibbonGallery::GetLastSelectedItem(ID_ORBIT);
		
	switch (ViewIndex)
	{
		case 0:

		m_Orbit = true;

		break;
	}
}
