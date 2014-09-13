
// PathDroppingView.cpp : implementation of the CPathDroppingView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "PathDropping.h"
#endif

#include "PathDroppingDoc.h"
#include "PathDroppingView.h"
#include "Ogre.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPathDroppingView

IMPLEMENT_DYNCREATE(CPathDroppingView, CView)

BEGIN_MESSAGE_MAP(CPathDroppingView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_COMMAND(ID_TERRAIN_VISIBLE, &CPathDroppingView::OnTerrainVisible)
	ON_COMMAND(ID_TERRAIN_UNVISIBLE, &CPathDroppingView::OnTerrainUnvisible)
	ON_COMMAND(ID_WALKING_START, &CPathDroppingView::OnWalkingStart)
	ON_COMMAND(ID_WALKING_STOP, &CPathDroppingView::OnWalkingStop)
	ON_COMMAND(ID_WALKING_DRAP, &CPathDroppingView::OnWalkingDrap)
END_MESSAGE_MAP()

// CPathDroppingView construction/destruction

CPathDroppingView::CPathDroppingView()
{
	m_MouseNavigation = false;
	m_First = true;
m_PathIndex = 0;
}

CPathDroppingView::~CPathDroppingView()
{
}

BOOL CPathDroppingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CPathDroppingView drawing

void CPathDroppingView::OnDraw(CDC* /*pDC*/)
{
	CPathDroppingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CPathDroppingView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CPathDroppingView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CPathDroppingView diagnostics

void CPathDroppingView::EngineSetup(void)
{
	Ogre::Root *Root = ((CPathDroppingApp*)AfxGetApp())->m_Engine->GetRoot();
	Ogre::SceneManager *SceneManager = NULL;
	SceneManager = Root->createSceneManager(Ogre::ST_GENERIC, "Walking");
 
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
	Ogre::RenderTarget *RenderWindow = Root->getRenderTarget("Walking");

	if (RenderWindow == NULL)
	{
	try
	{
		m_RenderWindow = Root->createRenderWindow("Walking", rect.Width(), rect.Height(), false, &parms);
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
    m_Camera->setNearClipDistance(10);
	m_Camera->setFarClipDistance(10000); 
	m_Camera->setCastShadows(false);
	m_Camera->setUseRenderingDistance(true);

	Ogre::SceneNode *CameraNode = NULL;
	CameraNode = SceneManager->getRootSceneNode()->createChildSceneNode("CameraNode");

	Ogre::Viewport* Viewport = NULL;
	
	if (0 == m_RenderWindow->getNumViewports())
	{
		Viewport = m_RenderWindow->addViewport(m_Camera);
		Viewport->setBackgroundColour(Ogre::ColourValue(0.8f, 0.8f, 0.8f));
	}

    // Alter the camera aspect ratio to match the viewport
    m_Camera->setAspectRatio(Ogre::Real(rect.Width()) / Ogre::Real(rect.Height()));

	Ogre::SceneNode *RobotNode = SceneManager->getRootSceneNode()->createChildSceneNode("Robot");
	Ogre::Entity *RobotEntity = SceneManager->createEntity("Robot", "robot.mesh");
	RobotNode->attachObject(RobotEntity);
	RobotEntity->getParentNode()->scale(10,10,10);

	Ogre::SceneNode *TopographyNode = SceneManager->getRootSceneNode()->createChildSceneNode("Topography");
	Ogre::Entity *TopographyEntity = SceneManager->createEntity("Topography", "Topography.mesh");
	TopographyNode->attachObject(TopographyEntity);
	Ogre::AxisAlignedBox TopographyBox = TopographyEntity->getBoundingBox();
	Ogre::Vector3 Minimum = TopographyBox.getMinimum();

	RobotNode->setPosition(TopographyBox.getMinimum());
	Ogre::Vector3 Center = TopographyBox.getCenter();
	m_Camera->setPosition(Ogre::Vector3(Minimum[0], Minimum[1] + 5000, Minimum[2]));
	m_Camera->lookAt(Center);
	
	m_Camera->setPolygonMode(Ogre::PolygonMode::PM_WIREFRAME);

	m_Animation = RobotEntity->getAnimationState("Walk");
	m_Animation->setEnabled(true);
	
	m_CollisionTools = new MOC::CollisionTools(SceneManager);
	Root->renderOneFrame();
}

#ifdef _DEBUG
void CPathDroppingView::AssertValid() const
{
	CView::AssertValid();
}

void CPathDroppingView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPathDroppingDoc* CPathDroppingView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPathDroppingDoc)));
	return (CPathDroppingDoc*)m_pDocument;
}
#endif //_DEBUG


// CPathDroppingView message handlers


void CPathDroppingView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_MousePosition = point;
	m_MouseNavigation = true;
	CView::OnLButtonDown(nFlags, point);
}


void CPathDroppingView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_MouseNavigation = false;

	CView::OnLButtonUp(nFlags, point);
}


void CPathDroppingView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_MouseNavigation)
	{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[0] = -(m_MousePosition.x - point.x);
	CameraMove[1] = m_MousePosition.y - point.y;

	CEngine * Engine = ((CPathDroppingApp*)AfxGetApp())->m_Engine;
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


BOOL CPathDroppingView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[2] = 10 * zDelta;

	CEngine * Engine = ((CPathDroppingApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return false;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return false;
	m_Camera->moveRelative(CameraMove);

	Root->renderOneFrame();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CPathDroppingView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CView::OnPaint() for painting messages
	CPathDroppingDoc *Document = GetDocument();
	CEngine *Engine = ((CPathDroppingApp*)AfxGetApp())->m_Engine;
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

void CPathDroppingView::OnTimer(UINT_PTR nIDEvent)
{
	if (0 == m_Path.GetSize())
	{
		CView::OnTimer(nIDEvent);
		return;
	}

	CEngine *Engine = ((CPathDroppingApp*)AfxGetApp())->m_Engine;		
	Ogre::Root *Root = Engine->GetRoot();
	Ogre::SceneNode *RobotNode = Root->getSceneManager("Walking")->getSceneNode("Robot");

	m_Animation->addTime(0.01);

	RobotNode->setPosition(m_Path[m_PathIndex]);
	m_PathIndex++;
	if (m_PathIndex == m_Path.GetSize())
	  KillTimer(1);

	Root->renderOneFrame();

	CView::OnTimer(nIDEvent);
}


void CPathDroppingView::DropPath(void)
{
	if (0 < m_Path.GetSize())
	{
		return;
	}

	CEngine *Engine = ((CPathDroppingApp*)AfxGetApp())->m_Engine;		
	Ogre::Root *Root = Engine->GetRoot();
	Ogre::SceneManager *SceneManager = Root->getSceneManager("Walking");
	Ogre::Entity *TopographyEntity = SceneManager->getEntity("Topography");
	Ogre::AxisAlignedBox TopographyBox = TopographyEntity->getBoundingBox();
		
	Ogre::Vector3 Start = TopographyBox.getMinimum();
	Ogre::Vector3 Finish = TopographyBox.getMaximum();

	double Length = Start.distance(Finish);
	Ogre::SceneNode *PathNode;

	Ogre::ManualObject *Path = SceneManager->createManualObject("Path");
	Path->begin("BumpyMetal", Ogre::RenderOperation::OT_LINE_LIST); 

	for (double Distance = 0; Distance < 1.0; Distance += 0.001)
	{
		Ogre::Vector3 Position = Start + ((Finish - Start) * Distance);
		float x = Position[0];
		float y = Position[1];
		float z = Position[2];

		m_CollisionTools->collidesWithEntity(Position, Ogre::Vector3(x,y-1,z), Ogre::Vector3(x,y+1,z), 100.0f, 0.0f, 4294967295);
		m_Path.Add(Position);

		Path->position(Position); 
	}

	Path->end(); 
			
	PathNode = SceneManager->getRootSceneNode()->createChildSceneNode("Path");
	PathNode->attachObject(Path);
}



void CPathDroppingView::OnTerrainVisible()
{
	CEngine *Engine = ((CPathDroppingApp*)AfxGetApp())->m_Engine;		
	Ogre::Root *Root = Engine->GetRoot();
	Ogre::SceneManager *SceneManager = Root->getSceneManager("Walking");
	Ogre::Entity *TopographyEntity = SceneManager->getEntity("Topography");
	TopographyEntity->setVisible(true);
	Root->renderOneFrame();
}


void CPathDroppingView::OnTerrainUnvisible()
{
	CEngine *Engine = ((CPathDroppingApp*)AfxGetApp())->m_Engine;		
	Ogre::Root *Root = Engine->GetRoot();
	Ogre::SceneManager *SceneManager = Root->getSceneManager("Walking");
	Ogre::Entity *TopographyEntity = SceneManager->getEntity("Topography");
	TopographyEntity->setVisible(false);
	Root->renderOneFrame();
}


void CPathDroppingView::OnWalkingStart()
{
	SetTimer(1, 1, 0);
}


void CPathDroppingView::OnWalkingStop()
{
	KillTimer(1);
}


void CPathDroppingView::OnWalkingDrap()
{
	CEngine *Engine = ((CPathDroppingApp*)AfxGetApp())->m_Engine;		
	Ogre::Root *Root = Engine->GetRoot();
	DropPath();
	Root->renderOneFrame();
}

