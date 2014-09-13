
// TerrainWalkingView.cpp : implementation of the CTerrainWalkingView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "TerrainWalking.h"
#endif

#include "TerrainWalkingDoc.h"
#include "TerrainWalkingView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTerrainWalkingView

IMPLEMENT_DYNCREATE(CTerrainWalkingView, CView)

BEGIN_MESSAGE_MAP(CTerrainWalkingView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_COMMAND(ID_WALKING_START, &CTerrainWalkingView::OnWalkingStart)
	ON_COMMAND(ID_WALKING_STOP, &CTerrainWalkingView::OnWalkingStop)
	ON_WM_KEYUP()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CTerrainWalkingView construction/destruction

CTerrainWalkingView::CTerrainWalkingView()
{
m_MouseNavigation = false;
m_First = true;
m_RelativeDistance = 0.0;
}

CTerrainWalkingView::~CTerrainWalkingView()
{
}

BOOL CTerrainWalkingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CTerrainWalkingView drawing

void CTerrainWalkingView::OnDraw(CDC* /*pDC*/)
{
	CTerrainWalkingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CTerrainWalkingView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CTerrainWalkingView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CTerrainWalkingView diagnostics

void CTerrainWalkingView::EngineSetup(void)
{
	Ogre::Root *Root = ((CTerrainWalkingApp*)AfxGetApp())->m_Engine->GetRoot();
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

	Ogre::SceneNode *TopographyNode = SceneManager->getRootSceneNode()->createChildSceneNode("Topography");
	Ogre::Entity *TopographyEntity = SceneManager->createEntity("Topography", "Topography.mesh");
	TopographyNode->attachObject(TopographyEntity);
	
	Ogre::AxisAlignedBox TopographyBox = TopographyEntity->getBoundingBox();
	
	Ogre::Vector3 Minimum = TopographyBox.getMinimum();
	Ogre::Vector3 Center = TopographyBox.getCenter();

	Ogre::SceneNode *RobotNode = SceneManager->getRootSceneNode()->createChildSceneNode("Robot");
	Ogre::Entity *RobotEntity = SceneManager->createEntity("Robot", "robot.mesh");
	RobotNode->attachObject(RobotEntity);
	RobotEntity->getParentNode()->scale(10,10,10);
	Ogre::AxisAlignedBox RobotBox = RobotEntity->getBoundingBox();

	RobotNode->setPosition(Ogre::Vector3(Minimum[0], Minimum[1] + RobotBox.getSize()[1], Minimum[2]));

	m_Camera->setPosition(Ogre::Vector3(Minimum[0], Minimum[1] + 5000, Minimum[2]));
	m_Camera->lookAt(Center);
	
	m_Camera->setPolygonMode(Ogre::PolygonMode::PM_WIREFRAME);

	m_Animation = RobotEntity->getAnimationState("Walk");
	m_Animation->setEnabled(true);
	
	m_CollisionTools = new MOC::CollisionTools(SceneManager);

	Root->renderOneFrame();
}

#ifdef _DEBUG
void CTerrainWalkingView::AssertValid() const
{
	CView::AssertValid();
}

void CTerrainWalkingView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTerrainWalkingDoc* CTerrainWalkingView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTerrainWalkingDoc)));
	return (CTerrainWalkingDoc*)m_pDocument;
}
#endif //_DEBUG


// CTerrainWalkingView message handlers


void CTerrainWalkingView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_MousePosition = point;
	m_MouseNavigation = true;
	CView::OnLButtonDown(nFlags, point);
}


void CTerrainWalkingView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_MouseNavigation = false;

	CView::OnLButtonUp(nFlags, point);
}


void CTerrainWalkingView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_MouseNavigation)
	{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[0] = -(m_MousePosition.x - point.x);
	CameraMove[1] = m_MousePosition.y - point.y;

	CEngine * Engine = ((CTerrainWalkingApp*)AfxGetApp())->m_Engine;
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


BOOL CTerrainWalkingView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[2] = 10 * zDelta;

	CEngine * Engine = ((CTerrainWalkingApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return false;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return false;
	m_Camera->moveRelative(CameraMove);

	Root->renderOneFrame();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CTerrainWalkingView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CView::OnPaint() for painting messages
	CTerrainWalkingDoc *Document = GetDocument();
	CEngine *Engine = ((CTerrainWalkingApp*)AfxGetApp())->m_Engine;
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


void CTerrainWalkingView::OnTimer(UINT_PTR nIDEvent)
{
	Ogre::Quaternion Quaternion;
	CEngine *Engine = ((CTerrainWalkingApp*)AfxGetApp())->m_Engine;		
	Ogre::Root *Root = Engine->GetRoot();
	Ogre::SceneNode *RobotNode = Root->getSceneManager("Walking")->getSceneNode("Robot");
	Ogre::Entity *RobotEntity = Root->getSceneManager("Walking")->getEntity("Robot");
	Ogre::AxisAlignedBox RobotBox = RobotEntity->getBoundingBox();

	Ogre::Entity *TopographyEntity = Root->getSceneManager("Walking")->getEntity("Topography");

	Ogre::AxisAlignedBox TopographyBox = TopographyEntity->getBoundingBox();
		
	Ogre::Vector3 Start = TopographyBox.getMinimum();
	Ogre::Vector3 Finish = TopographyBox.getMaximum();

	double x = Start[0] + (Finish[0] - Start[0]) * m_RelativeDistance;
	double y = Start[1] + (Finish[1] - Start[1]) * m_RelativeDistance;
	double z = Start[2] + (Finish[2] - Start[2]) * m_RelativeDistance;
	
	Ogre::Vector3 Elevation(x, y, z);
	Ogre::Vector3 CameraMove;

	switch (nIDEvent)
	{
		case 1:

			m_Animation->addTime(0.01);
			m_RelativeDistance += 0.01;

			if (m_CollisionTools->collidesWithEntity(Elevation, Ogre::Vector3(x,y - 1,z), Ogre::Vector3(x,y + 1,z), 100.0f, 0.0f, 4294967295))
			{
				x = Elevation[0];
				y = Elevation[1] + RobotBox.getSize()[1];
				z = Elevation[2];
			}

			RobotNode->setPosition(x, y, z);
			m_Camera->lookAt(x, y, z);
			
			if (m_RelativeDistance > 1.0)
			  KillTimer(1);

		break;

		case 2:

			CameraMove[0] = -100;
			CameraMove[1] = 0;
			CameraMove[2] = 0;
			m_Camera->moveRelative(CameraMove);

		break;

		case 3:

			CameraMove[0] = 0;
			CameraMove[1] = 100;
			CameraMove[2] = 0;
			m_Camera->moveRelative(CameraMove);

			break;

		case 4:

			CameraMove[0] = 100;
			CameraMove[1] = 0;
			CameraMove[2] = 0;
			m_Camera->moveRelative(CameraMove);

			break;

		case 5:

			CameraMove[0] = 0;
			CameraMove[1] = -100;
			CameraMove[2] = 0;
			m_Camera->moveRelative(CameraMove);

			break;
	}

	Root->renderOneFrame();
	CView::OnTimer(nIDEvent);
}


void CTerrainWalkingView::OnWalkingStart()
{
	SetTimer(1, 1, 0);
}


void CTerrainWalkingView::OnWalkingStop()
{
	KillTimer(1);
}


void CTerrainWalkingView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	KillTimer(2);
	KillTimer(3);
	KillTimer(4);
	KillTimer(5);

	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CTerrainWalkingView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	KillTimer(2);
	KillTimer(3);
	KillTimer(4);
	KillTimer(5);

	switch (nChar)
	{
		case VK_LEFT: //left
		case 65: //A
		case 97: //a

			  SetTimer(2, 10, NULL);

			break;

		case VK_UP:  //up
		case 87:  //W
		case 119: //w

	  SetTimer(3, 10, NULL);

			break;

		case VK_RIGHT: //right
		case 68: //D
		case 100: //d

	  SetTimer(4, 10, NULL);

			break;

		case VK_DOWN: //down
		case 83: //S
		case 115://s

	  SetTimer(5, 10, NULL);

			break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
