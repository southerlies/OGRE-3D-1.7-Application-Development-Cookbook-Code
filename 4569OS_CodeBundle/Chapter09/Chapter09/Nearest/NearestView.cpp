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

// NearestView.cpp : implementation of the CNearestView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Nearest.h"
#endif

#include "NearestDoc.h"
#include "NearestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNearestView

IMPLEMENT_DYNCREATE(CNearestView, CView)

BEGIN_MESSAGE_MAP(CNearestView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_COMMAND(ID_ENTITY, &CNearestView::OnEntity)
	ON_COMMAND(ID_BILLBOARD, &CNearestView::OnBillboard)
	ON_COMMAND(ID_CAMERA, &CNearestView::OnCamera)
	ON_COMMAND(ID_SELECT, &CNearestView::OnSelect)
END_MESSAGE_MAP()

// CNearestView construction/destruction

CNearestView::CNearestView()
{
	m_First = true;
	m_QueryMask = 0;
	m_MouseNavigation = false;
}

CNearestView::~CNearestView()
{
}

BOOL CNearestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CNearestView drawing

void CNearestView::OnDraw(CDC* /*pDC*/)
{
	CNearestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CNearestView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CNearestView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CNearestView diagnostics

#ifdef _DEBUG
void CNearestView::AssertValid() const
{
	CView::AssertValid();
}

void CNearestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CNearestDoc* CNearestView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNearestDoc)));
	return (CNearestDoc*)m_pDocument;
}
#endif //_DEBUG


// CNearestView message handlers


void CNearestView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_MousePosition = point;
	m_MouseNavigation = true;

	CView::OnLButtonDown(nFlags, point);
}


void CNearestView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_MouseNavigation = false;

	CView::OnLButtonUp(nFlags, point);
}


void CNearestView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_MouseNavigation)
{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[0] = -(m_MousePosition.x - point.x);
	CameraMove[1] = m_MousePosition.y - point.y;

	CEngine * Engine = ((CNearestApp*)AfxGetApp())->m_Engine;
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


BOOL CNearestView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[2] = 0.1 * zDelta;

	CEngine * Engine = ((CNearestApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return false;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return false;
	
	m_Camera->moveRelative(CameraMove);
	
	Root->renderOneFrame();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CNearestView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CNearestDoc *Document = GetDocument();
	CEngine *Engine = ((CNearestApp*)AfxGetApp())->m_Engine;
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


void CNearestView::OnTimer(UINT_PTR nIDEvent)
{
	m_WalkAnimation->addTime(0.001);

	CEngine *Engine = ((CNearestApp*)AfxGetApp())->m_Engine;
	Ogre::Root *Root = Engine->GetRoot();
    Root->renderOneFrame();

	CView::OnTimer(nIDEvent);
}

void CNearestView::EngineSetup(void)
{
	Ogre::FloatRect TextureCoordinates[]=
	{
		Ogre::FloatRect(113.0/5000.0,121.0/5000.0,851.0/5000.0,1073.0/5000.0),
		Ogre::FloatRect(1021.0/5000.0,114.0/5000.0,3386.0/5000.0,1984.0/5000.0),
		Ogre::FloatRect(3825.0/5000.0,1049.0/5000.0,4871.0/5000.0,3588.0/5000.0),
		Ogre::FloatRect(1739.0/5000.0,2418.0/5000.0,2796.0/5000.0,4774.0/5000.0),
		Ogre::FloatRect(221.0/5000.0,2723.0/5000.0,1464.0/5000.0,3795.0/5000.0),
		Ogre::FloatRect(505.0/5000.0,4391.0/5000.0,805.0/5000.0,4662.0/5000.0),
		Ogre::FloatRect(339.0/5000.0,2085.0/5000.0,482.0/5000.0,2216.0/5000.0),
		Ogre::FloatRect(2803.0/5000.0,3355.0/5000.0,3891.0/5000.0,4912.0/5000.0)
	};

	Ogre::Root *Root = ((CNearestApp*)AfxGetApp())->m_Engine->GetRoot();

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
	m_Camera->setPosition(Ogre::Vector3(0.0, 50.0, 100.0));
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
	Ogre::SceneNode *RobotNode = m_SceneManager->getRootSceneNode()->createChildSceneNode();
	RobotNode->attachObject(RobotEntity);

	Ogre::AxisAlignedBox Box = RobotEntity->getBoundingBox();
	Ogre::Vector3 Center = Box.getCenter();
	m_Camera->lookAt(Center);

	Ogre::BillboardSet *Trees = m_SceneManager->createBillboardSet("Trees");

	Trees->setTextureCoords(TextureCoordinates, 8);
	Trees->setMaterialName("Trees");
	Trees->setCastShadows(true);
	Trees->setSortingEnabled(true);
	Trees->setBillboardType(Ogre::BBT_ORIENTED_COMMON);

	Ogre::Vector3 TreePosition(0.0, 0.0, 0.0);
	Ogre::Billboard* Tree = Trees->createBillboard(TreePosition);
	Tree->setDimensions(20.0, 100.0);
	Tree->setTexcoordIndex(0);

	Ogre::SceneNode *TreeNode = m_SceneManager->getRootSceneNode()->createChildSceneNode();

	TreeNode->setPosition(0.0, 0.0, -100.0);
	TreeNode->setDirection(Ogre::Vector3::NEGATIVE_UNIT_Y);
	TreeNode->attachObject(Trees);

	Ogre::ManualObject *Screen = m_SceneManager->createManualObject("Screen");
	Screen->setDynamic(true);
	Screen->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);

	Screen->position(0,0,0);
	Screen->textureCoord(0,0);

	Screen->position(100.0,0,0);
	Screen->textureCoord(1,0);

	Screen->position(100.0, 100.0 ,0);
	Screen->textureCoord(1,1);

	Screen->triangle(0, 1, 2);

	Screen->position(0,0,0);
	Screen->textureCoord(0,0);

	Screen->position(100.0, 100.0, 0);
	Screen->textureCoord(1,1);

	Screen->position(0, 100.0,0);
	Screen->textureCoord(0,1);

	Screen->triangle(3, 4, 5);
	 
	Screen->end();

	Ogre::SceneNode* node = m_SceneManager->getRootSceneNode()->createChildSceneNode();
	node->setPosition(-50.0, 0, -20.0);
	node->attachObject(Screen);

	Root->renderOneFrame();
}

void CNearestView::OnEntity()
{
	m_QueryMask |= Ogre::SceneManager::ENTITY_TYPE_MASK;
}

void CNearestView::OnBillboard()
{
	m_QueryMask |= Ogre::SceneManager::FX_TYPE_MASK;
}

void CNearestView::OnCamera()
{
	m_QueryMask |= Ogre::SceneManager::FRUSTUM_TYPE_MASK;
}

void CNearestView::OnSelect()
{
	m_SceneManager->_updateSceneGraph(m_Camera);
	Ogre::AxisAlignedBox Box = m_SceneManager->getRootSceneNode()->_getWorldAABB();
	Ogre::AxisAlignedBoxSceneQuery* Query = m_SceneManager->createAABBQuery(Box, m_QueryMask);
	Ogre::SceneQueryResult &Result = Query->execute();

	Ogre::SceneQueryResultMovableList::iterator Iterator;
		
	for (Iterator = Result.movables.begin(); Iterator != Result.movables.end(); Iterator++)
	{
		Ogre::String ObjectName = (*Iterator)->getName();
		Ogre::MovableObject *Object = (*Iterator);
		
		Object->setVisible(false);
	}

	Ogre::Root *Root = ((CNearestApp*)AfxGetApp())->m_Engine->GetRoot();
	Root->renderOneFrame();
}
