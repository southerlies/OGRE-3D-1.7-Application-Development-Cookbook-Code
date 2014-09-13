
// LinkedControllersView.cpp : implementation of the CLinkedControllersView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "LinkedControllers.h"
#endif

#include "LinkedControllersDoc.h"
#include "LinkedControllersView.h"

#include "LightControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLinkedControllersView

IMPLEMENT_DYNCREATE(CLinkedControllersView, CView)

BEGIN_MESSAGE_MAP(CLinkedControllersView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_COMMAND(ID_ANIMATION_START, &CLinkedControllersView::OnAnimationStart)
	ON_COMMAND(ID_ANIMATION_STOP, &CLinkedControllersView::OnAnimationStop)
END_MESSAGE_MAP()

// CLinkedControllersView construction/destruction

CLinkedControllersView::CLinkedControllersView()
{
m_MouseNavigation = false;
}

CLinkedControllersView::~CLinkedControllersView()
{
}

BOOL CLinkedControllersView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CLinkedControllersView drawing

void CLinkedControllersView::OnDraw(CDC* /*pDC*/)
{
	CLinkedControllersDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CLinkedControllersView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CLinkedControllersView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CLinkedControllersView diagnostics

void CLinkedControllersView::EngineSetup(void)
{
	Ogre::Root *Root = ((CLinkedControllersApp*)AfxGetApp())->m_Engine->GetRoot();
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
	m_Camera->setPosition(Ogre::Vector3(0.0, 0.0, 500.0));
	Ogre::SceneNode *CameraNode = NULL;
	CameraNode = SceneManager->getRootSceneNode()->createChildSceneNode("CameraNode");

	Ogre::Viewport* Viewport = NULL;
	
	if (0 == m_RenderWindow->getNumViewports())
	{
		Viewport = m_RenderWindow->addViewport(m_Camera);
		Viewport->setBackgroundColour(Ogre::ColourValue(0.5, 0.5, 0.5));
	}

    // Alter the camera aspect ratio to match the viewport
    m_Camera->setAspectRatio(Ogre::Real(rect.Width()) / Ogre::Real(rect.Height()));
	SceneManager->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));
	Ogre::SceneNode *SphereNode = SceneManager->getRootSceneNode()->createChildSceneNode("Sphere", Ogre::Vector3(0,0,0));
	Ogre::Entity *SphereEntity = SceneManager->createEntity("Sphere", "sphere.mesh");
	SphereEntity->setMaterialName("Wall/White");
	SphereNode->attachObject(SphereEntity);
	SphereEntity->getParentNode()->scale(0.5,0.5,0.5);

	m_Camera->lookAt(Ogre::Vector3(0.0, 0.0, 0.0));
 
	m_Lights = SceneManager->createBillboardSet("Lights");
    m_Lights->setMaterialName("light");
	SphereNode->attachObject(m_Lights);

 m_RedBillBoard = m_Lights->createBillboard(Ogre::Vector3(0, 0, 200));
 m_RedBillBoard->setColour(ColourValue::Red);
 
 m_BlueBillBoard = m_Lights->createBillboard(Ogre::Vector3(200, 0, 0));
 m_BlueBillBoard->setColour(ColourValue::Blue);
 
 m_WhiteBillBoard = m_Lights->createBillboard(Ogre::Vector3(-200, 0, 0));
 m_WhiteBillBoard->setColour(ColourValue::White);

 m_RedLight = SceneManager->createLight("RedLight");
 m_RedLight->setType(Light::LT_POINT);
 m_RedLight->setPosition(Ogre::Vector3(0, 0, 200));
 m_RedLight->setDiffuseColour(ColourValue::Red);
 
 m_BlueLight = SceneManager->createLight("BlueLight");
 m_BlueLight->setType(Light::LT_POINT);
 m_BlueLight->setPosition(Ogre::Vector3(200, 0, 0));
 m_BlueLight->setDiffuseColour(ColourValue::Blue);
 
 m_WhiteLight = SceneManager->createLight("WhiteLight");
 m_WhiteLight->setType(Light::LT_POINT);
 m_WhiteLight->setPosition(Ogre::Vector3(-200, 0, 0));
 m_WhiteLight->setDiffuseColour(ColourValue::White);


#undef new

	Ogre::ControllerFunctionRealPtr RedFunc(OGRE_NEW Ogre::WaveformControllerFunction(Ogre::WFT_SINE));//, 0.5, 0.0));
	Ogre::ControllerFunctionRealPtr BlueFunc(OGRE_NEW Ogre::WaveformControllerFunction(Ogre::WFT_SQUARE));//, 0.5, 0.5));
	Ogre::ControllerFunctionRealPtr WhiteFunc(OGRE_NEW Ogre::WaveformControllerFunction(Ogre::WFT_TRIANGLE));//, 4, 0.0));

	Ogre::ControllerValueRealPtr RedDest(OGRE_NEW LightControl(m_RedLight, m_RedBillBoard, Ogre::ColourValue::Red));
	Ogre::ControllerValueRealPtr BlueDest(OGRE_NEW LightControl(m_BlueLight, m_BlueBillBoard, Ogre::ColourValue::Blue));
	Ogre::ControllerValueRealPtr WhiteDest(OGRE_NEW LightControl(m_WhiteLight, m_WhiteBillBoard, Ogre::ColourValue::White));

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

	Ogre::ControllerManager& ControllerManager = Ogre::ControllerManager::getSingleton();

	m_RedController = ControllerManager.createController(ControllerManager.getFrameTimeSource(), RedDest, RedFunc);
	m_BlueController = ControllerManager.createController(ControllerManager.getFrameTimeSource(), BlueDest, BlueFunc);
	m_WhiteController = ControllerManager.createController(ControllerManager.getFrameTimeSource(), WhiteDest, WhiteFunc);

	Root->renderOneFrame();
}

#ifdef _DEBUG
void CLinkedControllersView::AssertValid() const
{
	CView::AssertValid();
}

void CLinkedControllersView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLinkedControllersDoc* CLinkedControllersView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLinkedControllersDoc)));
	return (CLinkedControllersDoc*)m_pDocument;
}
#endif //_DEBUG


// CLinkedControllersView message handlers


void CLinkedControllersView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_MousePosition = point;
	m_MouseNavigation = true;
	CView::OnLButtonDown(nFlags, point);
}


void CLinkedControllersView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_MouseNavigation = false;

	CView::OnLButtonUp(nFlags, point);
}


void CLinkedControllersView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_MouseNavigation)
	{	
         Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[0] = -(m_MousePosition.x - point.x);
	CameraMove[1] = m_MousePosition.y - point.y;

	CEngine * Engine = ((CLinkedControllersApp*)AfxGetApp())->m_Engine;
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


BOOL CLinkedControllersView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[2] = 0.1 * zDelta;

	CEngine * Engine = ((CLinkedControllersApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return false;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return false;
	m_Camera->moveRelative(CameraMove);

	Root->renderOneFrame();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CLinkedControllersView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CView::OnPaint() for painting messages
	CLinkedControllersDoc *Document = GetDocument();
	CEngine *Engine = ((CLinkedControllersApp*)AfxGetApp())->m_Engine;
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


void CLinkedControllersView::OnTimer(UINT_PTR nIDEvent)
{
	CEngine *Engine = ((CLinkedControllersApp*)AfxGetApp())->m_Engine;
	Ogre::Root *Root = Engine->GetRoot();
    Root->renderOneFrame();

	CView::OnTimer(nIDEvent);
}


void CLinkedControllersView::OnAnimationStart()
{
	SetTimer(1, 1, 0);
}


void CLinkedControllersView::OnAnimationStop()
{
	KillTimer(1);
}
