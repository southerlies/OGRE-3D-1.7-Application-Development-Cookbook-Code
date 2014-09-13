
// BlendingAnimationsView.cpp : implementation of the CBlendingAnimationsView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "BlendingAnimations.h"
#endif

#include "BlendingAnimationsDoc.h"
#include "BlendingAnimationsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBlendingAnimationsView

IMPLEMENT_DYNCREATE(CBlendingAnimationsView, CView)

BEGIN_MESSAGE_MAP(CBlendingAnimationsView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_COMMAND(ID_ANIMATION_START, &CBlendingAnimationsView::OnAnimationStart)
	ON_COMMAND(ID_ANIMATION_STOP, &CBlendingAnimationsView::OnAnimationStop)
	ON_COMMAND(ID_ANIMATION_STARTWALK, &CBlendingAnimationsView::OnAnimationStartwalk)
	ON_COMMAND(ID_ANIMATION_STARTSLUMP, &CBlendingAnimationsView::OnAnimationStartslump)
	ON_COMMAND(ID_ANIMATION_STOPWALK, &CBlendingAnimationsView::OnAnimationStopwalk)
	ON_COMMAND(ID_ANIMATION_STOPSLUMP, &CBlendingAnimationsView::OnAnimationStopslump)
END_MESSAGE_MAP()

// CBlendingAnimationsView construction/destruction

CBlendingAnimationsView::CBlendingAnimationsView()
{
	m_WeightDlg = NULL;
	m_MouseNavigation = false;
}

CBlendingAnimationsView::~CBlendingAnimationsView()
{
}

BOOL CBlendingAnimationsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CBlendingAnimationsView drawing

void CBlendingAnimationsView::OnDraw(CDC* /*pDC*/)
{
	CBlendingAnimationsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CBlendingAnimationsView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CBlendingAnimationsView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CBlendingAnimationsView diagnostics

void CBlendingAnimationsView::EngineSetup(void)
{
	Ogre::Root *Root = ((CBlendingAnimationsApp*)AfxGetApp())->m_Engine->GetRoot();
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

	Ogre::SceneNode *RobotNode = SceneManager->getRootSceneNode()->createChildSceneNode("Robot", Ogre::Vector3(50,0,0));
	Ogre::Entity *RobotEntity = SceneManager->createEntity("Robot", "robot.mesh");
	Ogre::MeshPtr Mesh = RobotEntity->getMesh();

	RobotNode->attachObject(RobotEntity);
	RobotEntity->getParentNode()->scale(0.2,0.2,0.2);

	m_Camera->lookAt(Ogre::Vector3(0.0, 0.0, 0.0));
	
	RobotEntity->setDisplaySkeleton(true);
	m_Camera->setPolygonMode(Ogre::PolygonMode::PM_WIREFRAME);
	 
	m_WalkAnimation = RobotEntity->getAnimationState("Walk");
	m_WalkAnimation->setEnabled(true);

	m_SlumpAnimation = RobotEntity->getAnimationState("Slump");
	m_SlumpAnimation->setEnabled(true);

	if (m_WeightDlg == NULL)
	{
		m_WeightDlg = new CWeightDlg();
		m_WeightDlg->Create(IDD_WEIGHT_CONTROL);
	}

	m_WeightDlg->ShowWindow(SW_SHOW);

	Root->renderOneFrame();
}

#ifdef _DEBUG
void CBlendingAnimationsView::AssertValid() const
{
	CView::AssertValid();
}

void CBlendingAnimationsView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBlendingAnimationsDoc* CBlendingAnimationsView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBlendingAnimationsDoc)));
	return (CBlendingAnimationsDoc*)m_pDocument;
}
#endif //_DEBUG


// CBlendingAnimationsView message handlers


void CBlendingAnimationsView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_MousePosition = point;
	m_MouseNavigation = true;
	CView::OnLButtonDown(nFlags, point);
}


void CBlendingAnimationsView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_MouseNavigation = false;

	CView::OnLButtonUp(nFlags, point);
}


void CBlendingAnimationsView::OnMouseMove(UINT nFlags, CPoint point)
{	
    if (m_MouseNavigation)
	{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[0] = -(m_MousePosition.x - point.x);
	CameraMove[1] = m_MousePosition.y - point.y;

	CEngine * Engine = ((CBlendingAnimationsApp*)AfxGetApp())->m_Engine;
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


BOOL CBlendingAnimationsView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[2] = 0.1 * zDelta;

	CEngine * Engine = ((CBlendingAnimationsApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return false;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return false;
	m_Camera->moveRelative(CameraMove);

	Root->renderOneFrame();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CBlendingAnimationsView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CView::OnPaint() for painting messages
	CBlendingAnimationsDoc *Document = GetDocument();
	CEngine *Engine = ((CBlendingAnimationsApp*)AfxGetApp())->m_Engine;
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


void CBlendingAnimationsView::OnTimer(UINT_PTR nIDEvent)
{
	CEngine *Engine = ((CBlendingAnimationsApp*)AfxGetApp())->m_Engine;			
	Ogre::Root *Root = Engine->GetRoot();
	Ogre::SceneNode *RobotNode = Root->getSceneManager("Walking")->getSceneNode("Robot");

	Ogre::Entity *RobotEntity = Root->getSceneManager("Walking")->getEntity("Robot");
	Ogre::Skeleton *Skeleton = RobotEntity->getSkeleton();

	if (m_WeightDlg->m_IsAverage)
	{
		Skeleton->setBlendMode(Ogre::SkeletonAnimationBlendMode::ANIMBLEND_AVERAGE);
	}
	else
	{
		Skeleton->setBlendMode(Ogre::SkeletonAnimationBlendMode::ANIMBLEND_CUMULATIVE);
	}

	double WalkWeight;
	double SlumpWeight;

	switch(nIDEvent)
	{
	case 1:
	
		WalkWeight = m_WeightDlg->m_WalkWeight.GetPos() / 10.0;
		SlumpWeight = m_WeightDlg->m_SlumpWeight.GetPos() / 10.0;

		m_WalkAnimation->setWeight(WalkWeight);
		m_SlumpAnimation->setWeight(SlumpWeight);
		
		m_WalkAnimation->addTime(0.01);
		m_SlumpAnimation->addTime(0.01);

		break;
	
	case 2:

		m_WalkAnimation->addTime(0.01);

	break;

	case 3:
		
		m_SlumpAnimation->addTime(0.01);

		break;
	}

	Root->renderOneFrame();
	CView::OnTimer(nIDEvent);
}


void CBlendingAnimationsView::OnAnimationStart()
{
	SetTimer(1, 1, 0);
	KillTimer(2);
	KillTimer(3);
}


void CBlendingAnimationsView::OnAnimationStop()
{
	KillTimer(1);
}


void CBlendingAnimationsView::OnAnimationStartwalk()
{
	SetTimer(2, 1, 0);
	KillTimer(1);
	KillTimer(3);
}


void CBlendingAnimationsView::OnAnimationStartslump()
{
	SetTimer(3, 1, 0);
	KillTimer(2);
	KillTimer(1);
}


void CBlendingAnimationsView::OnAnimationStopwalk()
{
	KillTimer(2);
}


void CBlendingAnimationsView::OnAnimationStopslump()
{
	KillTimer(3);
}
