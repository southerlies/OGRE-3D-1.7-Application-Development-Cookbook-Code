
// UsingControllersView.cpp : implementation of the CUsingControllersView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "UsingControllers.h"
#endif

#include "UsingControllersDoc.h"
#include "UsingControllersView.h"

#include "Ogre.h"
#include "SphereScale.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUsingControllersView

IMPLEMENT_DYNCREATE(CUsingControllersView, CView)

BEGIN_MESSAGE_MAP(CUsingControllersView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_COMMAND(ID_ANIMATION_START, &CUsingControllersView::OnAnimationStart)
	ON_COMMAND(ID_ANIMATION_STOP, &CUsingControllersView::OnAnimationStop)
END_MESSAGE_MAP()

// CUsingControllersView construction/destruction

CUsingControllersView::CUsingControllersView()
{
m_MouseNavigation = false;
}

CUsingControllersView::~CUsingControllersView()
{
}

BOOL CUsingControllersView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CUsingControllersView drawing

void CUsingControllersView::OnDraw(CDC* /*pDC*/)
{
	CUsingControllersDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CUsingControllersView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CUsingControllersView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CUsingControllersView diagnostics

void CUsingControllersView::EngineSetup(void)
{
	Ogre::Root *Root = ((CUsingControllersApp*)AfxGetApp())->m_Engine->GetRoot();
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
	m_Camera->setPosition(Ogre::Vector3(0.0, 0.0, 1000.0));
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

	m_Camera->lookAt(Ogre::Vector3(0.0, 0.0, 0.0));

#undef new

	Ogre::ControllerFunctionRealPtr func(OGRE_NEW Ogre::WaveformControllerFunction(Ogre::WFT_SINE, 0.0, 1.0));
	Ogre::ControllerValueRealPtr dest(OGRE_NEW SphereScale(SphereNode, 1.0));

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

	Ogre::ControllerManager& ControllerManager = Ogre::ControllerManager::getSingleton();
	m_SphereController = ControllerManager.createController(ControllerManager.getFrameTimeSource(), dest, func);

	Root->renderOneFrame();
}

#ifdef _DEBUG
void CUsingControllersView::AssertValid() const
{
	CView::AssertValid();
}

void CUsingControllersView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CUsingControllersDoc* CUsingControllersView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CUsingControllersDoc)));
	return (CUsingControllersDoc*)m_pDocument;
}
#endif //_DEBUG


// CUsingControllersView message handlers


void CUsingControllersView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_MousePosition = point;
	m_MouseNavigation = true;
	CView::OnLButtonDown(nFlags, point);
}


void CUsingControllersView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_MouseNavigation = false;

	CView::OnLButtonUp(nFlags, point);
}


void CUsingControllersView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_MouseNavigation)
	{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[0] = -(m_MousePosition.x - point.x);
	CameraMove[1] = m_MousePosition.y - point.y;

	CEngine * Engine = ((CUsingControllersApp*)AfxGetApp())->m_Engine;
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


BOOL CUsingControllersView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[2] = 0.1 * zDelta;

	CEngine * Engine = ((CUsingControllersApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return false;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return false;
	m_Camera->moveRelative(CameraMove);

	Root->renderOneFrame();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CUsingControllersView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CView::OnPaint() for painting messages
	CUsingControllersDoc *Document = GetDocument();
	CEngine *Engine = ((CUsingControllersApp*)AfxGetApp())->m_Engine;
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


void CUsingControllersView::OnTimer(UINT_PTR nIDEvent)
{
	CEngine *Engine = ((CUsingControllersApp*)AfxGetApp())->m_Engine;
	Ogre::Root *Root = Engine->GetRoot();
    Root->renderOneFrame();

	CView::OnTimer(nIDEvent);
}


void CUsingControllersView::OnAnimationStart()
{
	SetTimer(1, 100, 0);
}


void CUsingControllersView::OnAnimationStop()
{
	KillTimer(1);
}
