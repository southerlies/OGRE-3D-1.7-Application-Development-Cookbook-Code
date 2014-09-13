
// SceneNodeAnimationView.cpp : implementation of the CSceneNodeAnimationView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "SceneNodeAnimation.h"
#endif

#include "SceneNodeAnimationDoc.h"
#include "SceneNodeAnimationView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSceneNodeAnimationView

IMPLEMENT_DYNCREATE(CSceneNodeAnimationView, CView)

BEGIN_MESSAGE_MAP(CSceneNodeAnimationView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_COMMAND(ID_ANIMATION_STOP, &CSceneNodeAnimationView::OnAnimationStop)
	ON_COMMAND(ID_ANIMATION_START, &CSceneNodeAnimationView::OnAnimationStart)
END_MESSAGE_MAP()

// CSceneNodeAnimationView construction/destruction

CSceneNodeAnimationView::CSceneNodeAnimationView()
{
m_MouseNavigation = false;
}

CSceneNodeAnimationView::~CSceneNodeAnimationView()
{
}

BOOL CSceneNodeAnimationView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CSceneNodeAnimationView drawing

void CSceneNodeAnimationView::OnDraw(CDC* /*pDC*/)
{
	CSceneNodeAnimationDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CSceneNodeAnimationView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CSceneNodeAnimationView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CSceneNodeAnimationView diagnostics

void CSceneNodeAnimationView::EngineSetup(void)
{
	Ogre::Root *Root = ((CSceneNodeAnimationApp*)AfxGetApp())->m_Engine->GetRoot();
	Ogre::SceneManager *SceneManager = NULL;
	SceneManager = Root->createSceneManager(Ogre::ST_GENERIC, "SceneNodeAnimation");
 
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
	Ogre::RenderTarget *RenderWindow = Root->getRenderTarget("SceneNodeAnimation");

	if (RenderWindow == NULL)
	{
	try
	{
		m_RenderWindow = Root->createRenderWindow("SceneNodeAnimation", rect.Width(), rect.Height(), false, &parms);
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
	m_Camera->setPosition(Ogre::Vector3(0.0, 0.0, 500.0));
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

	m_Camera->lookAt(Ogre::Vector3(0.0, 0.0, 0.0));
	Ogre::Animation *SphereAnimation = SceneManager->createAnimation("SphereAnimation", 10);
	SphereAnimation->setInterpolationMode(Ogre::Animation::InterpolationMode::IM_SPLINE);

	Ogre::NodeAnimationTrack* SphereTrack = SphereAnimation->createNodeTrack(1, SphereNode);
    Ogre::TransformKeyFrame* SphereKey = SphereTrack->createNodeKeyFrame(0); 
            
	SphereKey->setRotation(Ogre::Quaternion(1,1.5,0,0));
	SphereKey = SphereTrack->createNodeKeyFrame(2.5);
	SphereKey->setTranslate(Ogre::Vector3(70,50,-100));
    SphereKey = SphereTrack->createNodeKeyFrame(5);
    SphereKey->setTranslate(Ogre::Vector3(-1500,1000,-600));
    SphereKey = SphereTrack->createNodeKeyFrame(7.5);
    SphereKey->setTranslate(Ogre::Vector3(0,-100,0));
    SphereKey = SphereTrack->createNodeKeyFrame(10);
    SphereKey->setTranslate(Ogre::Vector3(0,0,0));
            
	Ogre::AnimationState *SphereAnimationState = SceneManager->createAnimationState("SphereAnimation");
    SphereAnimationState->setEnabled(true);
    SphereAnimationState->setLoop(true);

	Root->renderOneFrame();
}

#ifdef _DEBUG
void CSceneNodeAnimationView::AssertValid() const
{
	CView::AssertValid();
}

void CSceneNodeAnimationView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSceneNodeAnimationDoc* CSceneNodeAnimationView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSceneNodeAnimationDoc)));
	return (CSceneNodeAnimationDoc*)m_pDocument;
}
#endif //_DEBUG


// CSceneNodeAnimationView message handlers


void CSceneNodeAnimationView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_MousePosition = point;
	m_MouseNavigation = true;
	CView::OnLButtonDown(nFlags, point);
}


void CSceneNodeAnimationView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_MouseNavigation = false;

	CView::OnLButtonUp(nFlags, point);
}


void CSceneNodeAnimationView::OnMouseMove(UINT nFlags, CPoint point)
{

	if (m_MouseNavigation)
	{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[0] = -(m_MousePosition.x - point.x);
	CameraMove[1] = m_MousePosition.y - point.y;

	CEngine * Engine = ((CSceneNodeAnimationApp*)AfxGetApp())->m_Engine;
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


BOOL CSceneNodeAnimationView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[2] = 0.1 * zDelta;

	CEngine * Engine = ((CSceneNodeAnimationApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return false;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return false;
	m_Camera->moveRelative(CameraMove);

	Root->renderOneFrame();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CSceneNodeAnimationView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CView::OnPaint() for painting messages
	CSceneNodeAnimationDoc *Document = GetDocument();
	CEngine *Engine = ((CSceneNodeAnimationApp*)AfxGetApp())->m_Engine;
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


void CSceneNodeAnimationView::OnTimer(UINT_PTR nIDEvent)
{
	CEngine *Engine = ((CSceneNodeAnimationApp*)AfxGetApp())->m_Engine;
	Ogre::Root *Root = Engine->GetRoot();

	Ogre::SceneManager *SceneManager = Root->getSceneManager("SceneNodeAnimation");
	Ogre::AnimationState *AnimationState = SceneManager->getAnimationState("SphereAnimation");
	AnimationState->addTime(0.01);
    Root->renderOneFrame();

	CView::OnTimer(nIDEvent);
}


void CSceneNodeAnimationView::OnAnimationStop()
{
	KillTimer(1);
}


void CSceneNodeAnimationView::OnAnimationStart()
{
	SetTimer(1, 1, 0);
}
