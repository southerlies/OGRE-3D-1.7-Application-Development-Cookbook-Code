
// LinkedMovementView.cpp : implementation of the CLinkedMovementView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "LinkedMovement.h"
#endif

#include "LinkedMovementDoc.h"
#include "LinkedMovementView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLinkedMovementView

IMPLEMENT_DYNCREATE(CLinkedMovementView, CView)

BEGIN_MESSAGE_MAP(CLinkedMovementView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_COMMAND(ID_MANAGEMENT_START, &CLinkedMovementView::OnManagementStart)
	ON_COMMAND(ID_MANAGEMENT_STOP, &CLinkedMovementView::OnManagementStop)
END_MESSAGE_MAP()

// CLinkedMovementView construction/destruction

CLinkedMovementView::CLinkedMovementView()
{
	m_First = true;
	m_MouseNavigation = false;
	m_SystemNode = NULL;
	m_RotationAngle = 0.0;
	m_AngleIncrement = Ogre::Math::PI/500.0;
}

CLinkedMovementView::~CLinkedMovementView()
{
}

BOOL CLinkedMovementView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CLinkedMovementView drawing

void CLinkedMovementView::OnDraw(CDC* /*pDC*/)
{
	CLinkedMovementDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CLinkedMovementView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CLinkedMovementView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CLinkedMovementView diagnostics

void CLinkedMovementView::EngineSetup(void)
{
	Ogre::Root *Root = ((CLinkedMovementApp*)AfxGetApp())->m_Engine->GetRoot();
	Ogre::SceneManager *SceneManager = NULL;
	SceneManager = Root->createSceneManager(Ogre::ST_GENERIC, "MouseInput");
 
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

	m_SystemNode = SceneManager->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(0,0,0));

	Ogre::SceneNode *SunNode = m_SystemNode->createChildSceneNode("Sun", Ogre::Vector3(0,0,0));
	Ogre::SceneNode *EarthNode = m_SystemNode->createChildSceneNode("Earth", Ogre::Vector3(50,0,0));
	Ogre::SceneNode *MoonNode = EarthNode->createChildSceneNode("Moon", Ogre::Vector3(200,0,0));

	MoonNode->setInheritScale(false);

	Ogre::Entity *SunEntity = SceneManager->createEntity("Sun", "sphere.mesh");
	Ogre::Entity *EarthEntity = SceneManager->createEntity("Earth", "sphere.mesh");
	Ogre::Entity *MoonEntity = SceneManager->createEntity("Moon", "sphere.mesh");

	SunNode->attachObject(SunEntity);
	SunEntity->getParentNode()->scale(0.1,0.1,0.1);

	EarthNode->attachObject(EarthEntity);
	EarthEntity->getParentNode()->scale(0.1,0.1,0.1);

	MoonNode->attachObject(MoonEntity);
	MoonEntity->getParentNode()->scale(0.5,0.5,0.5);
	
	Ogre::AxisAlignedBox Box = EarthEntity->getBoundingBox();
	Ogre::Vector3 Center = Box.getCenter();
	m_Camera->setPolygonMode(Ogre::PolygonMode::PM_WIREFRAME);
	m_Camera->lookAt(Center);

	Root->renderOneFrame();
}

#ifdef _DEBUG
void CLinkedMovementView::AssertValid() const
{
	CView::AssertValid();
}

void CLinkedMovementView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLinkedMovementDoc* CLinkedMovementView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLinkedMovementDoc)));
	return (CLinkedMovementDoc*)m_pDocument;
}
#endif //_DEBUG


// CLinkedMovementView message handlers


void CLinkedMovementView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_MousePosition = point;
	m_MouseNavigation = true;

	CView::OnLButtonDown(nFlags, point);
}


void CLinkedMovementView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_MouseNavigation = false;

	CView::OnLButtonUp(nFlags, point);
}


void CLinkedMovementView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_MouseNavigation)
	{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[0] = -(m_MousePosition.x - point.x);
	CameraMove[1] = m_MousePosition.y - point.y;

	CEngine * Engine = ((CLinkedMovementApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return;
	m_Camera->moveRelative(CameraMove);

	m_MousePosition = point;

	Root->renderOneFrame();
	}

	CView::OnMouseMove(nFlags, point);
}


BOOL CLinkedMovementView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[2] = 0.1 * zDelta;

	CEngine * Engine = ((CLinkedMovementApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return false;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return false;
	m_Camera->moveRelative(CameraMove);

	Root->renderOneFrame();
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CLinkedMovementView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CView::OnPaint() for painting messages

	CLinkedMovementDoc *Document = GetDocument();
	CEngine *Engine = ((CLinkedMovementApp*)AfxGetApp())->m_Engine;
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

void CLinkedMovementView::OnTimer(UINT_PTR nIDEvent)
{
	CLinkedMovementDoc *Document = GetDocument();
	CEngine *Engine = ((CLinkedMovementApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
	  return;

	Ogre::Root *Root = Engine->GetRoot();

	Ogre::Quaternion Quaternion;

	Quaternion.FromAngleAxis(m_RotationAngle, Ogre::Vector3(0.0, 0.0, 1.0));
	
	m_SystemNode->setOrientation(Quaternion);
	Ogre::SceneNode *EarthNode = (Ogre::SceneNode *)m_SystemNode->getChild("Earth");
	EarthNode->setOrientation(Quaternion);
	
	Ogre::SceneNode *MoonNode = (Ogre::SceneNode *)EarthNode->getChild("Moon");
	MoonNode->setOrientation(Quaternion);

	m_RotationAngle += m_AngleIncrement;
	
	Root->renderOneFrame();
	CView::OnTimer(nIDEvent);
}


void CLinkedMovementView::OnManagementStart()
{
	SetTimer(1, 1, 0);	
}


void CLinkedMovementView::OnManagementStop()
{
	KillTimer(1);
}
