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

// ViewManagerView.cpp : implementation of the CViewManagerView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ViewManager.h"
#endif

#include "ViewManagerDoc.h"
#include "ViewManagerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CViewManagerView

IMPLEMENT_DYNCREATE(CViewManagerView, CView)

BEGIN_MESSAGE_MAP(CViewManagerView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_COMMAND(ID_3DVIEWS, &CViewManagerView::On3dViews)
	ON_COMMAND(ID_START_ANIMATION, &CViewManagerView::OnStartAnimation)
	ON_COMMAND(ID_STOP_ANIMATION, &CViewManagerView::OnStopAnimation)
END_MESSAGE_MAP()

// CViewManagerView construction/destruction

CViewManagerView::CViewManagerView()
{
	m_First = true;
	m_MouseNavigation = false;
}

CViewManagerView::~CViewManagerView()
{
}

BOOL CViewManagerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CViewManagerView drawing

void CViewManagerView::OnDraw(CDC* /*pDC*/)
{
	CViewManagerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CViewManagerView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CViewManagerView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CViewManagerView diagnostics

#ifdef _DEBUG
void CViewManagerView::AssertValid() const
{
	CView::AssertValid();
}

void CViewManagerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CViewManagerDoc* CViewManagerView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CViewManagerDoc)));
	return (CViewManagerDoc*)m_pDocument;
}
#endif //_DEBUG


// CViewManagerView message handlers


void CViewManagerView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_MousePosition = point;
	m_MouseNavigation = true;

	CView::OnLButtonDown(nFlags, point);
}


void CViewManagerView::OnLButtonUp(UINT nFlags, CPoint point)
{
m_MouseNavigation = false;

	CView::OnLButtonUp(nFlags, point);
}


void CViewManagerView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_MouseNavigation)
{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[0] = -(m_MousePosition.x - point.x);
	CameraMove[1] = m_MousePosition.y - point.y;

	CEngine * Engine = ((CViewManagerApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return;
	Ogre::Vector3 OldPosition = m_Camera->getPosition();
	m_Camera->moveRelative(CameraMove);

	m_MousePosition = point;

	Root->renderOneFrame();
	}
	CView::OnMouseMove(nFlags, point);
}


BOOL CViewManagerView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[2] = 0.1 * zDelta;

	CEngine * Engine = ((CViewManagerApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return false;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return false;
	
	m_Camera->moveRelative(CameraMove);
	
	Root->renderOneFrame();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CViewManagerView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CViewManagerDoc *Document = GetDocument();
	CEngine *Engine = ((CViewManagerApp*)AfxGetApp())->m_Engine;
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


void CViewManagerView::OnTimer(UINT_PTR nIDEvent)
{
	m_WalkAnimation->addTime(0.001);

	CEngine *Engine = ((CViewManagerApp*)AfxGetApp())->m_Engine;
	Ogre::Root *Root = Engine->GetRoot();
    Root->renderOneFrame();

	CView::OnTimer(nIDEvent);
}

void CViewManagerView::EngineSetup(void)
{
	Ogre::Root *Root = ((CViewManagerApp*)AfxGetApp())->m_Engine->GetRoot();

	m_SceneManager = Root->createSceneManager(Ogre::ST_GENERIC, "Nearest");

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
		m_RenderWindow = Root->createRenderWindow("Nearest", rect.Width(), rect.Height(), false, &parms);
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
	m_Camera->setPosition(Ogre::Vector3(200.0, 50.0, 100.0));

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
		
	Ogre::Entity *CubeEntity = m_SceneManager->createEntity("Cube", "cube.mesh");
	CubeEntity->setMaterialName("Examples/SceneCubeMap1");
	Ogre::SceneNode *CubeNode = m_SceneManager->getRootSceneNode()->createChildSceneNode("Cube");
	CubeNode->attachObject(CubeEntity);

	Ogre::AxisAlignedBox Box = CubeEntity->getBoundingBox();
	Ogre::Vector3 Center = Box.getCenter();
	m_Camera->lookAt(Center);

	Root->renderOneFrame();
}

void CViewManagerView::On3dViews()
{
	int ViewIndex = CMFCRibbonGallery::GetLastSelectedItem(ID_3DVIEWS);
	CEngine *Engine = ((CViewManagerApp*)AfxGetApp())->m_Engine;
	Ogre::Root *Root = Engine->GetRoot();
	m_SceneManager->_updateSceneGraph(m_Camera);
	Ogre::SceneNode *CubeNode = m_SceneManager->getSceneNode("Cube");

	Ogre::AxisAlignedBox Box = m_SceneManager->getRootSceneNode()->_getWorldAABB();
	Ogre::Vector3 Center = Box.getCenter();
	Ogre::Vector3 Position;
	Ogre::Vector3 Destination;

	switch (ViewIndex)
	{
		case 0: //top

			Position = Center;
			Position.y += 4.0 * Box.getSize().y;
			Destination = Center;

		break;
		case 1://bottom
			
			Position = Center;
			Position.y -= 4.0 * Box.getSize().y;
			Destination = Center;

		break;
		case 2:

			Position = Center;
			Position.x += 4.0 * Box.getSize().x;
			Destination = Center;

		break;
		case 3:

			Position = Center;
			Position.x -= 4.0 * Box.getSize().x;
			Destination = Center;

		break;
		case 4:

			Position = Center;
			Position.z -= 4.0 * Box.getSize().z;
			Destination = Center;

		break;
		case 5:

			Position = Center;
			Position.z += 4.0 * Box.getSize().z;
			Destination = Center;

		break;

		case 9:

			CubeNode->roll(Ogre::Radian(-atan(sin(Ogre::Math::PI/4.0))));
			CubeNode->yaw(Ogre::Radian(Ogre::Math::PI/4.0));
			Destination = Center;
			Position = m_Camera->getPosition();

		break;
	}

	m_Camera->setPosition(Position);
	m_Camera->lookAt(Destination);
	Root->renderOneFrame();
}




void CViewManagerView::OnStartAnimation()
{
	SetTimer(1, 1, 0);
}


void CViewManagerView::OnStopAnimation()
{
	KillTimer(1);
}
