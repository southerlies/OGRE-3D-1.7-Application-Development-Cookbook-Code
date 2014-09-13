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

// VisibilityView.cpp : implementation of the CVisibilityView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Visibility.h"
#endif

#include "VisibilityDoc.h"
#include "VisibilityView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVisibilityView

IMPLEMENT_DYNCREATE(CVisibilityView, CView)

BEGIN_MESSAGE_MAP(CVisibilityView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_TIMER()
	ON_COMMAND(ID_CHECK, &CVisibilityView::OnCheckVisibility)
END_MESSAGE_MAP()

// CVisibilityView construction/destruction

CVisibilityView::CVisibilityView()
{
	m_IsWallExists = false;
	m_IsRotated = true;
	m_First = true;
	m_MouseNavigation = false;
}

CVisibilityView::~CVisibilityView()
{
}

BOOL CVisibilityView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CVisibilityView drawing

void CVisibilityView::OnDraw(CDC* /*pDC*/)
{
	CVisibilityDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CVisibilityView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CVisibilityView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CVisibilityView diagnostics

#ifdef _DEBUG
void CVisibilityView::AssertValid() const
{
	CView::AssertValid();
}

void CVisibilityView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CVisibilityDoc* CVisibilityView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVisibilityDoc)));
	return (CVisibilityDoc*)m_pDocument;
}
#endif //_DEBUG


// CVisibilityView message handlers


void CVisibilityView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_MousePosition = point;
	m_MouseNavigation = true;

	CView::OnLButtonDown(nFlags, point);
}


void CVisibilityView::OnLButtonUp(UINT nFlags, CPoint point)
{
m_MouseNavigation = false;

	CView::OnLButtonUp(nFlags, point);
}


void CVisibilityView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CVisibilityDoc *Document = GetDocument();
	CEngine *Engine = ((CVisibilityApp*)AfxGetApp())->m_Engine;
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
	// Do not call CView::OnPaint() for painting messages}
}

void CVisibilityView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_MouseNavigation)
{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[0] = -(m_MousePosition.x - point.x);
	CameraMove[1] = m_MousePosition.y - point.y;

	CEngine * Engine = ((CVisibilityApp*)AfxGetApp())->m_Engine;
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


BOOL CVisibilityView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[2] = 0.1 * zDelta;

	CEngine * Engine = ((CVisibilityApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return false;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return false;
	
	m_Camera->moveRelative(CameraMove);
	
	Root->renderOneFrame();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CVisibilityView::OnTimer(UINT_PTR nIDEvent)
{
	m_WalkAnimation->addTime(0.001);

	CEngine *Engine = ((CVisibilityApp*)AfxGetApp())->m_Engine;
	Ogre::Root *Root = Engine->GetRoot();
    Root->renderOneFrame();
	CView::OnTimer(nIDEvent);
}

#define COLLIDABLE 1
void CVisibilityView::EngineSetup(void)
{
	Ogre::Root *Root = ((CVisibilityApp*)AfxGetApp())->m_Engine->GetRoot();

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
		
	Ogre::Entity *RobotEntity1;
	Ogre::Entity *RobotEntity2;

	Ogre::SceneNode *RobotNode1;
	Ogre::SceneNode *RobotNode2;

	Ogre::Entity *WallEntity;
	Ogre::SceneNode *WallNode;
		
	Ogre::Quaternion Quaternion(Ogre::Quaternion::IDENTITY);

	RobotEntity1 = m_SceneManager->createEntity("Robot1", "robot.mesh");
	RobotEntity1->setQueryFlags(COLLIDABLE);
	RobotNode1 = m_SceneManager->getRootSceneNode()->createChildSceneNode("Robot1",Ogre::Vector3::ZERO, Ogre::Quaternion::IDENTITY);
	RobotNode1->attachObject(RobotEntity1);

	RobotEntity2 = m_SceneManager->createEntity("Robot2", "robot.mesh");
	RobotEntity2->setQueryFlags(0);

	Quaternion.FromAngleAxis(Ogre::Radian(Ogre::Math::PI), Ogre::Vector3::NEGATIVE_UNIT_Y);
	RobotNode2 = m_SceneManager->getRootSceneNode()->createChildSceneNode("Robot2", Ogre::Vector3(200.0, 0.0, 0.0), Quaternion);
	RobotNode2->attachObject(RobotEntity2);

	Ogre::Real Height = 0.75 * RobotEntity1->getBoundingRadius();
	Ogre::Vector3 Center1 = RobotNode1->getPosition();
	Ogre::Vector3 Center2 = RobotNode2->getPosition();

	Center1[1] += Height;
	Center2[1] += Height;

	Ogre::ManualObject *Ray = m_SceneManager->createManualObject("Ray");
	Ray->begin("BumpyMetal", Ogre::RenderOperation::OT_LINE_LIST); 
	Ray->position(Center1); 
	Ray->position(Center2); 
	Ray->end(); 
	Ray->setVisible(false);
	Ray->setQueryFlags(0);
			
	RobotNode2->attachObject(Ray);

	WallEntity = m_SceneManager->createEntity("Wall", Ogre::SceneManager::PrefabType::PT_SPHERE);
	WallNode = m_SceneManager->getRootSceneNode()->createChildSceneNode("Wall", 0.5 * (Center1 + Center2), Quaternion);
	WallNode->attachObject(WallEntity);
	WallNode->translate(1000,0,0);
	WallEntity->setQueryFlags(COLLIDABLE);
	WallEntity->setVisible(false);

	m_Camera->lookAt(0.5 * (Center1 + Center2));

	Root->renderOneFrame();
}



void CVisibilityView::OnCheckVisibility()
{
	int ViewIndex = CMFCRibbonGallery::GetLastSelectedItem(ID_CHECK);

	Ogre::Quaternion Quaternion;
	CEngine *Engine = ((CVisibilityApp*)AfxGetApp())->m_Engine;
	Ogre::Root *Root = Engine->GetRoot();
	Ogre::Entity * WallEntity;
	Ogre::Entity * RobotEntity1 = m_SceneManager->getEntity("Robot1");
	Ogre::Entity * RobotEntity2 = m_SceneManager->getEntity("Robot2");
	Ogre::SceneNode *RobotNode2 = (Ogre::SceneNode *)m_SceneManager->getRootSceneNode()->getChild("Robot2");
	Ogre::ManualObject *Ray = (Ogre::ManualObject*)RobotNode2->getAttachedObject("Ray");
	
	Ray->setVisible(false);

	switch (ViewIndex)
	{
		case 0:
			{
				//check visibility
				Ogre::Entity *Entity = NULL;
				Ogre::RaySceneQueryResult Result;
				Ogre::SceneNode *RobotNode1 = (Ogre::SceneNode *)m_SceneManager->getRootSceneNode()->getChild("Robot1");


				Ogre::Real Height = 0.75 * RobotEntity1->getBoundingRadius();
				Ogre::Vector3 Center1 = RobotNode1->getPosition();
				Ogre::Vector3 Center2 = RobotNode2->getPosition();

				Center1[1] += Height;
				Center2[1] += Height;

				Ogre::Ray SearchRay;
				SearchRay.setOrigin(Center2);
				SearchRay.setDirection(RobotNode2->_getDerivedOrientation().xAxis());
				Ogre::RaySceneQuery *ObjectRaySceneQuery = m_SceneManager->createRayQuery(SearchRay,COLLIDABLE);
				ObjectRaySceneQuery->setSortByDistance(true);
				Result = ObjectRaySceneQuery->execute();
				if(!Result.empty()) {
					Ogre::RaySceneQueryResult::iterator Iterator;
					Iterator = Result.begin();
				
					Ogre::String ObjectName = Iterator->movable->getName();
					if(ObjectName.compare("Robot1") == 0) {
						Ray->setVisible(true);
					}
				}
			}
			break;

		case 1:
			{
				Ogre::SceneNode *WallNode = (Ogre::SceneNode *)m_SceneManager->getRootSceneNode()->getChild("Wall");
				WallEntity = m_SceneManager->getEntity("Wall");
				m_IsWallExists = !m_IsWallExists;
				WallEntity->setVisible(m_IsWallExists);
				if(m_IsWallExists) {
					WallNode->translate(-1000,0,0);
				}
				else {
					WallNode->translate(1000,0,0);
				}
			}
			//wall is visible
			break;

		case 2:
			// rotate robot
			if (m_IsRotated)
			{
				Quaternion.FromAngleAxis(Ogre::Radian(0.0), Ogre::Vector3::NEGATIVE_UNIT_Y);
			}
			else
			{
				Quaternion.FromAngleAxis(Ogre::Radian(Ogre::Math::PI), Ogre::Vector3::NEGATIVE_UNIT_Y);
			}

			RobotEntity2->getParentNode()->setOrientation(Quaternion);

			m_IsRotated = !m_IsRotated;
			break;
	}

	Root->renderOneFrame();
}
