
// LinkedAnimationView.cpp : implementation of the CLinkedAnimationView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "LinkedAnimation.h"
#endif

#include "LinkedAnimationDoc.h"
#include "LinkedAnimationView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLinkedAnimationView

IMPLEMENT_DYNCREATE(CLinkedAnimationView, CView)

BEGIN_MESSAGE_MAP(CLinkedAnimationView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_COMMAND(ID_ANIMATION_START, &CLinkedAnimationView::OnAnimationStart)
	ON_COMMAND(ID_ANIMATION_STOP, &CLinkedAnimationView::OnAnimationStop)
END_MESSAGE_MAP()

// CLinkedAnimationView construction/destruction

CLinkedAnimationView::CLinkedAnimationView()
{
m_MouseNavigation = false;
}

CLinkedAnimationView::~CLinkedAnimationView()
{
}

BOOL CLinkedAnimationView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CLinkedAnimationView drawing

void CLinkedAnimationView::OnDraw(CDC* /*pDC*/)
{
	CLinkedAnimationDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CLinkedAnimationView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CLinkedAnimationView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CLinkedAnimationView diagnostics

void CLinkedAnimationView::EngineSetup(void)
{
	Ogre::Root *Root = ((CLinkedAnimationApp*)AfxGetApp())->m_Engine->GetRoot();
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

	Ogre::SceneNode *RobotNode1 = SceneManager->getRootSceneNode()->createChildSceneNode("Robot1", Ogre::Vector3(-50,0,0));
	Ogre::Entity *RobotEntity1 = SceneManager->createEntity("Robot1", "robot.mesh");
	RobotNode1->attachObject(RobotEntity1);
	Ogre::Skeleton *Skeleton1 = RobotEntity1->getSkeleton();
	RobotEntity1->setDisplaySkeleton(true);

	Ogre::SceneNode *RobotNode2 = SceneManager->getRootSceneNode()->createChildSceneNode("Robot2", Ogre::Vector3(50,0,0));
	Ogre::Entity *RobotEntity2 = SceneManager->createEntity("Robot2", "robot.mesh");
	RobotNode2->attachObject(RobotEntity2);
	Ogre::Skeleton *Skeleton2 = RobotEntity2->getSkeleton(); 
	RobotEntity2->setDisplaySkeleton(true);

	Skeleton1->addLinkedSkeletonAnimationSource(Skeleton2->getName(), 2.0);

//	RobotEntity1->_initialise(true);
//	RobotEntity2->_initialise(true);

//	RobotEntity1->refreshAvailableAnimationState();
	m_Camera->lookAt(Ogre::Vector3(0.0, 0.0, 0.0));
	
	m_AnimationState1 = RobotEntity1->getAnimationState("Walk");
	m_AnimationState1->setEnabled(true);

	m_AnimationState2 = RobotEntity2->getAnimationState("Slump");
	m_AnimationState2->setEnabled(true);

	Root->renderOneFrame();
}

#ifdef _DEBUG
void CLinkedAnimationView::AssertValid() const
{
	CView::AssertValid();
}

void CLinkedAnimationView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLinkedAnimationDoc* CLinkedAnimationView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLinkedAnimationDoc)));
	return (CLinkedAnimationDoc*)m_pDocument;
}
#endif //_DEBUG


// CLinkedAnimationView message handlers


void CLinkedAnimationView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_MousePosition = point;
	m_MouseNavigation = true;
	CView::OnLButtonDown(nFlags, point);
}


void CLinkedAnimationView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_MouseNavigation = false;

	CView::OnLButtonUp(nFlags, point);
}


void CLinkedAnimationView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_MouseNavigation)
	{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[0] = -(m_MousePosition.x - point.x);
	CameraMove[1] = m_MousePosition.y - point.y;

	CEngine * Engine = ((CLinkedAnimationApp*)AfxGetApp())->m_Engine;
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


BOOL CLinkedAnimationView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[2] = 0.1 * zDelta;

	CEngine * Engine = ((CLinkedAnimationApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return false;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return false;
	m_Camera->moveRelative(CameraMove);

	Root->renderOneFrame();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CLinkedAnimationView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CView::OnPaint() for painting messages
	CLinkedAnimationDoc *Document = GetDocument();
	CEngine *Engine = ((CLinkedAnimationApp*)AfxGetApp())->m_Engine;
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


void CLinkedAnimationView::OnTimer(UINT_PTR nIDEvent)
{
	CEngine *Engine = ((CLinkedAnimationApp*)AfxGetApp())->m_Engine;
	Ogre::Root *Root = Engine->GetRoot();

	m_AnimationState1->addTime(0.01);
	m_AnimationState2->addTime(0.01);

	Root->renderOneFrame();
	CView::OnTimer(nIDEvent);
}


void CLinkedAnimationView::OnAnimationStart()
{
	SetTimer(1, 1, 0);
}


void CLinkedAnimationView::OnAnimationStop()
{
	KillTimer(1);
}
