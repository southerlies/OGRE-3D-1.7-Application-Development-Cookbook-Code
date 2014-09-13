
// NumericValueAnimationView.cpp : implementation of the CNumericValueAnimationView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "NumericValueAnimation.h"
#endif

#include "NumericValueAnimationDoc.h"
#include "NumericValueAnimationView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNumericValueAnimationView

IMPLEMENT_DYNCREATE(CNumericValueAnimationView, CView)

BEGIN_MESSAGE_MAP(CNumericValueAnimationView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_COMMAND(ID_ANIMATION_STOP, &CNumericValueAnimationView::OnAnimationStop)
	ON_COMMAND(ID_ANIMATION_START, &CNumericValueAnimationView::OnAnimationStart)
END_MESSAGE_MAP()

// CNumericValueAnimationView construction/destruction

CNumericValueAnimationView::CNumericValueAnimationView()
{
m_MouseNavigation = false;
m_TimeIndex = 0;
}

CNumericValueAnimationView::~CNumericValueAnimationView()
{
}

BOOL CNumericValueAnimationView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CNumericValueAnimationView drawing

void CNumericValueAnimationView::OnDraw(CDC* /*pDC*/)
{
	CNumericValueAnimationDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CNumericValueAnimationView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CNumericValueAnimationView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CNumericValueAnimationView diagnostics

void CNumericValueAnimationView::EngineSetup(void)
{
	Ogre::Root *Root = ((CNumericValueAnimationApp*)AfxGetApp())->m_Engine->GetRoot();
	Ogre::SceneManager *SceneManager = NULL;
	SceneManager = Root->createSceneManager(Ogre::ST_GENERIC, "NumericValue");
 
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
	m_Camera->setPosition(Ogre::Vector3(0.0, 0.0, 200.0));
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

//	Ogre::SceneNode *RobotNode = SceneManager->getRootSceneNode()->createChildSceneNode("Robot", Ogre::Vector3(50,0,0));
//	Ogre::Entity *RobotEntity = SceneManager->createEntity("Robot", "robot.mesh");

//	RobotNode->attachObject(RobotEntity);
//	RobotEntity->getParentNode()->scale(0.2,0.2,0.2);

//	Ogre::AxisAlignedBox Box = RobotEntity->getBoundingBox();
//	Ogre::Vector3 Center = Box.getCenter();
	m_Camera->lookAt(Ogre::Vector3(0.0, 0.0, 0.0));
	
	//m_Camera->setPolygonMode(Ogre::PolygonMode::PM_WIREFRAME);

//	m_Animation = RobotEntity->getAnimationState("Walk");
//	m_Animation->setEnabled(true);

	Ogre::ColourValue lightInitialColor = Ogre::ColourValue(0.0f, 0.0f, 0.0f, 1.0f);
	Ogre::NumericKeyFrame* kf;
	Ogre::Light *Light = SceneManager->createLight("AnimatedLight");
	Ogre::AnimableValuePtr animableValue = Light->createAnimableValue("diffuseColour");
	animableValue->setValue(lightInitialColor);
	animableValue->setCurrentStateAsBaseValue ();

	Ogre::Animation *Animation = SceneManager->createAnimation("AnimateValue", 5);
	Ogre::NumericAnimationTrack *track = Animation->createNumericTrack(0, animableValue);

	float colourValue = 0.0;

	for(int i = 1; i <= 1000; i++)
	{
		kf = track->createNumericKeyFrame(i * 0.025);
		colourValue = colourValue + 0.01;
		kf->setValue( Ogre::AnyNumeric(Ogre::ColourValue(colourValue, 0.0, 0.0, 1.0)) );
	}

	Root->renderOneFrame();
}

#ifdef _DEBUG
void CNumericValueAnimationView::AssertValid() const
{
	CView::AssertValid();
}

void CNumericValueAnimationView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CNumericValueAnimationDoc* CNumericValueAnimationView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNumericValueAnimationDoc)));
	return (CNumericValueAnimationDoc*)m_pDocument;
}
#endif //_DEBUG


// CNumericValueAnimationView message handlers


void CNumericValueAnimationView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_MousePosition = point;
	m_MouseNavigation = true;
	CView::OnLButtonDown(nFlags, point);
}


void CNumericValueAnimationView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_MouseNavigation = false;

	CView::OnLButtonUp(nFlags, point);
}


void CNumericValueAnimationView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_MouseNavigation)
	{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[0] = -(m_MousePosition.x - point.x);
	CameraMove[1] = m_MousePosition.y - point.y;

	CEngine * Engine = ((CNumericValueAnimationApp*)AfxGetApp())->m_Engine;
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


BOOL CNumericValueAnimationView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[2] = 0.1 * zDelta;

	CEngine * Engine = ((CNumericValueAnimationApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return false;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return false;
	m_Camera->moveRelative(CameraMove);

	Root->renderOneFrame();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CNumericValueAnimationView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CView::OnPaint() for painting messages
	CNumericValueAnimationDoc *Document = GetDocument();
	CEngine *Engine = ((CNumericValueAnimationApp*)AfxGetApp())->m_Engine;
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


void CNumericValueAnimationView::OnTimer(UINT_PTR nIDEvent)
{
	CEngine *Engine = ((CNumericValueAnimationApp*)AfxGetApp())->m_Engine;	
	Ogre::Root *Root = Engine->GetRoot();
	Ogre::SceneManager *SceneManager = Root->getSceneManager("NumericValue");
	Ogre::Camera *Camera = SceneManager->getCamera("Camera");
	Ogre::Viewport *Viewport = Camera->getViewport();
	Ogre::Animation *Animation = SceneManager->getAnimation("AnimateValue");
	Ogre::NumericAnimationTrack *track = Animation->getNumericTrack(0);
	Ogre::NumericKeyFrame *frame = track->getNumericKeyFrame(m_TimeIndex++);
	Ogre::AnyNumeric value = frame->getValue();
	Ogre::ColourValue color = Ogre::any_cast<Ogre::ColourValue>(value);
	Viewport->setBackgroundColour(color);
    Root->renderOneFrame();

	CView::OnTimer(nIDEvent);
}


void CNumericValueAnimationView::OnAnimationStop()
{
	KillTimer(1);
}


void CNumericValueAnimationView::OnAnimationStart()
{
	SetTimer(1, 1, 0);
}
