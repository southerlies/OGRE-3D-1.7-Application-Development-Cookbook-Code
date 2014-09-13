
// AnimatedLightView.cpp : implementation of the CAnimatedLightView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "AnimatedLight.h"
#endif

#include "AnimatedLightDoc.h"
#include "AnimatedLightView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAnimatedLightView

IMPLEMENT_DYNCREATE(CAnimatedLightView, CView)

BEGIN_MESSAGE_MAP(CAnimatedLightView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_COMMAND(ID_ANIMATION_START, &CAnimatedLightView::OnAnimationStart)
	ON_COMMAND(ID_ANIMATION_STOP, &CAnimatedLightView::OnAnimationStop)
END_MESSAGE_MAP()

// CAnimatedLightView construction/destruction

CAnimatedLightView::CAnimatedLightView()
{
m_MouseNavigation = false;
}

CAnimatedLightView::~CAnimatedLightView()
{
}

BOOL CAnimatedLightView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CAnimatedLightView drawing

void CAnimatedLightView::OnDraw(CDC* /*pDC*/)
{
	CAnimatedLightDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CAnimatedLightView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CAnimatedLightView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CAnimatedLightView diagnostics

void CAnimatedLightView::EngineSetup(void)
{
	Ogre::Root *Root = ((CAnimatedLightApp*)AfxGetApp())->m_Engine->GetRoot();
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
	m_Camera->setPosition(Ogre::Vector3(0.0, 0.0, 100.0));
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
	SphereEntity->getParentNode()->scale(0.2,0.2,0.2);

	Ogre::Light* AnimatedLight = SceneManager->createLight("AnimLight");
 
	AnimatedLight->setType(Ogre::Light::LT_SPOTLIGHT);
	AnimatedLight->setDiffuseColour(Ogre::ColourValue(0.25f,0.25f,0.0f));
	AnimatedLight->setSpecularColour(Ogre::ColourValue(0.25f,0.25f,0.0f));
	AnimatedLight->setAttenuation(8000,1,0.0005,0);
	AnimatedLight->setSpotlightRange(Ogre::Degree(60), Ogre::Degree(70));
	AnimatedLight->setDirection(Ogre::Vector3::NEGATIVE_UNIT_Y);

	Ogre::BillboardSet* LightBillboardSet = SceneManager->createBillboardSet("LightBillboardSet", 1);
	LightBillboardSet->setMaterialName("Examples/Flare");
	Ogre::Billboard* LightBillboard = LightBillboardSet->createBillboard(0,0,0,Ogre::ColourValue(0.5,0.3,0.0f));

	Ogre::SceneNode* AnimLightNode = SceneManager->getRootSceneNode()->createChildSceneNode("AnimLightNode");
 
	AnimLightNode->attachObject(AnimatedLight);
	AnimLightNode->attachObject(LightBillboardSet);
 
	AnimLightNode->setPosition(20.0, 20.0, 0.0);
	AnimLightNode->setScale(0.05f, 0.05f, 0.05f);

	Ogre::Animation* Animation = SceneManager->createAnimation("LightAnimation", 4.0);
	Animation->setInterpolationMode(Ogre::Animation::IM_SPLINE);
	Ogre::NodeAnimationTrack* track = Animation->createNodeTrack(0, AnimLightNode);

	Ogre::TransformKeyFrame* key;
 
	key = track->createNodeKeyFrame(0.0f);
	key->setTranslate(Ogre::Vector3(-20.0f, -20.0f, 0.0f));
	key->setScale(Ogre::Vector3(0.05f,0.05f,0.05f));
 
	key = track->createNodeKeyFrame(1);
	key->setTranslate(Ogre::Vector3( -20.0f, 20.0f,0.0f));
	key->setScale(Ogre::Vector3(0.05f,0.05f,0.05f));
 
	key = track->createNodeKeyFrame(2.0);
	key ->setTranslate(Ogre::Vector3( 20.0f, 20.0f, 0.0f));
	key->setScale(Ogre::Vector3(0.05f,0.05f,0.05f));
 
	key = track->createNodeKeyFrame(3.0);
	key->setTranslate(Ogre::Vector3(20.0f, -20.0f, 0.0f));
	key->setScale(Ogre::Vector3(0.05f,0.05f,0.05f));
 
	key = track->createNodeKeyFrame(4.0);
	key->setTranslate(Ogre::Vector3(-20.0f, -20.0f,0.0f));
	key->setScale(Ogre::Vector3(0.05f,0.05f,0.05f));

	m_LightAnimationState = SceneManager->createAnimationState("LightAnimation");
	m_LightAnimationState->setEnabled(true);
	m_LightAnimationState->setLoop(true);

	m_Camera->lookAt(Ogre::Vector3(0.0, 0.0, 0.0));
	
	Root->renderOneFrame();
}

#ifdef _DEBUG
void CAnimatedLightView::AssertValid() const
{
	CView::AssertValid();
}

void CAnimatedLightView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAnimatedLightDoc* CAnimatedLightView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAnimatedLightDoc)));
	return (CAnimatedLightDoc*)m_pDocument;
}
#endif //_DEBUG


// CAnimatedLightView message handlers


void CAnimatedLightView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_MousePosition = point;
	m_MouseNavigation = true;
	CView::OnLButtonDown(nFlags, point);
}


void CAnimatedLightView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_MouseNavigation = false;

	CView::OnLButtonUp(nFlags, point);
}


void CAnimatedLightView::OnMouseMove(UINT nFlags, CPoint point)
{

	if (m_MouseNavigation)
	{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[0] = -(m_MousePosition.x - point.x);
	CameraMove[1] = m_MousePosition.y - point.y;

	CEngine * Engine = ((CAnimatedLightApp*)AfxGetApp())->m_Engine;
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


BOOL CAnimatedLightView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[2] = 0.1 * zDelta;

	CEngine * Engine = ((CAnimatedLightApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return false;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return false;
	m_Camera->moveRelative(CameraMove);

	Root->renderOneFrame();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CAnimatedLightView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CView::OnPaint() for painting messages
	CAnimatedLightDoc *Document = GetDocument();
	CEngine *Engine = ((CAnimatedLightApp*)AfxGetApp())->m_Engine;
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


void CAnimatedLightView::OnTimer(UINT_PTR nIDEvent)
{
	CEngine *Engine = ((CAnimatedLightApp*)AfxGetApp())->m_Engine;
	Ogre::Root *Root = Engine->GetRoot();

	m_LightAnimationState->addTime(0.01);

    Root->renderOneFrame();

	CView::OnTimer(nIDEvent);
}


void CAnimatedLightView::OnAnimationStart()
{
	SetTimer(1, 1, 0);
}


void CAnimatedLightView::OnAnimationStop()
{
	KillTimer(1);
}
