
// WalkingView.cpp : implementation of the CWalkingView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Walking.h"
#endif

#include "WalkingDoc.h"
#include "WalkingView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWalkingView

IMPLEMENT_DYNCREATE(CWalkingView, CView)

BEGIN_MESSAGE_MAP(CWalkingView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_COMMAND(ID_WALKING_START, &CWalkingView::OnWalkingStart)
	ON_COMMAND(ID_WALKING_STOP, &CWalkingView::OnWalkingStop)
END_MESSAGE_MAP()

// CWalkingView construction/destruction

CWalkingView::CWalkingView()
{
m_MouseNavigation = false;
m_Offset = 0.0;
m_Angle = 0.0;
}

CWalkingView::~CWalkingView()
{
}

BOOL CWalkingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CWalkingView drawing

void CWalkingView::OnDraw(CDC* /*pDC*/)
{
	CWalkingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CWalkingView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CWalkingView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CWalkingView diagnostics

void CWalkingView::EngineSetup(void)
{
	Ogre::Root *Root = ((CWalkingApp*)AfxGetApp())->m_Engine->GetRoot();
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
	Ogre::RenderTarget *RenderWindow = Root->getRenderTarget("Mouse Input");

	if (RenderWindow == NULL)
	{
	try
	{
		m_RenderWindow = Root->createRenderWindow("Mouse Input", rect.Width(), rect.Height(), false, &parms);
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
	m_Camera->setPosition(Ogre::Vector3(/*20*/0.0, /*5*/0.0, 100.0));
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

	Ogre::SceneNode *SphereNode = SceneManager->getRootSceneNode()->createChildSceneNode("Sphere", Ogre::Vector3(0,0,0));
	Ogre::Entity *SphereEntity = SceneManager->createEntity("Sphere", "sphere.mesh");
	SphereEntity->setMaterialName("Wall/Screen");
	SphereNode->attachObject(SphereEntity);
	SphereEntity->getParentNode()->scale(0.5,0.5,0.5);

	Ogre::SceneNode *RobotNode = SceneManager->getRootSceneNode()->createChildSceneNode("Robot", Ogre::Vector3(50,0,0));
	Ogre::Entity *RobotEntity = SceneManager->createEntity("Robot", "robot.mesh");

	RobotNode->attachObject(RobotEntity);
	RobotEntity->getParentNode()->scale(0.2,0.2,0.2);

	Ogre::AxisAlignedBox Box = RobotEntity->getBoundingBox();
	Ogre::Vector3 Center = Box.getCenter();
	m_Camera->lookAt(Ogre::Vector3(0.0, 0.0, 0.0));
	
	//m_Camera->setPolygonMode(Ogre::PolygonMode::PM_WIREFRAME);

	m_Animation = RobotEntity->getAnimationState("Walk");
	m_Animation->setEnabled(true);

	Root->renderOneFrame();
}

#ifdef _DEBUG
void CWalkingView::AssertValid() const
{
	CView::AssertValid();
}

void CWalkingView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWalkingDoc* CWalkingView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWalkingDoc)));
	return (CWalkingDoc*)m_pDocument;
}
#endif //_DEBUG


// CWalkingView message handlers


void CWalkingView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_MousePosition = point;
	m_MouseNavigation = true;
	CView::OnLButtonDown(nFlags, point);
}


void CWalkingView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_MouseNavigation = false;

	CView::OnLButtonUp(nFlags, point);
}


void CWalkingView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_MouseNavigation)
	{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[0] = -(m_MousePosition.x - point.x);
	CameraMove[1] = m_MousePosition.y - point.y;

	CEngine * Engine = ((CWalkingApp*)AfxGetApp())->m_Engine;
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


BOOL CWalkingView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[2] = 0.1 * zDelta;

	CEngine * Engine = ((CWalkingApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return false;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return false;
	m_Camera->moveRelative(CameraMove);

	Root->renderOneFrame();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CWalkingView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CView::OnPaint() for painting messages
	CWalkingDoc *Document = GetDocument();
	CEngine *Engine = ((CWalkingApp*)AfxGetApp())->m_Engine;
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


void CWalkingView::OnTimer(UINT_PTR nIDEvent)
{
	Ogre::Quaternion Quaternion;
	CEngine *Engine = ((CWalkingApp*)AfxGetApp())->m_Engine;		
	Ogre::Root *Root = Engine->GetRoot();
	Ogre::SceneNode *RobotNode = Root->getSceneManager("Walking")->getSceneNode("Robot");

	m_Animation->addTime(0.01);

	const Ogre::Real Radius = 50.0;

	Ogre::Real x = Radius * Ogre::Math::Cos(m_Angle);
	Ogre::Real y = Radius * Ogre::Math::Sin(m_Angle);

	Ogre::Vector3 Destination = Ogre::Vector3(x, 0.0, y);
	Ogre::Vector3 Direction = Destination - RobotNode->getPosition(); 
	Ogre::Vector3 Origin = RobotNode->getOrientation() * Ogre::Vector3::UNIT_Z; 
    Ogre::Real Distance = Direction.normalise();                 
	Quaternion = Origin.getRotationTo(Direction);         

	m_Angle += 0.01;
	RobotNode->setPosition(x, 0.0, y);

	if (m_Camera == NULL)
		return;

	Root->renderOneFrame();
	CView::OnTimer(nIDEvent);
}


void CWalkingView::OnWalkingStart()
{
	SetTimer(1, 1, 0);
}


void CWalkingView::OnWalkingStop()
{
	KillTimer(1);
}
