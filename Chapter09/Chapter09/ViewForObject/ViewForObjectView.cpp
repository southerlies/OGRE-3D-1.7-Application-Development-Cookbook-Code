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

// ViewForObjectView.cpp : implementation of the CViewForObjectView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ViewForObject.h"
#endif

#include "ViewForObjectDoc.h"
#include "ViewForObjectView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CViewForObjectView

IMPLEMENT_DYNCREATE(CViewForObjectView, CView)

BEGIN_MESSAGE_MAP(CViewForObjectView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_COMMAND(ID_SELECT, &CViewForObjectView::OnSelect)
END_MESSAGE_MAP()

// CViewForObjectView construction/destruction

CViewForObjectView::CViewForObjectView()
{
	m_SelectionMode = false;
	m_First = true;
	m_MouseNavigation = false;
}

CViewForObjectView::~CViewForObjectView()
{
}

BOOL CViewForObjectView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CViewForObjectView drawing

void CViewForObjectView::OnDraw(CDC* /*pDC*/)
{
	CViewForObjectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CViewForObjectView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CViewForObjectView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CViewForObjectView diagnostics

#ifdef _DEBUG
void CViewForObjectView::AssertValid() const
{
	CView::AssertValid();
}

void CViewForObjectView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CViewForObjectDoc* CViewForObjectView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CViewForObjectDoc)));
	return (CViewForObjectDoc*)m_pDocument;
}
#endif //_DEBUG


// CViewForObjectView message handlers


void CViewForObjectView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_MousePosition = point;
	
	if (m_SelectionMode)
	{
		Ogre::Entity *SelectedEntity = GetEntity(point);

		if (SelectedEntity != NULL)
		{
			Ogre::Vector3 Center = SelectedEntity->getParentNode()->getPosition();
			Ogre::Real Radius = SelectedEntity->getBoundingRadius();
			m_Camera->setPosition(Ogre::Vector3(Center.x, Center.y + 0.5 * Radius, Center.z + Radius));
			m_Camera->lookAt(Ogre::Vector3(Center.x, Center.y + 0.5 * Radius, Center.z));
			CEngine * Engine = ((CViewForObjectApp*)AfxGetApp())->m_Engine;
			Ogre::Root *Root = Engine->GetRoot();
			Root->renderOneFrame();
		}
	}
	else
	{
		m_MouseNavigation = true;
	}

	CView::OnLButtonDown(nFlags, point);
}


void CViewForObjectView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_MouseNavigation = false;

	CView::OnLButtonUp(nFlags, point);
}


void CViewForObjectView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_MouseNavigation)
{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[0] = -(m_MousePosition.x - point.x);
	CameraMove[1] = m_MousePosition.y - point.y;

	CEngine * Engine = ((CViewForObjectApp*)AfxGetApp())->m_Engine;
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


BOOL CViewForObjectView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[2] = 0.1 * zDelta;

	CEngine * Engine = ((CViewForObjectApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return false;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return false;
	
	m_Camera->moveRelative(CameraMove);
	
	Root->renderOneFrame();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CViewForObjectView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CViewForObjectDoc *Document = GetDocument();
	CEngine *Engine = ((CViewForObjectApp*)AfxGetApp())->m_Engine;
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

	if (m_SelectionMode)
	{
		::SetCursor(::LoadCursor(NULL, IDC_HAND));
	}
	// Do not call CView::OnPaint() for painting messages
}


void CViewForObjectView::OnTimer(UINT_PTR nIDEvent)
{
	m_WalkAnimation->addTime(0.001);

	CEngine *Engine = ((CViewForObjectApp*)AfxGetApp())->m_Engine;
	Ogre::Root *Root = Engine->GetRoot();
    Root->renderOneFrame();

	CView::OnTimer(nIDEvent);
}

void CViewForObjectView::EngineSetup(void)
{
	Ogre::Root *Root = ((CViewForObjectApp*)AfxGetApp())->m_Engine->GetRoot();

	m_SceneManager = Root->createSceneManager(Ogre::ST_GENERIC, "OrbitView");

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
		m_RenderWindow = Root->createRenderWindow("OrbitView", rect.Width(), rect.Height(), false, &parms);
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
		
	Ogre::Entity *RobotEntity;
	Ogre::SceneNode *RobotNode;

	for (int RobotIndex = 0; RobotIndex < 4; RobotIndex++)
	{
		char Number[20];
		itoa(RobotIndex, Number, 10);
		Ogre::String RobotName = "Robot";
		RobotName += Ogre::String(Number);
		RobotEntity = m_SceneManager->createEntity(RobotName, "robot.mesh");
		RobotNode = m_SceneManager->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(RobotIndex * 100, RobotIndex * 100, 0));
		RobotNode->attachObject(RobotEntity);
	}

	m_SceneManager->_updateSceneGraph(m_Camera);
	Ogre::AxisAlignedBox Box = m_SceneManager->getRootSceneNode()->_getWorldAABB();
	Ogre::Vector3 Center = Box.getCenter();
	m_Camera->setPosition(Ogre::Vector3(Center[0], Center[1], Center[2] + 500.0));
	m_Camera->lookAt(Center);

	Root->renderOneFrame();
}



void CViewForObjectView::OnSelect()
{
	m_SelectionMode = true;
}

Ogre::Entity* CViewForObjectView::GetEntity(CPoint point)
{
	Ogre::Camera *Camera = m_SceneManager->getCamera("Camera");
	Ogre::Entity *Entity = NULL;
	Ogre::RaySceneQueryResult Result;

	CRect   rect;
    this->GetClientRect(&rect);

    Ogre::Ray SearchRay = Camera->getCameraToViewportRay(((float)point.x)/((float)rect.Width()), ((float)point.y)/((float)rect.Height()));
	Ogre::RaySceneQuery *ObjectRaySceneQuery = m_SceneManager->createRayQuery(SearchRay);
	ObjectRaySceneQuery->setSortByDistance(true);
	Result = ObjectRaySceneQuery->execute();
				
	Ogre::RaySceneQueryResult::iterator Iterator;
		
	Iterator = Result.begin();
	Ogre::String ObjectName = Iterator->movable->getName();
	Ogre::MovableObject *MovableObject = Iterator->movable;
		
	Entity = static_cast<Ogre::Entity*>(MovableObject);

	return Entity;
}
