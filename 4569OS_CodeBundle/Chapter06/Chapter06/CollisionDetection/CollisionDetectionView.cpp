
// CollisionDetectionView.cpp : implementation of the CCollisionDetectionView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "CollisionDetection.h"
#endif

#include "CollisionDetectionDoc.h"
#include "CollisionDetectionView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCollisionDetectionView

IMPLEMENT_DYNCREATE(CCollisionDetectionView, CView)

BEGIN_MESSAGE_MAP(CCollisionDetectionView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

// CCollisionDetectionView construction/destruction

CCollisionDetectionView::CCollisionDetectionView()
{
       m_MouseNavigation = false;
	m_Camera = NULL;
}

CCollisionDetectionView::~CCollisionDetectionView()
{
}

BOOL CCollisionDetectionView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CCollisionDetectionView drawing

void CCollisionDetectionView::OnDraw(CDC* /*pDC*/)
{
	CCollisionDetectionDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CCollisionDetectionView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CCollisionDetectionView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CCollisionDetectionView diagnostics

void CCollisionDetectionView::EngineSetup(void)
{
	Ogre::Root *Root = ((CCollisionDetectionApp*)AfxGetApp())->m_Engine->GetRoot();
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

	m_CollisionTools = new MOC::CollisionTools(SceneManager);

	Ogre::SceneNode *SunNode = SceneManager->getRootSceneNode()->createChildSceneNode("Sun", Ogre::Vector3(0,0,0));
	Ogre::SceneNode *EarthNode = SceneManager->getRootSceneNode()->createChildSceneNode("Earth", Ogre::Vector3(50,0,0));
	Ogre::SceneNode *MoonNode = SceneManager->getRootSceneNode()->createChildSceneNode("Moon", Ogre::Vector3(100,0,0));

	Ogre::Entity *SunEntity = SceneManager->createEntity("Sun", "sphere.mesh");
	Ogre::Entity *EarthEntity = SceneManager->createEntity("Earth", "sphere.mesh");
	Ogre::Entity *MoonEntity = SceneManager->createEntity("Moon", "sphere.mesh");

	SunNode->attachObject(SunEntity);
	SunEntity->getParentNode()->scale(0.1,0.1,0.1);

	EarthNode->attachObject(EarthEntity);
	EarthEntity->getParentNode()->scale(0.1,0.1,0.1);

	MoonNode->attachObject(MoonEntity);
	MoonEntity->getParentNode()->scale(0.1,0.1,0.1);
	
	Ogre::AxisAlignedBox Box = EarthEntity->getBoundingBox();
	Ogre::Vector3 Center = Box.getCenter();
	m_Camera->lookAt(Center);

	Ogre::uint32 QueryMask = 0xFFFFFFFF;
	if (m_CollisionTools->collidesWithEntity(m_Camera->getPosition(), Ogre::Vector3(50,0,0), 200.0f, 0.0f, QueryMask))
	{
				// undo move
	//			mCamNode->setPosition(oldPos);
	}

	Root->renderOneFrame();
}

#ifdef _DEBUG
void CCollisionDetectionView::AssertValid() const
{
	CView::AssertValid();
}

void CCollisionDetectionView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCollisionDetectionDoc* CCollisionDetectionView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCollisionDetectionDoc)));
	return (CCollisionDetectionDoc*)m_pDocument;
}
#endif //_DEBUG


// CCollisionDetectionView message handlers


void CCollisionDetectionView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CView::OnPaint() for painting messages

	CCollisionDetectionDoc *Document = GetDocument();
	CEngine *Engine = ((CCollisionDetectionApp*)AfxGetApp())->m_Engine;
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


void CCollisionDetectionView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnTimer(nIDEvent);
}


void CCollisionDetectionView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_MousePosition = point;
	m_MouseNavigation = true;

	CView::OnLButtonDown(nFlags, point);
}


void CCollisionDetectionView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_MouseNavigation = false;

	CView::OnLButtonUp(nFlags, point);
}


void CCollisionDetectionView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_MouseNavigation)
	{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[0] = -(m_MousePosition.x - point.x);
	CameraMove[1] = m_MousePosition.y - point.y;

	CEngine * Engine = ((CCollisionDetectionApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return;
	Ogre::Vector3 OldPosition = m_Camera->getPosition();
	m_Camera->moveRelative(CameraMove);
		
	if (m_CollisionTools->collidesWithEntity(m_Camera->getPosition(), Ogre::Vector3(50,0,0), 100.0f, 0.0f, 4294967295))
	{
				// undo move
	//			mCamNode->setPosition(oldPos);
	}
	m_MousePosition = point;


	Root->renderOneFrame();
	}


	CView::OnMouseMove(nFlags, point);
}


BOOL CCollisionDetectionView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[2] = 0.1 * zDelta;

	CEngine * Engine = ((CCollisionDetectionApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return false;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return false;
	m_Camera->moveRelative(CameraMove);

	Root->renderOneFrame();
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}
