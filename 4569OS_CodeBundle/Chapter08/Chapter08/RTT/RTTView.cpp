
// RTTView.cpp : implementation of the CRTTView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "RTT.h"
#endif

#include "RTTDoc.h"
#include "RTTView.h"
#include "RTTListener.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRTTView

IMPLEMENT_DYNCREATE(CRTTView, CView)

BEGIN_MESSAGE_MAP(CRTTView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_COMMAND(ID_ANIMATION_START, &CRTTView::OnAnimationStart)
	ON_COMMAND(ID_ANIMATION_STOP, &CRTTView::OnAnimationStop)
END_MESSAGE_MAP()

// CRTTView construction/destruction

CRTTView::CRTTView()
{
	m_First = true;
	m_MouseNavigation = false;
}

CRTTView::~CRTTView()
{
}

BOOL CRTTView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CRTTView drawing

void CRTTView::OnDraw(CDC* /*pDC*/)
{
	CRTTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CRTTView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CRTTView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CRTTView diagnostics

#ifdef _DEBUG
void CRTTView::AssertValid() const
{
	CView::AssertValid();
}

void CRTTView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRTTDoc* CRTTView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRTTDoc)));
	return (CRTTDoc*)m_pDocument;
}
#endif //_DEBUG


// CRTTView message handlers


void CRTTView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_MousePosition = point;
	m_MouseNavigation = true;

	CView::OnLButtonDown(nFlags, point);
}


void CRTTView::OnLButtonUp(UINT nFlags, CPoint point)
{
m_MouseNavigation = false;

	CView::OnLButtonUp(nFlags, point);
}


void CRTTView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_MouseNavigation)
	{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[0] = -(m_MousePosition.x - point.x);
	CameraMove[1] = m_MousePosition.y - point.y;

	CEngine * Engine = ((CRTTApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return;
	Ogre::Vector3 OldPosition = m_Camera->getPosition();
	m_Camera->moveRelative(CameraMove);
	
//	m_RTTCamera->setPosition(m_Camera->getPosition());
//	m_RTTCamera->setOrientation(m_Camera->getOrientation());

	m_MousePosition = point;

	Root->renderOneFrame();
	}
	CView::OnMouseMove(nFlags, point);
}


BOOL CRTTView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[2] = 0.1 * zDelta;

	CEngine * Engine = ((CRTTApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return false;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return false;
	
	m_Camera->moveRelative(CameraMove);
	
//	m_RTTCamera->setPosition(m_Camera->getPosition());
//	m_RTTCamera->setOrientation(m_Camera->getOrientation());

	Root->renderOneFrame();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CRTTView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRTTDoc *Document = GetDocument();
	CEngine *Engine = ((CRTTApp*)AfxGetApp())->m_Engine;
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
	// Do not call CView::OnPaint() for painting messages
}


void CRTTView::OnTimer(UINT_PTR nIDEvent)
{
	m_WalkAnimation->addTime(0.001);

	CEngine *Engine = ((CRTTApp*)AfxGetApp())->m_Engine;
	Ogre::Root *Root = Engine->GetRoot();
    Root->renderOneFrame();

	CView::OnTimer(nIDEvent);
}

void CRTTView::EngineSetup(void)
{
	Ogre::Root *Root = ((CRTTApp*)AfxGetApp())->m_Engine->GetRoot();

	m_SceneManager = Root->createSceneManager(Ogre::ST_GENERIC, "RTT");

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

	try
	{
		m_RenderWindow = Root->createRenderWindow("RTT", rect.Width(), rect.Height(), false, &parms);
	}
    catch(...)
	{
		MessageBox("Cannot initialize\nCheck that graphic-card driver is up-to-date", "Initialize Render System", MB_OK | MB_ICONSTOP);
		exit(EXIT_SUCCESS);
	}
// Load resources
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    // Create the camera
    m_Camera = m_SceneManager->createCamera("Camera");
    m_Camera->setNearClipDistance(0.5);
	m_Camera->setFarClipDistance(5000); 
	m_Camera->setCastShadows(false);
	m_Camera->setUseRenderingDistance(true);
	m_Camera->setPosition(Ogre::Vector3(200.0, 50.0, 100.0));
	Ogre::SceneNode *CameraNode = NULL;
	CameraNode = m_SceneManager->getRootSceneNode()->createChildSceneNode("CameraNode");

	Ogre::Viewport* Viewport = NULL;
	
	if (0 == m_RenderWindow->getNumViewports())
	{
		Viewport = m_RenderWindow->addViewport(m_Camera);
		Viewport->setBackgroundColour(Ogre::ColourValue(0.8f, 1.0f, 0.8f));
	}

    // Alter the camera aspect ratio to match the viewport
    m_Camera->setAspectRatio(Ogre::Real(rect.Width()) / Ogre::Real(rect.Height()));
		
	Ogre::Entity *RobotEntity = m_SceneManager->createEntity("Robot", "robot.mesh");
	Ogre::SceneNode *RobotNode = m_SceneManager->getRootSceneNode()->createChildSceneNode();
	RobotNode->attachObject(RobotEntity);

	Ogre::AxisAlignedBox Box = RobotEntity->getBoundingBox();
	Ogre::Vector3 Center = Box.getCenter();
	m_Camera->lookAt(Center);

	m_RTTCamera = m_SceneManager->createCamera("RTTCam");
    m_RTTCamera->setNearClipDistance(m_Camera->getNearClipDistance());
    m_RTTCamera->setFarClipDistance(m_Camera->getFarClipDistance());
    m_RTTCamera->setAspectRatio( (Ogre::Real)m_RenderWindow->getViewport(0)->getActualWidth() /
                         (Ogre::Real)m_RenderWindow->getViewport(0)->getActualHeight());

	m_RTTCamera->setPosition(Ogre::Vector3(200.0, 50.0, 100.0));
	m_RTTCamera->lookAt(Center);

//	m_RTTCamera->setPosition(m_Camera->getPosition());
//	m_RTTCamera->setOrientation(m_Camera->getOrientation());

	m_WalkAnimation = RobotEntity->getAnimationState("Walk");
	m_WalkAnimation->setEnabled(true);

	createRTT("RTT","RTT", m_RTTCamera, 1000, 1000, Ogre::Vector3(100.0, 0.0, 0.0));
	Root->renderOneFrame();
}

void CRTTView::createRTT(Ogre::String planeName,
                        Ogre::String texName,
                        Ogre::Camera* Camera,
                        Ogre::Real sizeX,
                        Ogre::Real sizeY,
                        Ogre::Vector3 position
						)
{
#undef new
    m_Plane = new Ogre::MovablePlane(planeName + "_RTTPlane");
#define new DEBUG_NEW
    m_Plane->d = 0;
    m_Plane->normal = Ogre::Vector3::UNIT_Y;

    Ogre::MeshManager::getSingleton().createPlane(planeName + "_RTTPlane",
      Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
      *m_Plane, sizeX, sizeY,
      1, 1, true, 1, 1, 1, Ogre::Vector3::UNIT_Z);

	m_PlaneEntity = m_SceneManager->createEntity(planeName + "_Plane", planeName + "_RTTPlane");

	Ogre::SceneNode *PlaneNode = m_SceneManager->getRootSceneNode()->createChildSceneNode();

	PlaneNode->attachObject(m_PlaneEntity);
	PlaneNode->attachObject(m_Plane);

	PlaneNode->setPosition(position);

	Ogre::TexturePtr texture = Ogre::TextureManager::getSingleton().createManual(texName,
         Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D,
         512, 512, 0, Ogre::PF_R8G8B8, Ogre::TU_RENDERTARGET);

	Ogre::RenderTarget *RenderTarget = texture->getBuffer()->getRenderTarget();
   
    Ogre::Viewport *Viewport = RenderTarget->addViewport(Camera);
    Viewport->setClearEveryFrame(true);
	Viewport->setBackgroundColour(Ogre::ColourValue::White);

    Ogre::MaterialPtr Material = Ogre::MaterialManager::getSingleton().create(texName,
                Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

    Ogre::TextureUnitState* TextureUnitState = Material->getTechnique(0)->getPass(0)->createTextureUnitState(texName);

    TextureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
    TextureUnitState->setProjectiveTexturing(true, Camera);
	RenderTarget->setAutoUpdated(true);

	m_PlaneEntity->setMaterialName(Material.get()->getName());
	RTTListener *Listener = new RTTListener;
	Listener->m_PlaneEntity = m_PlaneEntity;
    RenderTarget->addListener(Listener);

	Camera->enableCustomNearClipPlane(m_Plane);
}

void CRTTView::OnAnimationStart()
{
	SetTimer(1, 1, 0);
}


void CRTTView::OnAnimationStop()
{
	KillTimer(1);
}
